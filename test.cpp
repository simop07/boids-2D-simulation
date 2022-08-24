#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "evolve.hpp"

TEST_CASE("Testing vector_2d") {
  Vector_2d v1{2., -4.};
  Vector_2d v2{0., 0.};
  Vector_2d v3{1., 0.};
  Vector_2d v4{0., -7.};

  SUBCASE("Testing vector operations") {
    CHECK(v1.xcomp() == (2.));
    CHECK(v2.ycomp() == (0.));
    CHECK(v1 != v2);
    CHECK(Vector_2d{v3 + v4} == Vector_2d{1., -7.});
    CHECK(Vector_2d{v1 - v3} == Vector_2d{1., -4.});
    CHECK(Vector_2d{v1 * 5.} == Vector_2d{10., -20.});
    CHECK(Vector_2d{v1 * 0.} == v2);
    v2.setx(5.);
    v2.sety(-5.);
    CHECK(v2.xcomp() == (5.));
    CHECK(v2.ycomp() == (-5.));
  }

  SUBCASE("Testing norm") {
    CHECK(v1.norm() == doctest::Approx(sqrt(20.)));
    CHECK(v2.norm() == doctest::Approx(0.));  // Norm of zero-vector is zero
    CHECK(v3.norm() == doctest::Approx(1.));
    CHECK(v4.norm() == doctest::Approx(7.));
  }
}

TEST_CASE("Testing distance") {
  Boid b1{{3., 5.}, {0., 0.}};
  Boid b2{{2., 3.}, {0., 0.}};
  Boid b3{{0., 0.}, {0., 0.}};
  Boid b4{{-4., -2.}, {0., 0.}};

  SUBCASE("Boid-boid distance") {
    CHECK(distance(b1, b2) == doctest::Approx(sqrt(5.)));
    CHECK(distance(b1, b1) == doctest::Approx(0.));
    CHECK(distance(b1, b3) == doctest::Approx(sqrt(34.)));
    CHECK(distance(b1, b4) == doctest::Approx(7 * sqrt(2.)));
  }

  SUBCASE("Boid-predator distance") {
    Predator p1{{3., 5.}, {0., 0.}};
    Predator p2{{2., 3.}, {0., 0.}};
    Predator p3{{0., 0.}, {0., 0.}};
    Predator p4{{-4., -2.}, {0., 0.}};

    CHECK(distance(b1, p2) == doctest::Approx(sqrt(5.)));
    CHECK(distance(b1, p1) == doctest::Approx(0.));
    CHECK(distance(b1, p3) == doctest::Approx(sqrt(34.)));
    CHECK(distance(b1, p4) == doctest::Approx(7 * sqrt(2.)));
  }
}
TEST_CASE("Testing c_m relative to b_i") {
  std::vector<Boid> flock;

  SUBCASE("Two points c_m") {
    Boid b1{{-2., 1.}, {0., 0.}};
    Boid b2{{0., 0.}, {5., 5.}};
    flock.push_back(b1);
    flock.push_back(b2);

    CHECK(calc_c_m_b_i(flock, b1).xcomp() == doctest::Approx(0.));
    CHECK(calc_c_m_b_i(flock, b1).ycomp() == doctest::Approx(0.));
    CHECK(calc_c_m_b_i(flock, b2).xcomp() == doctest::Approx(-2.));
    CHECK(calc_c_m_b_i(flock, b2).ycomp() == doctest::Approx(1.));
  }

  SUBCASE("Six points c_m") {
    Boid b1{{1., 3.}, {0., 0.}};
    Boid b2{{4., 5.}, {0., 0.}};
    Boid b3{{0., 1.}, {0., 0.}};
    Boid b4{{3., 0.}, {0., 0.}};
    Boid b5{{-4., 2.}, {0., 0.}};
    Boid b6{{5., -3.}, {0., 0.}};

    flock.push_back(b1);
    flock.push_back(b2);
    flock.push_back(b3);
    flock.push_back(b4);
    flock.push_back(b5);
    flock.push_back(b6);

    CHECK(calc_c_m_b_i(flock, b1).xcomp() == doctest::Approx(1.6));
    CHECK(calc_c_m_b_i(flock, b1).ycomp() == doctest::Approx(1.));
    CHECK(calc_c_m_b_i(flock, b2).xcomp() == doctest::Approx(1.));
    CHECK(calc_c_m_b_i(flock, b2).ycomp() == doctest::Approx(0.6));
    CHECK(calc_c_m_b_i(flock, b3).xcomp() == doctest::Approx(1.8));
    CHECK(calc_c_m_b_i(flock, b3).ycomp() == doctest::Approx(1.4));
    CHECK(calc_c_m_b_i(flock, b4).xcomp() == doctest::Approx(1.2));
    CHECK(calc_c_m_b_i(flock, b4).ycomp() == doctest::Approx(1.6));
    CHECK(calc_c_m_b_i(flock, b5).xcomp() == doctest::Approx(2.6));
    CHECK(calc_c_m_b_i(flock, b5).ycomp() == doctest::Approx(1.2));
    CHECK(calc_c_m_b_i(flock, b6).xcomp() == doctest::Approx(0.8));
    CHECK(calc_c_m_b_i(flock, b6).ycomp() == doctest::Approx(2.2));
  }
}

