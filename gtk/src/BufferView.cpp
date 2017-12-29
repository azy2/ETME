#include <iostream>
#include "BufferView.h"
#include "EtmeWindow.h"

BufferView::BufferView(Buffer* buffer, EtmeWindow* window) : buffer(buffer), window(window) {
    this->add_events(Gdk::KEY_PRESS_MASK);
    widgetWidth = 0;
    widgetHeight = 0;
    font.set_family("DejaVu Sans Mono");
    font.set_size(14 * PANGO_SCALE);
    auto context = get_pango_context();
    context->set_font_description(font);
    auto layout = create_pango_layout("a");
    textWidth = 0;
    textHeight = 0;
    layout->get_size(textWidth, textHeight);
    textWidth /= Pango::SCALE;
    textHeight /= Pango::SCALE;
    visible_lines_offset = 0;
    num_lines = 0;
}

void BufferView::key_press_event(GdkEventKey* event) {
#ifdef DEBUG
    window->eventStart(EtmeWindow::EventType::keypress);
    window->eventStart(EtmeWindow::EventType::total);
#endif
    if (event->keyval == GDK_KEY_Up || control(event, GDK_KEY_p)) {
        buffer->up();
    } else if (event->keyval == GDK_KEY_Down || control(event, GDK_KEY_n)) {
        buffer->down();
    } else if (event->keyval == GDK_KEY_Left || control(event, GDK_KEY_b)) {
        buffer->back();
    } else if (event->keyval == GDK_KEY_Right || control(event, GDK_KEY_f)) {
        buffer->forward();
    } else if (meta(event, GDK_KEY_b)) {
        buffer->back_word();
    } else if (meta(event, GDK_KEY_f)) {
        buffer->forward_word();
    } else if (control(event, GDK_KEY_a)) {
        buffer->beginning_of_line();
    } else if (control(event, GDK_KEY_e)) {
        buffer->end_of_line();
    } else if (meta(event, GDK_KEY_less)) {
        buffer->beginning_of_buffer();
    } else if (meta(event, GDK_KEY_greater)) {
        buffer->end_of_buffer();
    } else if (event->keyval == GDK_KEY_BackSpace) {
        buffer->backspace();
    } else if (event->keyval >= 32 && event->keyval <= 126 && ((event->state & (GDK_CONTROL_MASK | GDK_MOD1_MASK)) == 0)) {
        buffer->insert(static_cast<char>(event->keyval));
    } else if (event->keyval == GDK_KEY_Return) {
        buffer->insert('\n');
    }
    this->queue_draw();
#ifdef DEBUG
    window->eventEnd(EtmeWindow::EventType::keypress);
#endif
}

void BufferView::on_size_allocate(Gtk::Allocation& allocation) {
    super::on_size_allocate(allocation);
    widgetWidth = allocation.get_width();
    widgetHeight = allocation.get_height();

    total_lines = static_cast<size_t>(widgetHeight / textHeight) - 1;
    num_lines = min(total_lines, buffer->num_lines() - visible_lines_offset);
}

bool BufferView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
#ifdef DEBUG
    window->eventStart(EtmeWindow::EventType::render);
#endif
    cr->set_source_rgb(0 / 256.0, 43 / 256.0, 54 / 256.0);
    cr->rectangle(0, 0, widgetWidth, widgetHeight);
    cr->fill();

    cr->set_source_rgb(131 / 256.0, 148 / 256.0, 150 / 256.0);
    cr->set_antialias(Cairo::ANTIALIAS_DEFAULT);

    if (buffer->get_y() < visible_lines_offset) {
        visible_lines_offset = buffer->get_y();
    } else if (buffer->get_y() > visible_lines_offset + num_lines - 1) {
        visible_lines_offset = buffer->get_y() - num_lines + 1;
    }

    int i = 0;
    vector<Buffer::Line> line_data = buffer->get_lines(visible_lines_offset, num_lines);
    for (const auto& line : line_data) {
        Glib::ustring text(line.rope.c_str());
        auto layout = create_pango_layout(text);

        cr->move_to(0, textHeight * i);
        layout->show_in_cairo_context(cr);
        ++i;
    }

    const int cursorWidth = 1;

    cr->rectangle(buffer->get_x() * textWidth,
                  (buffer->get_y() - visible_lines_offset) * textHeight,
                  cursorWidth,
                  textHeight);
    cr->fill();

    cr->set_source_rgb(7 / 256.0, 54 / 256.0, 66 / 256.0);
    cr->rectangle(0, widgetHeight - textHeight, widgetWidth, textHeight);
    cr->fill();

    cr->set_source_rgb(147 / 256.0, 161 / 256.0, 161 / 256.0);
    Glib::ustring filename(buffer->filename);
    auto filename_layout = create_pango_layout(filename);
    Glib::ustring cursor_pos(to_string(buffer->get_y()) + ":" + to_string(buffer->get_x()));
    auto cursor_pos_layout = create_pango_layout(cursor_pos);
    cr->move_to(0, widgetHeight - textHeight);
    filename_layout->show_in_cairo_context(cr);
    int cursor_pos_width, cursor_pos_height;
    cursor_pos_layout->get_size(cursor_pos_width, cursor_pos_height);
    cr->move_to(widgetWidth - (cursor_pos_width / Pango::SCALE), widgetHeight - textHeight);
    cursor_pos_layout->show_in_cairo_context(cr);

#ifdef DEBUG
    window->eventEnd(EtmeWindow::EventType::render);
    window->eventEnd(EtmeWindow::EventType::total);
#endif

    return true;
}

bool BufferView::control(GdkEventKey *key, guint c) {
    bool ctrl = ((key->state & GDK_CONTROL_MASK) == GDK_CONTROL_MASK);
    return c != 0 ? ctrl && key->keyval == c : ctrl;
}

bool BufferView::meta(GdkEventKey *key, guint c) {
    bool meta = ((key->state & GDK_MOD1_MASK) == GDK_MOD1_MASK);
    return c != 0 ? meta && key->keyval == c : meta;
}

bool BufferView::control_meta(GdkEventKey* key, guint c) {
    bool ctrl_meta = ((key->state & (GDK_CONTROL_MASK | GDK_MOD1_MASK)) == (GDK_CONTROL_MASK | GDK_MOD1_MASK));
    return c != 0 ? ctrl_meta && key->keyval == c : ctrl_meta;
}

bool BufferView::shift(GdkEventKey *key, guint c) {
    bool shift = ((key->state & GDK_SHIFT_MASK) == GDK_SHIFT_MASK);
    return c != 0 ? shift && key->keyval == c : shift;
}
