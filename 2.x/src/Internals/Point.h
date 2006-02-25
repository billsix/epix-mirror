/*** 
 ***  Point.h -- ePiX2::Point class
 ***
 *** This file is part of ePiX, a preprocessor for creating high-quality 
 *** line figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: December 20, 2005
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
 *** This file provides: The Point class
 ***   - coordinate functions x1(), etc
 ***   - vector projection, perpendicular projection, reflection
 ***   - (in)equality
 ***/

#ifndef EPIX2_POINT
#define EPIX2_POINT

#include <cmath>
#include "Constants.h"

namespace ePiX2 {

  /* * * Point.h * * */
  class Vector;
  class Point {

  public:

    Point(double x1=0, double x2=0, double x3=0) 
      : Px1(x1), Px2(x2), Px3(x3) { }

    // Cartesian coordinate functions
    double x1() const { return Px1; }
    double x2() const { return Px2; }
    double x3() const { return Px3; }

    Point& operator&= (const Vector N); // project to N
    Point& operator%= (const Vector N); // project to N perp
    Point& operator^= (const Vector N); // reflect in N perp

    // (in)equality
    bool operator == (const Point& arg) const;
    bool operator != (const Point& arg) const;

  private:
    // Cartesian coordinates
    double Px1, Px2, Px3;

  }; // end of class Point

  const Point Origin=Point(0,0,0);

  Point operator& (Point arg, const Vector N); // projection to N
  Point operator% (Point arg, const Vector N); // projection to N perp
  Point operator^ (Point arg, const Vector N); // reflection in N perp

} /* end of namespace */

#endif /* EPIX2_POINT */
