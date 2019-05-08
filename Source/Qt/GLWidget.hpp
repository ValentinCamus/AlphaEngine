//
// Created by Valentin on 2019-01-31.
//

#pragma once

#define WIDTH  1024
#define HEIGHT 1024

#include <QKeyEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>

#include <Core/Core.hpp>

class GLWidget :
    public QOpenGLWidget, 
    public QOpenGLFunctions_4_1_Core 
{

public:
    explicit GLWidget(QWidget * parent = nullptr);

    ~GLWidget() override = default;

    // OpenGL management
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    // Keyboard events
    void keyPressEvent(QKeyEvent * event) override;

    // Mouse events
    void mousePressEvent(QMouseEvent * event) override;
    void mouseMoveEvent(QMouseEvent * event) override;

private:
    long m_lastTime;
};

