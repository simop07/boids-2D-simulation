#include "evolve.hpp"

Boid evolve_boid(std::vector<Boid> const &flock, Boid &b_i,
                 double const delta_t, Stats const &s, Predator const &p) {
  // Definition of useful objects
  Vector_2d v_sep;
  Vector_2d v_all;
  Vector_2d v_coe;
  int n = flock.size();

  // If a boid got not even one boid in its range of influence, its velocity
  // remains unchanged, beacuse the 3 velocity v_sep, v_all, v_coe are
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
  // - obviously boid's direction remains unchanged
  if (b_i.vel.norm() > s.v_max) {
    b_i.vel.setx(b_i.vel.xcomp() * 0.5);
    b_i.vel.sety(b_i.vel.ycomp() * 0.5);
  }

  // If the boid is closer to the predator than the parameter d_pred, a velocity
  // "v_sep" is added to its current velocity to get it away from the predator
  if (distance(b_i, p) < s.d_pred) {
    b_i.vel += (-(p.pos - b_i.pos) * s.s);
  }

  // The functions that takes care of the border of the simulation is applied
  b_i.pos = pacman(b_i.pos, s);

  return b_i;
}

Predator evolve_predator(Predator &p, double const delta_t, Stats const &s) {
  Predator res{{p.pos + p.vel * delta_t}, {p.vel}};
  res.pos = pacman(res.pos, s);
  return res;
}

void evolve_flock(std::vector<Boid> &flock, double const delta_t,
                  Stats const &s, Predator const &p) {
  // A vector f_state is created in order to mantain the initial state of the
  // flock from which every boid is evolved
  std::vector<Boid> f_state;
  f_state.reserve(flock.size());

  std::transform(
      flock.begin(), flock.end(), std::back_inserter(f_state), [&](Boid b_i) {
        return evolve_boid(influence(flock, b_i, s.d), b_i, delta_t, s, p);
      });

  assert(flock.size() == f_state.size());

  flock = f_state;
}

void eat_boid(std::vector<Boid> &flock, Predator const &p, double const d_eat) {
  auto b_i = flock.begin();
  auto b_l = flock.end();
  int n = flock.size();

  // This cycle check if a boid should be eliminated; in case of positive
  // answer, the iterator to b_it element must be replaced by the current one
  // (returned by the method erase) and the iterator b_l must be reassigned to
  // the updated last iterator. By not reassigning, there would be pointers to
  // element of the vector that doesn't exist anymore, resulting in segmentation
  // fault errors
  while (b_i != b_l) {
    Boid b_it = *b_i;
    if (distance(b_it, p) < d_eat && n >= 3) {
      b_i = flock.erase(b_i);
      b_l = flock.end();
      n = flock.size();
    } else {
      ++b_i;
    }
  }

  // This part checks if the predator gets close to a boid when the flock has a
  // size smaller than 3
  std::for_each(flock.begin(), flock.end(), [&](Boid const &b_i) {
    if (distance(b_i, p) < d_eat && n < 3) {
      std::cout << "Predator isn't hungry anymore..." << '\n';
    }
  });
}