/*** 
 ***  Point.h -- ePiX2::Point class
 ***
 *** This file is part of ePiX, a preprocessor for creating high-quality 
 *** line figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: July 29, 2005
 ***
 *** 
 *** Copyright (C) 2001, 2002, 2003, 2004, 2005
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
 *** This file provides:
 ***   Point class, coordinate functions, coordinate projections, (in)equality
 ***/

#ifndef EPIX2_POINT
#define EPIX2_POINT

#include <cmath>
#include "Constants.h"

namespace ePiX2 {

  /* * * Point.h * * */
  class Point {

    friend class Vector;

  private:
    // Cartesian coordinates
    double Px1, Px2, Px3;

  public:

    Point(double x1=0, double x2=0, double x3=0) 
      : Px1(x1), Px2(x2), Px3(x3) {}

    // Cartesian coordinate functions
    double x1() const { return Px1; }
    double x2() const { return Px2; }
    double x3() const { return Px3; }

    // defined in Vector.cc
    Point operator& (const Vector N); // projection to N
    Point operator% (const Vector N); // projection to N perp

    // (in)equality
    bool operator == (const Point& arg) const
      {
	double diff1=fabs(arg.Px1 - Px1);
	double diff2=fabs(arg.Px2 - Px2);
	double diff3=fabs(arg.Px3 - Px3);
	// norm(Vector) depends on this file
	return (sqrt(diff1*diff1+diff2*diff2+diff3*diff3)<EPIX2_EPSILON);
      }

    bool operator != (const Point& arg) const
      {
	return !((*this) == arg);
      }

  }; // end of class Point

  const Point Origin=Point(0,0,0);

} /* end of namespace */

#endif /* EPIX2_POINT */
