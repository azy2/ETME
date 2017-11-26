#include <gtkmm.h>
#include "../src/BufferView.h"

using namespace std;

class TextArea : public Gtk::DrawingArea {
    typedef Gtk::DrawingArea super;
public:
    explicit TextArea(BufferView* bufferView) : bufferView(bufferView) {
        this->add_events(Gdk::KEY_PRESS_MASK);
        widgetWidth = 0;
        widgetHeight = 0;
        font.set_family("DejaVu Sans Mono");
        font.set_size(16 * PANGO_SCALE);
        auto layout = create_pango_layout("a");
        layout->set_font_description(font);
        textWidth = 0;
        textHeight = 0;
        layout->get_size(textWidth, textHeight);
        textWidth /= Pango::SCALE;
        textHeight /= Pango::SCALE;
    }

    void key_press_event(GdkEventKey* event) {
        if (event->keyval == GDK_KEY_Up) {
            bufferView->up();
        } else if (event->keyval == GDK_KEY_Down) {
            bufferView->down();
        } else if (event->keyval == GDK_KEY_Left) {
            bufferView->left();
        } else if (event->keyval == GDK_KEY_Right) {
            bufferView->right();
        } else if (event->keyval == GDK_KEY_BackSpace) {
            Optional<size_t> it_start = bufferView->backspace();
            if (it_start) {
                Glib::ustring deletedLineText = lines[bufferView->y+1]->get_text();
                lines.erase(lines.begin() + bufferView->y+1);
                Glib::ustring curLineText = lines[bufferView->y]->get_text();
                curLineText.append(deletedLineText);
                lines[bufferView->y]->set_text(curLineText);
            } else {
                Glib::ustring text = lines[bufferView->y]->get_text();
                text.erase(bufferView->x, 1);
                lines[bufferView->y]->set_text(text);
            }
        } else if (event->keyval >= 32 && event->keyval <= 126 && ((event->state & (GDK_CONTROL_MASK | GDK_MOD1_MASK)) == 0)) {
            Glib::ustring text = lines[bufferView->y]->get_text();
            text.insert(bufferView->x, 1, event->keyval);
            lines[bufferView->y]->set_text(text);
            bufferView->ins(static_cast<char>(event->keyval));
        }
        this->queue_draw();
    }
protected:

    void on_size_allocate(Gtk::Allocation& allocation) override {
        super::on_size_allocate(allocation);
        widgetWidth = allocation.get_width();
        widgetHeight = allocation.get_height();

        bufferView->height = static_cast<size_t>(widgetHeight / textHeight);
        bufferView->width = static_cast<size_t>(widgetWidth / textWidth);
        lines.erase(lines.begin(), lines.end());

        auto it = bufferView->buffer->begin();
        auto end = bufferView->buffer->end();
        size_t x = 0, i = 0;
        for (size_t y = 0; y < bufferView->height && it != end; ++y) {
            Glib::ustring lineText;
            while (it != end && *it != '\n') {
                if (i == bufferView->buffer->cursor.start) {
                    bufferView->y = y;
                    bufferView->x = x;
                }
                lineText.push_back(*it);
                ++x;
                ++it;
                ++i;
            }
            bufferView->visible_lines.emplace_back(x, i);
            lines.push_back(create_pango_layout(lineText));
            ++it;
            lines[y]->set_font_description(font);
            x = 0;
        }
    }

    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override {
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

        cr->rectangle(bufferView->x * textWidth,
                      bufferView->y * textHeight,
                      cursorWidth,
                      textHeight);
        cr->fill();

        return true;
    }

private:
    BufferView* bufferView;
    int widgetWidth, widgetHeight, textWidth{}, textHeight{};
    Pango::FontDescription font;
    vector<Glib::RefPtr<Pango::Layout>> lines;
};

class EtmeWindow : public Gtk::Window {
public:
    EtmeWindow() : Gtk::Window::Window() {
        set_default_size(1920, 2000);
        set_title("ETME");
        add_events(Gdk::KEY_PRESS_MASK);


        buffer = new Buffer("test");
        bufferView = new BufferView(buffer);

        area = new TextArea(bufferView);
        add(*area);
        area->show();
    }
private:
    bool on_key_press_event(GdkEventKey* event) override {
        if (event->keyval == GDK_KEY_c && (event->state & GDK_CONTROL_MASK) == GDK_CONTROL_MASK) {
            this->close();
        }
        area->key_press_event(event);
        return false;
    }

    TextArea* area;
    Buffer* buffer;
    BufferView* bufferView;
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.azy2.etme");

    EtmeWindow window;

    return app->run(window);
}
