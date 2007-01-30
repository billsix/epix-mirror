/* 
 * functions.cc -- non-standard mathematical functions
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX
 *
 * Version 1.0.23
 * Last Change: January 29, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
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
#include "errors.h"

#include "triples.h"

#include "functions.h"

namespace ePiX {

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

  double Cos(double t) { return ePiX::cos(t); }
  double Sin(double t) { return ePiX::sin(t); }
  double Tan(double t) { return ePiX::tan(t); }

  double Sec(double t) { return ePiX::sec(t); }
  double Csc(double t) { return ePiX::csc(t); }
  double Cot(double t) { return ePiX::cot(t); }

  double Acos(double t)  { return ePiX::acos(t); }
  double Asin(double t)  { return ePiX::asin(t); }
  double Atan(double t)  { return ePiX::atan(t); }

  // Additional hyperbolic tric functions and inverses
  double sech(double x) { return 1.0/cosh(x); }
  double csch(double x) { return 1.0/sinh(x); }
  double coth(double x) { return 1.0/tanh(x); }

  double asech(double x)
  {
    return log((1+sqrt((1-x)*(1+x)))/x);
  }

  double acsch(double x)
  {
    return log((1+sqrt(1+x*x))/x);
  }

  double acoth(double x)
  {
    return 0.5*log((x+1)/(x-1));
  }


  P xyz(double x, double y, double z) 
  {
    return P(x, y, z);
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

  P polar(double r, double t)
  {
    return cyl(r, t, 0);
  }
  P cis(double t)
  {
    return cyl(1, t, 0);
  }

  P log_log(double x, double y, double z)
  {
    return P(log10(x), log10(y), z);
  }

  P log_lin(double x, double y, double z)
  {
    return P(log10(x), y, z);
  }

  P lin_log(double x, double y, double z)
  {
    return P(x, log10(y), z);
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

  double min(const double a, const double b)
  {
    return a < b ? a : b;
  }
  double max(const double a, const double b)
  {
    return b < a ? a : b;
  }

  double snip_to(double var, const double arg1, const double arg2)  
  {
    if (var < min(arg1, arg2))
      var = min(arg1,arg2);
    else if (var > max(arg1, arg2))
      var = max(arg1,arg2);
    return var;
  }

  // inf and sup of f on [a,b]
  double inf (double f(double), double a, double b)
  {
    const int N((int) ceil(fabs(b-a))); // N >= 1 unless a=b
    double y(f(a));
    const double dx((b-a)/(N*EPIX_ITERATIONS));

    for (int i=1; i <= N*EPIX_ITERATIONS; ++i)
      y = min(y, f(a + i*dx));

    return y;
  }

  double sup (double f(double), double a, double b)
  {
    const int N((int) ceil(fabs(b-a))); // N >= 1 unless a=b
    double y(f(a));
    const double dx((b-a)/(N*EPIX_ITERATIONS));

    for (int i=1; i <= N*EPIX_ITERATIONS; ++i)
      y = max(y, f(a + i*dx));

    return y;
  }

  // Integral class helper
  double integrand(double f(double), double t, double dt)
  {
    return (1.0/6)*(f(t) + 4*f(t+0.5*dt)+f(t + dt))*dt;
  } // Simpson's rule

  Integral::Integral(double func(double), double a)
    : f(func), x0(a) { }

  double Integral::eval(const double t) const
  {
    double sum(0); 
    const int N(16*(int)ceil(fabs(t - x0))); // hardwired constant 16

    if (N > 0)
      {
	const double dx((t - x0)/N);

	for (int i=0; i < N; ++i)
	  sum += integrand(f, x0+i*dx, dx);
      }

    return sum;
  }

  P Integral::operator() (const P& arg) const
  {
    double t(arg.x1());
    return P(t, eval(t), 0);
  }

  double newton (double f(double), double g(double), double start)
  {
    double guess(start);
    int count(0); // number of iterations

    // Hardwired constant 5
    const int ITERS(5);
    while ( (fabs(f(guess)-g(guess)) > EPIX_EPSILON) && (count < ITERS) )
      {
	if (fabs(deriv(f, guess)-deriv(g, guess)) < EPIX_EPSILON)
	  {
	    epix_warning("Returning critical point in Newton's method");
	    return guess;
	  }

	guess -= (f(guess)-g(guess))/(deriv(f, guess)-deriv(g, guess));
	++count;
      }

    if (count == ITERS)
      epix_warning("Maximum number of iterations in Newton's method");

    return guess;
  }

  double newton (double f(double), double start)
  {
    return newton(f, zero, start);
  }

  // Member functions
  Deriv::Deriv(double func(double), const double eps)
    : f(func), dt(eps) { }

  P Deriv::operator() (const P& arg) const
  {
    double t(arg.x1());
    return P(t, deriv(f, t, dt), 0);
  }

  double Deriv::eval(const double t) const
  {
    return deriv(f, t, dt);
  }

  // one-sided derivatives
  double Deriv::right(const double t) const
  {
    return  (2.0/dt)*(f(t+0.5*dt) - f(t));
  }

  double Deriv::left(const double t) const
  {
    return (2.0/dt)*(f(t) - f(t-0.5*dt));
  }
} // end of namespace
