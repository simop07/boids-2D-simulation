#ifndef evolve_hpp
#define evolve_hpp

#include "functions.hpp"
#include "objects.hpp"

// Evolve_boid takes in input a vector of boids, a specific boid (not necessarly
// of that vector), e double for the time of evolution, a stats and a predator.
// It evolves a single boid according to the 3 rules, then it reduces the
// velocity of the boid keeping it lower than max velocity and applies the
// separation rule for the predator and the function pacman taking care of the
// borders
boid evolve_boid(std::vector<boid> const &flock, boid b_i, double delta_t,
                 stats s, predator p);

// Evolve predator simply updates the position of the predator after a time
// delta_t and applies the function pacman
predator evolve_predator(predator p, double delta_t, stats s);

// Evolve_flock uses evolve_boid to evolve every single boid of the flock only
// considering the boids that are in a certain range of the boid that's
// currently evolving using the function influence
void evolve_flock(std::vector<boid> &flock, double delta_t, stats s,
                  predator p);

// Eat_boid checks if a boid is at a smaller distance than d_pred from the
// predator, in that case if the flock contains more than two boids it erases
// that specific boid that got to close to the predator
void eat_boid(std::vector<boid> &flock, predator p, double d_pred);
#endif