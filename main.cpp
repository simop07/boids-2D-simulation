#include "sfml.hpp"

int main() {
  std::vector<Boid> flock;
  int n;
  double s;
  double a;
  double c;

  // Parameteres input
  std::cout << "Insert number of boids (must be greater than 2, can be changed "
               "during the simulation):"
            << '\n';
  std::cin >> n;
  if (std::cin.fail() || n < 2) {
    throw std::runtime_error{
        "The chosen value must be a number greater than 2!"};
  }
  std::cout << "Insert parameters s, a and c respectevly, the suggested values "
               "are s = 0.7, a = 0.02, c = 0.01\n(can be changed during the "
               "simulation): "
            << '\n';
  std::cin >> s >> a >> c;
  if (std::cin.fail()) {
    throw std::runtime_error{"The chosen values must be real numbers!"};
  }
  std::cout << "The simulation is starting, press 'H' for help..." << '\n';
  stats st{1., 10., s, a, c, 0., 100., 100., 0., 5., 50.};

  // Gaussian generator for the boids
  std::default_random_engine generator;
  std::normal_distribution<double> pos_d(st.u_b / 2., st.u_b * 2.);
  std::normal_distribution<double> vel_d(0., st.v_max / 2.);

  Predator p{{st.r_b / 4., st.u_b / 4.}, {0., 0.}};

  for (int i = 0; i < n; ++i) {
    Boid b_i{{pos_d(generator), pos_d(generator)},
             {vel_d(generator), vel_d(generator)}};
    flock.push_back(b_i);
  }

  // sfml window is rendered
  run_simulation(flock, p, st);
}
