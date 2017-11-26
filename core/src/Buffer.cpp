#include "Buffer.h"
#include "string.h"

Buffer::Buffer() : cursor(0, 0), filename("*scratch*") {
    rope = new crope();
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
}

crope::const_iterator Buffer::begin() {
    return rope->begin();
}

size_t Buffer::length() {
    return rope->length();
}

void Buffer::erase(size_t i, size_t n) {
    rope->erase(i, n);
}

void Buffer::backspace() {
    if (cursor.isRegion()) {
        erase(min(cursor.start, cursor.end), static_cast<size_t>(abs((int)cursor.start - (int)cursor.end)));
        cursor.start = cursor.end = min(cursor.start, cursor.end);
    } else if (cursor.start > 0) {
        erase(cursor.start - 1, 1);
        --cursor.start;
        --cursor.end;
    }
}

void Buffer::insert(char ch) {
    if (cursor.isRegion()) {
        erase(min(cursor.start, cursor.end), static_cast<size_t>(abs((int)cursor.start - (int)cursor.end)));
        cursor.start = cursor.end = min(cursor.start, cursor.end);
    }
    rope->insert(cursor.start, ch);
    ++cursor.start;
    ++cursor.end;
}

void Buffer::insert(const char *s) {
    if (cursor.isRegion()) {
        erase(min(cursor.start, cursor.end), static_cast<size_t>(abs((int)cursor.start - (int)cursor.end)));
        cursor.start = cursor.end = min(cursor.start, cursor.end);
    }

    rope->insert(cursor.start, s);
    size_t len = strlen(s);
    cursor.start += len;
    cursor.end += len;
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
    if (cursor.isRegion()) {
        if (cursor.end < this->length()) {
            cursor.end += n;
        }
    } else {
        if (cursor.start < this->length()) {
            cursor.start += n;
            cursor.end += n;
        }
    }
}

void Buffer::left(size_t n) {
    if (cursor.isRegion()) {
        if (cursor.end > 0) {
            cursor.end -= n;
        }
    } else {
        if (cursor.start > 0) {
            cursor.start -= n;
            cursor.end -= n;
        }
    }
}

crope::const_iterator Buffer::end() {
    return rope->end();
}

void Buffer::fill_line_boundaries(size_t start, size_t num) {
    size_t idx;
    if (line_boundaries.count(start) == 0) {
        idx = 0;
    } else {
        idx = line_boundaries[start].start_idx + line_boundaries[start].length + 1;
    }

    auto it = rope->begin() + idx;
    for (size_t l = start; l < start + num; ++l) {
        if (line_boundaries.count(l) == 1) {
            it += line_boundaries[l].length;
            idx += line_boundaries[l].length;
            continue;
        }
        size_t start_idx = idx;
        while (it != rope->end() && *it != '\n') {
            ++it;
            ++idx;
        }
        line_boundaries[l] = {start_idx, idx - start_idx};
        ++it;
        ++idx;
    }
}

vector<line> Buffer::get_lines(size_t start, size_t num) {
    if (line_boundaries.count(start) == 0) {
        auto less = line_boundaries.lower_bound(start);
        if (less == line_boundaries.cbegin()) {
            fill_line_boundaries(0, start + num);
        } else {
            fill_line_boundaries(less->second.start_idx, start + num - less->second.start_idx);
        }
    } else {
        fill_line_boundaries(start, num);
    }
    vector<line> lines;
    for (size_t l = start; l < start + num; l++) {
        crope r = rope->substr(line_boundaries[l].start_idx, line_boundaries[l].length);
        lines.push_back({r, line_boundaries[l].start_idx});
    }

    return lines;
}

const crope &Buffer::get_rope() {
    return *rope;
}

