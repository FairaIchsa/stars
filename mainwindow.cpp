#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "header.h"
#include "fstream"

ifstream fin("../Course_Work_Dec_21/input.txt");
galaxy *Gal = new galaxy(fin);
ofstream fout("../Course_Work_Dec_21/output.txt");

// Параметры для отрисовки в окне
const int TOP_X0 = 100, TOP_Y0 = 100, WIDTH = 800, HEIGHT = 800;
double COEF_X = WIDTH / Gal->SYSTEM_RADIUS / 2,
        COEF_Y = HEIGHT / Gal->SYSTEM_RADIUS / 2;
int CENTER_X = WIDTH / 2,
    CENTER_Y = HEIGHT / 2;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    paused = true;
    connect(ui->Start_Button,  SIGNAL(clicked()), this, SLOT(Press_Button()));
    timer->start(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Press_Button(){
    QPushButton *button = ui->Start_Button;
    paused = !paused;
    button->setText(Button_text[paused]);
    if (paused){
        disconnect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));
    }
    else{
        connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));
    }
}

void MainWindow::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    QPen pen(Qt::black, 1, Qt::SolidLine);
    painter.setPen(pen);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    brush.setColor(QColor(0, 0, 0));
    for (int i = 0; i < Gal->SQUARES_IN_ROW; i++){
        for (int j = 0; j < Gal->SQUARES_IN_ROW; j++){
           painter.drawLine(Gal->squares[i][j]->coord_0[0] * COEF_X + CENTER_X + TOP_X0, Gal->squares[i][j]->coord_0[1] * COEF_Y + CENTER_Y + TOP_Y0,
                   Gal->squares[i][j]->coord_0[0] * COEF_X + CENTER_X + TOP_X0, Gal->squares[i][j]->coord_1[1] * COEF_Y + CENTER_Y + TOP_Y0);

           painter.drawLine(Gal->squares[i][j]->coord_0[0] * COEF_X + CENTER_X + TOP_X0, Gal->squares[i][j]->coord_0[1] * COEF_Y + CENTER_Y + TOP_Y0,
                   Gal->squares[i][j]->coord_1[0] * COEF_X + CENTER_X + TOP_X0, Gal->squares[i][j]->coord_0[1] * COEF_Y + CENTER_Y + TOP_Y0);
        }
    }

    for (int i = 0; i < Gal->NUM_STARS; i++){
        if (Gal->stars[i]){
            brush.setColor(Gal->stars[i]->color);
            painter.setBrush(brush);
            painter.drawEllipse(Gal->stars[i]->coord[0] * COEF_X + CENTER_X + TOP_X0,
                    Gal->stars[i]->coord[1] * COEF_Y + CENTER_Y + TOP_Y0,
                    6 + 4 * !i, 6 + 4 * !i);
        }
    }

    if(!paused){
        Gal->move();
        fout << *Gal;
    }

        ui->time_val->setText(QString::number(Gal->current_t));
        ui->count_val->setText(QString::number(Gal->count));
        ui->pulse_0_val->setText(QString::number(Gal->pulse[0]));
        ui->pulse_1_val->setText(QString::number(Gal->pulse[1]));
        ui->energy_val->setText(QString::number(Gal->energy));
        ui->mass_val->setText(QString::number(Gal->mass));
}
