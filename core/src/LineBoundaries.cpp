#include "LineBoundaries.h"

LineBoundaries::LineBoundaries(size_t capacity, crope::const_iterator begin, crope::const_iterator end)
        : offset(0), capacity(capacity) {
    size_t l = 0;
    while (begin != end && *begin != '\n') {
        ++l;
        ++begin;
    }
    LineBoundary lineBoundary = {0, l};
    line_boundaries.push_back(lineBoundary);
}

const LineBoundaries::LineBoundary& LineBoundaries::at(size_t n) const {
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

void LineBoundaries::expand_boundary(const crope &r, Dir dir, size_t n) {
    if (dir == END) {
        for (size_t i = 0; i < n; i++) {
            LineBoundaries::LineBoundary lastBoundary = line_boundaries[line_boundaries.size() - 1];
            size_t start_idx = lastBoundary.start_idx + lastBoundary.length + 1;
            auto it = r.begin() + start_idx;
            size_t l = 0;
            while(it < r.end() && *it != '\n') {
                ++l;
                ++it;
            }
            LineBoundaries::LineBoundary newBoundary = {start_idx, l};
            line_boundaries.push_back(newBoundary);
        }
        trim(BEGINNING);
    } else {
        for (size_t i = 0; i < n; i++) {
            LineBoundaries::LineBoundary lastBoundary = line_boundaries[0];
            size_t end_idx = lastBoundary.start_idx - 2;
            auto it = r.begin() + end_idx;
            size_t l = 0;
            while (it > r.begin() && *it != '\n') {
                ++l;
                --it;
            }
            if (it == r.begin()) {
                ++l;
            }
            LineBoundaries::LineBoundary newBoundary = {end_idx - l + 1, l};
            line_boundaries.insert(line_boundaries.begin(), newBoundary);
            --offset;
        }
        trim(END);
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
    lengthen(pos + 1, static_cast<int>(length));
    for (size_t i = pos + 2; i < line_boundaries.size(); ++i) {
        line_boundaries[i].start_idx += 1;
    }
    // TODO: Maybe I should be trimming the end sometimes?
    trim(BEGINNING);
}

size_t LineBoundaries::last_index() {
    return line_boundaries.size() + offset - 1;
}

size_t LineBoundaries::first_index() {
    return offset;
}

void LineBoundaries::beginning_of_buffer(const crope &r) {
    if (offset == 0) {
        return;
    } else if (offset < capacity) {
        expand_boundary(r, BEGINNING, offset);
    } else {
        offset = 0;
        auto it = r.begin();
        size_t l = 0;
        while(it < r.end() && *it != '\n') {
            ++l;
            ++it;
        }
        line_boundaries[0].length = l;
        line_boundaries[0].start_idx = 0;
        for (size_t i = 1; i < capacity; i++) {
            LineBoundary lastBoundary = line_boundaries[i - 1];
            size_t start_idx = lastBoundary.start_idx + lastBoundary.length + 1;
            it = r.begin() + start_idx;
            l = 0;
            while(it < r.end() && *it != '\n') {
                ++l;
                ++it;
            }
            line_boundaries[i].length = l;
            line_boundaries[i].start_idx = start_idx;
        }
    }
}

void LineBoundaries::end_of_buffer(const crope &r) {
    (void)r;
    if (last_line && offset == last_line.value() - capacity) {
        return;
    } else {

    }
}

void LineBoundaries::set_capacity(size_t new_capacity) {
    capacity = new_capacity;
}

void LineBoundaries::trim(Dir dir) {
    if (line_boundaries.size() > capacity) {
        size_t amt = line_boundaries.size() - capacity;
        if (dir == BEGINNING) {
            offset += amt;
            line_boundaries.erase(line_boundaries.begin(), line_boundaries.begin() + amt);
        } else {
            line_boundaries.erase(line_boundaries.end() - amt, line_boundaries.end());
        }

    }
}
