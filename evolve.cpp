#include "evolve.hpp"

boid evolve_boid(std::vector<boid> const &flock, boid b_i, double delta_t,
                 stats s, predator p) {

  // Definizione degli oggetti utili

  vector_2d v_sep;
  vector_2d v_all;
  vector_2d v_coe;
  int n = flock.size();

  // L'if serve per verificare che ci sia almeno un altro boid che influenza
  // b_i, calcolando v_all e v_coe con un flock di un solo boid si avrebbe la
  // condizione che attiva l'eccezione nelle due funzioni. In questo modo se il
  // boid b_i non è influenzato da nessuno la sua velocità rimane invariata

  if (n > 1) {
    v_sep = sep(flock, b_i, s.s, s.d_s);
    v_all = all(flock, b_i, s.a);
    v_coe = coe(b_i, calc_c_m_b_i(flock, b_i), s.c);
  }

  // Aggiorno posizione e velocità del boid

  b_i.pos += b_i.vel * delta_t;
  b_i.vel += v_sep + v_all + v_coe;

  // Se i boid superano la velocità massima, le componenti delle loro velocità
  // vengono riscalate per tornare al di sotto del limite

  if (b_i.vel.norm() > s.v_max) {
    b_i.vel.setx(b_i.vel.xcomp() * 0.8);
    b_i.vel.sety(b_i.vel.ycomp() * 0.8);
  }

  // Se i boid sono troppo vicini al predatore si attiva la regola di
  // separazione fra di essi

  if ((b_i.pos - p.pos).norm() < s.d_pred) {
    b_i.vel += (p.pos - b_i.pos) * (-s.s * 1.5);
  }

  // Effetto pac-man con parametri l_b, r_b, u_b, b_b di stats

  b_i.pos = pacman(b_i.pos, s);

  return b_i;
}

predator evolve_predator(predator p, double delta_t, stats s) {
  predator res{{p.pos + p.vel * delta_t}, {p.vel}};
  res.pos = pacman(res.pos, s);
  return res;
}

void evolve_flock(std::vector<boid> &flock, double delta_t, stats s,
                  predator p) {
  std::vector<boid> f_state;
  int n = flock.size();
  for (int i = 0; i < n; ++i) {
    boid b_i = flock[i];
    f_state.push_back(
        evolve_boid(influence(flock, b_i, s.d), b_i, delta_t, s, p));
  };
  flock = f_state;
  return;
}

void eat_boid(std::vector<boid> &flock, predator p, double d_pred) {
  auto b_i = flock.begin();
  auto b_l = flock.end();
  for (; b_i != b_l; ++b_i) {
    boid b_it = *b_i;
    if ((b_it.pos - p.pos).norm() < d_pred / 2. && flock.size() >= 3) {
      flock.erase(b_i);
    }
  }
  return;
}