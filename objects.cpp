#include "objects.hpp"

double Vector_2d::xcomp() const { return m_x; }

double Vector_2d::ycomp() const { return m_y; }

double Vector_2d::norm() const { return sqrt(m_x * m_x + m_y * m_y); }

Vector_2d& Vector_2d::operator+=(Vector_2d const& v) {
  m_x += v.m_x;
  m_y += v.m_y;
  return *this;
}

Vector_2d& Vector_2d::operator*=(double k) {
  m_x *= k;
  m_y *= k;
  return *this;
}

void Vector_2d::setx(double x) { m_x = x; }

void Vector_2d::sety(double y) { m_y = y; }

Vector_2d operator-(Vector_2d const& v) { return {-v.xcomp(), -v.ycomp()}; }

Vector_2d operator+(Vector_2d const& l, Vector_2d const& r) {
  auto result{l};
  return result += r;
}

Vector_2d operator-(Vector_2d const& l, Vector_2d const& r) { return l + (-r); }

Vector_2d operator*(Vector_2d const& l, double k) {
  auto result{l};
  return result *= k;
}

bool operator==(Vector_2d const& l, Vector_2d const& r) {
  return l.xcomp() == r.xcomp() && l.ycomp() == r.ycomp();
}

bool operator!=(Vector_2d const& l, Vector_2d const& r) { return !(l == r); }
