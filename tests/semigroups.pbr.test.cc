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

TEST_CASE("Semigroup of PBRs 01",
          "[quick][semigroup][pbr][finite][01]") {
  std::vector<PBR> gens = {PBR({{5, 3},
                                {5, 4, 3, 0, 1, 2},
                                {5, 4, 3, 0, 2},
                                {5, 3, 0, 1, 2},
                                {5, 0, 2},
                                {5, 4, 3, 1, 2}}),
                           PBR({{5, 4, 3, 0},
                                {5, 4, 2},
                                {5, 1, 2},
                                {5, 4, 3, 2},
                                {5, 4, 3, 2},
                                {4, 1, 2}}),
                           PBR({{5, 4, 3, 0},
                                {5, 4, 2},
                                {5, 1, 2},
                                {5, 4, 3, 2},
                                {5, 4, 3, 2},
                                {4, 1, 2}})};
  Semigroup<PBR>   S(gens);

  S.reserve(4);
  REPORTER.set_report(SEMIGROUPS_REPORT);

  REQUIRE(S.size() == 4);
  REQUIRE(S.nridempotents() == 2);
  size_t pos = 0;

  for (auto it = S.cbegin(); it < S.cend(); ++it) {
    REQUIRE(S.position(*it) == pos);
    pos++;
  }
  S.add_generators({PBR(
      {{5, 4, 3}, {5, 4, 2}, {4, 2, 1}, {5, 3, 0}, {5, 3, 2, 1}, {3, 1, 2}})});
  REQUIRE(S.size() == 6);
  S.closure({PBR(
      {{5, 4, 3}, {5, 4, 2}, {4, 2, 1}, {5, 3, 0}, {5, 3, 2, 1}, {3, 1, 2}})});
  REQUIRE(S.size() == 6);
  REQUIRE(S.minimal_factorisation(PBR({{5, 3},
                                       {5, 4, 3, 0, 1, 2},
                                       {5, 4, 3, 0, 2},
                                       {5, 3, 0, 1, 2},
                                       {5, 0, 2},
                                       {5, 4, 3, 1, 2}})
                                  * PBR({{5, 4, 3},
                                         {5, 4, 2},
                                         {4, 2, 1},
                                         {5, 3, 0},
                                         {5, 3, 2, 1},
                                         {3, 1, 2}}))
          == word_t({0, 0}));
  REQUIRE(S.minimal_factorisation(5) == word_t({3, 3}));
  REQUIRE(S.at(5)
          == PBR({{5, 4, 3},
                  {5, 4, 2},
                  {4, 2, 1},
                  {5, 3, 0},
                  {5, 3, 2, 1},
                  {3, 1, 2}})
                 * PBR({{5, 4, 3},
                        {5, 4, 2},
                        {4, 2, 1},
                        {5, 3, 0},
                        {5, 3, 2, 1},
                        {3, 1, 2}}));
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
