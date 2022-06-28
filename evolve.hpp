#ifndef evolve_hpp
#define evolve_hpp

#include "functions.hpp"
#include "objects.hpp"

// Funzione evolve che prende in input un vettore (tutti i boids nel piano), un
// singolo boid b_i e restituisce il boid b_i evoluto di un delta t con dei
// parametri s

boid evolve_boid(std::vector<boid> const &flock, boid b_i, double delta_t,
                 stats s, predator p) {
  boid b_f = b_i;
  // Preparo le 3 velocità da aggiungere settate a 0
  vector_2d v_sep;
  vector_2d v_all;
  vector_2d v_coe;
  double n = flock.size();

  // L'if serve per verificare che ci sia almeno un altro boid che influenza
  // b_i, altrimenti in v_all e v_coe si dovrebbe dividere per 0. In questo modo
  // se il boid b_i non è influenzato da nessuno rimane uguale

  if (n != 1.) {
    v_sep = sep(flock, b_f, s.s, s.d_s);
    v_all = all(flock, b_f, s.a);
    v_coe = coe(b_f, calc_c_m_b_i(flock, b_f), s.c);
  }
  b_f.pos += b_f.vel * delta_t;
  b_f.vel += v_sep + v_all + v_coe;

  if ((b_i.pos - p.pos).norm() < s.d_pred) {
    b_f.vel += (p.pos - b_i.pos) * (-s.s);
  }

  // Effetto pac-man con parametri l_b, r_b, u_b, b_b di stats

  if (b_f.pos.xcomp() < s.l_b) {
    b_f.pos.setx(s.r_b - abs(s.l_b - b_f.pos.xcomp()));
  };
  if (b_f.pos.xcomp() > s.r_b) {
    b_f.pos.setx(s.l_b + abs(b_f.pos.xcomp() - s.r_b));
  };
  if (b_f.pos.ycomp() > s.u_b) {
    b_f.pos.sety(s.b_b + abs(b_f.pos.ycomp() - s.u_b));
  };
  if (b_f.pos.ycomp() < s.b_b) {
    b_f.pos.sety(s.u_b - abs(s.b_b - b_f.pos.ycomp()));
  };

  return b_f;
};

// La funzione evolve_flock prende in input un vettore flock con tutti i boids
// del piano, con un loop evolve ogni singolo boid tenendo conto solo del suo
// range di influenza e restituisce un vettore flock con tutti i boids del piano
// evoluti correttamente tenendo conto solo degli altri boid ad una distanza
// massima d

void evolve_flock(std::vector<boid> &flock, double delta_t, stats s,
                  predator p) {
  std::vector<boid> f_state = flock;
  int n = flock.size();
  for (int i = 0; i < n; ++i) {
    boid b_i = flock[i];
    f_state[i] =
        evolve_boid(influence(flock, b_i, s.d, s.theta), b_i, delta_t, s, p);
  };
  flock = f_state;
  return;
};

#endif