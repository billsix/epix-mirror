/* 
 * Pair.h -- Ordered pairs and operations for Screen coordinates
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: July 29, 2005
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005
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
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/* 
 * This file provides the:
 *   - Pair class, for Cartesian Screen coordinates
 *   - Affine operators
 */

#ifndef EPIX2_PAIRS
#define EPIX2_PAIRS

#include <cmath>
// #include "Functions.h"

namespace ePiX2 {

  class Pair {
  private:
    double h, v; // horizontal and vertical

  public:
    double x1() const { return h; }
    double x2() const { return v; }

    Pair(const double x=0, const double y=0) : h(x), v(y) {}

    // affine operations
    Pair operator +=(const Pair arg)
      {
	h += arg.h;
	v += arg.v;
	return *this;
      }

    Pair operator -=(const Pair arg)
      {
	h -= arg.h;
	v -= arg.v;
	return *this;
      }
    // scalar multiplication
    Pair operator *=(const double c)
      {
	h *= c;
	v *= c;
	return *this;
      }

    // (a,b)&=(x,y) -> (ax,by)
    Pair operator &=(const Pair arg)
      {
	h *= arg.h;
	v *= arg.v;
	return *this;
      }

  bool operator== (const Pair& arg)
    { return (h == arg.h && v == arg.v); }

  bool operator!= (const Pair& arg)
    { return (h != arg.h || v != arg.v); }

  }; // end of Pair class

  // Multiplication by i
  Pair J(const Pair);

  // complex multiplication
  Pair operator *(const Pair, const Pair);

  inline double norm(Pair arg) { return hypot(arg.x1(), arg.x2()); }

  // dot product
  double operator |(const Pair, const Pair);

  inline Pair operator+ (Pair arg1, const Pair arg2) { return arg1 += arg2; }
  inline Pair operator- (Pair arg1, const Pair arg2) { return arg1 -= arg2; }

  // inline Pair operator *(Pair arg1, const  double c) { return arg1 *= c; }
  inline Pair operator* (const  double c, Pair arg1) { return arg1 *= c; }

  // (a,b)&(x,y)=(ax,by), for unit/coordinate conversion
  inline Pair operator& (Pair arg1, const Pair arg2) { return arg1 &= arg2; }

} /* end of namespace */

#endif /* EPIX2_PAIRS */
