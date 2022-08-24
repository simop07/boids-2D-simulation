#ifndef evolve_hpp
#define evolve_hpp

#include "functions.hpp"

// Evolve_boid takes in input a vector of boids, a specific boid (not necessarly
// of that vector), a double for the time of evolution, a struct Stats and a
// predator. It evolves a single boid according to the 3 rules, then it reduces
// its velocity keeping it lower than a fixed max velocity, applies the
// separation rule for the predator and the function pacman taking care of the
// borders
Boid evolve_boid(std::vector<Boid> const &flock, Boid &b_i, double delta_t,
                 Stats const &s, Predator const &p);

// Evolve_predator updates the position of the predator after a time
// delta_t and applies the function pacman
Predator evolve_predator(Predator &p, double delta_t, Stats const &s);

// Evolve_flock uses evolve_boid to evolve every single boid of the flock only
// considering the boids that are in a certain range of the boid that's
// currently evolving using the function influence
void evolve_flock(std::vector<Boid> &flock, double delta_t, Stats const &s,
                  Predator const& p);

// Eat_boid checks if a boid is at a smaller distance than d_pred from the
// predator, in that case if the flock contains more than two boids it erases
// that specific boid that got to close to the predator
void eat_boid(std::vector<Boid> &flock, Predator p, double d_pred);
#endif