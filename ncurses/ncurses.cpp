#include <ncurses.h>
#include <time.h>
#include "../src/BufferView.h"
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

void initialize_display(BufferView& bufferView) {
    initscr();
    raw();
    noecho();
    keypad(stdscr, true);

    getmaxyx(stdscr, bufferView.height, bufferView.width);
}

void draw_buffer(BufferView& bufferView) {
    UPDATE_CUR(0, 0);

    bufferView.visible_lines.lines.erase(bufferView.visible_lines.lines.begin(), bufferView.visible_lines.lines.end());

    size_t tmpy = 0, tmpx = 0, i = 0;
    auto it = bufferView.buffer->begin();
    while (tmpy < bufferView.height && tmpx < bufferView.width && it != bufferView.buffer->end()) {
        if (i == bufferView.buffer->cursor.start) {
            bufferView.y = tmpy;
            bufferView.x = tmpx;
        }
        if (*it == '\n') {
            bufferView.visible_lines.emplace_back(tmpx, i);
            addch('~' | A_DIM);
            tmpx = 0;
            ++tmpy;
            UPDATE_CUR(tmpy, tmpx);
        } else if (tmpx < bufferView.width) {
            char w = *it;
            addch(w);
            ++tmpx;
        }
        ++it;
        ++i;
    }
    while (tmpy < bufferView.height) {
        mvaddch(tmpy++, 0, '~');
    }

    UPDATE_CUR(bufferView.y, bufferView.x);

    refresh();
}

void ncurses_loop(BufferView& bufferView) {
    int c;
    while ((c = getch()) != 0) {
        struct timespec start = timer_start();
        if (c == CTRL('c')) {
            break;
        } else if (c == KEY_F(5)) {
            for (size_t tmpy = 0; tmpy < bufferView.height; ++tmpy) {
                for (size_t tmpx = 0; tmpx < bufferView.width; ++tmpx) {
                    mvaddch(tmpy, tmpx, ' ');
                }
            }
            draw_buffer(bufferView);
        } else if (c == KEY_BACKSPACE) {
            std::optional<size_t> it_start = bufferView.backspace();
            if (it_start) {
                deleteln();
                UPDATE_CUR(bufferView.y, bufferView.x);
                size_t tmpx = bufferView.x - 1;
                auto it = bufferView.buffer->begin() + it_start.value();
                for (size_t i = it_start.value();
                        i <= bufferView.visible_lines[bufferView.y].i &&
                        i < bufferView.buffer->length(); ++i) {
                    addch(*it);
                    UPDATE_CUR(bufferView.y, ++tmpx);
                    ++it;
                }
                addch(A_DIM | '~');
                UPDATE_CUR(bufferView.y, bufferView.x);
                refresh();
            } else {
                mvdelch(bufferView.y, bufferView.x);
                refresh();
            }
        } else if (c == KEY_RIGHT) {
            bufferView.right();
            UPDATE_CUR(bufferView.y, bufferView.x);
            refresh();
        } else if (c == KEY_LEFT) {
            bufferView.left();
            UPDATE_CUR(bufferView.y, bufferView.x);
            refresh();
        } else if (c == KEY_UP && bufferView.y > 0) {
            bufferView.up();
            UPDATE_CUR(bufferView.y, bufferView.x);
            refresh();
        } else if (c == KEY_DOWN && bufferView.y < bufferView.visible_lines.length()) {
            bufferView.down();
            UPDATE_CUR(bufferView.y, bufferView.x);
            refresh();
        } else if (c > 26 && c < 127) {
            insch((chtype)c);
            refresh();
            bufferView.ins((char)c);
        }
        long end = timer_end(start);
        mvprintw(static_cast<int>(bufferView.height - 1), 0, "ms: %f\t\tkey: %s\t\tfile: %s", (float)end / (1000000.0), keyname(c), bufferView.buffer->filename);
        clrtoeol();
        UPDATE_CUR(bufferView.y, bufferView.x);
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

    BufferView bufferView(buffer);

    initialize_display(bufferView);
    draw_buffer(bufferView);
    ncurses_loop(bufferView);

    delete buffer;
    return 0;
}
