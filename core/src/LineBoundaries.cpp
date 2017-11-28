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

const LineBoundary& LineBoundaries::at(size_t n) const {
    return line_boundaries[n - offset];
}

void LineBoundaries::remove_line(size_t idx) {
    LineBoundary lineBoundary = line_boundaries[idx - offset];
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

void LineBoundaries::expand_boundary(const crope &r, bool back_end, size_t n) {
    if (back_end) {
        for (size_t i = 0; i < n; i++) {
            LineBoundary lastBoundary = line_boundaries[line_boundaries.size() - 1];
            size_t start_idx = lastBoundary.start_idx + lastBoundary.length + 1;
            auto it = r.begin() + start_idx;
            size_t l = 0;
            while(it < r.end() && *it != '\n') {
                ++l;
                ++it;
            }
            this->insert(line_boundaries.size() - 1, l);
        }
    } else {
        assert(0 && "expanding down not implemented");
    }
}

void LineBoundaries::lengthen(size_t idx, int amt) {
    size_t i = idx - offset;
    line_boundaries[i].length += amt;
    for (i = i+1; i < line_boundaries.size(); ++i) {
        line_boundaries[i].start_idx += amt;
    }
}

void LineBoundaries::insert(size_t pos, size_t length) {
    LineBoundary lineBoundary = line_boundaries[pos - offset];
    LineBoundary newBoundary = {lineBoundary.start_idx + lineBoundary.length + 1, 0};
    line_boundaries.insert(line_boundaries.begin() + pos - offset + 1, newBoundary);
    lengthen(pos + 1, length);
    for (size_t i = pos + 2; i < line_boundaries.size(); ++i) {
        line_boundaries[i].start_idx += 1;
    }

}

size_t LineBoundaries::last_index() {
    return line_boundaries.size() + offset - 1;
}

size_t LineBoundaries::first_index() {
    return offset;
}

