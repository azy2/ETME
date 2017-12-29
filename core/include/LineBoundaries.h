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
    explicit LineBoundaries(const crope& r);
    const LineBoundary& at(size_t n) const;
    void remove_line(size_t idx);
    void lengthen(size_t idx, int amt);
    void insert(size_t pos, size_t length);
    size_t size();

private:
    vector<LineBoundary> line_boundaries;

    void push_back(const LineBoundary& value);
};

#endif //ETME_LINEBOUNDARIES_H
