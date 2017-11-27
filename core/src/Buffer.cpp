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
    return cursor.start - (*line_boundaries)[cursor.start_line].start_idx;
}

crope::const_iterator Buffer::begin() {
    return rope->begin();
}

size_t Buffer::length() {
    return rope->length();
}

void Buffer::backspace() {
    rope->erase(cursor.start - 1, 1);
    if (get_x() != 0) {
        --cursor.start;
    } else if (get_x() == 0 && cursor.start_line > 0) {
        --cursor.start;
        --cursor.start_line;
        size_t length_to_add = (*line_boundaries)[cursor.start_line + 1].length;
        line_boundaries->remove_line(cursor.start_line + 1);
        (*line_boundaries)[cursor.start_line].length += length_to_add;
    }
}

void Buffer::insert(char ch) {
    if (ch == '\n') {
        assert(0 && "inserting a newline not implemented");
    } else {
        rope->insert(cursor.start, ch);
        ++cursor.start;
        ++(*line_boundaries)[cursor.start_line].length;
    }
}

void Buffer::insert(string s) {
    rope->insert(cursor.start, s.c_str());
    size_t firstNl = s.find('\n');
    if (firstNl != string::npos) {
        line_boundaries->plus(cursor.start_line, firstNl);
        size_t nl = firstNl;
        while ((nl = s.find('\n', nl+1)) != string::npos) {
            line_boundaries->insert(cursor.start_line, nl - oldNL);
        }
    } else {
        line_boundaries->plus(cursor.start_line, s.size());
    }
    cursor.start += s.size();
}

char Buffer::cur() {
    return rope->at(cursor.start);
}

char Buffer::before() {
    return rope->at(cursor.start - 1);
}

char Buffer::next() {
    return rope->at(cursor.start + 1);
}

void Buffer::right(size_t n) {
    if (cursor.start < this->length()) {
        cursor.start += n;
    }
}

void Buffer::left(size_t n) {
    if (cursor.start > 0) {
        cursor.start -= n;
    }
}

crope::const_iterator Buffer::end() {
    return rope->end();
}

vector<Line> Buffer::get_lines(size_t start, size_t num) {
    if (!line_boundaries->has_range(start, num)) {
        line_boundaries->fill_range(*rope, start, num);
    }
    vector<Line> lines;
    for (size_t l = start; l < start + num; l++) {
        crope r = rope->substr((*line_boundaries)[l].start_idx, (*line_boundaries)[l].length);
        Line line = {r, (*line_boundaries)[l].start_idx};
        lines.push_back(line);
    }

    return lines;
}

const crope &Buffer::get_rope() {
    return *rope;
}


