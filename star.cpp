#include "header.h"


star::~star(){}

star::star(double m){
    for (int i = 0; i < DIM; i++){
         coord[i] = 0;
         velocity[i] = 0;
         force[i] = 0;
    }
    mass = m;
    set_color();
}

star::star(double *point, double *speed, double m){
    for (int i = 0; i < DIM; i++){
        coord[i] = point[i];
        velocity[i] = speed[i];
        force[i] = 0;
    }
    mass = m;
    set_color();
}

void star::operator+=(const star &obj){
    double new_mass = mass + obj.mass;
    for (int i = 0; i < DIM; i++){
        force[i] += obj.force[i];
        velocity[i] = (velocity[i]*mass + obj.velocity[i]*obj.mass) / new_mass;
    }
    mass = new_mass;
    set_color();
    delete &obj;
}

void star::set_color(){
    int col = 0;
    while (mass >= MASSES[col])
        col++;
    color = COLORS[col];
}
