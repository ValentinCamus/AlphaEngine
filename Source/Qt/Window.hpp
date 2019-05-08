//
// Created by Valentin on 2019-01-31.
//

#pragma once

#include <QTimer>
#include <QMainWindow>

namespace Ui { class Window; }

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(QWidget * parent = nullptr);

    ~Window() override;

private:
    Ui::Window * ui;

    QTimer * timer;
};