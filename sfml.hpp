#ifndef sfml_hpp
#define sfml_hpp

#include "evolve.hpp"
#include "sfml_objects.hpp"
#include <random>

void run_simulation(std::vector<boid> flock, predator p, stats s) {

  // Parameters needed to correctly create a window
  unsigned const display_width = .8 * sf::VideoMode::getDesktopMode().width;
  unsigned const display_height = .8 * sf::VideoMode::getDesktopMode().height;

  sf::RenderWindow window(sf::VideoMode(display_width, display_height),
                          "Flock evolution");

  window.setVerticalSyncEnabled(true);

  auto const min_x{s.l_b};
  auto const max_x{s.r_b};
  auto const min_y{s.b_b};
  auto const max_y{s.u_b};
  auto const scale_x = display_width / (max_x - min_x);
  auto const scale_y = display_height / (max_y - min_y);

  // Font loading
  sf::Font font;
  font.loadFromFile("OpenSans-Semibold.ttf");
  if (!font.loadFromFile("OpenSans-Semibold.ttf")) {
    throw std::runtime_error{"Cannot load font from file"};
  }

  // Gaussian generator of the boids
  std::default_random_engine generator;
  std::normal_distribution<double> pos_d(50., 40.);
  std::normal_distribution<double> vel_d(0., 10.);

  // Defining objects needed for the object data "result"
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

  // Button size
  sf::Vector2f buttonSize(80, 40);

  // Defining objects needed for the object button b1
  sf::Vector2f buttonPos1(0.02 * display_width, 0.02 * display_height);
  sf::RectangleShape rect1;
  sf::Text text1;
  std::string str1{"Add boid"};

  button b1(buttonPos1, buttonSize, rect1, text1, font);

  b1.setButtonColor(sf::Color::Yellow);
  b1.setTextColor(sf::Color::Black);
  b1.setTextContent(str1);
  b1.setTextSize(18);
  b1.setButtonOutline(sf::Color(8, 123, 3, 255), 2.f);
  b1.createButton();

  // Defining objects needed for the object button b2
  sf::Vector2f buttonPos2(buttonPos1.x,
                          buttonPos1.y + b1.getBox().getGlobalBounds().height);
  sf::RectangleShape rect2;
  sf::Text text2;
  std::string str2{"  Remove \n      boid"};

  button b2(buttonPos2, buttonSize, rect2, text2, font);

  b2.setButtonColor(sf::Color::Yellow);
  b2.setTextColor(sf::Color::Black);
  b2.setTextContent(str2);
  b2.setTextSize(16);
  b2.createButton();
  b2.setButtonOutline(sf::Color(8, 123, 3, 255), 2.f);

  // Defining objects needed for the object button b3
  sf::Vector2f buttonPos3(
      buttonPos1.x, buttonPos1.y + b1.getBox().getGlobalBounds().height + 50.f);
  sf::RectangleShape rect3;
  sf::Text text3;
  std::string str3{"    Pause\n evolution"};

  button b3(buttonPos3, buttonSize, rect3, text3, font);

  b3.setButtonColor(sf::Color::Yellow);
  b3.setTextColor(sf::Color::Black);
  b3.setTextContent(str3);
  b3.setTextSize(16);
  b3.createButton();
  b3.setButtonOutline(sf::Color(8, 123, 3, 255), 2.f);

  // Boids counter
  sf::Text counter;
  std::string str4{"N. Boids:\n"};
  std::string n_boids{std::to_string(flock.size())};
  std::string tot = str4 + n_boids;
  counter.setPosition(buttonPos1.x + b1.getBox().getGlobalBounds().width + 20.f,
                      buttonPos1.y);
  counter.setFont(font);
  counter.setFillColor(sf::Color::Black);
  counter.setCharacterSize(18);
  counter.setString(tot);

  // Poligons representing the boids and the predator
  sf::CircleShape boids{3.0f, 6};
  boids.setFillColor(sf::Color(234, 72, 18, 255));

  sf::CircleShape predator{7.0, 4};
  predator.setFillColor(sf::Color::Black);

  // Parameter used to keep track of how many frames have passed since the start
  // of the game loop
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

        // If a keyboard button is pressed, after verifying which button it was
        // the relative rules are applied
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

        // If the mouse pointer is moved, its position relative to the window
        // gets updated and relative rules are applied
      case sf::Event::MouseMoved:
        if (b1.hovering(
                window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
          b1.setButtonColor(sf::Color(8, 123, 3, 255));
        }
        if (!b1.hovering(
                window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
          b1.setButtonColor(sf::Color::Yellow);
        }
        if (b2.hovering(
                window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
          b2.setButtonColor(sf::Color(8, 123, 3, 255));
        }
        if (!b2.hovering(
                window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
          b2.setButtonColor(sf::Color::Yellow);
        }
        break;

        // If the mouse button is pressed while hovering a button the relative
        // rules are applied
      case sf::Event::MouseButtonPressed:
        if (b1.hovering(
                window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
          boid b_n{{pos_d(generator), pos_d(generator)},
                   {vel_d(generator), vel_d(generator)}};
          flock.push_back(b_n);
        }
        if (b2.hovering(
                window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
          if (flock.size() > 2) {
            flock.erase(flock.end() - 1);
          }
          if (flock.size() <= 2) {
            std::cout << "Cannot remove more boids" << '\n';
          }
        }
        if (b3.hovering(
                window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
          b3.buttonPressed();
          if (b3.buttonState()) {
            b3.setButtonColor(sf::Color(8, 123, 3, 255));
          }
          if (!b3.buttonState()) {
            b3.setButtonColor(sf::Color::Yellow);
          }
        }
        break;
      };
    }

    // The window with the objects of the previous frame is cleared
    window.clear(sf::Color(202, 227, 244, 255));

    // Every frame the flock and predator are evolved by 1/30 second, being the
    // simulation at 60 fps the ratio between real time and simulation time is
    // 1:2
    if (b3.buttonState()) {
      evolve_flock(flock, 1. / 30., s, p);
      p = evolve_predator(p, 1. / 30., s);
      eat_boid(flock, p, s.d_pred);
    }

    // Boids counter is updated
    std::string n_actual_boids{std::to_string(flock.size())};
    counter.setString(str4 + n_actual_boids);

    // Data gets updated every 60 frames
    ++i;
    if (i % 60 == 0) {
      result.setNewData(mean_distance(flock), std_dev_distance(flock),
                        mean_velocity(flock), std_dev_velocity(flock));
      result.createData();
    }

    // Every boid inside flock gets assigned a boids sf::CircleShape that are
    // drawn at the same position of the boid
    for (auto &b_i : flock) {
      boids.setPosition(b_i.pos.xcomp() * scale_x, b_i.pos.ycomp() * scale_y);
      window.draw(boids);
    }

    // Same as boids
    predator.setPosition(p.pos.xcomp() * scale_x, p.pos.ycomp() * scale_y);

    // Predator, buttons e data are drawn
    window.draw(predator);
    window.draw(b1.getBox());
    window.draw(b1.getText());
    window.draw(b2.getBox());
    window.draw(b2.getText());
    window.draw(b3.getBox());
    window.draw(b3.getText());
    window.draw(result.getData());
    window.draw(counter);

    // The window with every object is displayed
    window.display();
  }
}

#endif