/******************************************************************************\
 * This example is a very basic, non-interactive math service implemented     *
 * for both the blocking and the event-based API.                             *
\******************************************************************************/

#include "buffer.h"
#include "io.h"

int main() {
    actor_system_config cfg;
    actor_system system{cfg};
    buffer_actor buffer = system.spawn(buffer_actor_fun);
    scoped_actor self{system};

    self->request(buffer, infinite, open_file_atom::value, "temp").receive(
        [&](bool b) {
            if (b)
                io_loop(self, buffer);
        },
        [&](const error& err) {
            exit(1);
        });

    return 0;
}
