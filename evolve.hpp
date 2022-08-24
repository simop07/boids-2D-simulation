#ifndef evolve_hpp
#define evolve_hpp

#include "functions.hpp"

// Evolve_boid takes in input a vector of boids, a specific boid (not necessarly
// of that vector), e double for the time of evolution, a Stats and a predator.
// It evolves a single boid according to the 3 rules, then it reduces the
// velocity of the boid keeping it lower than max velocity and applies the
// separation rule for the predator and the function pacman taking care of the
// borders
Boid evolve_boid(std::vector<Boid> const &flock, Boid b_i, double delta_t,
                 Stats s, Predator p);

// Evolve predator simply updates the position of the predator after a time
// delta_t and applies the function pacman
Predator evolve_predator(Predator p, double delta_t, Stats s);

// Evolve_flock uses evolve_boid to evolve every single boid of the flock only
// considering the boids that are in a certain range of the boid that's
// currently evolving using the function influence
void evolve_flock(std::vector<Boid> &flock, double delta_t, Stats s,
                  Predator p);

// Eat_boid checks if a boid is at a smaller distance than d_pred from the
// predator, in that case if the flock contains more than two boids it erases
// that specific boid that got to close to the predator
void eat_boid(std::vector<Boid> &flock, Predator p, double d_pred);
#endif