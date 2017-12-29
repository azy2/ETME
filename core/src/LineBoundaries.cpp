#include "LineBoundaries.h"

LineBoundaries::LineBoundaries(const crope& rope) {
    if (rope.begin() == rope.end()) {
        LineBoundary empty = {0, 0};
        push_back(empty);
    }
    size_t l = 0;
    size_t start = 0;
    for (auto it = rope.begin(), end = rope.end(); it < end; ++it) {
        if (*it == '\n') {
            LineBoundary lineBoundary = {start, l};
            push_back(lineBoundary);
            start += l + 1;
            l = 0;
        } else {
            ++l;
        }
    }
}

const LineBoundaries::LineBoundary& LineBoundaries::at(size_t n) const {
    return line_boundaries.at(n);
}

void LineBoundaries::remove_line(size_t idx) {
    LineBoundary lineBoundary = line_boundaries[idx];
    line_boundaries.erase(line_boundaries.begin() + (idx));
    for (auto it = line_boundaries.begin() + idx;
         it != line_boundaries.end();
         ++it) {
        it->start_idx -= lineBoundary.length + 1;
    }
}

void LineBoundaries::lengthen(size_t idx, int amt) {
    line_boundaries[idx].length += amt;
    for (size_t i = idx+1; i < line_boundaries.size(); ++i) {
        line_boundaries[i].start_idx += amt;
    }
}

void LineBoundaries::insert(size_t pos, size_t length) {
    LineBoundary lineBoundary = line_boundaries[pos];
    LineBoundary newBoundary = {lineBoundary.start_idx + lineBoundary.length + 1, 0};
    line_boundaries.insert(line_boundaries.begin() + pos + 1, newBoundary);
    lengthen(pos + 1, static_cast<int>(length));
    for (size_t i = pos + 2; i < line_boundaries.size(); ++i) {
        line_boundaries[i].start_idx += 1;
    }
}

size_t LineBoundaries::size() {
    return line_boundaries.size();
}

void LineBoundaries::push_back(const LineBoundaries::LineBoundary &value) {
    line_boundaries.push_back(value);
}
