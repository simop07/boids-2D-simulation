#ifndef evolve_hpp
#define evolve_hpp

#include "functions.hpp"
#include "objects.hpp"

// Funzione evolve che prende in input un vettore (tutti i boids nel piano), un
// singolo boid b_i e restituisce il boid b_i evoluto di un delta t con dei
// parametri s
boid evolve_boid(std::vector<boid> const &flock, boid b_i, double delta_t,
                 stats s, predator p);

// Funzione evolve predator che fa evolvere il predatore secondo la sua
// velocità e applica l'effetto pac-man
predator evolve_predator(predator p, double delta_t, stats s);

// La funzione evolve_flock prende in input un vettore flock con tutti i boids
// del piano, con un loop evolve ogni singolo boid tenendo conto solo del suo
// range di influenza e restituisce un vettore flock con tutti i boids del piano
// evoluti correttamente tenendo conto solo degli altri boid ad una distanza
// massima d
void evolve_flock(std::vector<boid> &flock, double delta_t, stats s,
                  predator p);

// La funzione eat_boid controlla se un boid è troppo vicino al predatore, se lo
// è quel boid viene rimosso dal flock
void eat_boid(std::vector<boid> &flock, predator p, double d_pred);
#endif