#include <gtkmm.h>
#include "BufferView.h"

class EtmeWindow : public Gtk::Window {
public:
    EtmeWindow() : Gtk::Window::Window() {
        set_default_size(1920, 2000);
        set_title("ETME");
        add_events(Gdk::KEY_PRESS_MASK);

        buffer = new Buffer("test");

        area = new BufferView(buffer);
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

    BufferView* area;
    Buffer* buffer;
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.azy2.etme");

    EtmeWindow window;

    return app->run(window);
}
