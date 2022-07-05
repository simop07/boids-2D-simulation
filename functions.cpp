#include "functions.hpp"

double distance(boid b1, boid b2) {
  vector_2d pos_diff = b1.pos - (b2.pos);
  return pos_diff.norm();
};

vector_2d calc_c_m_b_i(std::vector<boid> const &flock, boid b_i) {
  vector_2d c_m;
  double n = flock.size();
  // Controlla che nel flock ci siano almeno 2 boid (il caso
  // n = 1. viene gestito in evolve_boid)
  if (n <= 1.) {
    throw std::runtime_error{"Flock must contain at least 2 boid"};
  }
  for (boid b_j : flock) {
    c_m += (b_j.pos * (1. / (n - 1.)));
  }
  c_m = c_m - (b_i.pos * (1. / (n - 1.)));
  return c_m;
}

// Le eccezioni nelle funzioni che restituiscono i dati richiesti servono per
// controllare che nel flock ci siano almeno 2 boid altrimenti non avrebbe senso
// calcolare velocità e distanze medie (a meno che non se ne inseriscano meno di
// due in input, il pulsante remove boid smette di funzionare una volta rimasti
// solo due boids)

double mean_distance(std::vector<boid> const &flock) {
  double n = flock.size();
  double sum_tot = 0.;
  double sum_par = 0.;
  if (n <= 1.) {
    throw std::runtime_error{"Flock must contain at least 2 boid"};
  }
  for (boid b_i : flock) {
    for (boid b_j : flock) {
      sum_par += distance(b_i, b_j);
    }
    sum_tot += sum_par;
    sum_par = 0.;
  }
  double res = sum_tot / (n * (n - 1.));
  sum_tot = 0.;
  sum_par = 0.;
  return res;
}

double std_dev_distance(std::vector<boid> const &flock) {
  double n = flock.size();
  double sum2 = 0.;
  double mean_d = mean_distance(flock);
  if (n <= 1.) {
    throw std::runtime_error{"Flock must contain at least 2 boid"};
  }
  for (boid b_i : flock) {
    double d_i = 0.;
    for (boid b_j : flock) {
      d_i += distance(b_i, b_j);
    }
    sum2 += ((d_i / (n - 1.)) - mean_d) * ((d_i / (n - 1.)) - mean_d);
    d_i = 0.;
  }
  double res = sqrt(sum2 / (n - 1.));
  sum2 = 0.;
  return res;
}

double mean_velocity(std::vector<boid> const &flock) {
  double n = flock.size();
  double sum_v = 0.;
  if (n <= 1.) {
    throw std::runtime_error{"Flock must contain at least 2 boid"};
  }
  for (boid b_i : flock) {
    sum_v += b_i.vel.norm();
  }
  double res = (sum_v * (1. / n));
  sum_v = 0.;
  return res;
}

double std_dev_velocity(std::vector<boid> const &flock) {
  double n = flock.size();
  double sum2 = 0.;
  double mean_v = mean_velocity(flock);
  if (n <= 1.) {
    throw std::runtime_error{"Flock must contain at least 2 boid"};
  }
  for (boid b_i : flock) {
    sum2 += (b_i.vel.norm() - mean_v) * (b_i.vel.norm() - mean_v);
  }
  double res = sqrt(sum2 / (n - 1.));
  sum2 = 0.;
  return res;
}

vector_2d sep(std::vector<boid> const &flock, boid b_i, double s, double d_s) {
  vector_2d v_sep;
  vector_2d sum_v;
  for (boid b_j : flock) {
    if (distance(b_i, b_j) < d_s) {
      sum_v += (b_j.pos - b_i.pos);
    }
  }
  return v_sep = sum_v * (s * (-1.));
}

vector_2d all(std::vector<boid> const &flock, boid b_i, double a) {
  vector_2d v_all;
  vector_2d sum_v;
  double n = flock.size();
  // Controlla che nel flock ci siano almeno 2 boid (il caso
  // n = 1. viene gestito in evolve_boid)
  if (n <= 1.) {
    throw std::runtime_error{"Flock must contain at least 2 boid"};
  }
  for (boid b_j : flock) {
    sum_v += b_j.vel;
  }
  vector_2d mean_v = (sum_v - b_i.vel) * (1. / (n - 1.));
  return v_all = (mean_v - b_i.vel) * a;
}

vector_2d coe(boid b_i, vector_2d c_m, double c) {
  vector_2d v_coe = (c_m - b_i.pos) * c;
  return v_coe;
}

vector_2d pacman(vector_2d pos, stats s) {
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

bool vision(boid b1, boid b2, double theta) {
  vector_2d distance = b2.pos - b1.pos;
  double alpha = b1.vel.angle(distance);
  if (alpha < (PI - theta / 2)) {
    return true;
  } else {
    return false;
  }
}

std::vector<boid> influence(std::vector<boid> const &flock, boid b_i, double d,
                            double theta) {
  std::vector<boid> range;
  for (boid b_j : flock) {
    if (distance(b_i, b_j) < d && vision(b_i, b_j, theta)) {
      range.push_back(b_j);
    }
  };
  return range;
}