#ifndef functions_hpp
#define functions_hpp

#include "objects.hpp"
#include <stdexcept>
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
  if (n == 1 || n == 0) {
    throw std::runtime_error{"Not enough boids"};
  }
  for (boid b_j : flock) {
    c_m += (b_j.pos * (1. / (n - 1.)));
  }
  c_m = c_m - (b_i.pos * (1. / (n - 1.))); // perche nella formula j e' diverso da i 
  return c_m; 
}



// Distanza media

double mean_distance(std::vector<boid> const &flock) {
  double n = flock.size();
  double sum_tot = 0.;
  double sum_par = 0.;
  for (boid b_i : flock) { //loop triangolare
    for (boid b_j : flock) {
      sum_par += distance(b_i, b_j); // per ogniu boid calcola la distanza tra gli altri 
    }
    sum_tot += sum_par;
    sum_par = 0.;
  }
  double res = sum_tot / (n * (n - 1.)); //ogni bois ha n-1 distanze, e ho n boids 
  sum_tot = 0.;
  sum_par = 0.; // se non diventavano zero non funzionava la funzione in sfml.hpp
  return res;
}

// Deviazione standard distanza

double std_dev_distance(std::vector<boid> const &flock) {
  double n = flock.size();
  double sum2 = 0.;
  double mean_d = mean_distance(flock);
  for (boid b_i : flock) {
    double d_i = 0.;
    for (boid b_j : flock) {
      d_i += distance(b_i, b_j);
    }
    sum2 += ((d_i / (n - 1.)) - mean_d) * ((d_i / (n - 1.)) - mean_d); // scarti al quadrato
    d_i = 0.;
  }
  double res = sqrt(sum2 / (n - 1.));
  sum2 = 0.;
  return res;
}

// Velocità media

double mean_velocity(std::vector<boid> const &flock) {
  double n = flock.size();
  double sum_v = 0.;
  for (boid b_i : flock) {
    sum_v += b_i.vel.norm();
  }
  double res = (sum_v * (1. / n));
  sum_v = 0.;
  return res;
}

// Deviazione standard velocità

double std_dev_velocity(std::vector<boid> const &flock) {
  double n = flock.size();
  double sum2 = 0.;
  double mean_v = mean_velocity(flock);
  for (boid b_i : flock) {
    sum2 += (b_i.vel.norm() - mean_v) * (b_i.vel.norm() - mean_v);
  }
  double res = sqrt(sum2 / (n - 1.));
  sum2 = 0.;
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
  return v_sep = sum_v * (s * (-1.));
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
  if (n == 1 || n == 0) {
    throw std::runtime_error{ "Not enough boids" };
  }
  return v_all = (mean_v - b_i.vel) * a;// per evitare quella del boi i-esimo 
}

// Velocità di coesione

vector_2d coe(boid b_i, vector_2d c_m, double c) {
  vector_2d v_coe = (c_m - b_i.pos) * c;
  return v_coe;
}

// Effetto pac-man

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

// Angolo cieco

bool vision(boid b1, boid b2, double theta) { // doesn't work
  vector_2d distance = b2.pos - b1.pos;
  double alpha = b1.vel.angle(distance);
  if (alpha < (PI - theta / 2)) {
    return true;
  } else {
    return false;
  }
}

// La funzione influence prende in input un vettore flock con tutti i boid del
// piano e restituisce un vettore range con solo i boids nel range di
// influenza di un certo boid b_i del piano

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