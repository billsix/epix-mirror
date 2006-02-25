/*** 
 *** Pair.cc -- ePiX2::Pair and Rect class functions
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
 ***/

#include "Constants.h"
//#include "Functions.h"
#include "Pair.h"

namespace ePiX2 {

  /*** Pair functions ***/
  bool Pair::operator== (const Pair arg) const
  { 
    return (fabs (h-arg.h) < EPIX2_EPSILON && fabs(v-arg.v) < EPIX2_EPSILON);
  }

  // complex arithmetic
  Pair& Pair::operator *=(const Pair arg)
  {
    double a=h, b=v, c=arg.h, d=arg.v; // this=a+bi, arg=c+di;
    h=a*c-b*d;
    v=a*d+b*c;

    return *this;
  }

  Pair& Pair::operator /=(const Pair arg)
  {
    double a=h, b=v, c=arg.h, d=-arg.v; // this=a+bi, arg=c-di;
    double denom=c*c+d*d;

    if (denom < EPIX2_EPSILON)
      throw div_by_zero();

    h=(a*c-b*d)/denom;
    v=(a*d+b*c)/denom;

    return *this;
  }

  Pair J(const Pair arg) { return Pair(-arg.x2(), arg.x1()); }

  double operator |(const Pair arg1, const Pair arg2)
    {
      return arg1.x1()*arg2.x1() + arg1.x2()*arg2.x2();
    }

  /*
  Pair& Pair::operator %=(const Pair arg)
  {
    double denom = (arg|arg);
    if (denom < EPIX2_EPSILON) // degenerate projection
      h = v = 0;
    else
      {
	double tmp=(h*arg.h + v*arg.v)/denom;
	h = tmp*arg.h;
	v = tmp*arg.v;
      }
    return *this;
  }

  Pair& Pair::operator ^=(const Pair arg)
  {
    double denom = (arg|arg);
    if (denom < EPIX2_EPSILON) // degenerate projection
      *this *= -1;
    else
      {
	double tmp=(h*arg.h + v*arg.v)/denom;
	*this *= -1;
	h += 2*tmp*arg.h;
	v += 2*tmp*arg.v;
      }
    return *this;
  }
  */

  static double min(double x, double y) { return x < y ? x : y; }
  static double max(double x, double y) { return x > y ? x : y; }


  /*** Rect functions ***/
  Rect::Rect(const Pair arg1, const Pair arg2)
  {
    low_left =Pair(min(arg1.x1(), arg2.x1()), min(arg1.x2(), arg2.x2()));
    top_right=Pair(max(arg1.x1(), arg2.x1()), max(arg1.x2(), arg2.x2()));
  }

  // corners and midpoints
  // N.B. min vs. mid
  double Rect::h_min() const { return low_left.x1(); }
  double Rect::v_min() const { return low_left.x2(); }

  double Rect::h_max() const { return top_right.x1(); }
  double Rect::v_max() const { return top_right.x2(); }

  // N.B. mid vs. min
  double Rect::h_mid() const { return 0.5*(top_right.x1()+low_left.x1()); }
  double Rect::v_mid() const { return 0.5*(top_right.x2()+low_left.x2()); }

  double Rect::h_size() const { return top_right.x1()-low_left.x1(); }
  double Rect::v_size() const { return top_right.x2()-low_left.x2(); }

  // alignment points
  Pair Rect::tr() const { return Pair(h_max(), v_max()); }
  Pair Rect::tl() const { return Pair(h_min(), v_max()); }
  Pair Rect::bl() const { return Pair(h_min(), v_min()); }
  Pair Rect::br() const { return Pair(h_max(), v_min()); }

  Pair Rect::t() const { return Pair(h_mid(), v_max()); }
  Pair Rect::b() const { return Pair(h_mid(), v_min()); }
  Pair Rect::l() const { return Pair(h_min(), v_mid()); }
  Pair Rect::r() const { return Pair(h_max(), v_mid()); }
  Pair Rect::c() const { return Pair(h_mid(), v_mid()); }

  // inside bounding box test
  bool Rect::shows(const Pair arg) const
  {
    const double x = arg.x1();
    const double y = arg.x2();
    return ((h_min() <= x) && (x <= h_max()) && 
	    (v_min() <= y) && (y <= v_max()));
  }

} /* end of namespace */
