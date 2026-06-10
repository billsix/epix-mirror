/*
 * Color_Utils.cc -- Functions for manipulating color densities
 *   These functions are not normally available to users.
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.17
 *
 * Last Change: September 14, 2007
 */

/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
 * Department of Mathematics and Computer Science
 * College of the Holy Cross
 * Worcester, MA, 01610-2395, USA
 *
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
#include <sstream>
#include <cmath>

#include "Color_Utils.h"

namespace ePiX {

// Magic number: 8-bits per color channel
const int COLORS(0xFF);

// Force double to [0,1]
double clip_to_unit(double t) {
  if (t < 0)
    return 0;
  else if (t > 1)
    return 1;
  else
    return t;
}

double rd(double arg)  // may assume arg in [0,1]
{
  return floor(COLORS * arg) / COLORS;  // round down to nearest 8-bit
}

// represent a double in [0,1] by an 8-bit hex string
std::string dtohex(double arg)  // assumes arg>0
{
  using std::ios_base;
  int dens((int)floor(arg * COLORS));

  std::ostringstream value;
  value.setf(ios_base::hex, ios_base::basefield);

  if (dens <= 0xF)  // pad
    value << "0";

  value << dens;
  return value.str();
}

// saw2th:R -> [-2,2] is (1) a sawtooth function; (2) the identity on [-2,2]
double saw2th(double x) { return x - 4 * floor((x + 2) / 4); }

/*
 *  clippy(x):
 *  1....          ____                   if (x >= 0)
 *      .\        /.  .                     return x <=  1 ? x :  1;
 *      . \      / .  .
 *  0......\____/  .  .                   else
 *      -2 -1   0  1  2 --> x               return x >= -1 ? 0 : -1-x;
 */
double clippy(double x) {
  x = saw2th(x);
  return (x >= 0) ? (x <= 1 ? x : 1) : (x >= -1 ? 0 : -1 - x);
}

double make_r(double r, double g, double b) {
  return clip_to_unit(clippy(r) + clippy(-g) + clippy(-b));
}

double make_g(double r, double g, double b) {
  return clip_to_unit(clippy(-r) + clippy(g) + clippy(-b));
}

double make_b(double r, double g, double b) {
  return clip_to_unit(clippy(-r) + clippy(-g) + clippy(b));
}
}  // namespace ePiX
