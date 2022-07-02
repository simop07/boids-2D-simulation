#ifndef sfml_objects_hpp
#define sfml_objects_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

// La classe button contiene un oggetto sf::RectangleShape, ovvero lo sfondo del
// pulsante, un sf::Text, ovvero il testo del pulsante, una posizione, una
// dimensione, un font e un bool isOn per controllare lo stato del pulsante
// (acceso o spento). I vari metodi consentono di personalizzare le
// caratteristiche del pulsante come colore dello sfondo e del testo. Il metodo
// createButton assegna i valori per costruire l'oggetto. I metodi getBox() e
// getText() sono necessari per chiamare window.draw() in modo da visualizzare
// il pulsante. Il metodo hovering permette di controllare se il puntatore del
// mouse si trova all'interno dell'area del pulsante.

class button {
private:
  sf::Vector2f m_pos;
  sf::Vector2f m_size;
  sf::RectangleShape m_rect;
  sf::Text m_text;
  sf::Font m_font;
  bool isOn{false};

public:
  button(sf::Vector2f pos, sf::Vector2f size, sf::RectangleShape rect,
         sf::Text text, sf::Font font)
      : m_pos{pos}, m_size{size}, m_rect{rect}, m_text{text}, m_font{font} {}

  void setButtonColor(sf::Color color) {
    m_rect.setFillColor(color);
    return;
  }

  void setButtonOutline(sf::Color color, float thickness) {
    m_rect.setOutlineColor(color);
    m_rect.setOutlineThickness(thickness);
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

  sf::RectangleShape getBox() { return m_rect; }

  sf::Text getText() { return m_text; }

  bool hovering(sf::Vector2f mousePos) {
    sf::Vector2f mousePosF{static_cast<float>(mousePos.x),
                           static_cast<float>(mousePos.y)};
    if (m_rect.getGlobalBounds().contains(mousePosF)) {
      return true;
    } else {
      return false;
    }
  }

  bool buttonState() { return isOn; }

  bool buttonPressed() {
    if (isOn == true) {
      isOn = false;
    } else {
      isOn = true;
    }
    return isOn;
  }
};

// La classe data contiene i valori dei dati d'interesse, un oggetto sf::Text un
// font e una posizione. Tramite le funzioni membro è possibile modificare le
// proprietà di m_text e i dati d'interesse in modo da aggiornarli. La funzione
// membro createData() consente di assegnare i valori di input del costruttore
// per costruire in maniera appropriata il testo oltre a costruire la stringa di
// testo da visualizzare. Il metodo getData() restituisce m_text in modo da
// poter chiamare window.draw() per poter visualizzare il testo

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

double getA(){
  return m_mean_d;
}

  sf::Text getData() { return m_text; }
};

class textBox {};

#endif