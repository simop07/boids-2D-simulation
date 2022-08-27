#include "functions.hpp"

double distance(Boid const &b1, Boid const &b2) {
  auto pos_diff{b1.pos - b2.pos};
  return pos_diff.norm();
};

double distance(Boid const &b, Predator const &p) {
  auto pos_diff{b.pos - p.pos};
  return pos_diff.norm();
};

Vector_2d calc_c_m_b_i(std::vector<Boid> const &flock, Boid const &b_i) {
  double n = flock.size();

  // This assert checks if there is less than one boid in flock (the case
  // n=1. is managed in evolve)

  assert(n > 1);

  /* if (n <= 1.) {
    throw std::runtime_error{"Flock must contain at least 2 boid"};
  } */
  auto c_m_j = std::accumulate(flock.begin(), flock.end(), Vector_2d{},
                               [&](Vector_2d c_m, Boid const &b_j) {
                                 c_m += (b_j.pos * (1 / (n - 1)));
                                 return c_m;
                               });

  return c_m_j - (b_i.pos * (1. / (n - 1.)));
}

// The exceptions in the data functions are there to make sure that there are at
// least two boids in the flock since it wouldn't make sense to calculate them
// with only one boid present

/* vedi sopra, modifica*/
double mean_distance(std::vector<Boid> const &flock) {
  double n = flock.size();
  int i{};
  double sum_tot{};
  double sum_par{};

  /* double sum_par{}; */
  /* if (n <= 1.) {
   throw std::runtime_error{"Flock must contain at least 2 boid"};
 } */
  assert(n > 1);

  std::for_each(flock.begin(), flock.end(), [&](Boid const &b_i) {
    auto nx = std::next((flock.begin() + i));
    for (; nx != flock.end(); ++nx) {
      sum_par += distance(b_i, *nx);
    }

    sum_tot += sum_par;
    sum_par = 0.;
    ++i;
  });

  // Number of distances equals the combinations of n boid, taken at groups of
  // two
  double c_n_2{(n * (n - 1.) / 2.)};
  double res = sum_tot / c_n_2;
  /* Vedi se devi togliere degli zeri */
  /*   sum_par = 0.;
   */
  return res;
}

double std_dev_distance(std::vector<Boid> const &flock) {
  double n = flock.size();
  double mean_d = mean_distance(flock);
  int i{};
  double sum_tot{};
  double sum_d_i2{};

  /* double sum_d_i2{}; */
  /* if (n <= 1.) {
   throw std::runtime_error{"Flock must contain at least 2 boid"};
 } */
  assert(n > 1);

  std::for_each(flock.begin(), flock.end(), [&](Boid const &b_i) {
    auto nx = std::next((flock.begin() + i));
    for (; nx != flock.end(); ++nx) {
      sum_d_i2 += ((distance(b_i, *nx)) * (distance(b_i, *nx)));
    }

    sum_tot += sum_d_i2;
    sum_d_i2 = 0.;
    ++i;
  });

  double c_n_2{(n * (n - 1.) / 2.)};
  double res;
  if (n != 2) {
    res = sqrt((sum_tot / (c_n_2 - 1.)) - mean_d * mean_d);
  } else {
    res = NAN;
  }
  sum_tot = 0.;
  return res;
}

double mean_velocity(std::vector<Boid> const &flock) {
  double n = flock.size();
  double sum_v{};
  /*  if (n <= 1.) {
     throw std::runtime_error{"Flock must contain at least 2 boid"};
   } */
  assert(n > 1);

  sum_v = std::accumulate(flock.begin(), flock.end(), 0.,
                          [](double sum_v, Boid const &b_j) {
                            sum_v += (b_j.vel.norm());
                            return sum_v;
                          });

  double res = sum_v * (1. / n);
  sum_v = 0.;
  return res;
}

double std_dev_velocity(std::vector<Boid> const &flock) {
  double n = flock.size();
  double sum_v_i2{};
  double sum_tot{};
  double mean_v = mean_velocity(flock);
  /* if (n <= 1.) {
    throw std::runtime_error{"Flock must contain at least 2 boid"};
  } */
  assert(n > 1);

  sum_v_i2 = std::accumulate(flock.begin(), flock.end(), 0.,
                             [](double sum_v, Boid const &b_j) {
                               sum_v += (b_j.vel.norm() * b_j.vel.norm());
                               return sum_v;
                             });

  sum_tot += sum_v_i2;
  double res = sqrt((sum_v_i2 / (n - 1.)) - mean_v * mean_v);
  sum_v_i2 = 0.;
  return res;
}

Vector_2d sep(std::vector<Boid> const &flock, Boid const &b_i, double const s,
              double const d_s) {
  Vector_2d sum_v;

  std::for_each(flock.begin(), flock.end(), [&](Boid const &b_j) {
    if (distance(b_i, b_j) < d_s) {
      sum_v += (b_j.pos - b_i.pos);
    }
  });
  return -sum_v * s;
}

Vector_2d all(std::vector<Boid> const &flock, Boid const &b_i, double const a) {
  Vector_2d sum_v;
  double n = flock.size();
  // This exception checks if there is less than one boid in flock (the case
  // n=1. is managed in evolve)
  /* if (n <= 1.) {
    throw std::runtime_error{"Flock must contain at least 2 boid"};
  } */
  assert(n > 1);

  std::for_each(flock.begin(), flock.end(),
                [&](Boid const &b_j) { sum_v += b_j.vel; });

  Vector_2d mean_v = (sum_v - b_i.vel) * (1. / (n - 1.));
  return (mean_v - b_i.vel) * a;
}

Vector_2d coe(Boid const &b_i, Vector_2d const &c_m, double const c) {
  return (c_m - b_i.pos) * c;
}

Vector_2d pacman(Vector_2d &pos, Stats const &s) {
  if (pos.xcomp() < s.l_b) {
    pos.setx(s.r_b - abs(pos.xcomp() - s.l_b));
  }
  if (pos.xcomp() > s.r_b) {
    pos.setx(s.l_b + abs(pos.xcomp() - s.r_b));
  }
  if (pos.ycomp() > s.u_b) {
    pos.sety(s.b_b + abs(pos.ycomp()) - s.u_b);
  }
  if (pos.ycomp() < s.b_b) {
    pos.sety(s.u_b - abs(pos.ycomp() - s.b_b));
  }
  return pos;
}

std::vector<Boid> influence(std::vector<Boid> const &flock, Boid const &b_i,
                            double const d) {
  std::vector<Boid> range;
  for (Boid const &b_j : flock) {
    if (distance(b_i, b_j) < d) {
      range.push_back(b_j);
    }
  }
  return range;
}