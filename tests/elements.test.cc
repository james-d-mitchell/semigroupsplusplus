//
// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2016 James D. Mitchell
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "catch.hpp"

#include "../src/elements.h"

using namespace libsemigroups;

TEST_CASE("Transformation 01: u_int16_t methods",
          "[quick][element][transformation][01]") {
  Element* x = new Transformation<u_int16_t>({0, 1, 0});
  Element* y
      = new Transformation<u_int16_t>(new std::vector<u_int16_t>({0, 1, 0}));
  REQUIRE(*x == *y);
  x->redefine(y, y);
  REQUIRE(*x == *y);
  REQUIRE((*x < *y) == false);

  Element* z
      = new Transformation<u_int16_t>(new std::vector<u_int16_t>({0, 1, 0, 3}));
  REQUIRE(*x < *z);
  z->really_delete();
  delete z;

  Element* expected = new Transformation<u_int16_t>({0, 0, 0});
  REQUIRE(*expected < *x);
  expected->really_delete();
  delete expected;

  REQUIRE(x->degree() == 3);
  REQUIRE(y->degree() == 3);
  REQUIRE(x->complexity() == 3);
  REQUIRE(y->complexity() == 3);
  REQUIRE(static_cast<Transformation<u_int16_t>*>(x)->crank() == 2);
  REQUIRE(static_cast<Transformation<u_int16_t>*>(y)->crank() == 2);
  Element* id = x->identity();

  expected = new Transformation<u_int16_t>({0, 1, 2});
  REQUIRE(*id == *expected);
  expected->really_delete();
  delete expected;

  Element* a = x->really_copy(10);
  REQUIRE(a->degree() == 13);
  a->really_delete();
  delete a;

  x->really_delete();
  delete x;
  y->really_delete();
  delete y;
  id->really_delete();
  delete id;
}

TEST_CASE("Transformation 02: u_int16_t hash",
          "[quick][element][transformation][02]") {
  Element* x = new Transformation<u_int16_t>({9, 7, 3, 5, 3, 4, 2, 7, 7, 1});
  for (size_t i = 0; i < 1000000; i++) {
    x->hash_value();
  }
  x->really_delete();
  delete x;
}

TEST_CASE("Transformation 03: u_int16_t delete/copy",
          "[quick][element][transformation][03]") {
  Element* x = new Transformation<u_int16_t>({9, 7, 3, 5, 3, 4, 2, 7, 7, 1});
  Element* y = x->really_copy();
  x->really_delete();  // deletes _vector in x but not in y
  delete x;

  Element* expected
      = new Transformation<u_int16_t>({9, 7, 3, 5, 3, 4, 2, 7, 7, 1});

  REQUIRE(*y == *expected);
  Transformation<u_int16_t> yy = *static_cast<Transformation<u_int16_t>*>(y);
  REQUIRE(yy == *y);
  Transformation<u_int16_t> zz(yy);
  delete y;  // does not delete the _vector in y, yy, or zz
  REQUIRE(zz == *expected);
  zz.really_delete();

  std::vector<u_int16_t>* imgs
      = new std::vector<u_int16_t>({9, 7, 3, 5, 3, 4, 2, 7, 7, 1});
  x = new Transformation<u_int16_t>(imgs);  // imgs is not copied into x!
  REQUIRE(*x == *expected);
  yy = *static_cast<Transformation<u_int16_t>*>(x);
  REQUIRE(yy == *expected);
  delete x;
  REQUIRE(yy == *expected);
  yy.really_delete();

  expected->really_delete();
  delete expected;
}

TEST_CASE("Transformation 04: u_int32_t methods",
          "[quick][element][transformation][04]") {
  Element* x = new Transformation<u_int32_t>({0, 1, 0});
  Element* y
      = new Transformation<u_int32_t>(new std::vector<u_int32_t>({0, 1, 0}));
  REQUIRE(*x == *y);
  x->redefine(y, y);
  REQUIRE(*x == *y);
  REQUIRE((*x < *y) == false);
  Element* expected = new Transformation<u_int32_t>({0, 0, 0});
  REQUIRE(*expected < *x);

  expected->copy(x);
  REQUIRE(*expected == *x);

  expected->really_delete();
  delete expected;

  REQUIRE(x->degree() == 3);
  REQUIRE(y->degree() == 3);
  REQUIRE(x->complexity() == 3);
  REQUIRE(y->complexity() == 3);
  REQUIRE(static_cast<Transformation<u_int32_t>*>(x)->crank() == 2);
  REQUIRE(static_cast<Transformation<u_int32_t>*>(y)->crank() == 2);
  Element* id = x->identity();

  expected = new Transformation<u_int32_t>({0, 1, 2});
  REQUIRE(*id == *expected);
  expected->really_delete();
  delete expected;

  x->really_delete();
  delete x;
  y->really_delete();
  delete y;
  id->really_delete();
  delete id;
}

TEST_CASE("Transformation 05: u_int32_t hash ~8ms",
          "[quick][element][transformation][05]") {
  Element* x = new Transformation<u_int32_t>({9, 7, 3, 5, 3, 4, 2, 7, 7, 1});
  for (size_t i = 0; i < 1000000; i++) {
    x->hash_value();
  }
  x->really_delete();
  delete x;
}

TEST_CASE("Transformation 06: u_int32_t delete/copy",
          "[quick][element][transformation][06]") {
  Element* x = new Transformation<u_int32_t>({9, 7, 3, 5, 3, 4, 2, 7, 7, 1});
  Element* y = x->really_copy();
  x->really_delete();  // deletes _vector in x but not in y
  delete x;

  Element* expected
      = new Transformation<u_int32_t>({9, 7, 3, 5, 3, 4, 2, 7, 7, 1});

  REQUIRE(*y == *expected);
  Transformation<u_int32_t> yy = *static_cast<Transformation<u_int32_t>*>(y);
  REQUIRE(yy == *y);
  Transformation<u_int32_t> zz(yy);
  delete y;  // does not delete the _vector in y, yy, or zz
  REQUIRE(zz == *expected);
  zz.really_delete();

  std::vector<u_int32_t>* imgs
      = new std::vector<u_int32_t>({9, 7, 3, 5, 3, 4, 2, 7, 7, 1});
  x = new Transformation<u_int32_t>(imgs);  // imgs is not copied into x!
  REQUIRE(*x == *expected);
  yy = *static_cast<Transformation<u_int32_t>*>(x);
  REQUIRE(yy == *expected);
  delete x;
  REQUIRE(yy == *expected);
  yy.really_delete();

  expected->really_delete();
  delete expected;
}

