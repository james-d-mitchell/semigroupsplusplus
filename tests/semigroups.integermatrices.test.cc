//
// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2018 James D. Mitchell
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

#include "../src/semigroups.h"
#include "catch.hpp"
#include <iostream>

#define SEMIGROUPS_REPORT false

using namespace libsemigroups;

TEST_CASE(
    "Semigroup of integer matrices 01",
    "[quick][semigroup][matrix][finite][01]") {
  Semiring<int64_t>*                       sr = new Integers();
  std::vector<MatrixOverSemiring<int64_t>> gens
      = {MatrixOverSemiring<int64_t>({{0, 1}, {0, -1}}, sr),
         MatrixOverSemiring<int64_t>({{0, 1}, {2, 0}}, sr)};
  Semigroup<MatrixOverSemiring<int64_t>> S
      = Semigroup<MatrixOverSemiring<int64_t>>(gens);

  S.reserve(10000);
  REPORTER.set_report(SEMIGROUPS_REPORT);

  S.enumerate(10000);
  REQUIRE(S.current_size() == 631);
  size_t pos = 0;

  for (auto it = S.cbegin(); it < S.cend(); ++it) {
    REQUIRE(S.position(*it) == pos);
    pos++;
  }
  S.enumerate(1000000);
  REQUIRE(S.current_size() == 631);
  REQUIRE(S.minimal_factorisation(
              MatrixOverSemiring<int64_t>({{0, 1}, {0, -1}}, sr)
              * MatrixOverSemiring<int64_t>({{0, 1}, {2, 0}}, sr)
              * MatrixOverSemiring<int64_t>({{0, 1}, {2, 0}}, sr))
          == word_t({0, 1, 0}));
  REQUIRE(S.minimal_factorisation(52)
          == word_t({0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}));
  REQUIRE(S.at(52) == MatrixOverSemiring<int64_t>({{64, 0}, {-64, 0}}, sr));
  REQUIRE_THROWS_AS(S.minimal_factorisation(1000000000),
                    LibsemigroupsException);
  delete sr;
}
