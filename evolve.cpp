#include "evolve.hpp"

Boid evolve_boid(std::vector<Boid> const &flock, Boid b_i, double delta_t,
                 stats s, Predator p) {
  // Definizione degli oggetti utili

  Vector_2d v_sep;
  Vector_2d v_all;
  Vector_2d v_coe;
  int n = flock.size();

  // If a boid got not even one boid in its range of influence, its velocity
  // remains unchanged beacuse the 3 velocity v_sep, v_all, v_coe are
  // initialized at {0.,0.}
  if (n > 1) {
    v_sep = sep(flock, b_i, s.s, s.d_s);
    v_all = all(flock, b_i, s.a);
    v_coe = coe(b_i, calc_c_m_b_i(flock, b_i), s.c);
  }

  // Update position and velocity
  b_i.pos += b_i.vel * delta_t;
  b_i.vel += v_sep + v_all + v_coe;

  // If the boid exceedes the maximum velocity, the component of its current
  // velocity get rescaled so that the norm gets under the one imposed by v_max
  // but the direction of the boids remains unchanged
  if (b_i.vel.norm() > s.v_max) {
    b_i.vel.setx(b_i.vel.xcomp() * 0.5);
    b_i.vel.sety(b_i.vel.ycomp() * 0.5);
  }

  // If the boid is closer to the predator than the parametre d_pred, a velocity
  // "v_sep" is added to its current velocity to get it away from the predator
  if ((b_i.pos - p.pos).norm() < s.d_pred) {
    b_i.vel += (p.pos - b_i.pos) * (-s.s);
  }

  // The functions that takes care of the border of the simulation is applied
  b_i.pos = pacman(b_i.pos, s);

  return b_i;
}

Predator evolve_predator(Predator p, double delta_t, stats s) {
  Predator res{{p.pos + p.vel * delta_t}, {p.vel}};
  res.pos = pacman(res.pos, s);
  return res;
}

void evolve_flock(std::vector<Boid> &flock, double delta_t, stats s,
                  Predator p) {
  // A vector f_state is created to mantain the initial state of the flock from
  // which every boid is evolved
  std::vector<Boid> f_state;
  int n = flock.size();
  for (int i = 0; i < n; ++i) {
    Boid b_i = flock[i];
    f_state.push_back(
        evolve_boid(influence(flock, b_i, s.d), b_i, delta_t, s, p));
  };
  flock = f_state;
  return;
}

void eat_boid(std::vector<Boid> &flock, Predator p, double d_eat) {
  auto b_i = flock.begin();
  auto b_l = flock.end();
  int n = flock.size();
  // This cycle check if a boid should be eliminated, in the case of a positive
  // answer, the iterator to the it element must be replaced by the current one
  // (returned by the method erase) and the iterator to the last element must be
  // reduced by one. By not doing that there would be pointers to element of the
  // vector that doesn't exist anymore resulting in a segmentation fault error
  while (b_i != b_l) {
    Boid b_it = *b_i;
    if ((b_it.pos - p.pos).norm() < d_eat && n >= 3) {
      b_i = flock.erase(b_i);
      b_l = std::prev(b_l);
      n = flock.size();
    } else {
      ++b_i;
    }
  }
  // This part checks if the predator gets close to a boid when the flock has a
  // size smaller than 3
  for (Boid b_i : flock) {
    if ((b_i.pos - p.pos).norm() < d_eat && n < 3) {
      std::cout << "Predator isn't hungry anymore..." << '\n';
    }
  }
  return;
}