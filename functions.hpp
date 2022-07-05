#ifndef functions_hpp
#define functions_hpp

#include "objects.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

// Funzione che calcola la distanza fra due boid

double distance(boid b1, boid b2);

// Funzione che calcola il centro di massa dei boid

vector_2d calc_c_m_b_i(std::vector<boid> const &flock, boid b_i);

// Distanza media

double mean_distance(std::vector<boid> const &flock);

// Deviazione standard distanza

double std_dev_distance(std::vector<boid> const &flock);

// Velocità media

double mean_velocity(std::vector<boid> const &flock);

// Deviazione standard velocità

double std_dev_velocity(std::vector<boid> const &flock);
// Velocità di separazione

vector_2d sep(std::vector<boid> const &flock, boid b_i, double s, double d_s);

// Velocità di allineamento

vector_2d all(std::vector<boid> const &flock, boid b_i, double a);

// Velocità di coesione

vector_2d coe(boid b_i, vector_2d c_m, double c);

// Effetto pac-man

vector_2d pacman(vector_2d pos, stats s);

// Angolo cieco

bool vision(boid b1, boid b2, double theta);

// La funzione influence prende in input un vettore flock con tutti i boid del
// piano e restituisce un vettore range con solo i boids nel range di
// influenza di un certo boid b_i del piano

std::vector<boid> influence(std::vector<boid> const &flock, boid b_i, double d,
                            double theta);

#endif