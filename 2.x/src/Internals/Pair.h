/*** 
 *** Pair.h -- ePiX2::Pair and rect classes
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: December 25, 2005
 ***
 *** 
 *** Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 *** Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
 *** Department of Mathematics and Computer Science
 *** College of the Holy Cross
 *** Worcester, MA, 01610-2395, USA
 ***
 ***
 *** ePiX is free software; you can redistribute it and/or modify it
 *** under the terms of the GNU General Public License as published by
 *** the Free Software Foundation; either version 2 of the License, or
 *** (at your option) any later version.
 ***
 *** ePiX is distributed in the hope that it will be useful, but WITHOUT
 *** ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *** or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 *** License for more details.
 ***
 *** You should have received a copy of the GNU General Public License
 *** along with ePiX; if not, write to the Free Software Foundation, Inc.,
 *** 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 ***
 *** 
 *** This file provides the classes
 ***   * Pair
 ***     - Vector addition, subtraction, scalar multiplication, dot product
 ***     - Complex multiplication, division, multiplication by i (*, /, J)
 ***  %% - Projection, reflection, and affine scaling operators  (%, ^, &)
 ***
 ***   Notes:
 ***     - Complex division throws div_by_zero if |denom| < EPIX2_EPSILON
 ***  %% - Reflection operator v^w returns -v if w=0
 ***
 ***   * Rect (rectangle data structure)
 ***     - size, corner, midpoint, and hiding functions
 ***/

#ifndef EPIX2_PAIRS
#define EPIX2_PAIRS

#include <cmath>
#include "Errors.h"

namespace ePiX2 {

  class Pair {
  private:
    double h, v; // horizontal and vertical

  public:
    double x1() const { return h; }
    double x2() const { return v; }

    Pair(const double x=0, const double y=0) : h(x), v(y) {}

    // vector operations
    Pair& operator +=(const Pair arg)
      {
	h += arg.h;
	v += arg.v;
	return *this;
      }

    Pair& operator -=(const Pair arg)
      {
	h -= arg.h;
	v -= arg.v;
	return *this;
      }
    // scalar multiplication
    Pair& operator *=(const double c)
      {
	h *= c;
	v *= c;
	return *this;
      }

    // complex multiplication and division
    Pair& operator *=(const Pair arg);
    Pair& operator /=(const Pair arg);

    // (a,b)&=(x,y) -> (ax,by)
    Pair& operator &=(const Pair arg)
      {
	h *= arg.h;
	v *= arg.v;
	return *this;
      }

    /*
    // projection to or reflection about arg
    Pair& operator %=(const Pair arg);
    Pair& operator ^=(const Pair arg);
    */

    bool operator== (const Pair arg) const;

    bool operator!= (const Pair arg) const
      { return !((*this)==arg); }

  }; // end of Pair class

  // Multiplication by i
  Pair J(const Pair);

  inline double norm(Pair arg) { return hypot(arg.x1(), arg.x2()); }

  // dot product
  double operator |(const Pair, const Pair);

  inline Pair operator+ (Pair arg1, const Pair arg2) { return arg1 += arg2; }
  inline Pair operator- (Pair arg1, const Pair arg2) { return arg1 -= arg2; }

  // inline Pair operator *(Pair arg1, const  double c) { return arg1 *= c; }
  inline Pair operator* (const  double c, Pair arg1) { return arg1 *= c; }

  // complex arithmetic
  inline Pair operator* (Pair arg, const Pair arg1) { return arg *= arg1; }
  inline Pair operator/ (Pair arg, const Pair arg1) { return arg /= arg1; }

  // affine scaling (a,b)&(x,y)=(ax,by), for unit/coordinate conversion
  inline Pair operator& (Pair arg1, const Pair arg2) { return arg1 &= arg2; }

  /*
  // projection v%w = proj_w(v) = v (mod w)
  inline Pair operator% (Pair arg1, const Pair arg2) { return arg1 %= arg2; }
  // reflection v^w = v reflected over w
  inline Pair operator^ (Pair arg1, const Pair arg2) { return arg1 ^= arg2; }
  */

  class Rect {
  public:
    Rect(void) { }
    Rect(const Pair arg1, const Pair arg2);

    double h_min(void) const;
    double v_min(void) const;

    double h_max(void) const;
    double v_max(void) const;

    double h_size(void) const;
    double v_size(void) const;

    // N.B. mid vs. min
    double h_mid(void) const;
    double v_mid(void) const;

    // alignment points
    Pair tr(void) const;
    Pair tl(void) const;
    Pair bl(void) const;
    Pair br(void) const;

    Pair  t(void) const;
    Pair  b(void) const;
    Pair  l(void) const;
    Pair  r(void) const;
    Pair  c(void) const;

    bool shows(const Pair arg) const;
    bool hides(const Pair arg) const { return !shows(arg); }

  private:
    Pair low_left;
    Pair top_right;

  }; // end of class Rect

} /* end of namespace */

#endif /* EPIX2_PAIRS */