TEST_CASE("Testing mean distance and std_dev") {
  std::vector<Boid> flock;

  SUBCASE("Three boids") {
    Boid b1{{1., 1.}, {0., 0.}};
    Boid b2{{2., 3.}, {0., 0.}};
    Boid b3{{4., 2.}, {0., 0.}};

    flock.push_back(b1);
    flock.push_back(b2);
    flock.push_back(b3);
    CHECK(mean_distance(flock) ==
          doctest::Approx((sqrt(10.) + 2 * sqrt(5.)) / 3.));
    CHECK(std_dev_distance(flock) == doctest::Approx(1.8772).epsilon(0.0001));

    SUBCASE("Adding one equal boid") {
      Boid b4{{1., 1.}, {1., 1.}};
      flock.push_back(b4);

      CHECK(mean_distance(flock) ==
            doctest::Approx((3. * sqrt(5.) + 2. * sqrt(10.)) / 6.));
      CHECK(std_dev_distance(flock) == doctest::Approx(1.5106).epsilon(0.0001));
    }
  }

  SUBCASE("Five boids") {
    Boid b1{{-1., 2.}, {0., 0.}};
    Boid b2{{4., 0.}, {0., 0.}};
    Boid b3{{5., 4.}, {0., 0.}};
    Boid b4{{1., 2.}, {1., 1.}};
    Boid b5{{2., 1.}, {1., 1.}};

    flock.push_back(b1);
    flock.push_back(b2);
    flock.push_back(b3);
    flock.push_back(b4);
    flock.push_back(b5);

    CHECK(mean_distance(flock) == doctest::Approx(3.6965).epsilon(0.0001));
    CHECK(std_dev_distance(flock) == doctest::Approx(1.97).epsilon(0.01));
  }
}

TEST_CASE("Testing mean velocity and std_dev") {
  std::vector<Boid> flock;
  Boid b1{{1., 3.}, {-4., 0.}};
  Boid b2{{4., 5.}, {0., 0.}};
  Boid b3{{0., 1.}, {2., -3.}};
  Boid b4{{3., 0.}, {0., 6.}};

  flock.push_back(b1);
  flock.push_back(b2);
  flock.push_back(b3);
  flock.push_back(b4);

  CHECK(mean_velocity(flock) == doctest::Approx((10. + sqrt(13.)) / 4.));
  CHECK(std_dev_velocity(flock) == doctest::Approx(3.1776).epsilon(0.0001));

  SUBCASE("Adding one boid") {
    Boid b5{{9., 2.}, {2., -3.}};
    flock.push_back(b5);

    CHECK(mean_velocity(flock) == doctest::Approx((10. + 2. * sqrt(13.)) / 5.));
    CHECK(std_dev_velocity(flock) == doctest::Approx(2.7660).epsilon(0.0001));
  }
}

