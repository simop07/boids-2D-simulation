#ifndef objects_hpp
#define objects_hpp

#include <cmath>

#define PI 3.141592

class vector_2d {

private:
  double m_x{};
  double m_y{}; //le due componenti 

public:
  vector_2d(double x = 0., double y = 0.) : m_x{x}, m_y{y} {}

  double xcomp() const { return m_x; }

  double ycomp() const { return m_y; }

  double norm() const { return sqrt(m_x * m_x + m_y * m_y); }

  // Prodotto scalare fra due vettori

  double dot_prod(vector_2d const &v) { return m_x * v.m_x + m_y * v.m_y; } // perche non due vector_2d?perche fai v.dot_prod(v1)

  // Angolo fra due vettori

  double angle(vector_2d const &v) {
    if ((m_x == 0. && m_y == 0.) || (v.m_x == 0. && v.m_y == 0.)) { 
      return 0.;
    }
    double dot = this->dot_prod(v); // perche il puntatore this?
    double t_norm = this->norm() * v.norm();
    double arg = dot / t_norm;

    if (arg <= -1) {
      return PI;
    } else if (arg >= 1) {
      return 0;
    }

    double res = acos(arg);
    return res;
  }

  vector_2d operator+(vector_2d const &v) {
    vector_2d res(this->m_x + v.m_x, this->m_y + v.m_y);
    return res;
  }

  vector_2d operator-(vector_2d const &v) {
    vector_2d res(this->m_x - v.m_x, this->m_y - v.m_y); // come mai definite cosi?
    return res;
  }

  vector_2d operator*(double k) {
    vector_2d res(this->m_x * k, this->m_y * k);
    return res;
  }

  vector_2d operator+=(vector_2d const &v) {
    this->m_x += v.m_x;
    this->m_y += v.m_y;
    return *this;
  }

  void setx(double x) { this->m_x = x; }

  void sety(double y) { this->m_y = y; }
};

struct boid {
  vector_2d pos;
  vector_2d vel;
};

struct predator {
  vector_2d pos;
  vector_2d vel;
};

struct stats { // per evitare di fornire troppi parametri alle funzioni 
  double d_s{}; // Parametro v_sep
  double d{};   // Parametro per ogni v, dove si influenzaano
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