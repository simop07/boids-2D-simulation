#include "sfml_objects.hpp"

// Class button

void button::setButtonColor(sf::Color color) {
    m_rect.setFillColor(color);
    return;
  }

  void button::setButtonOutline(sf::Color color, float thickness) {
    m_rect.setOutlineColor(color);
    m_rect.setOutlineThickness(thickness);
    return;
  }

  void button::setTextColor(sf::Color color) {
    m_text.setFillColor(color);
    return;
  }

  void button::setTextContent(std::string s) {
    m_text.setString(s);
    return;
  }

  void button::setTextSize(int size) {
    m_text.setCharacterSize(size);
    return;
  }

  void button::createButton() {
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

  sf::RectangleShape button::getBox() { return m_rect; }

  sf::Text button::getText() { return m_text; }

  bool button::hovering(sf::Vector2f mousePos) {
    sf::Vector2f mousePosF{static_cast<float>(mousePos.x),
                           static_cast<float>(mousePos.y)};
    if (m_rect.getGlobalBounds().contains(mousePosF)) {
      return true;
    } else {
      return false;
    }
  }

  bool button::buttonState() { return isOn; }

  bool button::buttonPressed() {
    if (isOn == true) {
      isOn = false;
    } else {
      isOn = true;
    }
    return isOn;
  }

  //Class data

  void data::setSize(int size) {
    m_text.setCharacterSize(size);
    return;
  }

  void data::setColor(sf::Color color) {
    m_text.setFillColor(color);
    return;
  }

  void data::setNewData(double a, double b, double c, double d) {
    m_mean_d = a;
    m_std_dev_d = b;
    m_mean_v = c;
    m_std_dev_v = d;
    return;
  }

  void data::createData() {
    m_text.setPosition(m_pos);
    m_text.setFont(m_font);
    std::string first{"Data         Mean              Std. Dev. \nDistance:  "};
    std::string Mean_d{std::to_string(m_mean_d)};
    std::string p_m{"  +/-  "};
    std::string std_devD{std::to_string(m_std_dev_d)};
    std::string second{" \nVelocity:   "};
    std::string Mean_v{std::to_string(m_mean_v)};
    std::string std_devV{std::to_string(m_std_dev_v)};
    std::string tot =
        first + Mean_d + p_m + std_devD + second + Mean_v + p_m + std_devV;
    m_text.setString(tot);
    return;
  }

  sf::Text data::getData() { return m_text; }