TEST_CASE("Testing v_sep") {
  std::vector<Boid> flock;

  SUBCASE("Four boids") {
    Boid b1{{2., 1.}, {3., 2.}};
    Boid b2{{2., 3.}, {4., 3.}};
    Boid b3{{3., 4.}, {0., 3.}};
    Boid b4{{2.5, 3}, {1., 2.}};

    flock.push_back(b1);
    flock.push_back(b2);
    flock.push_back(b3);
    flock.push_back(b4);

    CHECK(sep(flock, b1, 0.5, 4.).xcomp() == doctest::Approx(-0.75));
    CHECK(sep(flock, b1, 0.5, 4.).ycomp() == doctest::Approx(-3.5));
    CHECK(sep(flock, b2, 0.5, 4.).xcomp() == doctest::Approx(-0.75));
    CHECK(sep(flock, b2, 0.5, 4.).ycomp() == doctest::Approx(0.5));
  }

  SUBCASE("Null components if distance < d_s") {
    Boid b1{{2., 1.}, {3., 2.}};
    Boid b2{{2., 3.}, {4., 3.}};
    flock.push_back(b1);
    flock.push_back(b2);

    CHECK(sep(flock, b2, 0.4, 1.5).xcomp() == doctest::Approx(0.));
    CHECK(sep(flock, b2, 0.4, 1.5).ycomp() == doctest::Approx(0.));
  }
}

TEST_CASE("Testing v_all") {
  std::vector<Boid> flock;
  Boid b1{{2., 2.}, {1., 1.}};
  Boid b2{{4., 3.}, {4., 1.}};
  Boid b3{{7., 6.}, {2., 3.}};
  Boid b4{{1., 1.}, {-1., -2.}};

  SUBCASE("4 boids") {
    flock.push_back(b1);
    flock.push_back(b2);
    flock.push_back(b3);
    flock.push_back(b4);

    CHECK(all(flock, b1, 0.3).xcomp() == doctest::Approx(0.2));
    CHECK(all(flock, b1, 0.3).ycomp() == doctest::Approx(-0.1));
    CHECK(all(flock, b2, 0.3).xcomp() == doctest::Approx(-1.));
    CHECK(all(flock, b2, 0.3).ycomp() == doctest::Approx(-0.1));
  }

  /* SUBCASE("Not enough boids") {
    flock.push_back(b1);

    CHECK_THROWS(all(flock, b1, 3));
  } */

/*   SUBCASE("No boids") { CHECK_THROWS(all(flock, b1, 3)); }
 */}

TEST_CASE("Testing v_coe") {
  std::vector<Boid> flock;
  Boid b1{{2., 2.}, {1., 1.}};
  Boid b2{{4., 3.}, {4., 1.}};
  Boid b3{{7., 6.}, {2., 3.}};
  Boid b4{{1., 1.}, {-1., -2.}};

  flock.push_back(b1);
  flock.push_back(b2);
  flock.push_back(b3);
  flock.push_back(b4);

  CHECK(coe(b1, calc_c_m_b_i(flock, b1), 0.3).xcomp() == doctest::Approx(0.6));
  CHECK(coe(b1, calc_c_m_b_i(flock, b1), 0.3).ycomp() == doctest::Approx(0.4));
  CHECK(coe(b2, calc_c_m_b_i(flock, b2), 0.3).xcomp() == doctest::Approx(-0.2));
  CHECK(coe(b2, calc_c_m_b_i(flock, b2), 0.3).ycomp() == doctest::Approx(0.));

  /* SUBCASE(" Not enough boids") {
    flock.push_back(b1);
    CHECK_THROWS(coe(b1, calc_c_m_b_i(flock, b1), 0.3));
  } */

/*   SUBCASE(" No Boids") { CHECK_THROWS(coe(b1, calc_c_m_b_i(flock, b1), 0.3)); }
 */}

