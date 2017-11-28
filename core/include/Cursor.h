#ifndef CURSOR_H_
#define CURSOR_H_

#include <ext/rope>

using __gnu_cxx::size_t;

class Cursor {
public:
    Cursor(size_t s, size_t sl) : x(s), y(sl), last_x(0) {}
    size_t x;
    size_t y;
    size_t last_x;
    // TODO: Add support for regions
//    size_t end;
//    size_t end_line;
//    bool isRegion() {
//        return x != end;
//    };
};

#endif
