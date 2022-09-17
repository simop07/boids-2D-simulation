#include "sfml_objects.hpp"

// Button class

void Button::setButtonColor(sf::Color const& color) {
  m_rect.setFillColor(color);
}

void Button::setButtonOutline(sf::Color const& color, float thickness) {
  m_rect.setOutlineColor(color);
  m_rect.setOutlineThickness(thickness);
}

void Button::setTextColor(sf::Color const& color) {
  m_text.setFillColor(color);
}

void Button::setTextContent(std::string const& s) { m_text.setString(s); }

void Button::setTextSize(int size) { m_text.setCharacterSize(size); }

void Button::createButton() {
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
}

sf::RectangleShape Button::getBox() const { return m_rect; }

sf::Text Button::getText() const { return m_text; }

bool Button::hovering(sf::Vector2f const& mousePos) {
  sf::Vector2f mousePosF{(mousePos.x), (mousePos.y)};
  if (m_rect.getGlobalBounds().contains(mousePosF)) {
    return true;
  } else {
    return false;
  }
}

bool Button::buttonState() const { return isOn; }

bool Button::buttonPressed() {
  return (isOn == true) ? (isOn = false) : (isOn = true);
}

// Data class

void Data::setSize(int size) { m_text.setCharacterSize(size); }

void Data::setColor(sf::Color const& color) { m_text.setFillColor(color); }

void Data::setNewData(double a, double b, double c, double d) {
  m_mean_d = a;
  m_std_dev_d = b;
  m_mean_v = c;
  m_std_dev_v = d;
}

void Data::createData() {
  m_text.setPosition(m_pos);
  m_text.setFont(m_font);
  std::string first{"Data \t\tMean\t\t\t\tStd. Dev. \nDistance:  "};
  std::string Mean_d{std::to_string(m_mean_d)};
  std::string p_m{"  +/-  "};
  std::string std_devD{std::to_string(m_std_dev_d)};
  std::string second{" \nVelocity:   "};
  std::string Mean_v{std::to_string(m_mean_v)};
  std::string std_devV{std::to_string(m_std_dev_v)};
  std::string tot =
      first + Mean_d + p_m + std_devD + second + Mean_v + p_m + std_devV;
  m_text.setString(tot);
}

sf::Text Data::getData() const { return m_text; }