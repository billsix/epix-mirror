/*
 * map.h -- classes of map for function plotting
 *
 * Andrew D. Hwang  <ahwang -at- holycross -dot- edu>
 *
 * Version 1.2.5
 * Last Change: June 18, 2008
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
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

/*
 * This file provides classes that represent mappings R -> R^3 or R x R -> R^3,
 * both P-valued and assembled from double-valued components. Each class
 * provides an evaluation operator that takes 1, 2, or 3 doubles and returns
 * a P. Stubs for general coordinate systems are included.
 *
 *  - path_map:            R -> R^3
 *  - surface_map:     R x R -> R^3
 *  - space_map:   R x R x R -> R^3
 *
 *  - column_1var: assemble     R -> R^3 from components
 *  - column_2var: assemble R x R -> R^3 from components
 *
 * These classes are intended primarily for internal use, allowing the
 * plot algorithms to be templated. In new code, maps should be defined
 * as P-valued, rather than assembled from components; the "column" classes
 * are provided solely for backward compatibility.
 */

#ifndef EPIX_MAP
#define EPIX_MAP

#include "triples.h"
#include "functions.h"

namespace ePiX {

  // identity and coordinate projection functions
  template<typename T>T id(T arg) { return arg; }

  template<typename T1, typename T2>T1 proj1(T1 arg1, T2 arg2) { return arg1; }
  template<typename T1, typename T2>T2 proj2(T1 arg1, T2 arg2) { return arg2; }

  // R -> R^3
  class path_map {
  private:
    P (*y)(double);
    P (*Y)(P);

  public:
    path_map(P f(double), P F(P) = id) : y(f), Y(F) { }

    P operator() (const P& arg) const
      {
	return Y(y(arg.x1()));
      }

  }; // end of class path_map

  // R x R -> R^3
  class surface_map {
  private:
    P (*y)(double, double);
    P (*Y)(P);

  public:
    surface_map(P f(double, double), P F(P) = id) : y(f), Y(F) { }
    P operator() (const P& arg) const
      {
	return Y(y(arg.x1(), arg.x2()));
      }

  }; // end of class surface_map

  // R x R x R -> R^3
  class space_map {
  private:
    P (*y)(double, double, double);
    P (*Y)(P);

  public:
    space_map(P f(double, double, double), P F(P) = id) : y(f), Y(F) { }
    P operator() (const P& arg) const
      {
	return Y(y(arg.x1(), arg.x2(), arg.x3()));
      }

  }; // end of class space_map


  // assemble R -> R^3 from components
  class column_1var {
  private:
    double (*y1)(double);
    double (*y2)(double);
    double (*y3)(double);
    P (*Y)(P);

  public:
    column_1var(double f1(double), double f2(double), double f3(double) = zero,
		P F(P) = id) : y1(f1), y2(f2), y3(f3), Y(F) { }

    // parameterized path from f:R -> R
    column_1var(double f(double), P F(P) = id)
      : y1(id), y2(f), y3(zero), Y(F) { }
    P operator() (const P& arg) const
      { 
	double t = arg.x1();
	return Y(P(y1(t), y2(t), y3(t))); 
      }
  }; // end of class column_1var

  // assemble R x R -> R^3 from components
  class column_2var {
  private:
    double (*y1)(double, double);
    double (*y2)(double, double);
    double (*y3)(double, double);
    P (*Y)(P);

  public:
    column_2var(double f1(double, double), 
		double f2(double, double), 
		double f3(double, double),
		P F(P) = id) : y1(f1), y2(f2), y3(f3), Y(F) { }

    // omitted third component defaults to zero
    column_2var(double f1(double, double), 
		double f2(double, double), 
		P F(P) = id) : y1(f1), y2(f2), y3(zero), Y(F) { }

    // parameterized surface from f:R x R -> R
    column_2var(double f(double, double), P F(P) = id)
      : y1(proj1), y2(proj2), y3(f), Y(F) { }
    P operator() (const P& arg) const
      {
	double u(arg.x1());
	double v(arg.x2());
	return Y(P(y1(u, v), y2(u, v), y3(u, v))); 
      }
  }; // end of class column_2var
} // end of namespace

#endif /* EPIX_MAP */
