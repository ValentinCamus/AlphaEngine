#include <QApplication>

#include <Core/Core.hpp>

#include "Qt/Window.hpp"

int main(int argc, char *argv[])
{
    LOG_INFO("AlphaEngine : Running...");

    QApplication application(argc, argv);
    Window window;
    window.show();

    LOG_INFO("AlphaEngine : Exiting...");
    return application.exec();
}
