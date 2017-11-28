#include <cassert>
#include <algorithm>
#include "Buffer.h"

Buffer::Buffer() : cursor(0, 0), filename("*scratch*") {
    rope = new crope();
    line_boundaries = new LineBoundaries(rope->begin(), rope->end());
}

// TODO: Better error handeling
class file_char_prod : public char_producer<char> {
public:
    FILE* f;

    explicit file_char_prod(const char* file_name) {
        if (nullptr == (f = fopen(file_name, "rb")))
            exit(1);
    }

    ~file_char_prod() override { fclose(f); }

    void operator()(size_t start_pos, size_t len, char* buffer) override {
        if (fseek(f, start_pos, SEEK_SET)) exit(1);
        if (fread(buffer, sizeof(char), len, f) < len) exit(1);
    }

    size_t len() {
        if (fseek(f, 0, SEEK_END)) exit(1);
        return static_cast<size_t>(ftell(f));
    }
};

Buffer::Buffer(const char* filename) : cursor(0, 0), filename(filename) {
    auto *fcp = new file_char_prod(filename);
    rope = new crope(fcp, fcp->len(), true);
    line_boundaries = new LineBoundaries(rope->begin(), rope->end());
}

Buffer::~Buffer() {
    delete line_boundaries;
    delete rope;
}

size_t Buffer::get_x() {
    return cursor.x;
}

size_t Buffer::get_idx() {
    return line_boundaries->at(cursor.y).start_idx + cursor.x;
}

size_t Buffer::get_y() {
    return cursor.y;
}

crope::const_iterator Buffer::begin() {
    return rope->begin();
}

size_t Buffer::length() {
    return rope->length();
}

bool Buffer::backspace() {
    rope->erase(get_idx() - 1, 1);
    if (get_x() != 0) {
        --cursor.x;
        line_boundaries->lengthen(cursor.y, -1);
        return false;
    } else if (get_x() == 0 && get_y() > 0) {
        --cursor.y;
        cursor.x = line_boundaries->at(get_y()).length;
        int length_to_add = static_cast<int>(line_boundaries->at(get_y() + 1).length);
        line_boundaries->remove_line(get_y() + 1);
        line_boundaries->lengthen(get_y(), length_to_add);
        return true;
    } else {
        return false;
    }
}

bool Buffer::insert(char ch) {
    if (ch == '\n') {
        rope->insert(get_idx(), ch);
        line_boundaries->insert(get_y(), 0);
        int rest_of_line_length = static_cast<int>(line_boundaries->at(get_y()).length - get_x());
        line_boundaries->lengthen(get_y(), -rest_of_line_length);
        line_boundaries->lengthen(get_y() + 1, rest_of_line_length);
        cursor.x = 0;
        ++cursor.y;
        return true;
    } else {
        rope->insert(get_idx(), ch);
        ++cursor.x;
        line_boundaries->lengthen(get_y(), 1);
        return false;
    }
}

bool Buffer::insert(string s) {
    rope->insert(get_idx(), s.c_str());
    size_t firstNl = s.find('\n');
    if (firstNl != string::npos) {
        line_boundaries->lengthen(get_y(), firstNl);
        while (true) {
            size_t nextNl = s.find('\n', firstNl + 1);
            if (nextNl == string::npos) {
                line_boundaries->insert(get_y(), s.length() - firstNl - 1);
                ++cursor.y;
                cursor.x = line_boundaries->at(get_y()).length;
                return true;
            }
            line_boundaries->insert(get_y(), nextNl - firstNl - 1);
            firstNl = nextNl;
            ++cursor.y;
        }
    } else {
        line_boundaries->lengthen(get_y(), s.size());
        cursor.x += s.size();
        return false;
    }
}

void Buffer::right() {
    if (get_idx() < this->length()) {
        if (get_x() + 1 > line_boundaries->at(cursor.y).length) {
            cursor.x = 0;
            ++cursor.y;
        } else {
            ++cursor.x;
        }
        cursor.last_x = get_x();
    }
}

void Buffer::left() {
    if (get_idx() > 0) {
        if (get_x() == 0) {
            --cursor.y;
            cursor.x = line_boundaries->at(cursor.y).length;
        } else {
            --cursor.x;
        }
        cursor.last_x = get_x();
    }
}

void Buffer::up() {
    if (cursor.y == 0) {
        return;
    }
    else if (cursor.y == line_boundaries->first_index()) {
        assert(0 && "scrolling up into new text not implemented");
//        line_boundaries->expand_boundary(*rope, false, 1);
    } else {
        --cursor.y;
        if (cursor.last_x < line_boundaries->at(get_y()).length) {
            cursor.x = cursor.last_x;
        } else {
            cursor.x = line_boundaries->at(get_y()).length;
        }
    }
}

void Buffer::down() {
    // TODO: if cursor.y == last_line: return
    if (cursor.y == line_boundaries->last_index()) {
        line_boundaries->expand_boundary(*rope, true, 1);
    }
    ++cursor.y;
    if (cursor.last_x < line_boundaries->at(get_y()).length) {
        cursor.x = cursor.last_x;
    } else {
        cursor.x = line_boundaries->at(get_y()).length;
    }
}

crope::const_iterator Buffer::end() {
    return rope->end();
}

vector<Buffer::Line> Buffer::get_lines(size_t start, size_t num) {
    vector<Buffer::Line> lines;
    for (size_t l = start; l < start + num; l++) {
        lines.push_back(get_line(l));
    }

    return lines;
}

Buffer::Line Buffer::get_line(size_t y) {
    if (!line_boundaries->has(y)) {
        line_boundaries->expand_boundary(*rope, true, y - line_boundaries->last_index());
    }
    crope r = rope->substr(line_boundaries->at(y).start_idx, line_boundaries->at(y).length);
    return {r, line_boundaries->at(y).start_idx};
}

const crope &Buffer::get_rope() {
    return *rope;
}



