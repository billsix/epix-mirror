/*
 * length.h -- ePiX true and LaTeX length manipulation
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.2.0
 * Last Change: September 22, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
 * Department of Mathematics and Computer Science
 * College of the Holy Cross
 * Worcester, MA, 01610-2395, USA
 */

/*
 * ePiX is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ePiX is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ePiX; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*
 * This file provides the length class from ePiX2:
 *
 *   - length(std::string) assumes argument is "double [space(s)] unit"
 *     Recognized units are pt (points), in, cm, mm, pc (picas = 1/12 in), 
 *     bp (big points = 1/72 in)
 *
 *     Malformed strings are handled as follows:
 *     Missing doubles are assumed to be 0, units are assumed to be pt
 *
 *   - length(double) sets length to specified number of pt
 *
 *   - operator += Increments a length, returning answer in LHS units, e.g.
 *     1cm+=2in is 6.08cm
 *     2in+=1cm is 2.3937in
 *
 *   - operator *= scales a length by a double
 *   - to(string) converts a length to specified units
 *   - operator< compares true lengths
 *   - get_lengths(string, length, length) parses a string into ht and width
 *
 * A length can be negative.
 */

#ifndef EPIX_LENGTH
#define EPIX_LENGTH

#include <string>

namespace ePiX {

  class length {
  public:
    length(std::string); // not const std::string&
    explicit length(double = 0);

    length& operator+= (const length&); // increment by another length
    length& operator*= (double);  // scale
    length& to(std::string);            // convert to specified units

    // compare, with units conversion
    bool operator== (const length&) const;
    bool operator!= (const length& len) const { return !((*this) == len); }

    // for output
    double magnitude() const;
    std::string units() const;
    std::string name() const;

  private:
    double m_mag;
    double pts_per_unit; // constants defined in length.cc
    std::string m_units;
  }; // end of class length

  // value-returning operators
  length operator+ (length, const length&);
  length operator* (double, length len);

  // Compare true size; must pass by value...
  bool operator< (length arg1, length arg2);

  /*
   * Parse a string into two lengths (width and height).
   *
   * The "sz" argument is expected to look like "4in x 10cm" or "4 x 6 in",
   * specifically a double, an optional two-letter unitlength, an "x", a
   * double, and a two-letter unitlength (defaults to pt). Spaces are
   * unimportant, as is initial or trailing garbage. Badly malformed input
   * (e.g., "4,6 in") may confuse the parsing code on some platforms.
   */
  void get_lengths(std::string sz, length& length1, length& length2);

} // end of namespace

#endif /* EPIX_LENGTH */
