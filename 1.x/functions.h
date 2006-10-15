/*
 * functions.h  -- ePiX auxiliary functions
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.16
 * Last Change: October 14, 2006
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

#ifndef EPIX_FUNCTIONS
#define EPIX_FUNCTIONS

#include "globals.h"

namespace ePiX {

  class P;

  // ePiX:: trig functions are sensitive to angle units
  double cos(double);
  double sin(double);
  double tan(double); 

  // non-mathematical behavior (i.e. vanishing:) at poles
  double sec(double);
  double csc(double);
  double cot(double);

  // and inverses
  double acos(double);
  double asin(double);
  double atan(double);

  // more sanely-named aliases
  double Cos(double t);
  double Sin(double t);
  double Tan(double t);

  double Sec(double t);
  double Csc(double t);
  double Cot(double t);

  double Acos(double t);
  double Asin(double t);
  double Atan(double t);

  // identity and zero functions, coordinate projections
  template<class T>T id(T arg) { return arg; }
  template<class T>double zero(T arg) { return 0; }
  template<class T>double zero(T arg1, T arg2) { return 0; }

  template<class T1, class T2>T1 proj1(T1 arg1, T2 arg2) { return arg1; }
  template<class T1, class T2>T2 proj2(T1 arg1, T2 arg2) { return arg2; }

  // P constructors
  P xyz(double x, double y, double z=0);
  P cyl(double r, double t, double z);
  P sph(double r, double t, double phi);

  P cylindrical(P arg);
  P spherical(P arg);

  P polar(double r, double t);
  P cis(double t);

  double clip_to_unit(double t);
  // utility functions with discontinuities removed
  double recip (double);
  double sinx (double);
  double sgn (double);

  // period-2 extension of absolute value on [-1,1]: \/\/\/
  double cb (double);

  int gcd (int, int);

  // Scattered calls compare (e.g.) int to float
  // template<class T>T min(T a, T b) { return a < b ? a : b; }
  // template<class T>T max(T a, T b) { return b < a ? a : b; }

  double min(const double, const double);
  double max(const double, const double);

  double snip_to(double var, const double arg1, const double arg2);

  double inf (double f(double), double, double);
  double sup (double f(double), double, double);


  /*
  // constant functions
  class Const {
  private:
    double value;

  public:
    Const(double val=0) { value = val; }
    double operator() (double t) { return value; }
  }; // end of class Const
  */

  // derivative and integral operators for functions of one variable
  template<class T> T deriv(T f(double), double t, double dt=EPIX_EPSILON)
  {
    return (1.0/dt)*(f(t+0.5*dt) - f(t-0.5*dt));
  }

  // derivative class
  class Deriv {
  private:
    double (*f)(double);
    double dt;

  public:
    Deriv(double func(double), const double eps=EPIX_EPSILON);

    P operator() (const P&) const; // for plotting

    // numerical values
    double eval(const double t) const;

    // one-sided derivatives
    double right(const double t) const;
    double left(const double t) const;
  }; // end of class Deriv


  // definite integral class
  class Integral {
  private:
    double (*f)(double);
    double x0; // lower limit

  public:
    Integral(double func(double), double a=0);

    P operator() (const P&) const;

    double eval(const double) const;
  }; // end of class I

  // compatibility aliases
  typedef Deriv D;
  typedef Integral I;

  double newton (double f(double), double g(double), double start);
  double newton (double f(double), double start);

} // end of namespace

#endif /* EPIX_FUNCTIONS */
