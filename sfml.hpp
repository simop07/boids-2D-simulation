#ifndef sfml_hpp
#define sfml_hpp

#include "evolve.hpp"
#include "sfml_objects.hpp"
#include <random>

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

  // Generazione boid casuali

  std::default_random_engine generator;
  std::normal_distribution<double> pos_d(50., 40.);
  std::normal_distribution<double> vel_d(0., 10.);

  // Definizioni per costruzione dell'oggetto data result

  sf::Vector2f dataPos(display_width * 0.75, display_height * 0.02);
  sf::Text Tdata;
  double mean_d = 0.;
  double mean_v = 0.;
  double std_dev_d = 0.;
  double std_dev_v = 0.;

  data result(mean_d, mean_v, std_dev_d, std_dev_v, dataPos, font, Tdata);

  result.setSize(18);
  result.setColor(sf::Color::Black);
  result.createData();

  // Grandezza dei pulsanti

  sf::Vector2f buttonSize(80, 40);

  // Definizioni per costruzione dell'oggetto button b2

  sf::Vector2f buttonPos2(0.02 * display_width, 0.02 * display_height);
  sf::RectangleShape rect2;
  sf::Text text2;
  std::string str2{"Add boid"};

  button b2(buttonPos2, buttonSize, rect2, text2, font);

  b2.setButtonColor(sf::Color::Yellow);
  b2.setTextColor(sf::Color::Black);
  b2.setTextContent(str2);
  b2.setTextSize(18);
  b2.setButtonOutline(sf::Color(8, 123, 3, 255), 2.f);
  b2.createButton();

  // Definizioni per costruzione dell'oggetto b3

  sf::Vector2f buttonPos3(buttonPos2.x,
                          buttonPos2.y + b2.getBox().getGlobalBounds().height);
  sf::RectangleShape rect3;
  sf::Text text3;
  std::string str3{"  Remove \n      boid"};

  button b3(buttonPos3, buttonSize, rect3, text3, font);

  b3.setButtonColor(sf::Color::Yellow);
  b3.setTextColor(sf::Color::Black);
  b3.setTextContent(str3);
  b3.setTextSize(16);
  b3.createButton();
  b3.setButtonOutline(sf::Color(8, 123, 3, 255), 2.f);

  // Definizioni per costruzione dell'oggetto b4

  sf::Vector2f buttonPos4(
      buttonPos2.x, buttonPos2.y + b2.getBox().getGlobalBounds().height + 50.f);
  sf::RectangleShape rect4;
  sf::Text text4;
  std::string str4{"    Pause\n evolution"};

  button b4(buttonPos4, buttonSize, rect4, text4, font);

  b4.setButtonColor(sf::Color::Yellow);
  b4.setTextColor(sf::Color::Black);
  b4.setTextContent(str4);
  b4.setTextSize(16);
  b4.createButton();
  b4.setButtonOutline(sf::Color(8, 123, 3, 255), 2.f);

  // Contatore di boid

  sf::Text counter;
  std::string str5{"N. Boids:\n"};
  std::string n_boids{std::to_string(flock.size())};
  std::string tot = str5 + n_boids;
  counter.setPosition(buttonPos2.x + b2.getBox().getGlobalBounds().width + 20.f,
                      buttonPos2.y);
  counter.setFont(font);
  counter.setFillColor(sf::Color::Black);
  counter.setCharacterSize(18);
  counter.setString(tot);

  // Costruzione dei poligoni per rappresentare boids e predatore

  sf::CircleShape boids{3.0f, 6};
  boids.setFillColor(sf::Color(234, 72, 18, 255));

  sf::CircleShape predator{7.0, 4};
  predator.setFillColor(sf::Color::Black);

  // Parametro utile per tenere conto dei frame passati da quando parte il game
  // loop

  int i = 0.;

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

        // Quando il mouse viene mosso, se la sua posizione è all'interno dei
        // pulsanti, gli vengono cambiati i colori

      case sf::Event::MouseMoved:

        if (b2.hovering(
                window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
          b2.setButtonColor(sf::Color(8, 123, 3, 255));
        }
        if (!b2.hovering(
                window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
          b2.setButtonColor(sf::Color::Yellow);
        }
        if (b3.hovering(
                window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
          b3.setButtonColor(sf::Color(8, 123, 3, 255));
        }
        if (!b3.hovering(
                window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
          b3.setButtonColor(sf::Color::Yellow);
        }
        break;

        // Se viene premuto il pulsante del mouse mentre è all'interno di
        // b1 vengono aggiornati i dati di result, nell'ordine distanza
        // media, dev. standard della distanza, velocità media, dev standard
        // velocità, all'interno di b2 viene aggiunto un boid al flock con
        // velocità e posizione casuali, all'interno di b3 viene rimosso
        // l'ultimo boid del vettore flock

      case sf::Event::MouseButtonPressed:
        if (b2.hovering(
                window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
          boid b_n{{pos_d(generator), pos_d(generator)},
                   {vel_d(generator), vel_d(generator)}};
          flock.push_back(b_n);
        }
        if (b3.hovering(
                window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
          if (flock.size() > 2) {
            flock.erase(flock.end() - 1);
          }
          if (flock.size() <= 2) {
            std::cout << "Cannot remove more boids" << '\n';
          }
        }
        if (b4.hovering(
                window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
          b4.buttonPressed();
          if (b4.buttonState()) {
            b4.setButtonColor(sf::Color(8, 123, 3, 255));
          }
          if (!b4.buttonState()) {
            b4.setButtonColor(sf::Color::Yellow);
          }
        }
        break;
      };
    }

    window.clear(sf::Color(202, 227, 244, 255));

    // Ogni frame flock e il predatore vengono aggiornati 1000/60 volte di 0.001
    // secondi, con 60 fps si ha che ogni secondo essi vengono aggiornati 1000
    // volte ovvero 1 secondo, solo se il pulsante Pause evolution (b4) non è
    // attivo

    if (!b4.buttonState()) {
      for (int i = 0; i != 1000 / 60; ++i) {
        evolve_flock(flock, 0.001, s, p);
        p = evolve_predator(p, 0.001, s);
        eat_boid(flock, p, s.d_pred);
      }
    }

    // Aggiornamento del counter dei boids

    std::string n_actual_boids{std::to_string(flock.size())};
    counter.setString(str5 + n_actual_boids);

    // Aggiornamento dati

    ++i;
    if (i % 60 == 0) {
      result.setNewData(mean_distance(flock), std_dev_distance(flock),
                        mean_velocity(flock), std_dev_velocity(flock));
      result.createData();
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
    window.draw(b2.getBox());
    window.draw(b2.getText());
    window.draw(b3.getBox());
    window.draw(b3.getText());
    window.draw(b4.getBox());
    window.draw(b4.getText());
    window.draw(result.getData());
    window.draw(counter);

    // La finestra con tutti gli oggetti disegnati viene renderizzata

    window.display();
  }
}

#endif