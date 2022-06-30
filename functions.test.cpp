#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "evolve.hpp"  //Cambia con functions dopo

TEST_CASE("v_sep test") {
  boid b1{{1., 0.}, {0., 0.}};
  boid b2{{1.05, 0.}, {0., 0.}};
  vel v1 = sep(b1, b2, 0.1, 0.1);

  CHECK(v1.v_x == doctest::Approx(-0.005));
}

TEST_CASE("distance test") {
  boid b1{{1., 0.}, {0., 0.}};
  boid b2{{1.05, 0.}, {0., 0.}};
  double d1 = distance(b1, b2);

  CHECK(d1 == doctest::Approx(0.05));
}

TEST_CASE("v_all test") {
  boid b1{{1., 0.}, {3., 0.}};
  boid b2{{1.05, 0.}, {2.2, 0.}};
  vel v1 = all(b1, b2, 2, 0.5);

  CHECK(v1.v_x == doctest::Approx(-0.4));
}

TEST_CASE("pos test") {
  pos p1{1.0, 3.5};
  pos p2{2.5, 4.};
  pos p3 = p2 - p1;

  CHECK(p3.x == doctest::Approx(1.5));
}

TEST_CASE("evolve test") {
  std::vector<boid> flock;
  stats s{2.5, 3.5, 0.1, 0.2, 0.3, 0., 10., 10., 0., 0.};
  double delta_t = 0.2;
  boid b1{{1., 3.}, {3., 1.}};
  boid b2{{6., 2.}, {0., 0.}};
  boid b3{{6., 4.}, {1., 2.}};
  boid b4{{3., 5.}, {2., 0.}};
  boid b5{{5., 7.}, {0., 3.}};
  boid b6{{1., 7.}, {2., 2.}};
  boid b7{{3., 8.}, {1., 1.}};
  flock.push_back(b1);
  flock.push_back(b2);
  flock.push_back(b3);
  flock.push_back(b4);
  flock.push_back(b5);
  flock.push_back(b6);
  flock.push_back(b7);
}