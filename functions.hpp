#ifndef functions_hpp
#define functions_hpp

#include "objects.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

// Distance between two boids
double distance(boid b1, boid b2);

// Center of mass relative to a boid b_i
vector_2d calc_c_m_b_i(std::vector<boid> const &flock, boid b_i);

double mean_distance(std::vector<boid> const &flock);

double std_dev_distance(std::vector<boid> const &flock);

double mean_velocity(std::vector<boid> const &flock);

double std_dev_velocity(std::vector<boid> const &flock);

// Rule of separation
vector_2d sep(std::vector<boid> const &flock, boid b_i, double s, double d_s);

// Rule of allignment
vector_2d all(std::vector<boid> const &flock, boid b_i, double a);

// Rule of cohesion
vector_2d coe(boid b_i, vector_2d c_m, double c);

// This function takes care of the border of the simulation by teleporting every
// boid or predator that exceedes a certain border (specified in the stats) to
// the opposite one (like in pacman)
vector_2d pacman(vector_2d pos, stats s);

// Influence checks, given a flock, which boid is at a distance, from a certain
// boid b_i, smaller than the one needed to apply the 3 rules and returns a new
// vector "range" containing only those boids
std::vector<boid> influence(std::vector<boid> const &flock, boid b_i, double d);

#endif