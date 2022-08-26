#ifndef sfml_objects_hpp
#define sfml_objects_hpp

#include <SFML/Graphics.hpp>
#include <string>

// Button class is used to create interactive buttons in sfml, it has the
// necessary objects to create a rectangle and a text as its private members as
// well as a bool to check wether the button is pressed or not. The numerous
// methods allow a simple management of the visualitazion of the button itself
// and to get back the necessary information to draw it. The method
// createButton() is used to assign every parametre to the right sfml object in
// order to create a drawable object
class Button {
  sf::Vector2f m_pos;
  sf::Vector2f m_size;
  sf::RectangleShape m_rect;
  sf::Text m_text;
  sf::Font m_font;
  bool isOn{false};

 public:
  Button(sf::Vector2f pos, sf::Vector2f size, sf::RectangleShape rect,
         sf::Text text, sf::Font font)
      : m_pos{pos}, m_size{size}, m_rect{rect}, m_text{text}, m_font{font} {}

  void setButtonColor(sf::Color const& color);

  void setButtonOutline(sf::Color const& color, float thickness);

  void setTextColor(sf::Color const& color);

  void setTextContent(std::string const& s);

  void setTextSize(int size);

  void createButton();

  sf::RectangleShape getBox() const;

  sf::Text getText() const;

  bool hovering(sf::Vector2f const& mousePos);

  bool buttonState() const;

  bool buttonPressed();
};

// Data class is very similar to button class in having the necessary sfml
// objects as private members and methods that allow to create a drawable sfml
// object and to assign new values to the output data required
class Data {
  double m_mean_d;
  double m_mean_v;
  double m_std_dev_d;
  double m_std_dev_v;
  sf::Vector2f m_pos;
  sf::Font m_font;
  sf::Text m_text;

 public:
  Data(double mean_d, double mean_v, double std_dev_d, double std_dev_v,
       sf::Vector2f pos, sf::Font font, sf::Text text)
      : m_mean_d{mean_d},
        m_mean_v{mean_v},
        m_std_dev_d{std_dev_d},
        m_std_dev_v{std_dev_v},
        m_pos{pos},
        m_font{font},
        m_text{text} {}

  void setSize(int size);

  void setColor(sf::Color const& color);

  void setNewData(double a, double b, double c, double d);

  void createData();

  sf::Text getData() const;
};

#endif