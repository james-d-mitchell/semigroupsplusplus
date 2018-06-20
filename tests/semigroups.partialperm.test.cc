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

TEST_CASE("Semigroup of PartialPerms 01",
          "[quick][semigroup][partialperm][finite][095]") {
  std::vector<PartialPerm<u_int16_t>> gens
      = {PartialPerm<u_int16_t>({0, 3, 4, 5}, {1, 0, 3, 2}, 6),
         PartialPerm<u_int16_t>({1, 2, 3}, {0, 5, 2}, 6),
         PartialPerm<u_int16_t>({0, 2, 3, 4, 5}, {5, 2, 3, 0, 1}, 6)};

  Semigroup<PartialPerm<u_int16_t>> S(gens);

  S.reserve(102);
  REPORTER.set_report(SEMIGROUPS_REPORT);

  REQUIRE(S.size() == 102);
  REQUIRE(S.nridempotents() == 8);
  size_t pos = 0;

  for (auto it = S.cbegin(); it < S.cend(); ++it) {
    REQUIRE(S.position(*it) == pos);
    pos++;
  }

  S.add_generators({PartialPerm<u_int16_t>({0, 1, 2}, {3, 4, 5}, 6)});
  REQUIRE(S.size() == 396);
  S.closure({PartialPerm<u_int16_t>({0, 1, 2}, {3, 4, 5}, 6)});
  REQUIRE(S.size() == 396);
  REQUIRE(S.minimal_factorisation(
              PartialPerm<u_int16_t>({0, 1, 2}, {3, 4, 5}, 6)
              * PartialPerm<u_int16_t>({0, 2, 3, 4, 5}, {5, 2, 3, 0, 1}, 6))
          == word_t({3, 2}));
  REQUIRE(S.minimal_factorisation(10) == word_t({2, 1}));
  REQUIRE(S.at(10) == PartialPerm<u_int16_t>({2, 3, 5}, {5, 2, 0}, 6));
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
