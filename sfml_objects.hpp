#ifndef sfml_objects_hpp
#define sfml_objects_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class button {
private:
  sf::Vector2f m_pos;
  sf::Vector2f m_size;
  sf::RectangleShape m_rect;
  sf::Text m_text;
  sf::Font m_font;

public:
  button(sf::Vector2f pos, sf::Vector2f size, sf::RectangleShape rect,
         sf::Text text, sf::Font font)
      : m_pos{pos}, m_size{size}, m_rect{rect}, m_text{text}, m_font{font} {}

  void setButtonColor(sf::Color color) {
    m_rect.setFillColor(color);
    return;
  }

  void setTextColor(sf::Color color) {
    m_text.setFillColor(color);
    return;
  }

  void setTextContent(std::string s) {
    m_text.setString(s);
    return;
  }

  void setTextSize(int size) {
    m_text.setCharacterSize(size);
    return;
  }

  void createButton() {
    m_rect.setOrigin(
        m_rect.getGlobalBounds().left + m_rect.getGlobalBounds().width / 2.f,
        m_rect.getGlobalBounds().top + m_rect.getGlobalBounds().height / 2.f);
    m_rect.setPosition(m_pos);
    m_rect.setSize(m_size);
    m_text.setOrigin(
        m_text.getGlobalBounds().left + m_text.getGlobalBounds().width / 2.f,
        m_text.getGlobalBounds().top + m_text.getGlobalBounds().height / 2.f);
    m_text.setPosition(m_pos);
    m_text.setFont(m_font);
    return;
  }

  sf::RectangleShape getButton() { return m_rect; }

  sf::Text getText() { return m_text; }

  bool hovering(sf::Vector2i mousePos) {
    if (mousePos.x > m_rect.getPosition().x &&
        mousePos.x < (m_rect.getPosition().x + m_rect.getSize().x) &&
        mousePos.y > m_rect.getPosition().y &&
        mousePos.y < (m_rect.getPosition().y + m_rect.getSize().y)) {
      return true;
    } else {
      return false;
    }
  }
};

class data {
private:
  double m_mean_d;
  double m_mean_v;
  double m_std_dev_d;
  double m_std_dev_v;
  sf::Vector2f m_pos;
  sf::Font m_font;
  sf::Text m_text;

public:
  data(double mean_d, double mean_v, double std_dev_d, double std_dev_v,
       sf::Vector2f pos, sf::Font font, sf::Text text)
      : m_mean_d{mean_d}, m_mean_v{mean_v}, m_std_dev_d{std_dev_d},
        m_std_dev_v{std_dev_v}, m_pos{pos}, m_font{font}, m_text{text} {}

  void setSize(int size) {
    m_text.setCharacterSize(size);
    return;
  }
  void setColor(sf::Color color) {
    m_text.setFillColor(color);
    return;
  }

  void setNewData(double a, double b, double c, double d) {
    m_mean_d = a;
    m_std_dev_d = b;
    m_mean_v = c;
    m_std_dev_v = d;
    return;
  }

  void createData() {
    m_text.setPosition(m_pos);
    m_text.setFont(m_font);
    std::string first{"                            Data \n Distance:  "};
    std::string Mean_d{std::to_string(m_mean_d)};
    std::string p_m{" +/- "};
    std::string std_devD{std::to_string(m_std_dev_d)};
    std::string second{" \n Velocity:  "};
    std::string Mean_v{std::to_string(m_mean_v)};
    std::string std_devV{std::to_string(m_std_dev_v)};
    std::string tot =
        first + Mean_d + p_m + std_devD + second + Mean_v + p_m + std_devV;

    m_text.setString(tot);
    return;
  }

  sf::Text getData() { return m_text; }
};

#endif