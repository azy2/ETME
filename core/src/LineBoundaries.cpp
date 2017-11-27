#include "LineBoundaries.h"
#include <cassert>

LineBoundaries::LineBoundaries(crope::const_iterator begin, crope::const_iterator end) {
    offset = 0;
    size_t l = 0;
    while (begin != end && *begin != '\n') {
        ++l;
        ++begin;
    }
    LineBoundary lineBoundary = {0, l};
    line_boundaries.push_back(lineBoundary);
}

LineBoundary& LineBoundaries::operator[](size_t n) {
    return line_boundaries[n - offset];
}

const LineBoundary& LineBoundaries::operator[](size_t n) const {
    return line_boundaries[n - offset];
}

void LineBoundaries::remove_line(size_t idx) {
    LineBoundary lineBoundary = line_boundaries[idx];
    line_boundaries.erase(line_boundaries.begin() + (idx - offset));
    for (auto it = line_boundaries.begin() + (idx - offset);
         it != line_boundaries.end();
         ++it) {
        it->start_idx -= lineBoundary.length + 1;
    }
}

bool LineBoundaries::has(size_t idx) {
    long i = static_cast<long>(idx) - static_cast<long>(offset);
    return i >= 0 && i < static_cast<long>(line_boundaries.size());
}

bool LineBoundaries::has_range(size_t idx, size_t n) {
    return has(idx) && has(idx + n);
}

void LineBoundaries::fill_range(const crope& r, size_t start, size_t n) {
    if (start >= offset) {
        while(line_boundaries.size() < start + n) {
            LineBoundary lastBoundary = line_boundaries[line_boundaries.size() - 1];
            size_t start_idx = lastBoundary.start_idx + lastBoundary.length + 1;
            auto it = r.begin() + start_idx;
            size_t l = 0;
            while (it < r.end() && *it != '\n') {
                ++l;
                ++it;
            }
            LineBoundary newBoundary = {start_idx, l};
            line_boundaries.push_back(newBoundary);
            ++it;
        }
    } else {
        assert(0 && "fill_range case not implemented");
    }
}

void LineBoundaries::update_range(crope::const_iterator begin, crope::const_iterator end, size_t start, size_t n) {
    for (size_t i = start; i < start + n; i++) {
        size_t l = 0;
        while (begin != end && *begin != '\n') {
            ++l;
            ++begin;
        }
        LineBoundary lineBoundary = {0, l};
        line_boundaries[i - offset] = lineBoundary;
    }
}

void LineBoundaries::plus(size_t idx, size_t amt) {
    size_t i = idx - offset;
    line_boundaries[i].length += amt;
    for (i = i+1; i < line_boundaries.size(); ++i) {
        line_boundaries[i].start_idx += amt;
    }
}

size_t LineBoundaries::last_index() {
    return line_boundaries.size() + offset;
}
