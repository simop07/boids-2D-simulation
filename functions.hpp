#ifndef functions_hpp
#define functions_hpp

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "objects.hpp"

// Distance between two boids
double distance(Boid const& b1, Boid const& b2);

// Center of mass relative to a boid b_i
Vector_2d calc_c_m_b_i(std::vector<Boid> const &flock, Boid const&b_i);

double mean_distance(std::vector<Boid> const &flock);

double std_dev_distance(std::vector<Boid> const &flock);

double mean_velocity(std::vector<Boid> const &flock);

double std_dev_velocity(std::vector<Boid> const &flock);

// Rule of separation
Vector_2d sep(std::vector<Boid> const &flock, Boid b_i, double s, double d_s);

// Rule of allignment
Vector_2d all(std::vector<Boid> const &flock, Boid b_i, double a);

// Rule of cohesion
Vector_2d coe(Boid b_i, Vector_2d c_m, double c);

// This function takes care of the border of the simulation by teleporting every
// boid or predator that exceedes a certain border (specified in the stats) to
// the opposite one (like in pacman)
Vector_2d pacman(Vector_2d pos, stats s);

// Influence checks, given a flock, which boid is at a distance, from a certain
// boid b_i, smaller than the one needed to apply the 3 rules and returns a new
// vector "range" containing only those boids
std::vector<Boid> influence(std::vector<Boid> const &flock, Boid b_i, double d);

#endif