TEST_CASE("Testing pacman") {
  Stats s{0., 0., 0., 0., 0., 0., 10., 10., 0., 0., 1000.};

  Boid b1{{-1., 2.}, {0., 0.}};
  Boid b2{{3., -5.}, {0., 0.}};
  Boid b3{{4., 13.}, {0., 0.}};
  Boid b4{{17., 6.}, {0., 0.}};
  Boid b5{{13., -2.}, {0., 0.}};
  Boid b6{{-3., 11.}, {0., 0.}};
  Boid b7{{6., 7.}, {0., 0.}};

  // Boids b8 and b9 are located on the edges, but their position doesn't change
  Boid b8{{0., 10.}, {0., 0.}};
  Boid b9{{10., 0.}, {0., 0.}};

  Vector_2d v1 = pacman(b1.pos, s);
  Vector_2d v2 = pacman(b2.pos, s);
  Vector_2d v3 = pacman(b3.pos, s);
  Vector_2d v4 = pacman(b4.pos, s);
  Vector_2d v5 = pacman(b5.pos, s);
  Vector_2d v6 = pacman(b6.pos, s);
  Vector_2d v7 = pacman(b7.pos, s);
  Vector_2d v8 = pacman(b8.pos, s);
  Vector_2d v9 = pacman(b9.pos, s);

  CHECK(v1.xcomp() == doctest::Approx(9.));
  CHECK(v1.ycomp() == doctest::Approx(2.));
  CHECK(v2.xcomp() == doctest::Approx(3.));
  CHECK(v2.ycomp() == doctest::Approx(5.));
  CHECK(v3.xcomp() == doctest::Approx(4.));
  CHECK(v3.ycomp() == doctest::Approx(3.));
  CHECK(v4.xcomp() == doctest::Approx(7.));
  CHECK(v4.ycomp() == doctest::Approx(6.));
  CHECK(v5.xcomp() == doctest::Approx(3.));
  CHECK(v5.ycomp() == doctest::Approx(8.));
  CHECK(v6.xcomp() == doctest::Approx(7.));
  CHECK(v6.ycomp() == doctest::Approx(1.));
  CHECK(v7.xcomp() == doctest::Approx(6.));
  CHECK(v7.ycomp() == doctest::Approx(7.));
  CHECK(v8.xcomp() == doctest::Approx(0.));
  CHECK(v8.ycomp() == doctest::Approx(10.));
  CHECK(v9.xcomp() == doctest::Approx(10.));
  CHECK(v9.ycomp() == doctest::Approx(0.));
}

TEST_CASE("Testing influence") {
  std::vector<Boid> flock;
  Boid b1{{2., 2.}, {1., 1.}};
  Boid b2{{4., 3.}, {1., 1.}};
  Boid b3{{7., 6.}, {1., 1.}};
  Boid b4{{1., 1.}, {-1., -1.}};

  flock.push_back(b1);
  flock.push_back(b2);
  flock.push_back(b3);
  flock.push_back(b4);

  CHECK(influence(flock, b1, 3.5).size() == 3);
  CHECK(influence(flock, b2, 4.).size() == 3);
  CHECK(influence(flock, b3, 0.1).size() == 1);
  CHECK(influence(flock, b4, 0.1).size() == 1);
}

