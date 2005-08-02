/***
 ***  Functions.h -- epix2 utility functions
 ***
 *** This file is part of ePiX, a preprocessor for creating high-quality 
 *** line figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: July 13, 2005
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
 ***   This file provides:
 ***
 ***/

#ifndef EPIX2_FUNCTIONS
#define EPIX2_FUNCTIONS

#include <cmath>

#include "Point.h"

namespace ePiX2 {

  /* * * Functions.h * * */

  // ePiX:: trig functions are sensitive to angle units
  double Cos(double theta);
  double Sin(double theta);
  double Tan(double theta); 

  // non-mathematical behavior (i.e. vanishing:) at poles
  double Sec(double theta);
  double Csc(double theta);
  double Cot(double theta);

  // and inverses
  double Acos(double arg);
  double Asin(double arg);
  double Atan(double arg);

  // identity function for angle units
  double angle(double);

  //  inline double id (double x) { return x; }

  template<class T> T identity(T);
  double proj1 (double, double);
  double proj2 (double, double);

  // standard coordinate systems
  Point cartesian(double x, double y, double z);
  Point cylindrical(double r, double theta, double z);
  Point spherical(double rho, double longitude, double latitude);

  // utility functions with discontinuities removed
  double sinx (double);
  double sgn (double);
  double recip (double);
  double snip_to(double var, const double arg1, const double arg2);

  // period-2 extension of absolute value on [-1,1]: \/\/\/
  double cb (double);

  int gcd (int, int);

  inline double min(double a, double b) { return a<b ? a : b; }
  inline double max(double a, double b) { return a<b ? b : a; }

  double inf (double f(double), double, double);
  double sup (double f(double), double, double);

  double newton (double f(double), double seed);

} // end of namespace

#endif /* EPIX2_FUNCTIONS */
