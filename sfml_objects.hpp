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

  void setButtonColor(sf::Color color);

  void setButtonOutline(sf::Color color, float thickness);

  void setTextColor(sf::Color color);

  void setTextContent(std::string s);

  void setTextSize(int size);

  void createButton();

  sf::RectangleShape getBox();

  sf::Text getText();

  bool hovering(sf::Vector2f mousePos);

  bool buttonState();

  bool buttonPressed();
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

  void setSize(int size);

  void setColor(sf::Color color);

  void setNewData(double a, double b, double c, double d);

  void createData();

  sf::Text getData();
};
#endif