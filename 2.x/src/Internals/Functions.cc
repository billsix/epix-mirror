/* 
 * Functions.cc -- non-standard mathematical functions
 *
 * This file is part of ePiX, a program for creating high-quality 
 * figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: January 14, 2006
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
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

#include <string>
#include <cmath>

#include "Functions.h"
#include "Constants.h"
#include "Errors.h"

namespace ePiX2 {

  static double epix_angle_units=M_PI/180;

  //  extern double epix_angle_units;

  // extended reciprocal, with 1/0=0
  double recip (double x) 
  { 
    return (fabs(x) > EPIX2_INFTY || x== 0) ? 0 : 1/x;
  }

  double snip_to(double var, const double arg1, const double arg2)
    {
      if      (var < min(arg1,arg2)) var = min(arg1,arg2);
      else if (var > max(arg1,arg2)) var = max(arg1,arg2);
      return var;
    }

  // trig functions with angle units
  double Cos(double theta) { return std::cos(epix_angle_units*theta); }
  double Sin(double theta) { return std::sin(epix_angle_units*theta); }
  double Tan(double theta) { return std::tan(epix_angle_units*theta); }

  // non-mathematical behavior (i.e. vanishing:) at poles
  double Sec(double theta) { return recip(std::cos(epix_angle_units*theta)); }
  double Csc(double theta) { return recip(std::sin(epix_angle_units*theta)); }
  double Cot(double theta) { return recip(std::tan(epix_angle_units*theta)); }

  // and inverses
  double Acos(double arg) { return std::acos(arg)/epix_angle_units; }
  double Asin(double arg) { return std::asin(arg)/epix_angle_units; }
  double Atan(double arg) { return std::atan(arg)/epix_angle_units; }

  // identity function for angle units
  double angle(double t) { return epix_angle_units*t; }

  // standard coordinate systems
  Point cartesian(double x, double y, double z)
  {
    return Point(x, y, z);
  }

  Point cylindrical(double x, double y, double z)
  {
    return Point(x*Cos(y), x*Sin(y), z);
  }

  Point spherical(double x, double y, double z)
  {
    return Point(x*Cos(y)*Cos(z), x*Sin(y)*Cos(z), x*Sin(z));
  }

  /* identity map and coordinate projections */

  template<class T> T identity(T arg) { return arg; }

  double proj1 (double x, double y) { return x; }
  double proj2 (double x, double y) { return y; }

  // sin(x)/x
  double sinx (double x) { return (x != 0) ? Sin(x)/angle(x) : 1; }

  // signum, x/|x|, defined to be 0 at 0
  double sgn (double x)
  {
    if (x > 0)
      return 1;
    else if (x < 0)
      return -1;
    else
      return 0;
  }

  // Charlie Brown: Period-2 extension of |x| on [-1,1] /\/\/\/\/\/
  double cb (double x)
  {
    return fabs(x - 2*floor((x+1)/2));
  }

  // N.B.: gcd(0,i) = |i|
  int gcd (int i, int j)
  {
    int temp;

    i=abs(i);
    j=abs(j);

    if (i==0 || j==0) // (1,0) and (0,1) coprime, others not
      return i+j;

    else {
      if (j < i) // swap them
	{
	  temp = j;
	  j=i;
	  i=temp;
	}
      // Euclidean algorithm
      while ((temp = j%i)) // i does not evenly divide j
	{
	  j=i;
	  i=temp;
	}
    
      return i;
    }
  }

  // inf and sup of f on [a,b]
  double inf (double f(double), double a, double b)
  {
    const int N = (int) ceil(fabs(b-a)); // N >= 1 unless a=b
    double y = f(a);
    double dx = (b-a)/(N*EPIX2_ITERATIONS);

    for (int i=1; i <= N*EPIX2_ITERATIONS; ++i)
      if (f(a + i*dx) < y)
	y = f(a + i*dx);

    return y;
  }

  double sup (double f(double), double a, double b)
  {
    const int N = (int) ceil(fabs(b-a)); // N >= 1 unless a=b
    double y = f(a);
    double dx = (b-a)/(N*EPIX2_ITERATIONS);

    for (int i=1; i <= N*EPIX2_ITERATIONS; ++i)
      if (f(a + i*dx) > y)
	y = f(a + i*dx);

    return y;
  }

  static double diff(double f(double), double t)
  {
    /* Hardwired constant 0.01 */
    double dt = 0.01*EPIX2_EPSILON; // step size
    return (f(t+0.5*dt)-f(t-0.5*dt))/dt;
  }

  double newton (double f(double), double start)
  {
    double guess = start;
    int count=0; // number of iterations

    /* Hardwired constant 20 */
    while ( (fabs(f(guess)) > EPIX2_EPSILON) && (count < 20) )
      {
	if (fabs(diff(f, guess)) < EPIX2_EPSILON)
	  {
	    epix_warning("Returning critical point in Newton's method");
	    return guess;
	  }

	guess -= f(guess)/diff(f, guess);
	++count;
      }

    if (count == 20)
      epix_warning("20 iterations in Newton's method");

    return(guess);
  }

} /* end of namespace */
