#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>

#include "evolve.hpp"

void run_simulation(std::vector<boid> flock, std::vector<boid> flock2,
                    predator p, stats s, stats s2) {
  unsigned const display_width = .75 * sf::VideoMode::getDesktopMode().width;
  unsigned const display_height = .75 * sf::VideoMode::getDesktopMode().height;

  sf::RenderWindow window(sf::VideoMode(display_width, display_height),
                          "Flock evolution");

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
          p.vel.sety(p.vel.norm());
          p.vel.setx(0.);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
          p.vel.setx(p.vel.norm() * (-1.));
          p.vel.sety(0.);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
          p.vel.sety(p.vel.norm() * (-1));
          p.vel.setx(0.);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
          p.vel.setx(p.vel.norm());
          p.vel.sety(0.);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
          std::cout << mean_velocity(flock).norm() << " "
                    << std_dev_v(flock).norm() << '\n';
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
          p.vel = p.vel * 0.5;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
          p.vel = p.vel * 2.;
        }
        break;
      default:
        break;
      };
    }

    window.clear(sf::Color::Black);
    for (int i = 0; i != 1000 / 60; ++i) {
      evolve_flock(flock, 0.01, s, p);
      evolve_flock(flock2, 0.01, s2, p);
      p = evolve_predator(p, 0.01, s);
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