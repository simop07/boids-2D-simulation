#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "evolve.hpp"

TEST_CASE("norm") {
  vector_2d v1{2., -4.};
  vector_2d v2{0., 0.};
  vector_2d v3{1., 0.};
  vector_2d v4{0., -7.};

  CHECK(v1.norm() == doctest::Approx(sqrt(20.)));
  CHECK(v2.norm() == doctest::Approx(0.));  // La norma del vettore nullo è 0.
  CHECK(v3.norm() == doctest::Approx(1.));
  CHECK(v4.norm() == doctest::Approx(7.));
}

TEST_CASE("distance test") {
  boid b1{{3., 5.}, {0., 0.}};
  boid b2{{2., 3.}, {0., 0.}};
  boid b3{{0., 0.}, {0., 0.}};

  CHECK(distance(b1, b2) == doctest::Approx(sqrt(5.)));
  CHECK(distance(b1, b1) == doctest::Approx(0.));
  CHECK(distance(b1, b3) == doctest::Approx(sqrt(34.)));
}
TEST_CASE("two points cdm") {
  std::vector<boid> flock;
  boid b1{{0., 0.}, {0., 0.}};
  boid b2{{5., 4.}, {5., 5.}};

  flock.push_back(b1);
  flock.push_back(b2);

  CHECK(calc_c_m_b_i(flock, b1).xcomp() == doctest::Approx(5.));
  CHECK(calc_c_m_b_i(flock, b1).ycomp() == doctest::Approx(4.));
  CHECK(calc_c_m_b_i(flock, b2).xcomp() == doctest::Approx(0.));
  CHECK(calc_c_m_b_i(flock, b2).ycomp() == doctest::Approx(0.));
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
  boid b4{{1., 1.}, {1., 1.}};

  flock.push_back(b1);
  flock.push_back(b2);
  flock.push_back(b3);

  CHECK(mean_distance(flock) ==
        doctest::Approx((sqrt(5.) + sqrt(10.) + sqrt(5.)) / 3.));
  CHECK(std_dev_distance(flock) == doctest::Approx(0.26737));

  flock.push_back(b4);  // calcoliamo media e deviazione standard nel caso di
                        // due punti uguali

  CHECK(mean_distance(flock) ==
        doctest::Approx((6. * sqrt(5.) + 4. * sqrt(10.)) / 12.));
  CHECK(std_dev_distance(flock) == doctest::Approx(0.49873));
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

  boid b5{{9., 2.},{2.,-3,}};

  flock.push_back(b5);
  CHECK(mean_velocity(flock) == doctest::Approx((10. + 2. * sqrt(13.)) / 5.));
  CHECK(std_dev_velocity(flock) == doctest::Approx(2.1654));
}

TEST_CASE("v sep") {
  std::vector<boid> flock;
  boid b1{{2., 1.}, {3., 2.}};
  boid b2{{2., 3.}, {4., 3.}};
  boid b3{{3., 4.}, {0., 3.}};
  boid b4{{2.5, 3}, {1., 2.}};

  SUBCASE("4 boids") {
    flock.push_back(b1);
    flock.push_back(b2);
    flock.push_back(b3);
    flock.push_back(b4);

    CHECK(sep(flock, b1, 1., 4.).xcomp() == doctest::Approx(-1.5));
    CHECK(sep(flock, b1, 1., 4.).ycomp() == doctest::Approx(-7.));
    CHECK(sep(flock, b2, 1., 4.).xcomp() == doctest::Approx(-1.5));
    CHECK(sep(flock, b2, 1., 4.).ycomp() == doctest::Approx(1.));
  }

  SUBCASE("same components") {  // compoenenti  uguali tra due boids
    flock.push_back(b1);
    flock.push_back(b2);

    CHECK(sep(flock, b2, 1., 3.).xcomp() == doctest::Approx(0.));
    CHECK(sep(flock, b2, 1., 3.).ycomp() == doctest::Approx(2.));
  }
}

TEST_CASE("v all") {
  std::vector<boid> flock;
  boid b1{{2., 2.}, {1., 1.}};
  boid b2{{4., 3.}, {4., 1.}};
  boid b3{{7., 6.}, {2., 3.}};
  boid b4{{1., 1.}, {-1., -2.}};

  SUBCASE("4 boids") {
    flock.push_back(b1);
    flock.push_back(b2);
    flock.push_back(b3);
    flock.push_back(b4);

    CHECK(all(flock, b1, 3).xcomp() == doctest::Approx(2.0));
    CHECK(all(flock, b1, 3).ycomp() == doctest::Approx(-1.));
    CHECK(all(flock, b2, 3).xcomp() == doctest::Approx(-10.));
    CHECK(all(flock, b2, 3).ycomp() == doctest::Approx(-1.));
  }

  SUBCASE("Not enough boids") {
    flock.push_back(b1);

    CHECK_THROWS(all(flock, b1, 3));
  }

  SUBCASE("No boids") { CHECK_THROWS(all(flock, b1, 3)); }
}

