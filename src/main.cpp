#include "io.h"
#include "buffer.h"
#include <ext/rope>

using namespace __gnu_cxx;

int main(int argc, char** argv) {
    Buffer *buffer;
    if (argc == 2) {
        buffer = new Buffer(argv[1]);
    } else {
        buffer = new Buffer();
    }
    IO io(buffer);
    io.io_loop();

    return 0;
}