TEST_CASE("Transformation 07: exceptions",
          "[quick][element][transformation][07]") {
  REQUIRE_NOTHROW(Transformation<u_int16_t>(std::vector<u_int16_t>()));
  REQUIRE_NOTHROW(Transformation<u_int16_t>(std::vector<u_int16_t>({0})));
  REQUIRE_THROWS_AS(Transformation<u_int16_t>(std::vector<u_int16_t>({1})),
                    LibsemigroupsException);

  REQUIRE_NOTHROW(Transformation<u_int16_t>(std::vector<u_int16_t>({0, 1, 2})));
  REQUIRE_NOTHROW(
      Transformation<u_int16_t>(std::initializer_list<u_int16_t>({0, 1, 2})));
  // Implicit type initializer lists are not accepted.
  // REQUIRE_NOTHROW(Transformation<u_int16_t>({0, 1, 2})));

  std::vector<u_int16_t>* pimgs = new std::vector<u_int16_t>({1, 2, 3});
  REQUIRE_NOTHROW(Transformation<u_int16_t>(pimgs));
  REQUIRE_THROWS_AS(
      Transformation<u_int16_t>(std::vector<u_int16_t>({1, 2, 3})),
      LibsemigroupsException);
  REQUIRE_THROWS_AS(
      Transformation<u_int16_t>(std::initializer_list<u_int16_t>({1, 2, 3})),
      LibsemigroupsException);

  auto& UNDEF
      = PartialTransformation<u_int16_t, PartialPerm<u_int16_t>>::UNDEFINED;
  REQUIRE_THROWS_AS(Transformation<u_int16_t>(std::initializer_list<u_int16_t>(
                        {UNDEF, UNDEF, UNDEF})),
                    LibsemigroupsException);
}

TEST_CASE("PartialPerm 01: u_int16_t methods", "[quick][element][pperm][01]") {
  auto& UNDEF
      = PartialTransformation<u_int16_t, PartialPerm<u_int16_t>>::UNDEFINED;
  Element* x = new PartialPerm<u_int16_t>({4, 5, 0}, {9, 0, 1}, 10);
  Element* y = new PartialPerm<u_int16_t>({4, 5, 0}, {9, 0, 1}, 10);
  REQUIRE(*x == *y);
  x->redefine(y, y);
  PartialPerm<u_int16_t>* xx = static_cast<PartialPerm<u_int16_t>*>(x);
  REQUIRE(xx->at(0) == UNDEF);
  REQUIRE(xx->at(1) == UNDEF);
  REQUIRE(xx->at(2) == UNDEF);
  REQUIRE(xx->at(3) == UNDEF);
  REQUIRE(xx->at(4) == UNDEF);
  REQUIRE(xx->at(5) == 1);

  REQUIRE(*x < *y);
  REQUIRE(!(*x < *x));
  Element* expected = new PartialPerm<u_int16_t>({UNDEF, UNDEF, UNDEF});
  REQUIRE(*expected < *x);
  expected->really_delete();
  delete expected;

  REQUIRE(x->degree() == 10);
  REQUIRE(y->degree() == 10);
  REQUIRE(x->complexity() == 10);
  REQUIRE(y->complexity() == 10);
  REQUIRE(static_cast<PartialPerm<u_int16_t>*>(x)->crank() == 1);
  REQUIRE(static_cast<PartialPerm<u_int16_t>*>(y)->crank() == 3);
  Element* id = x->identity();

  expected = new PartialPerm<u_int16_t>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  REQUIRE(*id == *expected);
  expected->really_delete();
  delete expected;

  Element* a = x->really_copy(10);
  REQUIRE(a->degree() == 20);
  a->really_delete();
  delete a;

  x->really_delete();
  delete x;
  y->really_delete();
  delete y;
  id->really_delete();
  delete id;
}

TEST_CASE("PartialPerm 02: u_int16_t hash", "[quick][element][pperm][02]") {
  Element* x = new PartialPerm<u_int16_t>(
      {0, 1, 2, 3, 5, 6, 9}, {9, 7, 3, 5, 4, 2, 1}, 10);
  for (size_t i = 0; i < 1000000; i++) {
    x->hash_value();
  }
  x->really_delete();
  delete x;
}

TEST_CASE("PartialPerm 03: u_int16_t delete/copy",
          "[quick][element][pperm][03]") {
  Element* x = new PartialPerm<u_int16_t>(
      {0, 1, 2, 3, 5, 6, 9}, {9, 7, 3, 5, 4, 2, 1}, 10);
  Element* y = x->really_copy();
  x->really_delete();  // deletes _vector in x but not in y
  delete x;

  Element* expected = new PartialPerm<u_int16_t>(
      {0, 1, 2, 3, 5, 6, 9}, {9, 7, 3, 5, 4, 2, 1}, 10);
  REQUIRE(*y == *expected);

  PartialPerm<u_int16_t> yy = *static_cast<PartialPerm<u_int16_t>*>(y);
  REQUIRE(yy == *y);
  PartialPerm<u_int16_t> zz(yy);
  delete y;  // does not delete the _vector in y, yy, or zz
  REQUIRE(zz == *expected);
  zz.really_delete();
  expected->really_delete();
  delete expected;
}

TEST_CASE("PartialPerm 04: u_int32_t methods", "[quick][element][pperm][04]") {
  auto& UNDEF
      = PartialTransformation<u_int32_t, PartialPerm<u_int32_t>>::UNDEFINED;
  Element* x = new PartialPerm<u_int32_t>({4, 5, 0}, {10, 0, 1}, 11);
  Element* y = new PartialPerm<u_int32_t>({4, 5, 0}, {10, 0, 1}, 11);
  REQUIRE(*x == *y);
  x->redefine(y, y);
  PartialPerm<u_int32_t>* xx = static_cast<PartialPerm<u_int32_t>*>(x);
  REQUIRE(xx->at(0) == UNDEF);
  REQUIRE(xx->at(1) == UNDEF);
  REQUIRE(xx->at(2) == UNDEF);
  REQUIRE(xx->at(3) == UNDEF);
  REQUIRE(xx->at(4) == UNDEF);
  REQUIRE(xx->at(5) == 1);
  REQUIRE((*x < *y) == true);

  Element* z = new PartialPerm<u_int32_t>({UNDEF, UNDEF, UNDEF});
  REQUIRE(*z < *x);
  z->really_delete();
  delete z;

  REQUIRE(x->degree() == 11);
  REQUIRE(y->degree() == 11);
  REQUIRE(x->complexity() == 11);
  REQUIRE(y->complexity() == 11);
  REQUIRE(static_cast<PartialPerm<u_int32_t>*>(x)->crank() == 1);
  REQUIRE(static_cast<PartialPerm<u_int32_t>*>(y)->crank() == 3);
  Element* id = x->identity();

  Element* expected
      = new PartialPerm<u_int32_t>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
  REQUIRE(*id == *expected);
  expected->really_delete();
  delete expected;

  x->really_delete();
  delete x;
  y->really_delete();
  delete y;
  id->really_delete();
  delete id;
}

TEST_CASE("PartialPerm 05: u_int32_t hash", "[quick][element][pperm][05]") {
  Element* x = new PartialPerm<u_int32_t>(
      {0, 1, 2, 3, 5, 6, 9}, {9, 7, 3, 5, 4, 2, 1}, 10);
  for (size_t i = 0; i < 1000000; i++) {
    x->hash_value();
  }
  x->really_delete();
  delete x;
}

TEST_CASE("PartialPerm 06: u_int32_t delete/copy",
          "[quick][element][pperm][06]") {
  Element* x = new PartialPerm<u_int32_t>(
      {0, 1, 2, 3, 5, 6, 9}, {9, 7, 3, 5, 4, 2, 1}, 10);
  Element* y = x->really_copy();
  x->really_delete();  // deletes _vector in x but not in y
  delete x;

  Element* expected = new PartialPerm<u_int32_t>(
      {0, 1, 2, 3, 5, 6, 9}, {9, 7, 3, 5, 4, 2, 1}, 10);
  REQUIRE(*y == *expected);

  PartialPerm<u_int32_t> yy = *static_cast<PartialPerm<u_int32_t>*>(y);
  REQUIRE(yy == *y);
  PartialPerm<u_int32_t> zz(yy);
  delete y;  // does not delete the _vector in y, yy, or zz
  REQUIRE(zz == *expected);

  expected->really_delete();
  delete expected;
  zz.really_delete();
}

