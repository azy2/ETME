#ifndef IO_H_
#define IO_H_

#include <ext/rope>
#include <vector>
#include <utility>
#include "Buffer.h"
#include "Optional.h"

class LinePos {
public:
    LinePos(size_t x, size_t i) : x(x), i(i) {}
    size_t x;
    size_t i;
    LinePos& operator++() {
        ++x;
        ++i;
        return *this;
    }
    LinePos& operator--() {
        --x;
        --i;
        return *this;
    }
    LinePos operator++(int) {
        LinePos tmp(*this);
        operator++();
        return tmp;
    }
    LinePos operator--(int) {
        LinePos tmp(*this);
        operator--();
        return tmp;
    }
};

class LinePositions {
public:
    std::vector<LinePos> lines;

    size_t length() {
        return lines.size();
    }

    LinePos& operator[](size_t idx) {
        return lines[idx];
    }

    const LinePos& operator[](size_t idx) const {
        return lines[idx];
    }

    void inc(size_t idx) {
        auto s = lines.begin() + idx;
        (*s)++;
        for (auto l = s + 1; l != lines.end(); ++l) {
            l->i++;
        }
    }
    void dec(size_t idx) {
        auto s = lines.begin() + idx;
        (*s)--;
        for (auto l = s + 1; l != lines.end(); ++l) {
            l->i--;
        }
    }

    void add(size_t idx, size_t n) {
        auto s = lines.begin() + idx;
        s->i += n;
        s->x += n;
        for (auto l = s + 1; s != lines.end(); ++s) {
            l->i += n;
        }
    }

    void erase(size_t idx) {
        size_t x = lines[idx].x;
        lines.erase(lines.begin() + idx);
        for (auto l = lines.begin() + idx; l != lines.end(); ++l) {
            l->i -= x;
        }
    }

    void emplace_back(size_t x, size_t i) {
        lines.emplace_back(x, i);
    }
};



class BufferView {
public:
    explicit BufferView(Buffer* buffer);
    void right();
    void left();
    void up();
    void down();
    void ins(char c);
    Optional<size_t> backspace();

    LinePositions visible_lines;

    Buffer* buffer;
    size_t height, width;
    size_t y,x;
};

#endif
