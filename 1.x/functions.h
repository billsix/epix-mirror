/*
 * functions.h  -- ePiX auxiliary functions
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc16
 * Last Change: August 24, 2004
 *
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004
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

#include <cmath>

#include "globals.h"
#include "triples.h"

namespace ePiX {

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
  inline double Cos(double t) { return ePiX::cos(t); }
  inline double Sin(double t) { return ePiX::sin(t); }
  inline double Tan(double t) { return ePiX::tan(t); }

  inline double Sec(double t) { return ePiX::sec(t); }
  inline double Csc(double t) { return ePiX::csc(t); }
  inline double Cot(double t) { return ePiX::cot(t); }

  inline double Acos(double t)  { return ePiX::acos(t); }
  inline double Asin(double t)  { return ePiX::acos(t); }
  inline double Atan(double t)  { return ePiX::acos(t); }

  // identity and zero functions, coordinate projections
  template<class T>T id(T arg) { return arg; }
  template<class T>double zero(T arg) { return 0; }
  template<class T>double zero(T arg1, T arg2) { return 0; }

  template<class T1, class T2>T1 proj1(T1 arg1, T2 arg2) { return arg1; }
  template<class T1, class T2>T2 proj2(T1 arg1, T2 arg2) { return arg2; }

  // P constructors
  inline P xyz(double x, double y, double z=0) 
    {
      return P(x, y, z);
    }

  inline P cyl(double r, double t, double z) 
    {
      return P(r*ePiX::cos(t), r*ePiX::sin(t), z);
    }

  inline P sph(double r, double t, double phi) 
    {
      return P(r*(ePiX::cos(t))*(ePiX::cos(phi)), 
	       r*(ePiX::sin(t))*(ePiX::cos(phi)), 
	       r*(ePiX::sin(phi)));
    }

  inline P cylindrical(P arg)
    {
      return cyl(arg.x1(), arg.x2(), arg.x3());
    }

  inline P spherical(P arg)
    {
      return sph(arg.x1(), arg.x2(), arg.x3());
    }

  inline P polar(double r, double t) { return cyl(r, t, 0); }
  inline P cis(double t) { return cyl(1, t, 0); }


  double clip_to_unit(double t);
  // utility functions with discontinuities removed
  double recip (double);
  double sinx (double);
  double sgn (double);

  // period-2 extension of absolute value on [-1,1]: \/\/\/
  double cb (double);

  int gcd (int, int);

  double inf (double f(double), double, double);
  double sup (double f(double), double, double);


  // constant functions
  class Const {
  private:
    double value;

  public:
    Const(double val=0) { value = val; }
    double operator() (double t) { return value; }
  }; // end of class Const

  // derivative and integral operators for functions of one variable
  template<class T> T deriv(T f(double), double t, double dt=EPIX_EPSILON)
  {
    return (1.0/dt)*(f(t+0.5*dt) - f(t-0.5*dt));
  }

  class D {
  private:
    double (*f)(double);
    double dt;

  public:
    D(double func(double), const double eps=EPIX_EPSILON) 
      { f = func; dt = eps; }

    P operator() (const P arg) // for plotting
      { 
	double t=arg.x1();
	return P(t, deriv(f, t, dt), 0);
      }

    // numerical values
    double eval(const double t)
      {
	return deriv(f, t, dt);
      }

    // one-sided derivatives
    double right(const double t)
      {
	return  (2.0/dt)*(f(t+0.5*dt) - f(t));
      }

    double left(const double t)
      {
	return (2.0/dt)*(f(t) - f(t-0.5*dt));
      }

  }; // end of class D


  class I {
  private:
    double (*f)(double);
    double x0; // lower limit

  public:
    I(double func(double), double a=0)
      { f = func; x0 = a; }

    P operator() (const P arg);

    double eval(const double t);

  }; // end of class I

  double newton (double f(double), double g(double), double start);
  inline double newton (double f(double), double start)
    {
      return newton(f, zero, start);
    }

} /* end of namespace */

#endif /* EPIX_FUNCTIONS */
