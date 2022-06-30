#ifndef sfml_hpp
#define sfml_hpp

#include "evolve.hpp"
#include "sfml_objects.hpp"

void run_simulation(std::vector<boid> flock, predator p, stats s) {

  // Parametri per la costruzione della finestra

  unsigned const display_width = .8 * sf::VideoMode::getDesktopMode().width;
  unsigned const display_height = .8 * sf::VideoMode::getDesktopMode().height;

  sf::RenderWindow window(sf::VideoMode(display_width, display_height),
                          "Flock evolution");

  window.setFramerateLimit(60);

  auto const min_x{s.l_b};
  auto const max_x{s.r_b};
  auto const min_y{s.b_b};
  auto const max_y{s.u_b};
  auto const scale_x = display_width / (max_x - min_x);
  auto const scale_y = display_height / (max_y - min_y);

  // Load del font

  sf::Font font;
  font.loadFromFile("OpenSans-Semibold.ttf");
  if (!font.loadFromFile("OpenSans-Semibold.ttf")) {
    throw std::runtime_error{"Cannot load font from file"};
  }

  // Definizioni per costruzione dell'oggetto data result

  sf::Vector2f dataPos(display_width * 0.75, display_height * 0.04);
  sf::Text Tdata;
  double mean_d = 0.;
  double mean_v = 0.;
  double std_dev_d = 0.;
  double std_dev_v = 0.;

  data result(mean_d, mean_v, std_dev_d, std_dev_v, dataPos, font, Tdata);

  result.setSize(18);
  result.setColor(sf::Color::White);
  result.createData();

  // Definizioni per costruzione dell'oggetto button b1

  sf::Vector2f buttonPos(result.getData().getGlobalBounds().left +
                             result.getData().getGlobalBounds().width / 1.5f,
                         result.getData().getGlobalBounds().top +
                             result.getData().getGlobalBounds().height *
                                 (1.2f));
  sf::Vector2f buttonSize(80, 40);
  sf::RectangleShape rect;
  sf::Text text;
  std::string str{"Get data"};

  button b1(buttonPos, buttonSize, rect, text, font);

  b1.setButtonColor(sf::Color::Blue);
  b1.setTextColor(sf::Color::White);
  b1.setTextContent(str);
  b1.setTextSize(18);
  b1.createButton();

  // Costruzione dei poligoni per rappresentare boids e predatore

  sf::CircleShape boids{5.0f, 3};
  boids.setFillColor(sf::Color::White);

  sf::CircleShape predator{7.0, 4};
  predator.setFillColor(sf::Color::Red);

  // Game loop

  while (window.isOpen()) {

    sf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {

      default:
        break;

      case sf::Event::Closed:
        window.close();
        break;

        // Se un pulsante della tastiera viene premuto si verifica il tipo di
        // pulsante e vengono applicate di conseguenza le funzioni per far
        // muovere il predatore

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

        // Quando il mouse viene mosso, se la sua posizione è all'interno del
        // pulsante, gli viene cambiato colore

      case sf::Event::MouseMoved:
        if (b1.hovering(sf::Mouse::getPosition(window))) {
          b1.setButtonColor(sf::Color::Red);
        }
        if (!b1.hovering(sf::Mouse::getPosition(window))) {
          b1.setButtonColor(sf::Color::Blue);
        }
        break;

        // Se viene premuto il pulsante del mouse mentre è all'interno del
        // pulsante vengono aggiornati i dati di result

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

    // Ogni frame flock e il predatore vengono aggiornati 1000/60 volte di 0.01
    // secondi, con 60 fps si ha che ogni secondo essi vengono aggiornati 1000
    // volte ovvero 10 secondi

    for (int i = 0; i != 1000 / 60; ++i) {
      evolve_flock(flock, 0.01, s, p);
      p = evolve_predator(p, 0.01, s);
    }

    // Viene assegnato un oggetto sf::CircleShape a ogni boid b_i del flock la
    // cui posizione è settata come quella dei boid stessi ma riscalata in base
    // alle dimensioni dello schermo. Viene poi disegnato l'oggetto
    // sf::CircleShape assegnato al boid i-esimo

    for (auto &b_i : flock) {
      boids.setPosition(b_i.pos.xcomp() * scale_x, b_i.pos.ycomp() * scale_y);
      window.draw(boids);
    }

    // Come per i boid viene assegnato un oggetto sf::CircleShape anche al
    // predatore

    predator.setPosition(p.pos.xcomp() * scale_x, p.pos.ycomp() * scale_y);

    // Vengono disegnati predatore e tutti gli oggetti per ottenere i dati
    // d'interesse

    window.draw(predator);
    window.draw(b1.getBox());
    window.draw(b1.getText());
    window.draw(result.getData());

    // La finestra con tutti gli oggetti disegnati viene renderizzata

    window.display();
  }
}

#endif