#ifndef CURSOR_H_
#define CURSOR_H_

#include <ext/rope>

using __gnu_cxx::size_t;

class Cursor {
public:
    Cursor(size_t s, size_t e) : start(s), end(e) {}
    size_t start;
    size_t end;
    bool isRegion() {
        return start != end;
    };
};

#endif