TEST_CASE("PartialPerm 07: exceptions", "[quick][element][pperm][07]") {
  auto& UNDEF = PartialPerm<u_int16_t>::UNDEFINED;
  REQUIRE_NOTHROW(PartialPerm<u_int16_t>(std::vector<u_int16_t>()));
  REQUIRE_NOTHROW(PartialPerm<u_int16_t>(std::vector<u_int16_t>({0})));
  REQUIRE_NOTHROW(PartialPerm<u_int16_t>(std::vector<u_int16_t>({UNDEF})));
  REQUIRE_THROWS_AS(PartialPerm<u_int16_t>(std::vector<u_int16_t>({1})),
                    LibsemigroupsException);

  REQUIRE_NOTHROW(PartialPerm<u_int16_t>(std::vector<u_int16_t>({0, 1, 2})));
  REQUIRE_NOTHROW(
      PartialPerm<u_int16_t>(std::initializer_list<u_int16_t>({0, 1, 2})));
  REQUIRE_NOTHROW(
      PartialPerm<u_int16_t>(std::vector<u_int16_t>({0, UNDEF, 2})));
  REQUIRE_NOTHROW(PartialPerm<u_int16_t>(
      std::vector<u_int16_t>({0, UNDEF, 5, UNDEF, UNDEF, 1})));

  std::vector<u_int16_t>* pimgs = new std::vector<u_int16_t>({1, 2, 3});
  REQUIRE_NOTHROW(PartialPerm<u_int16_t>(pimgs));
  REQUIRE_THROWS_AS(PartialPerm<u_int16_t>(std::vector<u_int16_t>({1, 2, 3})),
                    LibsemigroupsException);
  REQUIRE_THROWS_AS(
      PartialPerm<u_int16_t>(std::vector<u_int16_t>({UNDEF, UNDEF, 3})),
      LibsemigroupsException);
  REQUIRE_THROWS_AS(
      PartialPerm<u_int16_t>(std::vector<u_int16_t>({1, UNDEF, 1})),
      LibsemigroupsException);
  REQUIRE_THROWS_AS(PartialPerm<u_int16_t>(
                        std::vector<u_int16_t>({3, UNDEF, 2, 1, UNDEF, 3})),
                    LibsemigroupsException);
  REQUIRE_THROWS_AS(
      PartialPerm<u_int16_t>(std::initializer_list<u_int16_t>({1, 2, 3})),
      LibsemigroupsException);
  REQUIRE_NOTHROW(PartialPerm<u_int16_t>(
      std::vector<u_int16_t>({1, 2}), std::vector<u_int16_t>({0, 3}), 5));
  REQUIRE_NOTHROW(PartialPerm<u_int16_t>(
      std::vector<u_int16_t>({1, 2}), std::vector<u_int16_t>({0, 5}), 6));
  REQUIRE_THROWS_AS(PartialPerm<u_int16_t>(std::vector<u_int16_t>({1, 2}),
                                           std::vector<u_int16_t>({0}),
                                           5),
                    LibsemigroupsException);
  REQUIRE_THROWS_AS(PartialPerm<u_int16_t>(std::vector<u_int16_t>({1, 2}),
                                           std::vector<u_int16_t>({0, 5}),
                                           4),
                    LibsemigroupsException);
  REQUIRE_THROWS_AS(PartialPerm<u_int16_t>(std::vector<u_int16_t>({1, 5}),
                                           std::vector<u_int16_t>({0, 2}),
                                           4),
                    LibsemigroupsException);
}

TEST_CASE("BooleanMat 01: methods", "[quick][element][booleanmat][01]") {
  Element* x = new BooleanMat({{1, 0, 1}, {0, 1, 0}, {0, 1, 0}});
  Element* y = new BooleanMat({{0, 0, 0}, {0, 0, 0}, {0, 0, 0}});
  Element* z = new BooleanMat({{0, 0, 0}, {0, 0, 0}, {0, 0, 0}});
  REQUIRE(*y == *z);
  z->redefine(x, y);
  REQUIRE(*y == *z);
  z->redefine(y, x);
  REQUIRE(*y == *z);
  REQUIRE(!(*y < *z));
  REQUIRE(x->degree() == 3);
  REQUIRE(y->degree() == 3);
  REQUIRE(z->degree() == 3);
  REQUIRE(x->complexity() == 27);
  REQUIRE(y->complexity() == 27);
  REQUIRE(z->complexity() == 27);
  Element* id = x->identity();
  z->redefine(id, x);
  REQUIRE(*z == *x);
  z->redefine(x, id);
  REQUIRE(*z == *x);
  x->really_delete();
  delete x;
  y->really_delete();
  delete y;
  z->really_delete();
  delete z;
  id->really_delete();
  delete id;
}

TEST_CASE("BooleanMat 02: hash ~8ms", "[quick][element][booleanmat][02]") {
  Element* x = new BooleanMat({{1, 0, 1}, {0, 1, 0}, {0, 1, 0}});
  for (size_t i = 0; i < 1000000; i++) {
    x->hash_value();
  }
  x->really_delete();
  delete x;
}

TEST_CASE("BooleanMat 03: delete/copy", "[quick][element][booleanmat][03]") {
  Element* x = new BooleanMat({{1, 0, 1}, {0, 1, 0}, {0, 1, 0}});
  Element* y = x->really_copy();
  x->really_delete();  // deletes _vector in x but not in y
  delete x;

  Element* expected = new BooleanMat({{1, 0, 1}, {0, 1, 0}, {0, 1, 0}});
  REQUIRE(*y == *expected);

  BooleanMat yy = *static_cast<BooleanMat*>(y);
  REQUIRE(yy == *y);
  BooleanMat zz(yy);
  delete y;  // does not delete the _vector in y, yy, or zz
  REQUIRE(zz == *expected);
  zz.really_delete();
  expected->really_delete();
  delete expected;
}

TEST_CASE("Bipartition 01: overridden methods",
          "[quick][element][bipart][01]") {
  Element* x = new Bipartition(
      {0, 1, 2, 1, 0, 2, 1, 0, 2, 2, 0, 0, 2, 0, 3, 4, 4, 1, 3, 0});
  Element* y = new Bipartition(
      {0, 1, 1, 1, 1, 2, 3, 2, 4, 5, 5, 2, 4, 2, 1, 1, 1, 2, 3, 2});
  Element* z = new Bipartition(
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
  REQUIRE(!(*y == *z));

  z->redefine(x, y, 0);
  Element* expected = new Bipartition(
      {0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1});
  REQUIRE(*z == *expected);
  expected->really_delete();
  delete expected;

  expected = new Bipartition(
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 3, 1, 2, 1});
  z->redefine(y, x, 0);
  REQUIRE(*z == *expected);
  expected->really_delete();
  delete expected;

  REQUIRE(!(*y < *z));
  REQUIRE(x->degree() == 10);
  REQUIRE(y->degree() == 10);
  REQUIRE(z->degree() == 10);
  REQUIRE(x->complexity() == 100);
  REQUIRE(y->complexity() == 100);
  REQUIRE(z->complexity() == 100);

  Element* id = x->identity();
  z->redefine(id, x, 0);
  REQUIRE(*z == *x);
  z->redefine(x, id, 0);
  REQUIRE(*z == *x);
  z->redefine(id, y, 0);
  REQUIRE(*z == *y);
  z->redefine(y, id, 0);
  REQUIRE(*z == *y);
  x->really_delete();
  delete x;
  y->really_delete();
  delete y;
  z->really_delete();
  delete z;
  id->really_delete();
  delete id;
}

