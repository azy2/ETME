#include "BufferView.h"

BufferView::BufferView(Buffer* buffer) : buffer(buffer) {
    this->add_events(Gdk::KEY_PRESS_MASK);
    widgetWidth = 0;
    widgetHeight = 0;
    font.set_family("DejaVu Sans Mono");
    font.set_size(16 * PANGO_SCALE);
    auto context = get_pango_context();
    context->set_font_description(font);
    auto layout = create_pango_layout("a");
    textWidth = 0;
    textHeight = 0;
    layout->get_size(textWidth, textHeight);
    textWidth /= Pango::SCALE;
    textHeight /= Pango::SCALE;
    visible_lines_offset = 0;
}

void BufferView::key_press_event(GdkEventKey* event) {
    if (event->keyval == GDK_KEY_Up) {
        up(static_cast<char>(event->keyval));
    } else if (event->keyval == GDK_KEY_Down) {
        down(static_cast<char>(event->keyval));
    } else if (event->keyval == GDK_KEY_Left) {
        left(static_cast<char>(event->keyval));
    } else if (event->keyval == GDK_KEY_Right) {
        right(static_cast<char>(event->keyval));
    } else if (event->keyval == GDK_KEY_BackSpace) {
        backspace(static_cast<char>(event->keyval));
    } else if (event->keyval >= 32 && event->keyval <= 126 && ((event->state & (GDK_CONTROL_MASK | GDK_MOD1_MASK)) == 0)) {
        ins(static_cast<char>(event->keyval));
    } else if (event->keyval == GDK_KEY_Return) {
        ins('\n');
    }
    this->queue_draw();
}

void BufferView::on_size_allocate(Gtk::Allocation& allocation) {
    super::on_size_allocate(allocation);
    widgetWidth = allocation.get_width();
    widgetHeight = allocation.get_height();

    size_t height = static_cast<size_t>(widgetHeight / textHeight);
    lines.erase(lines.begin(), lines.end());

    vector<Buffer::Line> line_data = buffer->get_lines(visible_lines_offset, height + visible_lines_offset + 1);
    for (const auto& line : line_data) {
        Glib::ustring text(line.rope.c_str());
        lines.push_back(create_pango_layout(text));
    }
}

bool BufferView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    cr->set_source_rgb(0 / 256.0, 43 / 256.0, 54 / 256.0);
    cr->rectangle(0, 0, widgetWidth, widgetHeight);
    cr->fill();

    cr->set_source_rgb(131 / 256.0, 148 / 256.0, 150 / 256.0);
    cr->set_antialias(Cairo::ANTIALIAS_DEFAULT);

    int i = 0;
    for (const auto& line : lines) {
        cr->move_to(0, textHeight * i);
        line->show_in_cairo_context(cr);
        ++i;
    }

    const int cursorWidth = 1;

    cr->rectangle(buffer->get_x() * textWidth,
                  (buffer->get_y() - visible_lines_offset) * textHeight,
                  cursorWidth,
                  textHeight);
    cr->fill();

    return true;
}

void BufferView::up(char c) {
    (void)c;
    buffer->up();
    if (buffer->get_y() < visible_lines_offset) {
        visible_lines_offset--;
        lines.erase(lines.end() - 1);
        Buffer::Line line = buffer->get_line(buffer->get_y());
        Glib::ustring text(line.rope.c_str());
        auto layout = create_pango_layout(text);
        lines.insert(lines.begin(), layout);
    }
}

void BufferView::down(char c) {
    (void)c;
    buffer->down();
    if (buffer->get_y() > static_cast<size_t>(widgetHeight / textHeight) + visible_lines_offset) {
        visible_lines_offset++;
        lines.erase(lines.begin());
        Buffer::Line line = buffer->get_line(buffer->get_y());
        Glib::ustring text(line.rope.c_str());
        auto layout = create_pango_layout(text);
        lines.push_back(layout);
    }
}

void BufferView::right(char c) {
    (void)c;
    buffer->right();
}

void BufferView::left(char c) {
    (void)c;
    buffer->left();
}

void BufferView::ins(char c) {
    if (c != '\n') {
        buffer->insert(c);
        Buffer::Line line_data = buffer->get_line(buffer->get_y());
        Glib::ustring text(line_data.rope.c_str());
        lines[buffer->get_y()]->set_text(text);
    } else {
        buffer->insert('\n');
        Buffer::Line line_data = buffer->get_line(buffer->get_y());
        Glib::ustring text(line_data.rope.c_str());
        auto layout = create_pango_layout(text);
        lines.insert(lines.begin() + buffer->get_y(), layout);
        lines.pop_back();
        Buffer::Line line_data_2 = buffer->get_line(buffer->get_y() - 1);
        Glib::ustring text_2(line_data_2.rope.c_str());
        lines[buffer->get_y() - 1]->set_text(text_2);
    }
}

void BufferView::backspace(char c) {
    (void)c;
    if (buffer->backspace()) {
        lines.erase(lines.begin() + buffer->get_y() + 1);
        Buffer::Line newLine = buffer->get_line(lines.size());
        Glib::ustring text(newLine.rope.c_str());
        auto layout = create_pango_layout(text);
        lines.push_back(layout);
    }
    Buffer::Line line_data = buffer->get_line(buffer->get_y());
    Glib::ustring text(line_data.rope.c_str());
    lines[buffer->get_y()]->set_text(text);
}