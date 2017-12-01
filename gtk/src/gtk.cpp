#include <gtkmm.h>
#include "EtmeWindow.h"

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.azy2.etme");

    EtmeWindow window;
    return app->run(window);
}
