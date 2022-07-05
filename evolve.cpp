#include "evolve.hpp"

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
  // b_i, calcolando v_all e v_coe con un flock di un solo boid si avrebbe la
  // condizione che attiva l'eccezione nelle due funzioni. In questo modo se il
  // boid b_i non è influenzato da nessuno la sua velocità rimane invariata

  if (n != 1) {
    v_sep = sep(flock, b_f, s.s, s.d_s);
    v_all = all(flock, b_f, s.a);
    v_coe = coe(b_f, calc_c_m_b_i(flock, b_f), s.c);
  }


  // Aggiorno posizione e velocità del boid

  b_f.pos += b_f.vel * delta_t;
  b_f.vel += v_sep + v_all + v_coe;

  // Se i boid superano la velocità massima, le componenti delle loro velocità
  // vengono riscalate per tornare al di sotto del limite

  if (b_f.vel.norm() > s.v_max) {
    b_f.vel.setx(b_f.vel.xcomp() * 0.8);
    b_f.vel.sety(b_f.vel.ycomp() * 0.8);
  }

  // Se i boid sono troppo vicini al predatore si attiva la regola di
  // separazione fra di essi

  if ((b_f.pos - p.pos).norm() < s.d_pred) {
    b_f.vel += (p.pos - b_f.pos) * (-s.s);
  }

  // Effetto pac-man con parametri l_b, r_b, u_b, b_b di stats

  b_f.pos = pacman(b_f.pos, s);

  return b_f;
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
        evolve_boid(influence(flock, b_i, s.d, s.theta), b_i, delta_t, s, p));
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