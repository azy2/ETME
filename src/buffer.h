#ifndef BUFFER_H_
#define BUFFER_H_

#include <ext/rope>
#include <vector>
#include "cursor.h"

using namespace __gnu_cxx;

class Buffer {
private:
    crope *rope;

    void erase(size_t i, size_t n);

public:
    Buffer();
    explicit Buffer(const char* filename);

    crope::const_iterator begin();
    size_t length();
    void backspace();
    void insert(char ch);
    char cur();
    char before();
    char next();
    void right(size_t n = 1);
    void left(size_t n = 1);

    Cursor cursor;
};

#endif