TEST_CASE("Bipartition 02: hash ~12ms", "[quick][element][bipart][02]") {
  Element* x = new Bipartition(
      {0, 1, 2, 1, 0, 2, 1, 0, 2, 2, 0, 0, 2, 0, 3, 4, 4, 1, 3, 0});
  for (size_t i = 0; i < 1000000; i++) {
    x->hash_value();
  }
  x->really_delete();
  delete x;
}

TEST_CASE("Bipartition 03: non-overridden methods",
          "[quick][element][bipart][03]") {
  Bipartition* x = new Bipartition(
      {0, 0, 0, 0, 0, 0, 1, 2, 0, 1, 0, 0, 1, 2, 3, 3, 0, 4, 1, 1});

  REQUIRE(x->rank() == 3);
  REQUIRE(x->at(0) == 0);
  REQUIRE(x->at(6) == 1);
  REQUIRE(x->at(10) == 0);
  REQUIRE(x->const_nr_blocks() == 5);
  REQUIRE(x->nr_blocks() == 5);
  REQUIRE(x->const_nr_blocks() == 5);
  REQUIRE(x->nr_blocks() == 5);
  REQUIRE(x->nr_left_blocks() == 3);
  REQUIRE(x->nr_right_blocks() == 5);
  REQUIRE(x->is_transverse_block(0));
  REQUIRE(x->is_transverse_block(1));
  REQUIRE(x->is_transverse_block(2));
  REQUIRE(!x->is_transverse_block(3));
  REQUIRE(!x->is_transverse_block(4));

  Bipartition* y = new Bipartition(
      {0, 0, 1, 2, 3, 3, 0, 4, 1, 1, 0, 0, 0, 0, 0, 0, 1, 2, 0, 1});

  Blocks* a = x->left_blocks();
  Blocks* b = y->right_blocks();
  REQUIRE(*a == *b);
  delete a;
  delete b;
  a = x->right_blocks();
  b = y->left_blocks();
  REQUIRE(*a == *b);
  delete a;
  delete b;
  x->really_delete();
  delete x;
  y->really_delete();
  delete y;

  x = new Bipartition(
      {0, 0, 0, 0, 0, 0, 1, 2, 0, 1, 0, 0, 1, 2, 3, 3, 0, 4, 1, 1});
  x->set_nr_blocks(5);
  REQUIRE(x->nr_blocks() == 5);
  x->really_delete();
  delete x;

  x = new Bipartition(
      {0, 0, 0, 0, 0, 0, 1, 2, 0, 1, 0, 0, 1, 2, 3, 3, 0, 4, 1, 1});
  x->set_nr_left_blocks(3);
  REQUIRE(x->nr_left_blocks() == 3);
  REQUIRE(x->nr_right_blocks() == 5);
  REQUIRE(x->nr_blocks() == 5);
  x->really_delete();
  delete x;

  x = new Bipartition(
      {0, 0, 0, 0, 0, 0, 1, 2, 0, 1, 0, 0, 1, 2, 3, 3, 0, 4, 1, 1});
  x->set_rank(3);
  REQUIRE(x->rank() == 3);
  x->really_delete();
  delete x;
}

TEST_CASE("Bipartition 04: delete/copy", "[quick][element][bipart][04]") {
  Element* x = new Bipartition({0, 0, 0, 0});
  Element* y = x->really_copy();
  x->really_delete();  // deletes _vector in x but not in y
  delete x;

  Element* expected = new Bipartition({0, 0, 0, 0});
  REQUIRE(*y == *expected);

  Bipartition yy = *static_cast<Bipartition*>(y);
  REQUIRE(yy == *y);
  Bipartition zz(yy);
  delete y;  // does not delete the _vector in y, yy, or zz
  REQUIRE(zz == *expected);
  expected->really_delete();
  delete expected;
  zz.really_delete();
}

TEST_CASE("Bipartition 05: degree 0", "[quick][element][bipart][05]") {
  Bipartition* x = new Bipartition(std::vector<u_int32_t>({}));
  REQUIRE(x->const_nr_blocks() == 0);
  REQUIRE(x->nr_left_blocks() == 0);

  Blocks* b = x->left_blocks();
  REQUIRE(b->degree() == 0);
  REQUIRE(b->nr_blocks() == 0);
  delete b;

  b = x->right_blocks();
  REQUIRE(b->degree() == 0);
  REQUIRE(b->nr_blocks() == 0);
  delete b;

  x->really_delete();
  delete x;
}

TEST_CASE("Bipartition 06: exceptions", "[quick][bipart][06]") {
  REQUIRE_NOTHROW(Bipartition(std::vector<u_int32_t>()));
  REQUIRE_THROWS_AS(Bipartition({0}), LibsemigroupsException);
  REQUIRE_THROWS_AS(Bipartition({1, 0}), LibsemigroupsException);
}

