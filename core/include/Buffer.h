#ifndef BUFFER_H_
#define BUFFER_H_

#include <ext/rope>
#include <vector>
#include <map>
#include "Cursor.h"

using namespace __gnu_cxx;
using namespace std;

struct line_boundary { size_t start_idx; size_t length; };
struct line { crope rope; size_t start_idx; };

class Buffer {
private:
    crope *rope;

    void erase(size_t i, size_t n);
    // A sorted map between line number and (start idx, length) of line
    map<size_t, line_boundary> line_boundaries;

    void fill_line_boundaries(size_t start, size_t num);

public:
    Buffer();
    explicit Buffer(const char* filename);

    const crope& get_rope();
    crope::const_iterator begin();
    crope::const_iterator end();
    size_t length();
    void backspace();
    void insert(char ch);
    void insert(const char* s);
    char cur();
    char before();
    char next();
    void right(size_t n = 1);
    void left(size_t n = 1);

    vector<line> get_lines(size_t start, size_t num);

    Cursor cursor;
    const char* filename;
};

#endif
