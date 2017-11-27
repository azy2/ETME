#ifndef BUFFER_H_
#define BUFFER_H_

#include <ext/rope>
#include <vector>
#include <map>
#include "Cursor.h"
#include "LineBoundaries.h"

using namespace __gnu_cxx;
using namespace std;

struct Line { crope rope; size_t start_idx; };

class Buffer {
private:
    crope *rope;

    LineBoundaries* line_boundaries;

public:
    Buffer();
    explicit Buffer(const char* filename);
    ~Buffer();

    size_t get_x();
    const crope& get_rope();
    crope::const_iterator begin();
    crope::const_iterator end();
    size_t length();
    void backspace();
    void insert(char ch);
    void insert(string s);
    char cur();
    char before();
    char next();
    void right(size_t n = 1);
    void left(size_t n = 1);

    vector<Line> get_lines(size_t start, size_t num);

    Cursor cursor;
    const char* filename;
};

#endif
