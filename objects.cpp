#include "objects.hpp"

double vector_2d::xcomp() const { return m_x; }

double vector_2d::ycomp() const { return m_y; }

double vector_2d::norm() const { return sqrt(m_x * m_x + m_y * m_y); }

vector_2d& vector_2d::operator+=(vector_2d const& v) {
  m_x += v.m_x;
  m_y += v.m_y;
  return *this;
}

vector_2d& vector_2d::operator*=(double k) {
  m_x *= k;
  m_y *= k;
  return *this;
}

void vector_2d::setx(double x) { m_x = x; }

void vector_2d::sety(double y) { m_y = y; }

vector_2d operator-(vector_2d const& v) {
  return {-v.xcomp(), -v.ycomp()};
}

vector_2d operator+(vector_2d const& l, vector_2d const& r) {
  auto result{l};
  return result += r;
}

vector_2d operator-(vector_2d const& l, vector_2d const& r) { return l + (-r); }

vector_2d operator*(vector_2d const& l, double k) {
  auto result{l};
  return result *= k;
}

bool operator==(vector_2d const& l, vector_2d const& r) {
  return l.xcomp() == r.xcomp() && l.ycomp() == r.ycomp();
}

bool operator!=(vector_2d const& l, vector_2d const& r) { return !(l == r); }
