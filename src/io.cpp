#include "io.h"

IO::IO(Buffer* buffer) : buffer(buffer) {
    height = width = y = x = 0;
}

void IO::right() {
    if (buffer->cur() == '\n') {
        ++y;
        x = 0;
    } else {
        ++x;
    }
    buffer->right();
}

void IO::left() {
    if (x == 0 && y > 0) {
        --y;
        x = visible_lines[y].x;
    } else if (x > 0) {
        --x;
    }
    buffer->left();
}

void IO::up() {
    if (visible_lines[y - 1].x < x) {
        buffer->left(x + 1);
        --y;
        x = visible_lines[y].x;
    } else {
        --y;
        buffer->left(visible_lines[y].x + 1);
    }
}

void IO::down() {
    if (visible_lines[y+1].x < x) {
        buffer->right(visible_lines[y + 1].x + (visible_lines[y].x - x) + 1);
        ++y;
        x = visible_lines[y].x;
    } else {
        buffer->right(visible_lines[y].x + 1);
        ++y;
    }
}

void IO::ins(char c) {
    ++x;
    visible_lines.inc(y);
    buffer->insert(c);
}

std::optional<size_t> IO::backspace() {
    buffer->backspace();
    if (x != 0) {
        --x;
        visible_lines.dec(y);
        return {};
    } else if (x == 0 && y > 0) {
        --y;
        x = visible_lines[y].x;
        size_t it_start = visible_lines[y].i - 1;
        visible_lines.add(y, visible_lines[y+1].x);
        visible_lines.erase(y + 1);
        return std::optional<size_t>{it_start};
    }
    return {};
}
