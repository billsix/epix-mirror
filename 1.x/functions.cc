/* 
 * functions.cc -- non-standard mathematical functions
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.7
 * Last Change: March 06, 2006
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

#include "globals.h"
#include "output.h"
#include "functions.h"

namespace ePiX {

  void epix_warning(std::string);

  // trig functions with angle units
  double cos(double t) { return std::cos(angle(t)); }
  double sin(double t) { return std::sin(angle(t)); }
  double tan(double t) { return std::tan(angle(t)); }

  // non-mathematical behavior (i.e. vanishing:) at poles
  double sec(double t) { return recip(std::cos(angle(t))); }
  double csc(double t) { return recip(std::sin(angle(t))); }
  double cot(double t) { return recip(std::tan(angle(t))); }

  // and inverses
  double acos(double arg) { return std::acos(arg)/angle(1); }
  double asin(double arg) { return std::asin(arg)/angle(1); }
  double atan(double arg) { return std::atan(arg)/angle(1); }

  void label_angle(double t) // declared in globals.h
 { 
   double temp = angle(t)/(2*M_PI); // t in rotations
   temp -= 0.5; // shift half a turn
   temp -= ceil(temp); // map to (-1,0]
   temp += 0.5; // shift back
   epix::labelangle = 360*temp; // (-180, 180]
 }

  P cyl(double r, double t, double z) 
  {
    return P(r*ePiX::cos(t), r*ePiX::sin(t), z);
  }

  P sph(double r, double t, double phi) 
  {
    return P(r*(Cos(t))*(Cos(phi)), r*(Sin(t))*(Cos(phi)), r*(Sin(phi)));
  }

  P cylindrical(P arg)
  {
    return cyl(arg.x1(), arg.x2(), arg.x3());
  }

  P spherical(P arg)
  {
    return sph(arg.x1(), arg.x2(), arg.x3());
  }


  // Force double to [0,1]
  double clip_to_unit(double t)
  {
    if (t < 0)       return 0;
    else if (t > 1)  return 1;
    else             return t;
  }

  // reciprocal, truncated to 0 near 0
  double recip (double x)
  {
    if (fabs(x) < 1.0/EPIX_INFTY)
      return 0;
    else
      return 1/x;
  }

  // sin(x)/x
  double sinx (double x)
  {
    if (1. + x*x == 1.) // from Don Hatch
      return 1.;
    else
      return Sin(x)/angle(x);
  }

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
    x = fabs(x);
    x -= 2*floor(0.5*x);
    return min(x, 2-x);
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
    double dx = (b-a)/(N*EPIX_ITERATIONS);

    for (int i=1; i <= N*EPIX_ITERATIONS; ++i)
      y = min(y, f(a + i*dx));

    return y;
  }

  double sup (double f(double), double a, double b)
  {
    const int N = (int) ceil(fabs(b-a)); // N >= 1 unless a=b
    double y = f(a);
    double dx = (b-a)/(N*EPIX_ITERATIONS);

    for (int i=1; i <= N*EPIX_ITERATIONS; ++i)
      y = max(y, f(a + i*dx));

    return y;
  }

  // I(ntegral) class helper
  static inline double integrand(double f(double), double t, double dt)
  { return (1.0/6)*(f(t) + 4*f(t+0.5*dt)+f(t + dt))*dt; } // Simpson's rule

  double I::eval(const double t)
  {
    double sum = 0; 
    int N = 16*(int)ceil(fabs(t - x0)); // hardwired constant 16

    if (N > 0)
      {
	const double dx = (t - x0)/N;

	for (int i=0; i < N; ++i)
	  sum += integrand(f, x0+i*dx, dx);
      }

    return sum;
  }

  P I::operator() (const P arg)
  {
    double t = arg.x1();
    return P(t, eval(t), 0);
  }

  double newton (double f(double), double g(double), double start)
  {
    double guess = start;
    int count=0; // number of iterations

    /* Hardwired constant 20 */
    while ( (fabs(f(guess)-g(guess)) > EPIX_EPSILON) && (count < 20) )
      {
	if (fabs(deriv(f, guess)-deriv(g, guess)) < EPIX_EPSILON)
	  {
	    epix_warning("Returning critical point in Newton's method");
	    return guess;
	  }

	guess -= (f(guess)-g(guess))/(deriv(f, guess)-deriv(g, guess));
	++count;
      }

    if (count == 20)
      epix_warning("20 iterations in Newton's method");

    return guess;
  }

  double newton (double f(double), double start)
  {
    return newton(f, zero, start);
  }

  // Member functions
  P D::operator() (const P& arg) const
  {
    double t=arg.x1();
    return P(t, deriv(f, t, dt), 0);
  }

  double D::eval(const double t)
  {
    return deriv(f, t, dt);
  }

  // one-sided derivatives
  double D::right(const double t)
  {
    return  (2.0/dt)*(f(t+0.5*dt) - f(t));
  }

  double D::left(const double t)
  {
    return (2.0/dt)*(f(t) - f(t-0.5*dt));
  }
} /* end of namespace */
