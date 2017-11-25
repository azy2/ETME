#include <ncurses.h>
#include <time.h>
#include "../src/io.h"
#include <optional>

#define CTRL(k) ((k) & 0x1f)

#define UPDATE_CUR(y, x) move(static_cast<int>(y),static_cast<int>(x))

struct timespec timer_start(){
    struct timespec start_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
    return start_time;
}

// call this function to end a timer, returning nanoseconds elapsed as a long
long timer_end(struct timespec start_time){
    struct timespec end_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
    long diffInNanos = end_time.tv_nsec - start_time.tv_nsec;
    return diffInNanos;
}

void initialize_display(IO& io) {
    initscr();
    raw();
    noecho();
    keypad(stdscr, true);

    getmaxyx(stdscr, io.height, io.width);
}

void draw_buffer(IO& io) {
    UPDATE_CUR(0, 0);

    io.visible_lines.lines.erase(io.visible_lines.lines.begin(), io.visible_lines.lines.end());

    size_t tmpy = 0, tmpx = 0, i = 0, size = io.buffer->length();
    auto it = io.buffer->begin();
    while (tmpy < io.height && tmpx < io.width) {
        if (i == io.buffer->cursor.start) {
            io.y = tmpy;
            io.x = tmpx;
        }
        if (i >= size) {
            break;
        }
        if (*it == '\n') {
            io.visible_lines.emplace_back(tmpx, i);
            addch('~' | A_DIM);
            tmpx = 0;
            ++tmpy;
            UPDATE_CUR(tmpy, tmpx);
        } else if (tmpx < io.width) {
            char w = *it;
            addch(w);
            ++tmpx;
        }
        ++it;
        ++i;
    }
    while (tmpy < io.height) {
        mvaddch(tmpy++, 0, '~');
    }

    UPDATE_CUR(io.y, io.x);

    refresh();
}

void ncurses_loop(IO& io) {
    int c;
    while ((c = getch()) != 0) {
        struct timespec start = timer_start();
        if (c == CTRL('c')) {
            break;
        } else if (c == KEY_F(5)) {
            for (size_t tmpy = 0; tmpy < io.height; ++tmpy) {
                for (size_t tmpx = 0; tmpx < io.width; ++tmpx) {
                    mvaddch(tmpy, tmpx, ' ');
                }
            }
            draw_buffer(io);
        } else if (c == KEY_BACKSPACE) {
            std::optional<size_t> it_start = io.backspace();
            if (it_start) {
                deleteln();
                UPDATE_CUR(io.y, io.x);
                size_t tmpx = io.x - 1;
                auto it = io.buffer->begin() + it_start.value();
                for (size_t i = it_start.value();
                        i <= io.visible_lines[io.y].i &&
                        i < io.buffer->length(); ++i) {
                    addch(*it);
                    UPDATE_CUR(io.y, ++tmpx);
                    ++it;
                }
                addch(A_DIM | '~');
                UPDATE_CUR(io.y, io.x);
                refresh();
            } else {
                mvdelch(io.y, io.x);
                refresh();
            }
        } else if (c == KEY_RIGHT) {
            io.right();
            UPDATE_CUR(io.y, io.x);
            refresh();
        } else if (c == KEY_LEFT) {
            io.left();
            UPDATE_CUR(io.y, io.x);
            refresh();
        } else if (c == KEY_UP && io.y > 0) {
            io.up();
            UPDATE_CUR(io.y, io.x);
            refresh();
        } else if (c == KEY_DOWN && io.y < io.visible_lines.length()) {
            io.down();
            UPDATE_CUR(io.y, io.x);
            refresh();
        } else if (c > 26 && c < 127) {
            insch((chtype)c);
            refresh();
            io.ins((char)c);
        }
        long end = timer_end(start);
        mvprintw(static_cast<int>(io.height - 1), 0, "%f", (float)end / (1000000.0));
        clrtoeol();
        UPDATE_CUR(io.y, io.x);
        refresh();
    }

    endwin();
}


int main(int argc, char** argv) {
    Buffer* buffer;
    if (argc == 2) {
        buffer = new Buffer(argv[1]);
    } else {
        buffer = new Buffer();
    }

    IO io(buffer);

    initialize_display(io);
    draw_buffer(io);
    ncurses_loop(io);

    delete buffer;
    return 0;
}
