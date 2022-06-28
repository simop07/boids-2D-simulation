#ifndef functions_hpp
#define functions_hpp

#include "objects.hpp"

#include <vector>

// Funzione che calcola la distanza fra due boid

double distance(boid b1, boid b2) {
  vector_2d pos_diff = b1.pos - (b2.pos);
  return pos_diff.norm();
};

// Funzione che calcola il centro di massa dei boid

vector_2d calc_c_m_b_i(std::vector<boid> const &flock, boid b_i) {
  vector_2d c_m;
  double n = flock.size();
  for (boid b_j : flock) {
    c_m += (b_j.pos * (1. / (n - 1.)));
  }
  c_m = c_m - (b_i.pos * (1. / (n - 1.)));
  return c_m;
}

// Velocità media

vector_2d mean_velocity(std::vector<boid> const &flock) {
  double n = flock.size();
  vector_2d m_vel{0., 0.};
  for (boid b_i : flock) {
    m_vel += b_i.vel;
  }
  return m_vel * (1. / n);
}

// Deviazione standard velocità

vector_2d std_dev_v(std::vector<boid> const &flock) {
  vector_2d mean_v = mean_velocity(flock);
  vector_2d sum;
  double n = flock.size();
  for (boid b_j : flock) {
    sum += (b_j.vel - mean_v).prod(b_j.vel - mean_v);
  }
  vector_2d res(sqrt(sum.xcomp() / n - 1.), sqrt(sum.ycomp() / n - 1.));
  return res;
}

// Velocità di separazione

vector_2d sep(std::vector<boid> const &flock, boid b_i, double s, double d_s) {
  vector_2d v_sep;
  vector_2d sum_v;
  for (boid b_j : flock) {
    if (distance(b_i, b_j) < d_s) {
      sum_v += (b_j.pos - b_i.pos);
    }
  }
  return v_sep = sum_v * (-s);
}

// Velocità di allineamento

vector_2d all(std::vector<boid> const &flock, boid b_i, double a) {
  vector_2d v_all;
  vector_2d sum_v;
  double n = flock.size();
  for (boid b_j : flock) {
    sum_v += b_j.vel;
  }
  vector_2d mean_v = (sum_v - b_i.vel) * (1. / (n - 1.));
  return v_all = (mean_v - b_i.vel) * a;
}

// Velocità di coesione

vector_2d coe(boid b_i, vector_2d c_m, double c) {
  vector_2d v_coe = (c_m - b_i.pos) * c;
  return v_coe;
}

// Angolo cieco

bool vision(boid b1, boid b2, double theta) {
  vector_2d distance = b2.pos - b1.pos;
  double alpha = b1.vel.angle(distance);
  if (alpha < (PI - theta / 2)) {
    return true;
  } else {
    return false;
  }
}

// La funzione influence prende in input un vettore flock con tutti i boid del
// piano e restituisce un vettore range con solo i boids nel range di influenza
// di un certo boid b_i del piano

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

#endif