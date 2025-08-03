/*
 * functions.h  -- ePiX auxiliary functions; Deriv, Integral classes
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.22
 * Last Change: September 24, 2007
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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef EPIX_FUNCTIONS
#define EPIX_FUNCTIONS

namespace ePiX {

  class P;

  // sensitive to angle units
  double Cos(double t);
  double Sin(double t);
  double Tan(double t);

  double Sec(double t);
  double Csc(double t);
  double Cot(double t);

  double Acos(double t);
  double Asin(double t);
  double Atan(double t);

  double Atan2(double y, double x);

  // additional hyperbolic functions and inverses
  double sech(double);
  double csch(double);
  double coth(double);

  double asech(double);
  double acsch(double);
  double acoth(double);

  // zero functions
  double zero(double);
  double zero(double, double);
  double zero(double, double, double);
  double zero(const P&);

  // P constructors
  P xyz(double x, double y, double z=0);
  P cyl(double r, double t, double z);
  P sph(double r, double t, double phi);

  // for log data plotting
  P log_log(double x, double y, double z=0);
  P log_lin(double x, double y, double z=0);
  P lin_log(double x, double y, double z=0);

  P cylindrical(P); // not const P&
  P spherical(P);

  P polar(double r, double t);
  P cis(double t);

  // utility functions
  double recip (double);
  double sinx (double); // discontinuity removed
  double sgn (double);

  // period-2 extension of absolute value on [-1,1]: \/\/\/
  double cb (double);

  int gcd (int, int);

  double min(double, double);
  double max(double, double);

  double snip_to(double var, double arg1, double arg2);

  double inf (double f(double), double, double);
  double sup (double f(double), double, double);


  // derivative class
  class Deriv {
  private:
    double (*f)(double);
    double dt;

  public:
    Deriv(double func(double));
    Deriv(double func(double), double eps);

    P operator() (const P&) const; // for plotting

    // numerical values
    double eval(double t) const;

    // one-sided derivatives
    double right(double t) const;
    double left(double t) const;
  }; // end of class Deriv


  // definite integral class
  class Integral {
  private:
    double (*f)(double);
    double x0; // lower limit

  public:
    Integral(double func(double), double a=0);

    P operator() (const P&) const;

    double eval(double) const;
  }; // end of class Integral

  double newton (double f(double), double g(double), double start);
  double newton (double f(double), double start);

} // end of namespace

#endif /* EPIX_FUNCTIONS */
