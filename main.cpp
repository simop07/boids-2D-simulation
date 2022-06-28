#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>
#include <random>

#include "evolve.hpp"

int main() {
  unsigned const display_width = .75 * sf::VideoMode::getDesktopMode().width;
  unsigned const display_height = .75 * sf::VideoMode::getDesktopMode().height;
  std::vector<boid> flock;
  std::vector<boid> flock2;
  stats s{1., 8., 0.6, 0.9, 0.3, 0., 100., 100., 0., 0., 10.};
  stats s2{.5, 3., 0.9, 0.6, 0.2, 0., 100., 100., 0., 0., 10.};
  std::default_random_engine generator;
  std::normal_distribution<double> pos_d(50., 10.);
  std::normal_distribution<double> vel_d(0., 10.);

  predator p{{20., 30.}, {10., 10.}};

  for (int i = 0; i < 100; ++i) {
    boid b_i{{pos_d(generator), pos_d(generator)},
             {vel_d(generator), vel_d(generator)}};
    flock.push_back(b_i);
  }

  for (int i = 0; i < 100; ++i) {
    boid b_i{{pos_d(generator), pos_d(generator)},
             {vel_d(generator), vel_d(generator)}};
    flock2.push_back(b_i);
  }

  sf::RenderWindow window(sf::VideoMode(display_width, display_height),
                          "Flock evolution");
  window.setFramerateLimit(60);
  auto const min_x{s.l_b};
  auto const max_x{s.r_b};
  auto const min_y{s.b_b};
  auto const max_y{s.u_b};
  auto const scale_x = display_width / (max_x - min_x);
  auto const scale_y = display_height / (max_y - min_y);

  sf::CircleShape triang{5.0f, 3};
  triang.setFillColor(sf::Color::White);

  sf::CircleShape exag{5.0f, 6};
  exag.setFillColor(sf::Color::Cyan);

  sf::CircleShape quad{7.0, 4};
  quad.setFillColor(sf::Color::Red);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
      case sf::Event::Closed:
        window.close();
        break;
      case sf::Event::KeyPressed:
        for (boid b_j : flock) {
          std::cout << mean_velocity(flock).norm() << '\n';
        }
        break;
        /* case sf::Event::MouseMoved:
           sf::Vector2i mousePos = sf::Mouse::getPosition();
           vector_2d newV(mousePos.x - p.pos.xcomp(), mousePos.y -
           p.pos.ycomp()); p.vel = newV;*/
      };
    }
    window.clear(sf::Color::Black);
    for (int i = 0; i != 100 / 60; ++i) {
      evolve_flock(flock, 0.01, s, p);
      evolve_flock(flock2, 0.01, s2, p);
      p.pos += p.vel * 0.01;
      if (p.pos.xcomp() < s.l_b) {
        p.pos.setx(s.r_b - abs(s.l_b - p.pos.xcomp()));
      };
      if (p.pos.xcomp() > s.r_b) {
        p.pos.setx(s.l_b + abs(p.pos.xcomp() - s.r_b));
      };
      if (p.pos.ycomp() > s.u_b) {
        p.pos.sety(s.b_b + abs(p.pos.ycomp() - s.u_b));
      };
      if (p.pos.ycomp() < s.b_b) {
        p.pos.sety(s.u_b - abs(s.b_b - p.pos.ycomp()));
      };
    }

    for (auto &b_i : flock2) {
      exag.setPosition(b_i.pos.xcomp() * scale_x, b_i.pos.ycomp() * scale_y);
      window.draw(exag);
    }
    for (auto &b_i : flock) {
      triang.setPosition(b_i.pos.xcomp() * scale_x, b_i.pos.ycomp() * scale_y);
      window.draw(triang);
    }

    quad.setPosition(p.pos.xcomp() * scale_x, p.pos.ycomp() * scale_y);
    window.draw(quad);

    window.display();
  }
}
