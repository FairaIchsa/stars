#include "header.h"

#include "iostream"

node::~node(){}

node::node(int index, node *right){
    star_index = index;
    next = right;
}

square::~square(){
    while (first){
        node *delete_node = first;
        delete delete_node;
        first = first->next;
    }
}

square::square(int x_index, int y_index, double size, double radius){
    first = last = nullptr;
    mass = 0;
    coord_0[0] = y_index * size - radius;
    coord_0[1] = x_index * size - radius;
    for (int i = 0; i < DIM; i++){
        coord_1[i] = coord_0[i] + size;
        coord[i] = (coord_0[i] + coord_1[i]) / 2;
    }
}

void square::add_star(int index, double m){
    if (last){
        last->next = new node(index);
        last = last->next;
    }
    else{
        first = last = new node(index);
    }
    mass += m;
}

void square::clear(){
    while (first){
        node *delete_node = first;
        first = first->next;
        delete delete_node;
    }
    last = nullptr;
    mass = 0;
}
