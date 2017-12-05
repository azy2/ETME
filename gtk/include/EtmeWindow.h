#ifndef ETME_ETMEWINDOW_H
#define ETME_ETMEWINDOW_H

#include <gtkmm/window.h>
#include "Buffer.h"
#include "BufferView.h"

class EtmeWindow : public Gtk::Window {
public:
    EtmeWindow() : Gtk::Window::Window() {
        set_default_size(width, height);
        set_title("ETME");
        add_events(Gdk::KEY_PRESS_MASK);

        buffer = new Buffer("../core/test/files/war_and_peace.txt");

        area = new BufferView(buffer, this);

#ifndef DEBUG
        add(*area);
        area->show();
#else
        VPane.set_position(1000);
        add(VPane);
        VPane.add1(*area);
        VPane.add2(debugView);
        show_all_children();
#endif
    }

    ~EtmeWindow() override {
        delete buffer;
        delete area;
    }

#ifdef DEBUG
    enum EventType {
        keypress,
        render,
        total
    };

    void eventStart(EventType event) {
        switch (event) {
            case keypress:
                keypressStart = timer_start();
                break;
            case render:
                renderStart = timer_start();
                break;
            case total:
                totalStart = timer_start();
                break;
        }
    }
    void eventEnd(EventType event) {
        switch (event) {
            case keypress:
                keypressTime = static_cast<float>((double)timer_end(keypressStart) / 1000000.0);
                break;
            case render:
                renderTime = static_cast<float>((double)timer_end(renderStart) / 1000000.0);
                break;
            case total:
                totalTime = static_cast<float>((double)timer_end(totalStart) / 1000000.0);
                break;
        }
        ostringstream debugStrBuilder;
        debugStrBuilder << "Keypress time: " << keypressTime << "ms\nRender time: " << renderTime << "ms\nTotal time: " << totalTime << "ms";
        debugStr = debugStrBuilder.str();
        debugView.set_text(debugStr);
    }
#endif

protected:
    bool on_key_press_event(GdkEventKey* event) override {
        if (event->keyval == GDK_KEY_c && (event->state & GDK_CONTROL_MASK) == GDK_CONTROL_MASK) {
            this->close();
        }
        area->key_press_event(event);
        return true;
    }

    const int height = 1000;
#ifdef DEBUG
    const int width = 1500;
#else
    const int width = 1000;
#endif
    BufferView* area;
    Buffer* buffer;
#ifdef DEBUG
    Gtk::Paned VPane;
    Gtk::Label debugView;
    Glib::ustring debugStr;
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
    struct timespec keypressStart;
    struct timespec renderStart;
    struct timespec totalStart;
    float keypressTime;
    float renderTime;
    float totalTime;
#endif
};

#endif //ETME_ETMEWINDOW_H
