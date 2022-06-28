#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "evolve.hpp" //Cambia con functions dopo

TEST_CASE("v_sep test") {

  predator p1{{0., 0.}, {10., 10.}};
  CHECK(p1.pos.xcomp() == 20.);
}

TEST_CASE("distance test") {}

TEST_CASE("v_all test") {}

TEST_CASE("pos test") {}

TEST_CASE("evolve test") {
  /*std::vector<boid> flock;
  stats s{2.5, 3.5, 0.1, 0.2, 0.3, 0., 10., 10., 0., PI/3};
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
  flock.push_back(b7);*/
}