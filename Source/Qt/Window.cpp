#include <QSurfaceFormat>
#include "Window.hpp"
#include "ui_Window.h"

#define FORMAT_VERSION 4, 1
#define FORMAT_DEPTH_BUFFER_SIZE 24
#define FPS 60

Window::Window(QWidget * Parent) : 
    QMainWindow(Parent), 
    ui(new Ui::Window)
{
    /* Initialise the default format */
    QSurfaceFormat format;
    format.setVersion(FORMAT_VERSION);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(FORMAT_DEPTH_BUFFER_SIZE);
    QSurfaceFormat::setDefaultFormat(format);

    /* Setup the UI */
    ui->setupUi(this);
    ui->widget->setFocus();
    this->setCentralWidget(ui->widget);

    /* Use to update the GLWidget every tick */
    timer = new QTimer(ui->widget);
    connect(timer, SIGNAL(timeout()), ui->widget, SLOT(update()));
    timer->start(1000 / FPS);
}

Window::~Window() 
{
    delete ui;
}