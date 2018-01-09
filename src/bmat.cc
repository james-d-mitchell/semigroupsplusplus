//
// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2017 Finn Smith
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

// This file contains an implementation of fast boolean matrices up to
// dimension 8 x 8.

#include "bmat.h"

#include <algorithm>
#include <random>
#include <string>
#include <vector>

#include "libsemigroups-debug.h"
#include "timer.h"

namespace libsemigroups {

  static_assert(std::is_trivial<BMat8>(), "BMat8 is not a trivial class!");
  std::vector<uint64_t> const BMat8::ROW_MASK = {0xff00000000000000,
                                                 0xff000000000000,
                                                 0xff0000000000,
                                                 0xff00000000,
                                                 0xff000000,
                                                 0xff0000,
                                                 0xff00,
                                                 0xff};

  std::vector<uint64_t> const BMat8::COL_MASK = {0x8080808080808080,
                                                 0x4040404040404040,
                                                 0x2020202020202020,
                                                 0x1010101010101010,
                                                 0x808080808080808,
                                                 0x404040404040404,
                                                 0x202020202020202,
                                                 0x101010101010101};

  std::array<uint64_t, 8> BMat8::for_sorting = {{0, 0, 0, 0, 0, 0, 0, 0}};

  // BMat methods
  std::random_device                    _rd;
  std::mt19937                          BMat8::_gen(_rd());
  std::uniform_int_distribution<size_t> BMat8::_dist(0, 0xffffffffffffffff);

  BMat8::BMat8(std::vector<std::vector<size_t>> const& mat) {
    LIBSEMIGROUPS_ASSERT(mat.size() <= 8);
    LIBSEMIGROUPS_ASSERT(0 < mat.size());
    _data        = 0;
    uint64_t pow = 1;
    pow          = pow << 63;
    for (auto row : mat) {
      LIBSEMIGROUPS_ASSERT(row.size() == mat.size());
      for (auto entry : row) {
        if (entry) {
          _data ^= pow;
        }
        pow = pow >> 1;
      }
      pow = pow >> (8 - mat.size());
    }
  }

  std::string BMat8::to_string() const {
    std::string out;
    uint64_t    bm  = _data;
    uint64_t    pow = 1;
    pow             = pow << 63;
    for (size_t i = 0; i < 8; ++i) {
      for (size_t j = 0; j < 8; ++j) {
        if (pow & bm) {
          out += "1";
        } else {
          out += "0";
        }
        bm = bm << 1;
      }
      out += "\n";
    }
    return out;
  }

  BMat8 BMat8::row_space_basis() const {
    uint64_t out            = 0;
    uint64_t combined_masks = 0;

    BMat8 bm(_data);
    bm.sort_rows();

    uint64_t no_dups = bm._data;

    for (size_t i = 0; i < 7; ++i) {
      combined_masks |= ROW_MASK[i];
      while ((no_dups & ROW_MASK[i + 1]) << 8 == (no_dups & ROW_MASK[i])
             && (no_dups & ROW_MASK[i]) != 0) {
        no_dups = ((no_dups & combined_masks)
                   | (no_dups & ~combined_masks & ~ROW_MASK[i + 1]) << 8);
      }
    }

    uint64_t cm = no_dups;

    for (size_t i = 0; i < 7; ++i) {
      cm = cyclic_shift(cm);
      out |= zero_if_row_not_contained(no_dups, cm);
    }
    for (size_t i = 0; i < 8; ++i) {
      if ((out & ROW_MASK[i]) == (no_dups & ROW_MASK[i])) {
        out &= ~ROW_MASK[i];
      } else {
        out |= (no_dups & ROW_MASK[i]);
      }
    }

    combined_masks = 0;
    for (size_t i = 0; i < 8; ++i) {
      combined_masks |= ROW_MASK[i];
      while ((out & ROW_MASK[i]) == 0 && ((out & ~combined_masks) != 0)) {
        out = (out & combined_masks) | ((out & ~combined_masks) << 8);
      }
    }
    return BMat8(out);
  }

  BMat8 BMat8::col_space_basis() const {
    return this->transpose().row_space_basis().transpose();
  }


  bool BMat8::operator()(size_t i, size_t j) const {
    LIBSEMIGROUPS_ASSERT(0 <= i && i < 8);
    uint64_t out = _data << (8 * i + j);
    return out >> 63;
  }

  BMat8 BMat8::random() {
    return BMat8(_dist(_gen));
  }

  BMat8 BMat8::random(size_t const dim) {
    LIBSEMIGROUPS_ASSERT(0 < dim && dim <= 8);
    BMat8 bm = BMat8::random();
    for (size_t i = dim + 1; i < 8; ++i) {
      bm._data &= ~ROW_MASK[i];
      bm._data &= ~COL_MASK[i];
    }
    return bm;
  }

  void BMat8::swap_rows(size_t i, size_t j) {
    LIBSEMIGROUPS_ASSERT(0 <= i && i < 8);
    LIBSEMIGROUPS_ASSERT(0 <= j && j < 8);
    LIBSEMIGROUPS_ASSERT(i != j);
    if (i > j) {
      swap_rows(j, i);
      return;
    }
    uint64_t y = (_data ^ (_data >> (j - i) * 8)) & ROW_MASK[j];
    _data ^= y ^ (y << (j - i) * 8);
  }

  void BMat8::assign(uint64_t data) {
    _data = data;
  }

  /*  BMat8 BMat8::lvalue(BMat8 rows, BMat8* tmp){
      tmp->redefine(rows, *this);
      *tmp = tmp->row_space_basis();
      return *tmp;
    }*/
}  // namespace libsemigroups
