#include "header.h"


galaxy::~galaxy(){
    for (int i = 0; i < NUM_STARS; i++){
        delete stars[i];
    }
    delete[] stars;
    for (int i = 0; i < SQUARES_IN_ROW; i++){
        for (int j = 0; j < SQUARES_IN_ROW; j++){
            delete squares[i][j];
        }
        delete[] squares[i];
    }
    delete[] squares;
}

galaxy::galaxy(ifstream &file){
    srand(time(NULL));

    file >> SYSTEM_RADIUS >> CONNECT_DIST_SQR;
    CONNECT_DIST_SQR *= CONNECT_DIST_SQR;

    file >> DELTA_T;
    current_t = 0;

    file >> SQUARES_IN_ROW >> DIST_TO_OBJECT;
    SQUARE_SIZE = 2 * SYSTEM_RADIUS / SQUARES_IN_ROW;
    squares = new square **[SQUARES_IN_ROW];
    for (int i = 0; i < SQUARES_IN_ROW; i++){
        squares[i] = new square *[SQUARES_IN_ROW];
        for (int j = 0; j < SQUARES_IN_ROW; j++){
            squares[i][j] = new square(i, j, SQUARE_SIZE, SYSTEM_RADIUS);
        }
    }

    file >> NUM_STARS;
    count = NUM_STARS;
    stars = new star* [NUM_STARS];

    file >> CENTER_MASS;
    stars[0] = new star(CENTER_MASS);

    file >> MIN_MASS;
    file >> MAX_MASS;
    for (int i = 1; i < NUM_STARS; i++){
        stars[i] = generate_star();
    }
}

star* galaxy::generate_star(){
    if (DIM != 2)
        throw "I can't do this((";
    double radius = double_rand(0., SYSTEM_RADIUS);
    double speed = sqrt(G*CENTER_MASS / radius); // первая космическая скорость
    double angle = double_rand(-M_PI, M_PI);

    double coords[DIM];
    double velocity[DIM];
    double mass = double_rand(MIN_MASS, MAX_MASS);
    coords[0] = radius * cos(angle);
    coords[1] = radius * sin(angle);
    velocity[0] = speed * -sin(angle);
    velocity[1] = speed * cos(angle);
    star *generated_star = new star(coords, velocity, mass);

    return generated_star;
}

double galaxy::find_distance_sqr(double *coord_1, double *coord_2){
    double delta;
    double distance = 0;
    for (int i = 0; i < DIM; i++){
        delta = *coord_1 - *coord_2;
        distance += delta*delta;
        coord_1++;
        coord_2++;
    }
    return distance;
}

void galaxy::find_grav_force(star *star_1, star *star_2, double dist_sqr){
    double dist = sqrt(dist_sqr);
    double force = G * star_1->mass * star_2->mass / dist_sqr;
    double force_x = force * (star_2->coord[0] - star_1->coord[0])/dist;
    double force_y = force * (star_2->coord[1] - star_1->coord[1])/dist;
    star_1->force[0] += force_x;
    star_2->force[0] -= force_x;
    star_1->force[1] += force_y;
    star_2->force[1] -= force_y;
}

void galaxy::find_grav_force(star *star_, square *square_, double dist_sqr){
    double dist = sqrt(dist_sqr);
    double force = G * star_->mass * square_->mass / dist_sqr;
    double force_x = force * (square_->coord[0] - star_->coord[0])/dist;
    double force_y = force * (square_->coord[1] - star_->coord[1])/dist;
    star_->force[0] += force_x;
    star_->force[1] += force_y;
}

void galaxy::move(){
    mass = 0;
    energy = 0;
    for (int i = 0; i < DIM; i++){
        pulse[i] = 0;
    }

    for (int i = 0; i < NUM_STARS; i++){ // распихиваем все тела по квадратам
        if (stars[i]){
            bool is_deleted = false;
            for (int j = 0; j < DIM; j++){
                stars[i]->force[j] = 0;
                stars[i]->indexes[j] = floor((stars[i]->coord[j]+SYSTEM_RADIUS)/SQUARE_SIZE);
                if ((stars[i]->indexes[j] < 0) or (stars[i]->indexes[j] >= SQUARES_IN_ROW)){
                    delete stars[i];
                    stars[i] = nullptr;
                    count--;
                    is_deleted = true;
                    break;
                }
            }
            if (!is_deleted)
                squares[stars[i]->indexes[1]][stars[i]->indexes[0]]->add_star(i, stars[i]->mass);
        }
    }

    for (int i = 0; i < NUM_STARS; i++){
        if (stars[i]){
            for (int j = 0; j < SQUARES_IN_ROW; j++){
                for (int k = 0; k < SQUARES_IN_ROW; k++){
                    if ((squares[j][k]->first) and (k >= stars[i]->indexes[0] - DIST_TO_OBJECT) and (k <= stars[i]->indexes[0] + DIST_TO_OBJECT)
                            and (j >= stars[i]->indexes[1] - DIST_TO_OBJECT) and (j <= stars[i]->indexes[1] + DIST_TO_OBJECT)){
                        node* ptr = squares[j][k]->first;
                        while (ptr){
                            if (ptr->star_index > i){
                                double dist_sqr = find_distance_sqr(stars[i]->coord, stars[ptr->star_index]->coord);
                                if (dist_sqr < CONNECT_DIST_SQR){
                                    node* prev_ptr = squares[j][k]->first;
                                    while (prev_ptr and (prev_ptr->next != ptr))
                                        prev_ptr = prev_ptr->next;
                                    if (prev_ptr){
                                        prev_ptr->next = ptr->next;
                                        *stars[i] += *stars[ptr->star_index];
                                        stars[ptr->star_index] = nullptr;
                                        count--;
                                        ptr = prev_ptr;
                                    }
                                }
                                else
                                    find_grav_force(stars[i], stars[ptr->star_index], dist_sqr);
                            }
                            ptr = ptr->next;
                        }
                    }
                    else{
                        double dist_sqr = find_distance_sqr(stars[i]->coord, squares[j][k]->coord);
                        find_grav_force(stars[i], squares[j][k], dist_sqr);
                    }
                }
            }
        }
    }

    for (int i = 0; i < SQUARES_IN_ROW; i++){
        for (int j = 0; j < SQUARES_IN_ROW; j++){
            squares[i][j]->clear();
        }
    }

    for (int i = 0; i < NUM_STARS; i++){
        if (stars[i]){
            double star_velocity_sqr = 0;
            for (int j = 0; j < DIM; j++){
                double acceleration = stars[i]->force[j] / stars[i]->mass;
                stars[i]->velocity[j] += acceleration * DELTA_T;
                star_velocity_sqr += stars[i]->velocity[j] * stars[i]->velocity[j];
                pulse[j] += stars[i]->velocity[j] * stars[i]->mass;
                stars[i]->coord[j] += stars[i]->velocity[j] * DELTA_T;
            }
            energy += star_velocity_sqr * stars[i]->mass / 2;
            mass += stars[i]->mass;
        }
    }
    current_t += DELTA_T;
}


ostream& operator<<(ostream& os, const galaxy& Gal){
    os << "time: " << Gal.current_t << '\n' <<
          "\tstars: " << Gal.count;
    os << "\n\tpulse:";
    for (int i = 0; i < DIM; i++){
        os << '\t' << Gal.pulse[i];
    }
    os << "\n\tenergy: " << Gal.energy << '\n' <<
          "\tmass: " << Gal.mass;
    return os << "\n\n";
}
