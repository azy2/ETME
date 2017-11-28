#ifndef ETME_LINEBOUNDARIES_H
#define ETME_LINEBOUNDARIES_H

#include <vector>
#include <ext/rope>

using namespace std;
using __gnu_cxx::crope;

struct LineBoundary { size_t start_idx; size_t length; };

class LineBoundaries {
private:
    size_t offset;
    vector<LineBoundary> line_boundaries;

public:
    LineBoundaries(crope::const_iterator begin, crope::const_iterator end);
    const LineBoundary& at(size_t n) const;
    void remove_line(size_t idx);
    bool has(size_t idx);
    bool has_range(size_t idx, size_t n);
    void expand_boundary(const crope& r, bool back_end, size_t n);
    void lengthen(size_t idx, int amt);
    void insert(size_t pos, size_t length);
    size_t last_index();
    size_t first_index();
};

#endif //ETME_LINEBOUNDARIES_H
