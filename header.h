#ifndef HEADER_H
#define HEADER_H

#include "cmath"
#include <QPainter>
#include "time.h"

#include "iostream"
#include "fstream"

using namespace std;

// const

const int DIM = 2; // размерность пространства

const double G = 6.67408e-11; // гравитационная постоянная


const double SUN_MASS   = 1.98892e30,
             JUP_MASS   = 1898.6e24,
             URAN_MASS  = 86.832e24,
             EARTH_MASS = 5.9742e24,
             VENUS_MASS = 4.867e24;

const double MASSES[] = {   // массы, по которым определяется цвет тела
                        1e25,
                        1e26,
                        1e27,
                        1e28,
                        1e29,
                        1e30,
                        INFINITY
                        };

const QColor COLORS[] = {   // цвета тел
                        QColor(0, 0, 255),
                        QColor(0, 191, 255),
                        QColor(0, 255, 0),
                        QColor(255, 255, 0),
                        QColor(255, 165, 0),
                        QColor(255, 0, 0),
                        QColor(139, 0, 255)
                        };


// "funcs.cpp"
double double_rand(double min, double max); // генерирует число типа double в диапазоне от min до max


// "star.cpp"
class  star{
public:
    double coord[DIM]; // координаты по осям
    double velocity[DIM]; // скорости по осям
    double force[DIM]; // силы притяжения по осям
    double mass; // масса тела
    int indexes[DIM]; // индекс квадрата, в котором находится тело
    QColor color; // цвет тела
    ~star();
    star(double m); // конструктор для центрального тела
    star(double *point, double *speed, double m); // конструктор для остальных тел
    void set_color(); // выбирает цвет в соответствии с массой
    void operator+=(const star &obj); // слияние звезд (второй объект удаляется)
};


// "square.cpp"
struct node{
    int star_index;
    node *next;
    ~node();
    node(int index, node *right = nullptr);
};

class square{ // квадраты, на которые будет разбиваться плоскость (работает только с DIM = 2)
private:
public:
    double coord_0[DIM], // левый верхний угол
            coord_1[DIM], // правый нижний угол
            coord[DIM]; // центр
    node *first, *last; // тела в конкретном квадрате
    double mass;
    ~square();
    square(int x_index, int y_index, double size, double radius);
    void add_star(int star_index, double m); // добавляет тело в квадрат
    void clear(); // удаляет все елементы из квадрата
};

// "galaxy.cpp"
class galaxy{
    star* generate_star(); // создает звезду со случайными параметрами
    double find_distance_sqr(double *coord_1, double *coord_2); // находит квадрат расстояния
    void find_grav_force(star *star_1, star *star_2, double dist_sqr); // рассчитывает силу притяжения двух тел
    void find_grav_force(star *star_, square *square_, double dist_sqr); // рассчитывает притяжение тела к квадрату
public:
    double SYSTEM_RADIUS; // максимальное отдаление от центра
    double CONNECT_DIST_SQR; // квадрат расстояния, на котором тела будут сливаться
    double DELTA_T; // шаг по времени
    int SQUARES_IN_ROW, DIST_TO_OBJECT; // количество квадратов в строке и расстояние взаимодействия
    double SQUARE_SIZE; // сторона квадрата
    int NUM_STARS; // количество звезд в галактике
    double CENTER_MASS, MIN_MASS, MAX_MASS; // масса нулевого объекта, минимальная и максимальная массы остальных объектов
    double pulse[DIM], energy;
    double mass;
    int count; // количество тел в системе
    double current_t; // время в системе
    star **stars;
    square ***squares;
    ~galaxy();
    galaxy(ifstream &file);
    void move(); // а здесь происходит вся магия
};

ostream& operator<<(ostream &os, const galaxy &Gal);

#endif // HEADER.H
