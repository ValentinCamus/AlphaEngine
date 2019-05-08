#include "GLWidget.hpp"

#include <QDateTime>
#include <QApplication>



#define LAST_TIME_START_VALUE 0

GLWidget::GLWidget(QWidget *parent) :
    QOpenGLWidget(parent), 
    QOpenGLFunctions_4_1_Core(),
    m_lastTime(LAST_TIME_START_VALUE)
{

}

void GLWidget::initializeGL()
{
    if (!initializeOpenGLFunctions())
    {
        LOG_ERROR("Cannot initialize OpenGL Functions");
    }
}

void GLWidget::paintGL()
{
    const auto startTime = QDateTime::currentMSecsSinceEpoch();

    glFinish();

    m_lastTime = QDateTime::currentMSecsSinceEpoch() - startTime;
}

void GLWidget::resizeGL(int width, int height)
{

}

void GLWidget::keyPressEvent(QKeyEvent * event)
{
    update();
}

void GLWidget::mousePressEvent(QMouseEvent * event)
{
    int buttonType; // buttons are 0(left), 1(right) to 2(middle)
    Qt::MouseButton button = event->button();

    if (button & Qt::LeftButton)
    {
        if ((event->modifiers() & Qt::ControlModifier)) buttonType = 2;
        else buttonType = 0;
    }
    else if (button & Qt::RightButton) buttonType = 1;
    else if (button & Qt::MiddleButton) buttonType = 2;
    else buttonType = 3;

    m_lastTime = QDateTime::currentMSecsSinceEpoch();
}

void GLWidget::mouseMoveEvent(QMouseEvent * event)
{
    update();
}
