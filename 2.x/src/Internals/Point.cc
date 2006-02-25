/*** 
 ***  Point.cc -- ePiX2::Point class
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
 ***/

#include "Vector.h"
#include "Errors.h"
#include "Point.h"

namespace ePiX2 {

  // project to N
  Point& Point::operator&= (const Vector N)
  {
    double denom=(N|N);
    if (denom < EPIX2_EPSILON)
      throw div_by_zero();

    Vector dir=(*this)-N.tail();
    double c = (dir|N)/denom;
    
    (*this) = (c*N).head();
    return *this;
  }

  Point& Point::operator%= (const Vector N) // project to N perp
  {
    double denom=(N|N);
    if (denom < EPIX2_EPSILON)
      throw div_by_zero();

    Vector dir=(*this)-N.tail();
    double c = (dir|N)/denom;
    Px1 -= c*N.dx1();
    Px2 -= c*N.dx2();
    Px3 -= c*N.dx3();

    return *this;
  }

  Point& Point::operator^= (const Vector N) // reflect in N perp
  {
    double denom=(N|N);
    if (denom < EPIX2_EPSILON)
      throw div_by_zero();

    Vector dir=(*this)-N.tail();
    double c = 2*(dir|N)/denom;
    Px1 -= c*N.dx1();
    Px2 -= c*N.dx2();
    Px3 -= c*N.dx3();

    return *this;
  }

  Point operator& (Point arg, const Vector N) { return arg &= N; }
  Point operator% (Point arg, const Vector N) { return arg %= N; }
  Point operator^ (Point arg, const Vector N) { return arg ^= N; }

  // (in)equality
  bool Point::operator == (const Point& arg) const
  {
    double diff1=fabs(arg.Px1 - Px1);
    double diff2=fabs(arg.Px2 - Px2);
    double diff3=fabs(arg.Px3 - Px3);
    // norm(Vector) depends on this file
    return (sqrt(diff1*diff1+diff2*diff2+diff3*diff3)<EPIX2_EPSILON);
  }

  bool Point::operator != (const Point& arg) const
  {
    return !((*this) == arg);
  }

} /* end of namespace */