TEST_CASE("ProjectiveMaxPlusMatrix 01: methods",
          "[quick][element][matrix][01]") {
  Semiring<int64_t>* sr = new MaxPlusSemiring();

  Element* x
      = new ProjectiveMaxPlusMatrix({{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  Element* expected = new ProjectiveMaxPlusMatrix(
      {{-4, 0, -2}, {-3, -2, -2}, {-1, -5, -1}}, sr);
  REQUIRE(*x == *expected);
  expected->really_delete();
  delete expected;

  REQUIRE(static_cast<MatrixOverSemiring<int64_t>*>(x)->semiring() == sr);

  Element* y = new ProjectiveMaxPlusMatrix(
      {{LONG_MIN, 0, 0}, {0, 1, 0}, {1, -1, 0}}, sr);
  expected = new ProjectiveMaxPlusMatrix(
      {{LONG_MIN, -1, -1}, {-1, 0, -1}, {0, -2, -1}}, sr);
  REQUIRE(*y == *expected);
  REQUIRE(!(*x == *y));
  expected->really_delete();
  delete expected;

  y->redefine(x, x);
  expected = new ProjectiveMaxPlusMatrix(
      {{-2, -1, -1}, {-2, -2, -2}, {-1, 0, -1}}, sr);
  REQUIRE(*y == *expected);
  expected->really_delete();
  delete expected;

  REQUIRE(*x < *y);
  REQUIRE(x->degree() == 3);
  REQUIRE(y->degree() == 3);
  REQUIRE(x->complexity() == 27);
  REQUIRE(y->complexity() == 27);
  Element* id = x->identity();
  y->redefine(id, x);
  REQUIRE(*y == *x);
  y->redefine(x, id);
  REQUIRE(*y == *x);
  x->really_delete();
  delete x;
  y->really_delete();
  delete y;
  id->really_delete();
  delete id;
  delete sr;
}

TEST_CASE("ProjectiveMaxPlusMatrix 02: hash ~11ms",
          "[quick][element][matrix][02]") {
  Semiring<int64_t>* sr = new MaxPlusSemiring();
  Element*           x
      = new ProjectiveMaxPlusMatrix({{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  for (size_t i = 0; i < 1000000; i++) {
    x->hash_value();
  }
  x->really_delete();
  delete x;
  delete sr;
}

TEST_CASE("ProjectiveMaxPlusMatrix 03: delete/copy",
          "[quick][element][matrix][03]") {
  Semiring<int64_t>* sr = new MaxPlusSemiring();
  Element*           x
      = new ProjectiveMaxPlusMatrix({{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  Element* y = x->really_copy();
  x->really_delete();  // deletes _vector in x but not in y
  delete x;

  Element* expected
      = new ProjectiveMaxPlusMatrix({{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  REQUIRE(*y == *expected);
  expected->really_delete();
  delete expected;

  ProjectiveMaxPlusMatrix yy = *static_cast<ProjectiveMaxPlusMatrix*>(y);
  REQUIRE(yy == *y);

  ProjectiveMaxPlusMatrix zz(yy);
  delete y;  // does not delete the _vector in y, yy, or zz
  expected
      = new ProjectiveMaxPlusMatrix({{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  REQUIRE(zz == *expected);
  expected->really_delete();
  delete expected;
  zz.really_delete();
  delete sr;
}

TEST_CASE("MatrixOverSemiring 01: Integers methods",
          "[quick][element][matrix][01]") {
  Semiring<int64_t>* sr = new Integers();

  Element* x = new MatrixOverSemiring<int64_t>(
      {{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  Element* expected = new MatrixOverSemiring<int64_t>(
      {{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  REQUIRE(*x == *expected);
  expected->really_delete();
  delete expected;

  Element* y = new MatrixOverSemiring<int64_t>(
      {{-100, 0, 0}, {0, 1, 0}, {1, -1, 0}}, sr);
  REQUIRE(!(*x == *y));

  y->redefine(x, x);
  expected = new MatrixOverSemiring<int64_t>(
      {{2, -4, 0}, {2, -2, 0}, {2, -1, 1}}, sr);
  REQUIRE(*y == *expected);
  expected->really_delete();
  delete expected;

  REQUIRE(*x < *y);
  REQUIRE(x->degree() == 3);
  REQUIRE(y->degree() == 3);
  REQUIRE(x->complexity() == 27);
  REQUIRE(y->complexity() == 27);
  Element* id = x->identity();
  y->redefine(id, x);
  REQUIRE(*y == *x);
  y->redefine(x, id);
  REQUIRE(*y == *x);
  x->really_delete();
  delete x;
  y->really_delete();
  delete y;
  id->really_delete();
  delete id;
  delete sr;
}

TEST_CASE("MatrixOverSemiring 02: Integers, hash ~11ms",
          "[quick][element][matrix][02]") {
  Semiring<int64_t>* sr = new Integers();
  Element*           x  = new MatrixOverSemiring<int64_t>(
      {{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  for (size_t i = 0; i < 1000000; i++) {
    x->hash_value();
  }
  x->really_delete();
  delete x;
  delete sr;
}

TEST_CASE("MatrixOverSemiring 03: MaxPlusSemiring methods",
          "[quick][element][matrix][03]") {
  Semiring<int64_t>* sr = new MaxPlusSemiring();

  Element* x = new MatrixOverSemiring<int64_t>(
      {{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  Element* expected = new MatrixOverSemiring<int64_t>(
      {{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  REQUIRE(*x == *expected);
  expected->really_delete();
  delete expected;

  Element* y = new MatrixOverSemiring<int64_t>(
      {{-100, 0, 0}, {0, 1, 0}, {1, -1, 0}}, sr);
  REQUIRE(!(*x == *y));

  y->redefine(x, x);
  expected
      = new MatrixOverSemiring<int64_t>({{1, 2, 2}, {1, 1, 1}, {2, 3, 2}}, sr);
  REQUIRE(*y == *expected);
  expected->really_delete();
  delete expected;

  REQUIRE(*x < *y);
  REQUIRE(x->degree() == 3);
  REQUIRE(y->degree() == 3);
  REQUIRE(x->complexity() == 27);
  REQUIRE(y->complexity() == 27);
  Element* id = x->identity();
  y->redefine(id, x);
  REQUIRE(*y == *x);
  y->redefine(x, id);
  REQUIRE(*y == *x);
  x->really_delete();
  delete x;
  y->really_delete();
  delete y;
  id->really_delete();
  delete id;
  delete sr;
}

TEST_CASE("MatrixOverSemiring 04: MaxPlusSemiring hash ~11ms",
          "[quick][element][matrix][04]") {
  Semiring<int64_t>* sr = new MaxPlusSemiring();
  Element*           x  = new MatrixOverSemiring<int64_t>(
      {{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  for (size_t i = 0; i < 1000000; i++) {
    x->hash_value();
  }
  x->really_delete();
  delete x;
  delete sr;
}

TEST_CASE("MatrixOverSemiring 05: MinPlusSemiring methods",
          "[quick][element][matrix][05]") {
  Semiring<int64_t>* sr = new MinPlusSemiring();

  Element* x = new MatrixOverSemiring<int64_t>(
      {{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  Element* expected = new MatrixOverSemiring<int64_t>(
      {{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  REQUIRE(*x == *expected);
  expected->really_delete();
  delete expected;

  Element* y = new MatrixOverSemiring<int64_t>(
      {{-100, 0, 0}, {0, 1, 0}, {1, -1, 0}}, sr);
  REQUIRE(!(*x == *y));

  y->redefine(x, x);
  expected = new MatrixOverSemiring<int64_t>(
      {{-4, -3, -2}, {-3, -3, -1}, {-4, -3, -3}}, sr);
  REQUIRE(*y == *expected);
  expected->really_delete();
  delete expected;

  REQUIRE(!(*x < *y));
  REQUIRE(x->degree() == 3);
  REQUIRE(y->degree() == 3);
  REQUIRE(x->complexity() == 27);
  REQUIRE(y->complexity() == 27);
  Element* id = x->identity();
  y->redefine(id, x);
  REQUIRE(*y == *x);
  y->redefine(x, id);
  REQUIRE(*y == *x);
  x->really_delete();
  delete x;
  y->really_delete();
  delete y;
  id->really_delete();
  delete id;
  delete sr;
}

TEST_CASE("MatrixOverSemiring 06: MinPlusSemiring hash ~11ms",
          "[quick][element][matrix][06]") {
  Semiring<int64_t>* sr = new MinPlusSemiring();
  Element*           x  = new MatrixOverSemiring<int64_t>(
      {{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  for (size_t i = 0; i < 1000000; i++) {
    x->hash_value();
  }
  x->really_delete();
  delete x;
  delete sr;
}

TEST_CASE("MatrixOverSemiring 07: TropicalMaxPlusSemiring methods",
          "[quick][element][matrix][07]") {
  Semiring<int64_t>* sr = new TropicalMaxPlusSemiring(33);

  Element* x = new MatrixOverSemiring<int64_t>(
      {{22, 21, 0}, {10, 0, 0}, {1, 32, 1}}, sr);
  Element* expected = new MatrixOverSemiring<int64_t>(
      {{22, 21, 0}, {10, 0, 0}, {1, 32, 1}}, sr);
  REQUIRE(*x == *expected);
  expected->really_delete();
  delete expected;

  REQUIRE_THROWS_AS(
      MatrixOverSemiring<int64_t>({{-100, 0, 0}, {0, 1, 0}, {1, -1, 0}}, sr),
      LibsemigroupsException);
  auto y
      = new MatrixOverSemiring<int64_t>({{10, 0, 0}, {0, 1, 0}, {1, 1, 0}}, sr);
  REQUIRE(!(*x == *y));

  y->redefine(x, x);
  expected = new MatrixOverSemiring<int64_t>(
      {{33, 33, 22}, {32, 32, 10}, {33, 33, 32}}, sr);
  REQUIRE(*y == *expected);
  expected->really_delete();

  delete expected;
  REQUIRE(*x < *y);
  REQUIRE(x->degree() == 3);
  REQUIRE(y->degree() == 3);
  REQUIRE(x->complexity() == 27);
  REQUIRE(y->complexity() == 27);
  Element* id = x->identity();
  y->redefine(id, x);
  REQUIRE(*y == *x);
  y->redefine(x, id);
  REQUIRE(*y == *x);
  x->really_delete();
  delete x;
  y->really_delete();
  delete y;
  id->really_delete();
  delete id;
  delete sr;
}

TEST_CASE("MatrixOverSemiring 08: TropicalMaxPlusSemiring hash ~11ms",
          "[quick][element][matrix][08]") {
  Semiring<int64_t>* sr = new TropicalMaxPlusSemiring(33);
  Element*           x  = new MatrixOverSemiring<int64_t>(
      {{22, 21, 0}, {10, 0, 0}, {1, 32, 1}}, sr);
  for (size_t i = 0; i < 1000000; i++) {
    x->hash_value();
  }
  x->really_delete();
  delete x;
  delete sr;
}

TEST_CASE("MatrixOverSemiring 09: TropicalMinPlusSemiring methods",
          "[quick][element][matrix][09]") {
  Semiring<int64_t>* sr = new TropicalMinPlusSemiring(33);

  Element* x = new MatrixOverSemiring<int64_t>(
      {{22, 21, 0}, {10, 0, 0}, {1, 32, 1}}, sr);

  Element* expected = new MatrixOverSemiring<int64_t>(
      {{22, 21, 0}, {10, 0, 0}, {1, 32, 1}}, sr);
  REQUIRE(*x == *expected);
  expected->really_delete();
  delete expected;

  Element* y
      = new MatrixOverSemiring<int64_t>({{10, 0, 0}, {0, 1, 0}, {1, 1, 0}}, sr);
  REQUIRE(!(*x == *y));

  y->redefine(x, x);
  expected = new MatrixOverSemiring<int64_t>(
      {{1, 21, 1}, {1, 0, 0}, {2, 22, 1}}, sr);
  REQUIRE(*y == *expected);
  expected->really_delete();
  delete expected;

  REQUIRE(!(*x < *y));
  REQUIRE(x->degree() == 3);
  REQUIRE(y->degree() == 3);
  REQUIRE(x->complexity() == 27);
  REQUIRE(y->complexity() == 27);
  Element* id = x->identity();
  y->redefine(id, x);
  REQUIRE(*y == *x);
  y->redefine(x, id);
  REQUIRE(*y == *x);
  x->really_delete();
  delete x;
  y->really_delete();
  delete y;
  id->really_delete();
  delete id;
  delete sr;
}

TEST_CASE("MatrixOverSemiring 10: TropicalMinPlusSemiring hash ~11ms",
          "[quick][element][matrix][10]") {
  Semiring<int64_t>* sr = new TropicalMinPlusSemiring(33);
  Element*           x  = new MatrixOverSemiring<int64_t>(
      {{22, 21, 0}, {10, 0, 0}, {1, 32, 1}}, sr);
  for (size_t i = 0; i < 1000000; i++) {
    x->hash_value();
  }
  x->really_delete();
  delete x;
  delete sr;
}

TEST_CASE("MatrixOverSemiring 11: NaturalSemiring methods",
          "[quick][element][matrix][11]") {
  Semiring<int64_t>* sr = new NaturalSemiring(33, 2);

  Element* x = new MatrixOverSemiring<int64_t>(
      {{22, 21, 0}, {10, 0, 0}, {1, 32, 1}}, sr);
  Element* expected = new MatrixOverSemiring<int64_t>(
      {{22, 21, 0}, {10, 0, 0}, {1, 32, 1}}, sr);
  REQUIRE(*x == *expected);
  expected->really_delete();
  delete expected;

  Element* y
      = new MatrixOverSemiring<int64_t>({{10, 0, 0}, {0, 1, 0}, {1, 1, 0}}, sr);
  REQUIRE(!(*x == *y));

  y->redefine(x, x);
  expected = new MatrixOverSemiring<int64_t>(
      {{34, 34, 0}, {34, 34, 0}, {33, 33, 1}}, sr);
  REQUIRE(*y == *expected);
  expected->really_delete();
  delete expected;

  REQUIRE(*x < *y);
  REQUIRE(x->degree() == 3);
  REQUIRE(y->degree() == 3);
  REQUIRE(x->complexity() == 27);
  REQUIRE(y->complexity() == 27);
  Element* id = x->identity();
  y->redefine(id, x);
  REQUIRE(*y == *x);
  y->redefine(x, id);
  REQUIRE(*y == *x);
  x->really_delete();
  delete x;
  y->really_delete();
  delete y;
  id->really_delete();
  delete id;
  delete sr;
}

TEST_CASE("MatrixOverSemiring 12: NaturalSemiring hash ~11ms",
          "[quick][element][matrix][12]") {
  Semiring<int64_t>* sr = new NaturalSemiring(33, 2);
  Element*           x  = new MatrixOverSemiring<int64_t>(
      {{22, 21, 0}, {10, 0, 0}, {1, 32, 1}}, sr);

  for (size_t i = 0; i < 1000000; i++) {
    x->hash_value();
  }
  x->really_delete();
  delete x;
  delete sr;
}

TEST_CASE("MatrixOverSemiring 13: Integers delete/copy",
          "[quick][element][matrix][13]") {
  Semiring<int64_t>* sr = new Integers();
  Element*           x  = new MatrixOverSemiring<int64_t>(
      {{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  Element* y = x->really_copy();

  x->really_delete();  // deletes _vector in x but not in y
  delete x;
  Element* expected = new MatrixOverSemiring<int64_t>(
      {{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  REQUIRE(*y == *expected);
  expected->really_delete();
  delete expected;

  MatrixOverSemiring<int64_t> yy
      = *static_cast<MatrixOverSemiring<int64_t>*>(y);
  REQUIRE(yy == *y);
  MatrixOverSemiring<int64_t> zz(yy);
  delete y;  // does not delete the _vector in y, yy, or zz
  REQUIRE(zz == yy);
  zz.really_delete();  // deletes _vector in yy, zz, and y
  delete sr;
}

TEST_CASE("MatrixOverSemiring 14: MaxPlusSemiring delete/copy",
          "[quick][element][matrix][14]") {
  Semiring<int64_t>* sr = new MaxPlusSemiring();
  Element*           x  = new MatrixOverSemiring<int64_t>(
      {{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  Element* y = x->really_copy();

  x->really_delete();  // deletes _vector in x but not in y
  delete x;
  Element* expected = new MatrixOverSemiring<int64_t>(
      {{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  REQUIRE(*y == *expected);
  expected->really_delete();
  delete expected;

  MatrixOverSemiring<int64_t> yy
      = *static_cast<MatrixOverSemiring<int64_t>*>(y);
  REQUIRE(yy == *y);
  MatrixOverSemiring<int64_t> zz(yy);
  delete y;  // does not delete the _vector in y, yy, or zz
  REQUIRE(zz == yy);
  zz.really_delete();  // deletes _vector in yy, zz, and y
  delete sr;
}

TEST_CASE("MatrixOverSemiring 15: MinPlusSemiring delete/copy",
          "[quick][element][matrix][15]") {
  Semiring<int64_t>* sr = new MinPlusSemiring();
  Element*           x  = new MatrixOverSemiring<int64_t>(
      {{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  Element* y = x->really_copy();

  x->really_delete();  // deletes _vector in x but not in y
  delete x;
  Element* expected = new MatrixOverSemiring<int64_t>(
      {{-2, 2, 0}, {-1, 0, 0}, {1, -3, 1}}, sr);
  REQUIRE(*y == *expected);
  expected->really_delete();
  delete expected;

  MatrixOverSemiring<int64_t> yy
      = *static_cast<MatrixOverSemiring<int64_t>*>(y);
  REQUIRE(yy == *y);
  MatrixOverSemiring<int64_t> zz(yy);
  delete y;  // does not delete the _vector in y, yy, or zz
  REQUIRE(zz == yy);
  zz.really_delete();  // deletes _vector in yy, zz, and y
  delete sr;
}

TEST_CASE("MatrixOverSemiring 16: TropicalMaxPlusSemiring delete/copy",
          "[quick][element][matrix][16]") {
  Semiring<int64_t>* sr = new TropicalMaxPlusSemiring(23);
  Element*           x
      = new MatrixOverSemiring<int64_t>({{2, 2, 0}, {1, 0, 0}, {1, 3, 1}}, sr);
  Element* y = x->really_copy();

  x->really_delete();  // deletes _vector in x but not in y
  delete x;
  Element* expected
      = new MatrixOverSemiring<int64_t>({{2, 2, 0}, {1, 0, 0}, {1, 3, 1}}, sr);
  REQUIRE(*y == *expected);
  expected->really_delete();
  delete expected;

  MatrixOverSemiring<int64_t> yy
      = *static_cast<MatrixOverSemiring<int64_t>*>(y);
  REQUIRE(yy == *y);
  MatrixOverSemiring<int64_t> zz(yy);
  delete y;  // does not delete the _vector in y, yy, or zz
  REQUIRE(zz == yy);
  zz.really_delete();  // deletes _vector in yy, zz, and y
  delete sr;
}

TEST_CASE("MatrixOverSemiring 17: TropicalMinPlusSemiring delete/copy",
          "[quick][element][matrix][17]") {
  Semiring<int64_t>* sr = new TropicalMinPlusSemiring(23);
  Element*           x
      = new MatrixOverSemiring<int64_t>({{2, 2, 0}, {1, 0, 0}, {1, 3, 1}}, sr);
  Element* y = x->really_copy();

  x->really_delete();  // deletes _vector in x but not in y
  delete x;
  Element* expected
      = new MatrixOverSemiring<int64_t>({{2, 2, 0}, {1, 0, 0}, {1, 3, 1}}, sr);
  REQUIRE(*y == *expected);
  expected->really_delete();
  delete expected;

  MatrixOverSemiring<int64_t> yy
      = *static_cast<MatrixOverSemiring<int64_t>*>(y);
  REQUIRE(yy == *y);
  MatrixOverSemiring<int64_t> zz(yy);
  delete y;  // does not delete the _vector in y, yy, or zz
  REQUIRE(zz == yy);
  zz.really_delete();  // deletes _vector in yy, zz, and y
  delete sr;
}

TEST_CASE("MatrixOverSemiring 18: NaturalSemiring delete/copy",
          "[quick][element][matrix][18]") {
  Semiring<int64_t>* sr = new NaturalSemiring(23, 1);
  Element*           x
      = new MatrixOverSemiring<int64_t>({{2, 2, 0}, {1, 0, 0}, {1, 3, 1}}, sr);
  Element* y = x->really_copy();

  x->really_delete();  // deletes _vector in x but not in y
  delete x;
  Element* expected
      = new MatrixOverSemiring<int64_t>({{2, 2, 0}, {1, 0, 0}, {1, 3, 1}}, sr);
  REQUIRE(*y == *expected);
  expected->really_delete();
  delete expected;

  MatrixOverSemiring<int64_t> yy
      = *static_cast<MatrixOverSemiring<int64_t>*>(y);
  REQUIRE(yy == *y);
  MatrixOverSemiring<int64_t> zz(yy);
  delete y;  // does not delete the _vector in y, yy, or zz
  REQUIRE(zz == yy);
  zz.really_delete();  // deletes _vector in yy, zz, and y
  delete sr;
}

TEST_CASE("MatrixOverSemiring 19: exceptions", "[quick][element][matrix][18]") {
  Semiring<int64_t>* sr = new NaturalSemiring(23, 1);
  REQUIRE_THROWS_AS(MatrixOverSemiring<int64_t>({{0, 0}, {0, 0}}, nullptr),
                    LibsemigroupsException);
  REQUIRE_THROWS_AS(
      MatrixOverSemiring<int64_t>(std::vector<std::vector<int64_t>>(), sr),
      LibsemigroupsException);
  REQUIRE_THROWS_AS(
      MatrixOverSemiring<int64_t>({{2, 2, 0}, {0, 0}, {1, 3, 1}}, sr),
      LibsemigroupsException);
  delete sr;
}

TEST_CASE("PBR 02: universal product", "[quick][element][pbr][02]") {
  Element* x
      = new PBR(new std::vector<std::vector<u_int32_t>>({{5, 3},
                                                         {5, 4, 3, 0, 1, 2},
                                                         {5, 4, 3, 0, 2},
                                                         {5, 3, 0, 1, 2},
                                                         {5, 0, 2},
                                                         {5, 4, 3, 1, 2}}));
  Element* y = new PBR(new std::vector<std::vector<u_int32_t>>({{5, 4, 3, 0},
                                                                {5, 4, 2},
                                                                {5, 1, 2},
                                                                {5, 4, 3, 2},
                                                                {5, 4, 3, 2},
                                                                {4, 1, 2}}));

  Element* z = new PBR(new std::vector<std::vector<u_int32_t>>({{5, 4, 3, 0},
                                                                {5, 4, 2},
                                                                {5, 1, 2},
                                                                {5, 4, 3, 2},
                                                                {5, 4, 3, 2},
                                                                {4, 1, 2}}));
  z->redefine(x, y);

  Element* expected
      = new PBR(new std::vector<std::vector<u_int32_t>>({{0, 1, 2, 3, 4, 5},
                                                         {0, 1, 2, 3, 4, 5},
                                                         {0, 1, 2, 3, 4, 5},
                                                         {0, 1, 2, 3, 4, 5},
                                                         {0, 1, 2, 3, 4, 5},
                                                         {0, 1, 2, 3, 4, 5}}));
  REQUIRE(*z == *expected);

  x->really_delete();
  delete x;
  y->really_delete();
  delete y;
  z->really_delete();
  delete z;
  expected->really_delete();
  delete expected;
}

TEST_CASE("PBR 03: product [bigger than previous]",
          "[quick][element][pbr][03]") {
  Element* x
      = new PBR(new std::vector<std::vector<u_int32_t>>({{5, 3},
                                                         {5, 4, 3, 0, 1, 2},
                                                         {5, 4, 3, 0, 2},
                                                         {5, 3, 0, 1, 2},
                                                         {5, 0, 2},
                                                         {5, 4, 3, 1, 2},
                                                         {},
                                                         {}}));
  Element* y
      = new PBR(new std::vector<std::vector<u_int32_t>>({{5, 3},
                                                         {5, 4, 3, 0, 1, 2},
                                                         {5, 4, 3, 0, 2},
                                                         {5, 3, 0, 1, 2},
                                                         {5, 0, 2},
                                                         {5, 4, 3, 1, 2},
                                                         {},
                                                         {6}}));
  x->redefine(y, y);
  Element* expected
      = new PBR(new std::vector<std::vector<u_int32_t>>({{0, 1, 2, 3, 4, 5},
                                                         {0, 1, 2, 3, 4, 5},
                                                         {0, 1, 2, 3, 4, 5},
                                                         {0, 1, 2, 3, 4, 5},
                                                         {0, 1, 2, 3, 4, 5},
                                                         {0, 1, 2, 3, 4, 5},
                                                         {},
                                                         {6}}));

  REQUIRE(*x == *expected);

  x->really_delete();
  delete x;
  y->really_delete();
  delete y;
  expected->really_delete();
  delete expected;

  x = new PBR(new std::vector<std::vector<u_int32_t>>(
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {7}}));
  y = new PBR(new std::vector<std::vector<u_int32_t>>(
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {7}}));

  x->redefine(y, y);
  expected = new PBR(new std::vector<std::vector<u_int32_t>>(
      {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {7}}));
  REQUIRE(*x == *expected);

  x->really_delete();
  delete x;
  y->really_delete();
  delete y;
  expected->really_delete();
  delete expected;
}

TEST_CASE("PBR 04: hash ~28ms", "[quick][element][pbr][04]") {
  Element* x = new PBR(new std::vector<std::vector<u_int32_t>>(
      {{1}, {4}, {3}, {1}, {0, 2}, {0, 3, 4, 5}}));
  for (size_t i = 0; i < 1000000; i++) {
    x->hash_value();
  }
  x->really_delete();
  delete x;
}

TEST_CASE("PBR 05: delete/copy", "[quick][element][pbr][05]") {
  Element* x = new PBR(new std::vector<std::vector<u_int32_t>>(
      {{1}, {4}, {3}, {1}, {0, 2}, {0, 3, 4, 5}}));
  Element* y = x->really_copy();
  x->really_delete();  // deletes _vector in x but not in y
  delete x;
  Element* z = new PBR(new std::vector<std::vector<u_int32_t>>(
      {{1}, {4}, {3}, {1}, {0, 2}, {0, 3, 4, 5}}));
  REQUIRE(*y == *z);
  z->really_delete();
  delete z;
  PBR yy = *static_cast<PBR*>(y);
  REQUIRE(yy == *y);
  PBR zz(yy);
  delete y;  // does not delete the _vector in y, yy, or zz
  Element* a = new PBR(new std::vector<std::vector<u_int32_t>>(
      {{1}, {4}, {3}, {1}, {0, 2}, {0, 3, 4, 5}}));
  REQUIRE(zz == *a);
  zz.really_delete();
  a->really_delete();
  delete a;
}

TEST_CASE("PBR 06: exceptions", "[quick][element][pbr][06]") {
  REQUIRE_THROWS_AS(PBR({{1}, {4}, {3}, {10}, {0, 2}, {0, 3, 4, 5}}),
                    LibsemigroupsException);
  REQUIRE_THROWS_AS(PBR({{4}, {3}, {0}, {0, 2}, {0, 3, 4, 5}}),
                    LibsemigroupsException);
}

template <class T> bool test_inverse(Permutation<T>* s) {
  auto i  = s->inverse();
  auto id = static_cast<Permutation<T>*>(s->identity());
  auto p  = static_cast<Permutation<T>*>(s->really_copy());
  p->redefine(s, i);
  bool res = (*p == *id);
  p->redefine(i, s);
  res &= (*p == *id);
  s->really_delete();
  delete s;
  p->really_delete();
  delete p;
  i->really_delete();
  delete i;
  id->really_delete();
  delete id;
  return res;
}

TEST_CASE("Permutation 01: inverse", "[quick][element][permutation][01]") {
  // Those two constructor if not passed a vector return an element
  // with _vector set to null (see issue #87).
  REQUIRE(
      test_inverse(new Permutation<u_int16_t>(new std::vector<u_int16_t>())));
  REQUIRE(test_inverse(
      new Permutation<u_int16_t>(new std::vector<u_int16_t>({0}))));
  REQUIRE(test_inverse(new Permutation<u_int16_t>({1, 0})));
  REQUIRE(test_inverse(new Permutation<u_int16_t>({0, 1})));
  REQUIRE(test_inverse(new Permutation<u_int16_t>({2, 0, 1, 4, 3})));
  REQUIRE(test_inverse(new Permutation<u_int16_t>({4, 2, 0, 1, 3})));
  REQUIRE(test_inverse(new Permutation<u_int16_t>({0, 1, 2, 3, 4})));
}

TEST_CASE("Permutation 02: exceptions", "[quick][element][permutation][02]") {
  REQUIRE_NOTHROW(Permutation<u_int16_t>(std::vector<u_int16_t>()));
  REQUIRE_NOTHROW(Permutation<u_int16_t>(std::vector<u_int16_t>({0})));
  REQUIRE_NOTHROW(Permutation<u_int16_t>(std::vector<u_int16_t>({0, 1})));
  REQUIRE_NOTHROW(Permutation<u_int16_t>(std::vector<u_int16_t>({1, 0})));
  REQUIRE_THROWS_AS(PartialPerm<u_int16_t>(std::vector<u_int16_t>({1, 2})),
                    LibsemigroupsException);
  REQUIRE_THROWS_AS(PartialPerm<u_int16_t>(std::vector<u_int16_t>({1, 0, 3})),
                    LibsemigroupsException);
  REQUIRE_NOTHROW(
      Permutation<u_int16_t>(std::vector<u_int16_t>({1, 4, 0, 3, 2})));
  REQUIRE_THROWS_AS(
      PartialPerm<u_int16_t>(std::vector<u_int16_t>({1, 0, 3, 6, 4})),
      LibsemigroupsException);
  REQUIRE_THROWS_AS(
      PartialPerm<u_int16_t>(std::vector<u_int16_t>({1, 5, 0, 3, 2})),
      LibsemigroupsException);
}
