#ifndef sfml_hpp
#define sfml_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "evolve.hpp"
#include "sfml_objects.hpp"

void run_simulation(std::vector<boid> flock, predator p, stats s) {
  unsigned const display_width = .8 * sf::VideoMode::getDesktopMode().width;
  unsigned const display_height = .8 * sf::VideoMode::getDesktopMode().height;

  sf::RenderWindow window(sf::VideoMode(display_width, display_height),
                          "Flock evolution");

  auto const min_x{s.l_b};
  auto const max_x{s.r_b};
  auto const min_y{s.b_b};
  auto const max_y{s.u_b};
  auto const scale_x = display_width / (max_x - min_x);
  auto const scale_y = display_height / (max_y - min_y);

  sf::Font font;
  font.loadFromFile("OpenSans-Regular.ttf");

  sf::Vector2f buttonPos(display_width * 0.08, display_height * 0.08);
  sf::Vector2f buttonSize(90, 60);
  sf::RectangleShape rect;
  sf::Text text;
  std::string str{"Get data"};

  button b1(buttonPos, buttonSize, rect, text, font);

  b1.setButtonColor(sf::Color::Blue);
  b1.setTextColor(sf::Color::White);
  b1.setTextContent(str);
  b1.setTextSize(15);
  b1.createButton();

  sf::Vector2f dataPos(display_width * 0.75, display_height * 0.08);
  sf::Text Tdata;
  double mean_d = 0.;
  double mean_v = 0.;
  double std_dev_d = 0.;
  double std_dev_v = 0.;

  data result(mean_d, mean_v, std_dev_d, std_dev_v, dataPos, font, Tdata);

  result.setSize(18);
  result.setColor(sf::Color::White);
  result.createData();

  sf::CircleShape boids{5.0f, 3};
  boids.setFillColor(sf::Color::White);

  sf::CircleShape predator{7.0, 4};
  predator.setFillColor(sf::Color::Red);

  while (window.isOpen()) {

    sf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
      default:
        break;
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
          if (p.vel.norm() == 0.) {
            p.vel.setx(5.);
          } else {
            p.vel.setx(0.);
            p.vel.sety(0.);
          }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
          p.vel = p.vel * 0.5;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
          p.vel = p.vel * 2.;
        }
        break;
      case sf::Event::MouseMoved:
        if (b1.hovering(sf::Mouse::getPosition(window))) {
          b1.setButtonColor(sf::Color::Red);
        }
        if (!b1.hovering(sf::Mouse::getPosition(window))) {
          b1.setButtonColor(sf::Color::Blue);
        }

        break;
      case sf::Event::MouseButtonPressed:
        if (b1.hovering(sf::Mouse::getPosition(window))) {
          result.setNewData(0., 0., mean_velocity(flock),
                            std_dev_velocity(flock));
          result.createData();
        }
        break;
      };
    }

    window.clear(sf::Color::Black);

    for (int i = 0; i != 1000 / 60; ++i) {
      evolve_flock(flock, 0.01, s, p);
      p = evolve_predator(p, 0.01, s);
    }

    for (auto &b_i : flock) {
      boids.setPosition(b_i.pos.xcomp() * scale_x, b_i.pos.ycomp() * scale_y);
      window.draw(boids);
    }

    predator.setPosition(p.pos.xcomp() * scale_x, p.pos.ycomp() * scale_y);

    window.draw(predator);

    window.draw(b1.getButton());

    window.draw(b1.getText());

    window.draw(result.getData());

    window.display();
  }
}

#endif