#include "DisplayProperties.h"
#include <LibDraw/PNGLoader.h>
#include <LibGUI/GApplication.h>
#include <LibGUI/GBoxLayout.h>
#include <LibGUI/GWidget.h>
#include <LibGUI/GWindow.h>

int main(int argc, char** argv)
{
    GApplication app(argc, argv);
    DisplayPropertiesWidget instance;

    auto* window = new GWindow();
    window->set_title("Display Properties");
    window->resize(400, 448);
    window->set_resizable(false);
    window->set_main_widget(instance.get_root_widget());
    window->set_icon(load_png("/res/icons/16x16/app-display-properties.png"));

    window->show();
    return app.exec();
}
