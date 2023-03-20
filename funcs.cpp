#include "header.h"

double double_rand(double min, double max){
    return min + double(rand())/RAND_MAX * (max-min);
}
