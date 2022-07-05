#include "objects.hpp"

double vector_2d::xcomp() const { return m_x; }

double vector_2d::ycomp() const { return m_y; }

double vector_2d::norm() const { return sqrt(m_x * m_x + m_y * m_y); }

vector_2d vector_2d::operator+(vector_2d const &v) {
  vector_2d res(this->m_x + v.m_x, this->m_y + v.m_y);
  return res;
}

vector_2d vector_2d::operator-(vector_2d const &v) {
  vector_2d res(this->m_x - v.m_x, this->m_y - v.m_y);
  return res;
}

vector_2d vector_2d::operator*(double k) {
  vector_2d res(this->m_x * k, this->m_y * k);
  return res;
}

vector_2d vector_2d::operator+=(vector_2d const &v) {
  this->m_x += v.m_x;
  this->m_y += v.m_y;
  return *this;
}

void vector_2d::setx(double x) { this->m_x = x; }

void vector_2d::sety(double y) { this->m_y = y; }
