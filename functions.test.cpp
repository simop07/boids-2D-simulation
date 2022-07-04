#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "evolve.hpp" //Cambia con functions dopo
#include "functions.hpp"

TEST_CASE("norm") {
  vector_2d v1{2., -4.};
  vector_2d v2{0., 0.};
  vector_2d v3{1., 0.};
  vector_2d v4{0., -7.};

  CHECK(v1.norm() == doctest::Approx(sqrt(20.)));
  CHECK(v2.norm() == doctest::Approx(0.));
  CHECK(v3.norm() == doctest::Approx(1.));
  CHECK(v4.norm() == doctest::Approx(7.));
}

TEST_CASE("scalar prod") {
  vector_2d v1{2., -4.};
  vector_2d v2{0., 0.};
  vector_2d v3{1., 0.};
  vector_2d v4{0., -7.};

  CHECK(v1.dot_prod(v1) == doctest::Approx(20.));
  CHECK(v1.dot_prod(v2) == doctest::Approx(0.));
  CHECK(v1.dot_prod(v3) == doctest::Approx(2.));
  CHECK(v3.dot_prod(v4) == doctest::Approx(0.));
}

TEST_CASE("angle") {
  vector_2d v1{2., -4.};
  vector_2d v2{0., 0.};
  vector_2d v3{1., 0.};
  vector_2d v4{0., -7.};
  vector_2d v5{3., 5.};

  CHECK(v1.angle(v1) == doctest::Approx(0.));
  CHECK(v1.angle(v2) == doctest::Approx(0.));
  CHECK(v1.angle(v3) == doctest::Approx(1.10715));
  CHECK(v1.angle(v5) == doctest::Approx(2.1375));
  CHECK(v2.angle(v1) == doctest::Approx(0.));
  CHECK(v3.angle(v4) == doctest::Approx(PI / 2.));
  CHECK(v4.angle(v5) == doctest::Approx(2.6012));
  CHECK(v5.angle(v1) == doctest::Approx(2.1375));
}

TEST_CASE("distance test") {
  boid b1{{3., 5.}, {0., 0.}};
  boid b2{{2., 3.}, {0., 0.}};

  CHECK(distance(b1, b2) == doctest::Approx(sqrt(5)));
}

TEST_CASE("cdm test") {
  std::vector<boid> flock;
  boid b1{{1., 3.}, {0., 0.}};
  boid b2{{4., 5.}, {0., 0.}};
  boid b3{{0., 1.}, {0., 0.}};
  boid b4{{3., 0.}, {0., 0.}};

  flock.push_back(b1);
  flock.push_back(b2);
  flock.push_back(b3);
  flock.push_back(b4);

  CHECK(calc_c_m_b_i(flock, b1).xcomp() == doctest::Approx(7. / 3.));
  CHECK(calc_c_m_b_i(flock, b1).ycomp() == doctest::Approx(2.));
  CHECK(calc_c_m_b_i(flock, b2).xcomp() == doctest::Approx(4. / 3.));
  CHECK(calc_c_m_b_i(flock, b2).ycomp() == doctest::Approx(4. / 3.));
  CHECK(calc_c_m_b_i(flock, b3).xcomp() == doctest::Approx(8. / 3.));
  CHECK(calc_c_m_b_i(flock, b3).ycomp() == doctest::Approx(8. / 3.));
  CHECK(calc_c_m_b_i(flock, b4).xcomp() == doctest::Approx(5. / 3.));
  CHECK(calc_c_m_b_i(flock, b4).ycomp() == doctest::Approx(3.));
}

TEST_CASE("mean distance and st dev") {
  std::vector<boid> flock;
  boid b1{{1., 1.}, {0., 0.}};
  boid b2{{2., 3.}, {0., 0.}};
  boid b3{{4., 2.}, {0., 0.}};

  flock.push_back(b1);
  flock.push_back(b2);
  flock.push_back(b3);

  CHECK(mean_distance(flock) ==
        doctest::Approx((sqrt(5.) + sqrt(10.) + sqrt(5.)) / 3.));
}

TEST_CASE("mean velocity and st dev") {
  std::vector<boid> flock;
  boid b1{{1., 3.}, {-4., 0.}};
  boid b2{{4., 5.}, {0., 0.}};
  boid b3{{0., 1.}, {2., -3.}};
  boid b4{{3., 0.}, {0., 6.}};

  flock.push_back(b1);
  flock.push_back(b2);
  flock.push_back(b3);
  flock.push_back(b4);

  CHECK(mean_velocity(flock) == doctest::Approx((10. + sqrt(13.)) / 4.));
  CHECK(std_dev_velocity(flock) == doctest::Approx(2.4981488895));
}

TEST_CASE("vision") {
  boid b1{{3., 3.}, {-1., -1.}};
  boid b2{{3., 1.}, {0., 1.}};
  boid b3{{6., 3.}, {0., 2.}};
  boid b4{{6., 5.}, {-3., -3.}};
  boid b5{{8., 5.}, {2., -1.}};
  boid b6{{11., 2.}, {-1., 1.}};
  boid b7{{7., 1.}, {0., 0.}};
  boid b8{{11., 2.}, {-3., 4.}};

  CHECK(vision(b1, b2, PI / 2.) == true);
  CHECK(vision(b1, b3, PI / 2.) == false);
  CHECK(vision(b1, b4, PI / 2.) == false);
  CHECK(vision(b3, b4, 0) == true);
  CHECK(vision(b4, b2, PI) == true);
  CHECK(vision(b4, b5, PI) == false);
  CHECK(vision(b6, b5, PI * 2.) == false);
  CHECK(vision(b6, b8, PI / 3 == false));
  CHECK(vision(b7, b3, PI / 3.) == true);
}

TEST_CASE("influence") {
  std::vector<boid> flock;
  boid b1{{2., 2.}, {1., 1.}};
  boid b2{{4., 3.}, {1., 1.}};
  boid b3{{7., 6.}, {1., 1.}};
  boid b4{{1., 1.}, {-1., -1.}};

  flock.push_back(b1);
  flock.push_back(b2);
  flock.push_back(b3);
  flock.push_back(b4);

  CHECK(influence(flock, b1, 3.5, PI / 3.).size() == 2);
}

TEST_CASE("evolve test") {
  std::vector<boid> flock;
  stats s{1.5, 3.5, 0.5, 0.4, 0.4, 0., 10., 10., 0., 0., 0., 1000.};
  double delta_t = 0.2;
  predator p{{0., 0.}, {0., 0.}};
  boid b1{{2., 2.}, {-1., -1.}};
  boid b2{{3., 2.}, {0., 1.}};
  boid b3{{2., 5.}, {1., -1.}};
  boid b4{{8., 8.}, {-4., -4.}};

  flock.push_back(b1);
  flock.push_back(b2);
  flock.push_back(b3);
  flock.push_back(b4);

  evolve_flock(flock, delta_t, s, p);

  CHECK(flock[0].vel.xcomp() == doctest::Approx(-0.7));
  CHECK(flock[0].vel.ycomp() == doctest::Approx(0.));
  CHECK(flock[1].vel.xcomp() == doctest::Approx(0.1));
  CHECK(flock[1].vel.ycomp() == doctest::Approx(0.8));
  CHECK(flock[2].vel.xcomp() == doctest::Approx(0.6));
  CHECK(flock[2].vel.ycomp() == doctest::Approx(-1.8));
  CHECK(flock[3].vel.xcomp() == doctest::Approx(-4.));
  CHECK(flock[3].vel.ycomp() == doctest::Approx(-4.));
}
