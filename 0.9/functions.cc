/* 
 * functions.cc -- ePiX utility functions
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.9.0rc1
 * Last Change: August 27, 2003
 */

/* 
 * Copyright (C) 2001, 2002, 2003
 * Andrew D. Hwang <ahwang@mathcs.holycross.edu>
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

#include <math.h>

#include "constants.h"
#include "functions.h"

namespace ePiX3d {

  // declaration must be changed once the Picture class is written
  extern double angle_units(void);

  // identity maps
  template<class T> T Id(T arg) { return arg; }
  /*
    double Id (double x) { return x; }
    P Id (P x) { return x;}
    pair Id (pair x) { return x; }
  */

  // real-valued functions with singularities removed
  // sin(x)/x
  double
  Sinx (double x)
  {
    if (x != 0)
      return sin(x)/x;
    else
      return 1;
  }

  // reciprocal; 1/x
  double
  Recip (double x)
  {
    if (x != 0)
      return 1/x;
    else
      return 0;
  }

  // signum; x/|x|
  double
  Sgn (double x)
  {
    if (x > 0) return 1;
    else if (x < 0) return -1;
    else return 0;
  }

  // Charlie Brown: Period-2 extension of |x| on [-1,1] /\/\/\/\/\/
  double
  cb (double x)
  {
    return fabs(x) - 2*floor(0.5*fabs(x));
  }

  // Force double to [0,1]
  double clip_to_unit(double t)
  {
    if (t < 0)
      return 0;
    else if (t > 1)
      return 1;
    else
      return t;
  }

  // N.B.: gcd(0,i) = |i| (feature, not bug:)
  int 
  gcd (int i, int j)
  {
    int temp;

    if (i < 0)
      i = -i;
    if (j < 0)
      j = -j;

    if (i==0 || j==0) // (1,0) and (0,1) coprime, others not
      return i+j;

    else {
      if (j < i) // swap them
	{
	  temp = j;
	  j=i;
	  i=temp;
	}
      /* Euclidean algorithm */    
      while ((temp = j%i)) // i does not evenly divide j
	{
	  j=i;
	  i=temp;
	}
    
      return i;
    }
  }

  // inf and sup of f on [a,b]
  double
  inf (double f(double), double a, double b)
  {
    const int N = (int) ceil(fabs(b-a)); // N >= 1 unless a=b
    double y = f(a);
    double temp;
    const double dx = (b-a)/(N*EPIX_ITERATIONS);

    for (int i=1; i <= N*EPIX_ITERATIONS; ++i)
      if ((temp=f(a + i*dx)) < y)
	y = temp;

    return y;
  }

  double
  sup (double f(double), double a, double b)
  {
    const int N = (int) ceil(fabs(b-a)); // N >= 1 unless a=b
    double y = f(a);
    double temp;
    const double dx = (b-a)/(N*EPIX_ITERATIONS);

    for (int i=1; i <= N*EPIX_ITERATIONS; ++i)
      if ((temp=f(a + i*dx)) > y)
	y = temp;

    return y;
  }

  // trig functions with angle units
  double cos(double theta) { return std::cos(angle_units()*theta); }
  double sin(double theta) { return std::sin(angle_units()*theta); }
  double tan(double theta) { return std::tan(angle_units()*theta); }
  double sec(double theta) { return Recip(std::cos(angle_units()*theta)); }

  // and inverses
  double acos(double arg) { return std::acos(arg)/angle_units(); }
  double asin(double arg) { return std::asin(arg)/angle_units(); }
  double atan(double arg) { return std::atan(arg)/angle_units(); }

  // logarithm with log base
  //  double log(double arg) { return std::ln(arg)/std::ln(log_base()); }

} /* end of namespace */
