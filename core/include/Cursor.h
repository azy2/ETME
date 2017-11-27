#ifndef CURSOR_H_
#define CURSOR_H_

#include <ext/rope>

using __gnu_cxx::size_t;

class Cursor {
public:
    Cursor(size_t s, size_t sl) : start(s), start_line(sl) {}
    size_t start;
    size_t start_line;
    // TODO: Add support for regions
//    size_t end;
//    size_t end_line;
//    bool isRegion() {
//        return start != end;
//    };
};

#endif
