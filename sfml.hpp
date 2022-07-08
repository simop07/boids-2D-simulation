#ifndef sfml_hpp
#define sfml_hpp

#include <random>

#include "evolve.hpp"
#include "sfml_objects.hpp"

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
  std::normal_distribution<double> pos_d(s.u_b / 2., s.u_b * 2.);
  std::normal_distribution<double> vel_d(0., s.v_max);

  // Defining objects needed for the object data "result"
  sf::Vector2f dataPos(display_width * 0.74, display_height * 0.02);
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

  // Stats display
  sf::Text stats;
  stats.setFont(font);
  stats.setPosition(0.02 * display_width, 0.83 * display_height);
  stats.setCharacterSize(18);
  stats.setFillColor(sf::Color::Black);

  // Poligons representing the boids and the predator
  sf::CircleShape boids{3.0f, 6};
  boids.setFillColor(sf::Color(234, 72, 18, 255));

  sf::CircleShape predator{7.0, 4};
  predator.setFillColor(sf::Color::Black);

  // Starting the clock needed to evolve the simulation and update data
  sf::Clock generalClock;
  sf::Clock clock;
  sf::Clock clock2;

  // Parameter used to change stats
  int changing_stats = 0;

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

          // If a keyboard button is pressed, after verifying which button it
          // was the relative rules are applied
        case sf::Event::KeyPressed:
          if (event.key.code == sf::Keyboard::Down) {
            p.vel.sety(p.vel.norm());
            p.vel.setx(0.);
          }
          if (event.key.code == sf::Keyboard::Left) {
            p.vel.setx(p.vel.norm() * (-1.));
            p.vel.sety(0.);
          }
          if (event.key.code == sf::Keyboard::Up) {
            p.vel.sety(p.vel.norm() * (-1));
            p.vel.setx(0.);
          }
          if (event.key.code == sf::Keyboard::Right) {
            p.vel.setx(p.vel.norm());
            p.vel.sety(0.);
          }
          if (event.key.code == sf::Keyboard::Space) {
            if (p.vel.norm() == 0.) {
              p.vel.setx(5.);
            } else {
              p.vel.setx(0.);
              p.vel.sety(0.);
            }
          }
          if (event.key.code == sf::Keyboard::LShift) {
            p.vel = p.vel * 0.5;
          }
          if (event.key.code == sf::Keyboard::LControl) {
            p.vel = p.vel * 2.;
          }
          if (event.key.code == sf::Keyboard::H) {
            std::cout
                << '\n'
                << "PREDATOR:\n\n'SpaceBar' to start/stop the "
                   "predator\n'Lshift' to slow down the predator\n'LCtrl' "
                   "to increase the speed of the "
                   "predator\n'Up/Down/Left/Right' "
                   "to change the predator's "
                   "direction\n(the predator can only eat after its color "
                   "turns "
                   "blue)\n\nBUTTONS:\n\npress 'Add/Remove boid' to "
                   "generate "
                   "or "
                   "remove a boid from the simulation\npress 'Pause evolution' "
                   "to pause the simulation\n\nSTATS:\n\npress 's/a/c' to "
                   "select the stats you want to change, then press "
                   "'Enter/Backspace' to increse/reduce it by 0.05"
                << '\n';
          }
          if (event.key.code == sf::Keyboard::S) {
            changing_stats = 1;
          }
          if (event.key.code == sf::Keyboard::A) {
            changing_stats = 2;
          }
          if (event.key.code == sf::Keyboard::C) {
            changing_stats = 3;
          }
          if (event.key.code == sf::Keyboard::Enter) {
            if (changing_stats == 1) {
              s.s += 0.05;
            }
            if (changing_stats == 2) {
              s.a += 0.05;
            }
            if (changing_stats == 3) {
              s.c += 0.05;
            }
          }
          if (event.key.code == sf::Keyboard::Backspace) {
            if (changing_stats == 1 && s.s > 0.05) {
              s.s -= 0.05;
            }
            if (changing_stats == 2 && s.a > 0.05) {
              s.a -= 0.05;
            }
            if (changing_stats == 3 && s.c > 0.05) {
              s.c -= 0.05;
            }
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

    // The clock is restarded registering how much time has passed during a
    // frame, then converted to a double usable in the evolve functions
    sf::Time el_time = clock.restart();
    double d_el_time = static_cast<double>(el_time.asSeconds());

    // Every frame the flock and predator are evolved by the time elapsed during
    // a frame (if 60 fps then 1/60 seconds) times 5, that means every second of
    // real time has passed, the simulation has evolved by 5 seconds. The
    // relation between the parameters s and d_eat (d_eat = s * (1.5/0.7) are
    // the result of a series of tests done during developement
    if (!b3.buttonState()) {
      evolve_flock(flock, d_el_time * 5., s, p);
      p = evolve_predator(p, d_el_time * 5., s);
      if (static_cast<int>(generalClock.getElapsedTime().asSeconds()) > 2) {
        eat_boid(flock, p, (s.s * (1.5 / 0.7)));
        predator.setFillColor(sf::Color::Blue);
      }
    }

    // Boids counter is updated
    std::string n_actual_boids{std::to_string(flock.size())};
    counter.setString(str4 + n_actual_boids);

    // Stats are updated
    std::string a;
    std::string c;
    std::string e;
    switch (changing_stats) {
      case 1:
        a = {"Stats:\n->s = "};
        c = {"\na = "};
        e = {"\nc = "};
        break;
      case 2:
        a = {"Stats:\ns = "};
        c = {"\n->a = "};
        e = {"\nc = "};
        break;
      case 3:
        a = {"Stats:\ns = "};
        c = {"\na = "};
        e = {"\n->c = "};
        break;
      default:
        a = {"Stats:\ns = "};
        c = {"\na = "};
        e = {"\nc = "};
        break;
    }
    std::string b = std::to_string(s.s);
    std::string d = std::to_string(s.a);
    std::string f = std::to_string(s.c);
    stats.setString(a + b + c + d + e + f);

    // Data gets updated every 2 seconds, this time the clock is not
    // restarded otherwise it would always get a time shorter than 2
    // seconds(since a frame last less than 2 seconds)
    sf::Time el_time2 = clock2.getElapsedTime();
    int i_el_time2 = static_cast<int>(el_time2.asSeconds());

    if (i_el_time2 % 3 == 2) {
      result.setNewData(mean_distance(flock), std_dev_distance(flock),
                        mean_velocity(flock), std_dev_velocity(flock));
      result.createData();
      clock2.restart();
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
    window.draw(stats);

    // The window with every object is displayed
    window.display();
  }
}

#endif