#ifndef ETME_LINEBOUNDARIES_H
#define ETME_LINEBOUNDARIES_H

#include <vector>
#include <optional>
#include <ext/rope>

using namespace std;
using __gnu_cxx::crope;

class LineBoundaries {
public:
    struct LineBoundary { size_t start_idx; size_t length; };
    enum Dir {BEGINNING, END};
    LineBoundaries(size_t capacity, crope::const_iterator begin, crope::const_iterator end);
    const LineBoundary& at(size_t n) const;
    void remove_line(size_t idx);
    bool has(size_t idx);
    bool has_range(size_t idx, size_t n);
    void expand_boundary(const crope& r, Dir dir, size_t n);
    void beginning_of_buffer(const crope& r);
    void end_of_buffer(const crope& r);
    void lengthen(size_t idx, int amt);
    void insert(size_t pos, size_t length);
    void set_capacity(size_t new_capacity);
    size_t last_index();
    size_t first_index();

private:
    size_t offset;
    size_t capacity;
    optional<size_t> last_line;
    vector<LineBoundary> line_boundaries;
    void trim(Dir dir);
};

#endif //ETME_LINEBOUNDARIES_H
