#ifndef objects_hpp
#define objects_hpp

#include <cmath>

#define PI 3.141592

class vector_2d {

private:
  double m_x{};
  double m_y{};

public:
  vector_2d(double x = 0., double y = 0.) : m_x{x}, m_y{y} {}

  double xcomp() const;

  double ycomp() const;

  double norm() const;

  // Prodotto scalare fra due vettori

  double dot_prod(vector_2d const &v);

  // Angolo fra due vettori

  double angle(vector_2d const &v);

  vector_2d operator+(vector_2d const &v);

  vector_2d operator-(vector_2d const &v);

  vector_2d operator*(double k);

  vector_2d operator+=(vector_2d const &v);

  void setx(double x);

  void sety(double y);
};

struct boid {
  vector_2d pos;
  vector_2d vel;
};

struct predator {
  vector_2d pos;
  vector_2d vel;
};

struct stats {
  double d_s{}; // Parametro v_sep
  double d{};   // Parametro per ogni v
  double s{};
  double a{};
  double c{};
  double l_b{};    // Bordo sinistro
  double r_b{};    // Bordo destro
  double u_b{};    // Bordo superiore
  double b_b{};    // Bordo inferiore
  double theta{};  // Angolo cieco
  double d_pred{}; // Distanza da cui si attiva l'effetto del predatore
  double v_max{};  // Velocità massima dei boid
};

#endif