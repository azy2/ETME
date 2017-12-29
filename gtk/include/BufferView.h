#ifndef ETME_BUFFERVIEW_H
#define ETME_BUFFERVIEW_H

#include <gtkmm.h>
#include "Buffer.h"
#ifdef DEBUG
#include <ctime>
#endif

class BufferView : public Gtk::DrawingArea {
    typedef Gtk::DrawingArea super;
public:
    explicit BufferView(Buffer* buffer, class EtmeWindow* window);
    void key_press_event(GdkEventKey* event);

protected:
    void on_size_allocate(Gtk::Allocation& allocation) override;
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

private:
    Buffer* buffer;
    int widgetWidth, widgetHeight, textWidth, textHeight;
    size_t num_lines;
    size_t total_lines;
    Pango::FontDescription font;
    size_t visible_lines_offset;

    bool control(GdkEventKey* key, guint c = 0);
    bool meta(GdkEventKey *key, guint c = 0);
    bool control_meta(GdkEventKey* key, guint c = 0);
    bool shift(GdkEventKey* key, guint c = 0);

    class EtmeWindow* window;
};


#endif //ETME_BUFFERVIEW_H
