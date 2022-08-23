#include "functions.hpp"

#include <numeric>

double distance(Boid const &b1, Boid const &b2) {
  Vector_2d pos_diff{b1.pos - b2.pos};
  return pos_diff.norm();
};

Vector_2d calc_c_m_b_i(std::vector<Boid> const &flock, Boid const &b_i) {
  double n = flock.size();

  // This assert checks if there is less than one boid in flock (the case
  // n=1. is managed in evolve)

  /* assert(flock.size() > 1); */

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
double mean_distance(std::vector<Boid> const &flock) {
  double n = flock.size();
  double sum_tot{};

  /* double sum_par{}; */
  /* if (n <= 1.) {
   throw std::runtime_error{"Flock must contain at least 2 boid"};
 } */

  for (auto pr = flock.begin(); pr != flock.end(); ++pr) {
    auto nx = std::next(pr);
    for (; nx != flock.end(); ++nx) {
      sum_tot += distance(*pr, *nx);
    }
  }

  // Number of distances equals the combinations of n boid, taken at groups of
  // two
  double c_n_2{(n * (n - 1.) / 2.)};
  double res = sum_tot / c_n_2;
  /* Vedi se devi togliere degli zeri */
  sum_tot = 0.;
  /*   sum_par = 0.;
   */
  return res;
}

double std_dev_distance(std::vector<Boid> const &flock) {
  double n = flock.size();
  double sum_tot{0.};
  double mean_d = mean_distance(flock);
  /* if (n <= 1.) {
    throw std::runtime_error{"Flock must contain at least 2 boid"};
  } */

  for (auto pr = flock.begin(); pr != flock.end(); ++pr) {
    auto nx = std::next(pr);
    auto sum_d_i2{0};
    for (; nx != flock.end(); ++nx) {
      sum_d_i2 += ((distance(*pr, *nx)) * (distance(*pr, *nx)));
    }
    sum_tot += sum_d_i2;
    sum_d_i2 = 0;
  }
  double c_n_2{(n * (n - 1.) / 2.)};
  double res = sqrt((sum_tot / (c_n_2 - 1.)) - mean_d * mean_d);
  sum_tot = 0.;
  return res;
}

double mean_velocity(std::vector<Boid> const &flock) {
  double n = flock.size();
  double sum_v = 0.;
  if (n <= 1.) {
    throw std::runtime_error{"Flock must contain at least 2 boid"};
  }
  for (Boid b_i : flock) {
    sum_v += b_i.vel.norm();
  }
  double res = (sum_v * (1. / n));
  sum_v = 0.;
  return res;
}

double std_dev_velocity(std::vector<Boid> const &flock) {
  double n = flock.size();
  double sum2 = 0.;
  double mean_v = mean_velocity(flock);
  if (n <= 1.) {
    throw std::runtime_error{"Flock must contain at least 2 boid"};
  }
  for (Boid b_i : flock) {
    sum2 += (b_i.vel.norm() - mean_v) * (b_i.vel.norm() - mean_v);
  }
  double res = sqrt(sum2 / (n - 1.));
  sum2 = 0.;
  return res;
}

Vector_2d sep(std::vector<Boid> const &flock, Boid b_i, double s, double d_s) {
  Vector_2d v_sep;
  Vector_2d sum_v;
  for (Boid b_j : flock) {
    if (distance(b_i, b_j) < d_s) {
      sum_v += (b_j.pos - b_i.pos);
    }
  }
  return v_sep = sum_v * (s * (-1.));
}

Vector_2d all(std::vector<Boid> const &flock, Boid b_i, double a) {
  Vector_2d v_all;
  Vector_2d sum_v;
  double n = flock.size();
  // This exception checks if there is less than one boid in flock (the case
  // n=1. is managed in evolve)
  if (n <= 1.) {
    throw std::runtime_error{"Flock must contain at least 2 boid"};
  }
  for (Boid b_j : flock) {
    sum_v += b_j.vel;
  }
  Vector_2d mean_v = (sum_v - b_i.vel) * (1. / (n - 1.));
  return v_all = (mean_v - b_i.vel) * a;
}

Vector_2d coe(Boid b_i, Vector_2d c_m, double c) {
  Vector_2d v_coe = (c_m - b_i.pos) * c;
  return v_coe;
}

Vector_2d pacman(Vector_2d pos, stats s) {
  if (pos.xcomp() < s.l_b) {
    pos.setx(s.r_b - abs(s.l_b - pos.xcomp()));
  };
  if (pos.xcomp() > s.r_b) {
    pos.setx(s.l_b + abs(pos.xcomp() - s.r_b));
  };
  if (pos.ycomp() > s.u_b) {
    pos.sety(s.b_b + abs(pos.ycomp() - s.u_b));
  };
  if (pos.ycomp() < s.b_b) {
    pos.sety(s.u_b - abs(s.b_b - pos.ycomp()));
  };
  return pos;
}

std::vector<Boid> influence(std::vector<Boid> const &flock, Boid b_i,
                            double d) {
  std::vector<Boid> range;
  for (Boid b_j : flock) {
    if (distance(b_i, b_j) < d) {
      range.push_back(b_j);
    }
  };
  return range;
}