#include "io.h"
#include <ncurses.h>
#include <ext/rope>

#define CTRL(k) ((k) & 0x1f)

void io_loop(caf::scoped_actor& self, buffer_actor& buffer) {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);

    int height, width;
    getmaxyx(stdscr, height, width);

    move(0, 0);

    wchar_t *buf = (wchar_t*)malloc(width * height * sizeof(wchar_t));
    self->request(buffer, infinite, copy_atom::value, (size_t)0, (size_t)(width * height), buf).receive(
        [&](bool b) {
            int y = 0, x = 0;
            int i = 0;
            while (y < height && x < width) {
                // if (it[i] == '\n') {
                //     printw("\n\r");
                //     x = 0;
                //     ++y;
                // } else if (x < width) {
                //     wchar_t w = it[i];
                //     addch(w);
                //     x++;
                // }
                ++i;
            }
        },
        [&](const error& err) {
        });

    move(0, 0);

    int c;
    size_t cursor = 1;
    while ((c = getch()) != 0) {
        if (c == CTRL('c')) {
            break;
        } else if (c == KEY_BACKSPACE) {
            cursor--;
            mvaddch(0, cursor-1, ' ');
            move(0, cursor-1);
            self->request(buffer, infinite, erase_atom::value, cursor, (size_t)1);
        } else if (c > 26) {
            addch(c);
            int y,x;
            getyx(stdscr,y,x);
            mvprintw(height-1, 0, "%d", c);
            clrtoeol();
            move(y,x);

            self->request(buffer, infinite, insert_atom::value, cursor++, (wchar_t)c);
        }

        refresh();
    }

    endwin();
}
