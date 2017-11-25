#include "buffer.h"
#include <cstdlib>

using namespace std;

Buffer::Buffer() : cursor(0, 0) {
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

Buffer::Buffer(const char* filename) : cursor(58, 58) {
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