TEST_CASE("Testing evolve_boid") {
  std::vector<Boid> flock;
  Stats s{1.5, 3.5, 0.5, 0.4, 0.4, 0., 10., 10., 0., 0., 1000.};
  double delta_t = 0.2;
  Predator p{{0., 0.}, {0., 0.}};
  Boid b1{{2., 2.}, {-1., -1.}};
  Boid b2{{3., 2.}, {0., 1.}};
  Boid b3{{2., 5.}, {1., -1.}};
  Boid b4{{8., 8.}, {-4., -4.}};
  Boid b5{{8., 0.}, {0., 0.}};

  flock.push_back(b1);
  flock.push_back(b2);
  flock.push_back(b3);
  flock.push_back(b4);
  flock.push_back(b5);

  evolve_flock(flock, delta_t, s, p);

  CHECK(flock[0].pos.xcomp() == doctest::Approx(1.8));
  CHECK(flock[0].pos.ycomp() == doctest::Approx(1.8));
  CHECK(flock[1].pos.xcomp() == doctest::Approx(3.));
  CHECK(flock[1].pos.ycomp() == doctest::Approx(2.2));
  CHECK(flock[2].pos.xcomp() == doctest::Approx(2.2));
  CHECK(flock[2].pos.ycomp() == doctest::Approx(4.8));
  CHECK(flock[3].pos.xcomp() == doctest::Approx(7.2));
  CHECK(flock[3].pos.ycomp() == doctest::Approx(7.2));
  CHECK(flock[0].vel.xcomp() == doctest::Approx(-0.7));
  CHECK(flock[0].vel.ycomp() == doctest::Approx(0.));
  CHECK(flock[1].vel.xcomp() == doctest::Approx(0.1));
  CHECK(flock[1].vel.ycomp() == doctest::Approx(0.8));
  CHECK(flock[2].vel.xcomp() == doctest::Approx(0.6));
  CHECK(flock[2].vel.ycomp() == doctest::Approx(-1.8));
  CHECK(flock[3].vel.xcomp() == doctest::Approx(-4.));
  CHECK(flock[3].vel.ycomp() == doctest::Approx(-4.));

  evolve_flock(flock, delta_t, s, p);

  CHECK(flock[0].pos.xcomp() == doctest::Approx(1.66));
  CHECK(flock[0].pos.ycomp() == doctest::Approx(1.8));
  CHECK(flock[1].pos.xcomp() == doctest::Approx(3.02));
  CHECK(flock[1].pos.ycomp() == doctest::Approx(2.36));
  CHECK(flock[2].pos.xcomp() == doctest::Approx(2.32));
  CHECK(flock[2].pos.ycomp() == doctest::Approx(4.44));
  CHECK(flock[3].pos.xcomp() == doctest::Approx(6.4));
  CHECK(flock[3].pos.ycomp() == doctest::Approx(6.4));
  CHECK(flock[0].vel.xcomp() == doctest::Approx(-0.56));
  CHECK(flock[0].vel.ycomp() == doctest::Approx(0.28));
  CHECK(flock[1].vel.xcomp() == doctest::Approx(0.24));
  CHECK(flock[1].vel.ycomp() == doctest::Approx(0.76));
  CHECK(flock[2].vel.xcomp() == doctest::Approx(0.32));
  CHECK(flock[2].vel.ycomp() == doctest::Approx(-2.04));
  CHECK(flock[3].vel.xcomp() == doctest::Approx(-4.));
  CHECK(flock[3].vel.ycomp() == doctest::Approx(-4.));
}

TEST_CASE("Testing evolve_predator") {
  Stats s{0., 0., 0., 0., 0., 0., 10., 10., 0., 0., 1000.};
  Predator p1{{3., 4.}, {-2., 5.}};
  Predator p2{{-3., 17.}, {0., 0.}};
  Predator p3{{13., 0.}, {5., 2.}};
  Predator p4{{5., -3.}, {5., -2.}};
  Predator p5{{9.5, 2.}, {13., 3.}};
  double delta_t = 0.2;

  Predator v1 = evolve_predator(p1, delta_t, s);
  Predator v2 = evolve_predator(p2, delta_t, s);
  Predator v3 = evolve_predator(p3, delta_t, s);
  Predator v4 = evolve_predator(p4, delta_t, s);
  Predator v5 = evolve_predator(p5, delta_t, s);

  CHECK(v1.pos.xcomp() == doctest::Approx(2.6));
  CHECK(v1.pos.ycomp() == doctest::Approx(5.));
  CHECK(v2.pos.xcomp() == doctest::Approx(7.));
  CHECK(v2.pos.ycomp() == doctest::Approx(7.));
  CHECK(v3.pos.xcomp() == doctest::Approx(4.0));
  CHECK(v3.pos.ycomp() == doctest::Approx(0.4));
  CHECK(v4.pos.xcomp() == doctest::Approx(6.));
  CHECK(v4.pos.ycomp() == doctest::Approx(7.));
  CHECK(v5.pos.xcomp() == doctest::Approx(2.));
  CHECK(v5.pos.ycomp() == doctest::Approx(2.6));
}

TEST_CASE("eat boids") {
  std::vector<Boid> flock;
  Predator p{{3., 5.}, {2., 2.}};
  Boid b1{{2.5, 5.4}, {-3., 4.}};
  Boid b2{{3., 5.}, {1., -2}};
  Boid b3{{6., 9.}, {0., 2.}};
  Boid b4{{5., 6.}, {2., 3.}};
  Boid b5{{3., 7.}, {1., -3.}};
  double d_pred = 2.4;

  flock.push_back(b1);
  flock.push_back(b2);
  flock.push_back(b3);
  flock.push_back(b4);
  flock.push_back(b5);

  eat_boid(flock, p, d_pred);

  int n = flock.size();

  CHECK(n ==
        2);  // tre boids vengono mangiati, quindi la dimensione del flock è 2.
}