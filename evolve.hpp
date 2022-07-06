#ifndef evolve_hpp
#define evolve_hpp

#include "functions.hpp"
#include "objects.hpp"

// Funzione evolve che prende in input un vettore (tutti i boids nel piano), un
// singolo boid b_i e restituisce il boid b_i evoluto di un delta t con dei
// parametri s

boid evolve_boid(std::vector<boid> const &flock, boid b_i, double delta_t,
                 stats s, predator p) {

  // Definizione degli oggetti utili, si crea una copia del boid da evolvere
  // così da mantenerne uno con i vecchi parametri utile per far evolvere i boid
  // successivi

  boid b_f = b_i;
  vector_2d v_sep;
  vector_2d v_all;
  vector_2d v_coe;
  int n = flock.size();

  // L'if serve per verificare che ci sia almeno un altro boid che influenza
  // b_i, altrimenti in v_all e v_coe si dovrebbe dividere per 0. In questo modo
  // se il boid b_i non è influenzato da nessuno la sua velocità rimane
  // invariata

  if (n != 1) {
    v_sep = sep(flock, b_f, s.s, s.d_s);
    v_all = all(flock, b_f, s.a);
    v_coe = coe(b_f, calc_c_m_b_i(flock, b_f), s.c);
  }

  // Se un boid ha velocità zero gli viene aggiunta una velocità verso il centro
  // di massa dello stormo

  if (b_f.vel.norm() == 0.) {
    vector_2d c_m = calc_c_m_b_i(flock, b_f);
    vector_2d pos_diff = c_m - b_f.pos;
    vector_2d vel_versor = pos_diff * (1. / pos_diff.norm()); // per la direzione
    b_f.vel = vel_versor;
  }

  // Aggiorno posizione e velocità del boid

  b_f.pos += b_f.vel * delta_t;
  b_f.vel += v_sep + v_all + v_coe;// se non e' influenzato da nessuno somma 0

  // Se i boid superano la velocità massima, le componenti delle loro velocità
  // vengono riscalate per tornare al di sotto del limite

  if (b_f.vel.norm() > s.v_max) {
    b_f.vel.setx(b_f.vel.xcomp() * 0.8);
    b_f.vel.sety(b_f.vel.ycomp() * 0.8);
  }

  // Se i boid sono troppo vicini al predatore si attiva la regola di
  // separazione fra di essi

  if ((b_f.pos - p.pos).norm() < s.d_pred) { // non posso fare distance perche e' definita per boids 
    b_f.vel += (p.pos - b_f.pos) * (-s.s);
  }

  // Effetto pac-man con parametri l_b, r_b, u_b, b_b di stats

  b_f.pos = pacman(b_f.pos, s);

  return b_f;
}

// Funzione evolve predator che fa evolvere il predatore secondo la sua velocità
// e applica l'effetto pac-man

predator evolve_predator(predator p, double delta_t, stats s) {
  predator res{{p.pos + p.vel * delta_t}, {p.vel}};
  res.pos = pacman(res.pos, s);
  return res;
}

// La funzione evolve_flock prende in input un vettore flock con tutti i boids
// del piano, con un loop evolve ogni singolo boid tenendo conto solo del suo
// range di influenza e restituisce un vettore flock con tutti i boids del piano
// evoluti correttamente tenendo conto solo degli altri boids ad una distanza
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
}

// l'effetto del predatore

void eat_boid(std::vector<boid> &flock, predator p, double d_pred) {
  auto b_i = flock.begin();
  auto b_l = std::prev(flock.end()); // cos'e' prev?
  for (; b_i != b_l; ++b_i) {
    boid b_it = *b_i;
    if ((b_it.pos - p.pos).norm() < d_pred / 2. && flock.size() != 1) {
      flock.erase(b_i);
    }
  }
  return;
}
#endif