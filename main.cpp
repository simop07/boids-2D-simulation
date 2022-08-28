#include "cstdlib"
#include "sfml.hpp"

int main() {
  std::vector<Boid> flock;
  int n;
  double s;
  double a;
  double c;

  // Input control: boids' number
  while (std::cout << "Please, insert the number of boids (it must be an "
                      "integer greater than 2,\nand can be changed during the "
                      "simulation): " &&
         !(std::cin >> n)) {
    std::cin.clear();
    std::string input;
    std::getline(std::cin, input);
    std::cerr << "Input \"" << input << "\" is invalid.\n";

    // Ctrl+D control
    if (std::cin.eof()) {
      std::cerr << "Unable to read input. EOF reached.\n";
      return EXIT_FAILURE;
    }
  }

  // Progam is terminated if the number given is < 2 or if it is not an integer
  if (std::cin.fail() || n < 2) {
    throw std::runtime_error{
        "The chosen value must be an integer number greater than 2!"};
  }

  std::cout
      << "Please, insert parameters of separation, alignment and cohesion - "
         "respectevly.\nSuggested values are s = 0.07, a = 0.06, c = 0.015 "
         "(they must be positive real numbers,\nand can be changed during the "
         "simulation):\n";

  // Input control: parameter s
  while (std::cout << "Parameter s: " && !(std::cin >> s)) {
    std::cin.clear();
    std::string input;
    std::getline(std::cin, input);
    std::cerr << "Input \"" << input << "\" is invalid.\n";

    // Ctrl+D control
    if (std::cin.eof()) {
      std::cerr << "Unable to read input. EOF reached.\n";
      return EXIT_FAILURE;
    }
  }

  // Input control: parameter a
  while (std::cout << "Parameter a: " && !(std::cin >> a)) {
    std::cin.clear();
    std::string input;
    std::getline(std::cin, input);
    std::cerr << "Input \"" << input << "\" is invalid.\n";

    // Ctrl+D control
    if (std::cin.eof()) {
      std::cerr << "Unable to read input. EOF reached.\n";
      return EXIT_FAILURE;
    }
  }

  // Input control: parameter c
  while (std::cout << "Parameter c: " && !(std::cin >> c)) {
    std::cin.clear();
    std::string input;
    std::getline(std::cin, input);
    std::cerr << "Input \"" << input << "\" is invalid.\n";

    // Ctrl+D control
    if (std::cin.eof()) {
      std::cerr << "Unable to read input. EOF reached.\n";
      return EXIT_FAILURE;
    }
  }

  // Progam is terminated if parameters are negative (otherwise it would be
  // senseless to refer to flight rules when building the simulation)
  if (std::cin.fail() || s < 0 || a < 0 || c < 0) {
    throw std::runtime_error{
        "The parameters must be non-negative real numbers!"};
  }

  std::cout << "\nThe simulation is starting, press \"H\" for help...\n\n";
  Stats st{1., 20., s, a, c, 0., 100., 100., 0., 12.5, 10., 2.};

  // Gaussian generator for boids
  std::default_random_engine generator;
  std::normal_distribution<double> pos_d(st.u_b / 2., st.u_b * 2.);
  std::normal_distribution<double> vel_d(0., st.v_max / 2.);

  // Creating predator
  Predator p{{st.r_b / 4., st.u_b / 4.}, {0., 0.}};

  for (int i{}; i != n; ++i) {
    Boid b_i{{pos_d(generator), pos_d(generator)},
             {vel_d(generator), vel_d(generator)}};
    flock.push_back(b_i);
  }

  // sfml window is rendered
  run_simulation(flock, p, st);
}