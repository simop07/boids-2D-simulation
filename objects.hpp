#ifndef objects_hpp
#define objects_hpp

#include <cmath>

// The class vector_2d defines a two dimensional vector useful to take care of
// the position and velocity of the boids, the methods included allow to get the
// private members of the class and apply basic math operation between vectors
class vector_2d {
  double m_x;
  double m_y;

 public:
  vector_2d(double x = 0., double y = 0.) : m_x{x}, m_y{y} {}

  double xcomp() const;

  double ycomp() const;

  double norm() const;

  vector_2d& operator+=(vector_2d const& v);

  vector_2d& operator*=(double k);

  void setx(double x);

  void sety(double y);
};

// Symmetric operators (e.g. operator+) are implemented in terms of member
// operator (e.g. operator+=)
vector_2d operator-(vector_2d const& v);

vector_2d operator+(vector_2d const& l, vector_2d const& r);

vector_2d operator-(vector_2d const& l, vector_2d const& r);

vector_2d operator*(vector_2d const& l, double k);

// Implemented comparison of vectors in order to use it for some tests
bool operator==(vector_2d const& l, vector_2d const& r);

bool operator!=(vector_2d const& l, vector_2d const& r);

// Boid and predator struct are basically the same but are defined separatly
// allowing an easier management of the two
struct boid {
  vector_2d pos{};
  vector_2d vel{};
};

struct predator {
  vector_2d pos{};
  vector_2d vel{};
};

// Stats is a struct that allow to define functions taking less input, e.g. if a
// function needed both a parameter s and d_s it's possible two use a single
// variable stats instead of two different ones
struct stats {
  double d_s{};  // Distance at which sep gets activated
  double d{};    // Distance of influence between boids
  double s{};
  double a{};
  double c{};
  double l_b{};     // Left bound
  double r_b{};     // Right bound
  double u_b{};     // Upper bound
  double b_b{};     // Bottom bound
  double d_pred{};  // Distance at which the effect of the predator is activated
  double v_max{};   // Maximum velocity of the boids
};

#endif