TEST_CASE("v coe") {
  std::vector<boid> flock;
  boid b1{{2., 2.}, {1., 1.}};
  boid b2{{4., 3.}, {4., 1.}};
  boid b3{{7., 6.}, {2., 3.}};
  boid b4{{1., 1.}, {-1., -2.}};

  SUBCASE("4 boids") {
    flock.push_back(b1);
    flock.push_back(b2);
    flock.push_back(b3);
    flock.push_back(b4);

    CHECK(coe(b1, calc_c_m_b_i(flock, b1), 0.3).xcomp() ==
          doctest::Approx(0.6));
    CHECK(coe(b1, calc_c_m_b_i(flock, b1), 0.3).ycomp() ==
          doctest::Approx(0.4));
    CHECK(coe(b2, calc_c_m_b_i(flock, b2), 0.3).xcomp() ==
          doctest::Approx(-0.2));
    CHECK(coe(b2, calc_c_m_b_i(flock, b2), 0.3).ycomp() == doctest::Approx(0.));
  }

  SUBCASE(" Not enough boids") {
    flock.push_back(b1);
    CHECK_THROWS(coe(b1, calc_c_m_b_i(flock, b1), 0.3));
  }

  SUBCASE(" No Boids") { CHECK_THROWS(coe(b1, calc_c_m_b_i(flock, b1), 0.3)); }
}

TEST_CASE("pacman") {
  stats s{0., 0., 0., 0., 0., 0., 10., 10., 0., 0., 1000.};

  boid b1{{-1., 2.}, {0., 0.}};
  boid b2{{3., -5.}, {0., 0.}};
  boid b3{{4., 13.}, {0., 0.}};
  boid b4{{17., 6.}, {0., 0.}};
  boid b5{{13., -2.}, {0., 0.}};
  boid b6{{-3., 11.}, {0., 0.}};
  boid b7{{6., 7.}, {0., 0.}};
  boid b8{{0., 10.}, {0., 0.}};  // In questo caso e in quello successivo i boids si
                                 // trovano sui bordi, ma la lorio posizione non varia.
  boid b9{{10., 0.}, {0., 0.}};

  vector_2d v1 = pacman(b1.pos, s);
  vector_2d v2 = pacman(b2.pos, s);
  vector_2d v3 = pacman(b3.pos, s);
  vector_2d v4 = pacman(b4.pos, s);
  vector_2d v5 = pacman(b5.pos, s);
  vector_2d v6 = pacman(b6.pos, s);
  vector_2d v7 = pacman(b7.pos, s);
  vector_2d v8 = pacman(b8.pos, s);
  vector_2d v9 = pacman(b9.pos, s);

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

  CHECK(influence(flock, b1, 3.5).size() == 3);
  CHECK(influence(flock, b3, 0.1).size() == 1);
}

TEST_CASE("evolve test") {
  std::vector<boid> flock;
  stats s{1.5, 3.5, 0.5, 0.4, 0.4, 0., 10., 10., 0., 0., 1000.};
  double delta_t = 0.2;
  predator p{{0., 0.}, {0., 0.}};
  boid b1{{2., 2.}, {-1., -1.}};
  boid b2{{3., 2.}, {0., 1.}};
  boid b3{{2., 5.}, {1., -1.}};
  boid b4{{8., 8.}, {-4., -4.}};
  boid b5{{8., 0.}, {0., 0.}};

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
  //CHECK(flock[4].pos.xcomp()== doctest::Approx(8.));
  //CHECK(flock[4].pos.ycomp()== doctest::Approx(0.));
  CHECK(flock[0].vel.xcomp() == doctest::Approx(-0.7));
  CHECK(flock[0].vel.ycomp() == doctest::Approx(0.));
  CHECK(flock[1].vel.xcomp() == doctest::Approx(0.1));
  CHECK(flock[1].vel.ycomp() == doctest::Approx(0.8));
  CHECK(flock[2].vel.xcomp() == doctest::Approx(0.6));
  CHECK(flock[2].vel.ycomp() == doctest::Approx(-1.8));
  CHECK(flock[3].vel.xcomp() == doctest::Approx(-4.));
  CHECK(flock[3].vel.ycomp() == doctest::Approx(-4.));
  //CHECK(flock[4].vel.xcomp() == doctest::Approx(-2.1));
  //CHECK(flock[4].vel.ycomp() == doctest::Approx(-1.2));

  

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

TEST_CASE("evolve predator") {
  stats s{0., 0., 0., 0., 0., 0., 10., 10., 0., 0., 1000.};
  predator p1{{3., 4.}, {-2., 5.}};
  predator p2{{-3., 17.}, {0., 0.}};
  predator p3{{13., 0.}, {5., 2.}};
  double delta_t = 0.2;

  predator v1 = evolve_predator(p1, delta_t, s);
  predator v2 = evolve_predator(p2, delta_t, s);
  predator v3 = evolve_predator(p3, delta_t, s);

  CHECK(v1.pos.xcomp() == doctest::Approx(2.6));
  CHECK(v1.pos.ycomp() == doctest::Approx(5.));
  CHECK(v2.pos.xcomp() == doctest::Approx(7.));
  CHECK(v2.pos.ycomp() == doctest::Approx(7.));
  CHECK(v3.pos.xcomp() == doctest::Approx(4.0));
  CHECK(v3.pos.ycomp() == doctest::Approx(0.4));
}

TEST_CASE("eat boids") {
  std::vector<boid> flock;
  predator p{{3., 5.}, {2., 2.}};
  boid b1{{2.5, 5.4}, {-3., 4.}};
  boid b2{{3., 5.}, {1., -2}};
  boid b3{{6., 9.}, {0., 2.}};
  boid b4{{5., 6.}, {2., 3.}};
  boid b5{{3., 7.}, {1., -3.}};
  double d_pred = 2.4;

  flock.push_back(b1);
  flock.push_back(b2);
  flock.push_back(b3);
  flock.push_back(b4);
  flock.push_back(b5);

  eat_boid(flock, p, d_pred);
  eat_boid(flock, p, d_pred);
  eat_boid(flock, p, d_pred);  // si applica la funzione per ogni boid che
                               // potrebbe essere mangiato.

  int n = flock.size();

  CHECK(n ==3);  // due boids vengono mangiati, quindi la dimensione del flock è 2.
}