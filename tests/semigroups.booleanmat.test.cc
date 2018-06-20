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

TEST_CASE("Semigroup of BooleanMats 01: non-pointer BooleanMats",
          "[quick][semigroup][booleanmat][finite][01]") {
  std::vector<BooleanMat> gens
      = {BooleanMat({0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0}),
         BooleanMat({0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1}),
         BooleanMat({0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1})};

  Semigroup<BooleanMat> S(gens);

  S.reserve(26);
  REPORTER.set_report(SEMIGROUPS_REPORT);

  REQUIRE(S.size() == 26);
  REQUIRE(S.nridempotents() == 4);
  size_t pos = 0;

  for (auto it = S.cbegin(); it < S.cend(); ++it) {
    REQUIRE(S.position(*it) == pos);
    pos++;
  }

  S.add_generators(
      {BooleanMat({1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0})});
  REQUIRE(S.size() == 29);
  S.closure({BooleanMat({1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0})});
  REQUIRE(S.size() == 29);
  REQUIRE(S.minimal_factorisation(
              BooleanMat({1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0})
              * BooleanMat({0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0}))
          == word_t({3, 0}));
  REQUIRE(S.minimal_factorisation(28) == word_t({3, 0}));
  REQUIRE(
      S.at(28)
      == BooleanMat({1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0})
             * BooleanMat({0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0}));
  REQUIRE_THROWS_AS(S.minimal_factorisation(1000000000),
                    LibsemigroupsException);
  pos = 0;
  for (auto it = S.cbegin_idempotents(); it < S.cend_idempotents(); ++it) {
    REQUIRE(*it * *it == *it);
    pos++;
  }
  REQUIRE(pos == S.nridempotents());
  for (auto it = S.cbegin_sorted() + 1; it < S.cend_sorted(); ++it) {
    REQUIRE(*(it - 1) < *it);
  }
}
