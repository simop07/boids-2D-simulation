#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>

#include "sfml.hpp"

int main() {
  std::vector<boid> flock;
  stats s{1., 8., 0.6, 0.9, 0.3, 0., 100., 100., 0., 0., 10., 100.};
  std::default_random_engine generator;
  std::normal_distribution<double> pos_d(50., 40.);
  std::normal_distribution<double> vel_d(0., 10.);

  predator p{{20., 30.}, {5., 0.}};

  for (int i = 0; i < 30; ++i) {
    boid b_i{{pos_d(generator), pos_d(generator)},
             {vel_d(generator), vel_d(generator)}};
    flock.push_back(b_i);
  }

  run_simulation(flock, p, s);
}
