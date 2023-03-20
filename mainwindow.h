#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QTime>

#include <iostream>
#include <fstream>
#include "time.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    const QString Button_text[2] = {"Stop", "Start"};
    bool paused;
    int curr_text = 0;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTimer *timer = new QTimer(this);

private:
    Ui::MainWindow *ui;

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void Press_Button();
};
#endif // MAINWINDOW_H
