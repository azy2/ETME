#include "io.h"
#include <ncurses.h>

#define CTRL(k) ((k) & 0x1f)

void IO::initialize_display() {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);

    getmaxyx(stdscr, height, width);

    draw_buffer();
}

void IO::draw_buffer() {
    move(0, 0);

    visible_lines.lines.erase(visible_lines.lines.begin(), visible_lines.lines.end());

    size_t tmpy = 0, tmpx = 0, i = 0, size = buffer->length();
    auto it = buffer->begin();
    while (tmpy < height && tmpx < width) {
        if (i == buffer->cursor.start) {
            y = tmpy;
            x = tmpx;
        }
        if (i >= size) {
            break;
        }
        if (*it == '\n') {
            visible_lines.emplace_back(tmpx, i);
            addch('~' | A_DIM);
            tmpx = 0;
            ++tmpy;
            move(tmpy, tmpx);
        } else if (tmpx < width) {
            char w = *it;
            addch(w);
            ++tmpx;
        }
        ++it;
        ++i;
    }
    while (tmpy < height) {
        mvaddch(tmpy++, 0, '~');
    }

    move(y, x);

    refresh();
}

void IO::io_loop() {
    initialize_display();

    int c;
    while ((c = getch()) != 0) {
        if (c == CTRL('c')) {
            break;
        } else if (c == KEY_F(5)) {
            for (size_t tmpy = 0; tmpy < height; ++tmpy) {
                for (size_t tmpx = 0; tmpx < width; ++tmpx) {
                    mvaddch(tmpy, tmpx, ' ');
                }
            }
            draw_buffer();
        } else if (c == KEY_BACKSPACE) {
            if (x != 0) {
                mvdelch(y, --x);
                refresh();
                visible_lines.dec(y);
            } else if (x == 0 && y > 0) {
                deleteln();
                --y;
                x = visible_lines[y].x;
                size_t it_start = visible_lines[y].i;
                visible_lines.add(y, visible_lines[y+1].x);
                visible_lines.erase(y + 1);
                move(y,x);
                size_t tmpx = x - 1;
                auto it = buffer->begin() + it_start;
                for (size_t i = it_start;
                     i <= visible_lines[y].i &&
                     i < buffer->length(); i++) {
                    addch(*it);
                    move(y, ++tmpx);
                    ++it;
                }
                addch(A_DIM | '~');
                move(y,x);
                refresh();
            }
            buffer->backspace();
        } else if (c == KEY_RIGHT) {
            if (buffer->cur() == '\n') {
                move(++y, x = 0);
                refresh();
            } else {
                move(y, ++x);
                refresh();
            }
            buffer->right();
        } else if (c == KEY_LEFT) {
            if (x == 0 && y > 0) {
                --y;
                move(y, x = visible_lines[y].x);
                refresh();
            } else if (x > 0) {
                move(y, --x);
                refresh();
            }
            buffer->left();
        } else if (c == KEY_UP && y > 0) {
            if (visible_lines[y-1].x < x) {
                size_t move_amt = x + 1;
                --y;
                x = visible_lines[y].x;
                move(y, x);
                refresh();
                buffer->left(move_amt);
            } else {
                move(--y, x);
                refresh();
                buffer->left(visible_lines[y].x + 1);
            }
        } else if (c == KEY_DOWN && y < visible_lines.length()) {
            if (visible_lines[y+1].x < x) {
                size_t move_amt = visible_lines[y+1].x + (visible_lines[y].x - x) + 1;
                ++y;
                x = visible_lines[y].x;
                move(y, x);
                refresh();
                buffer->right(move_amt);
            } else {
                move(++y, x);
                refresh();
                buffer->right(visible_lines[y-1].x + 1);
            }
        } else if (c > 26 && c < 127) {
            insch(c);
            move(y, ++x);
            refresh();
            visible_lines.inc(y);
            buffer->insert(c);
        }
    }

    endwin();
}
