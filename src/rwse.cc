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

#include "rwse.h"

#include <algorithm>
#include <string>

namespace libsemigroups {
  template <>
  word_t*
  Semigroup<RWSE*, std::hash<RWSE*>, std::equal_to<RWSE*>>::factorisation(
      RWSE* x) {
    return const_cast<word_t*>(RWS::rws_word_to_word(
        &(reinterpret_cast<RWSE const*>(x))->get_rws_word()));
  }

  bool RWSE::operator<(Element const& that) const {
    rws_word_t const& u = this->_rws_word;
    rws_word_t const& v = static_cast<RWSE const&>(that)._rws_word;
    if (u != v && (u.size() < v.size() || (u.size() == v.size() && u < v))) {
      // TODO allow other reduction orders here
      return true;
    } else {
      return false;
    }
  }

  RWSE* RWSE::really_copy(size_t) const {
    return new RWSE(_rws, this->_rws_word, false);
  }

  void RWSE::copy(Element const* x) {
    RWSE const* xx(static_cast<RWSE const*>(x));
    _rws_word.assign(xx->_rws_word.cbegin(), xx->_rws_word.cend());
    reset_hash_value();
  }

  void RWSE::swap(Element* x) {
    RWSE* xx = static_cast<RWSE*>(x);
    _rws_word.swap(xx->_rws_word);
    std::swap(this->_hash_value, xx->_hash_value);
  }

  void RWSE::redefine(Element const* x, Element const* y, size_t const& tid) {
    (void) tid;
    RWSE const* xx = static_cast<RWSE const*>(x);
    RWSE const* yy = static_cast<RWSE const*>(y);
    LIBSEMIGROUPS_ASSERT(xx->_rws == yy->_rws);
    _rws_word.clear();
    _rws_word.append(xx->_rws_word);
    _rws_word.append(yy->_rws_word);
    _rws->rewrite(&_rws_word);
    this->reset_hash_value();
  }
}  // namespace libsemigroups
