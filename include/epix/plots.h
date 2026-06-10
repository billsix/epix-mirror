/*
 * plots.h -- parametrized plotting algorithms
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Andrew D. Hwang   <ahwang -at- holycross -dot- edu>
 *
 * Version 1.0.23
 * Last Change: January 13, 2007
 */

/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
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
#ifndef EPIX_PLOTS
#define EPIX_PLOTS

#include "enums.h"

namespace ePiX {

class domain;
class domain_list;
class Deriv;
class Integral;
class P;

//// paths ////
// f:R -> R^3
void plot(P f(double), double t_min, double t_max, unsigned int n);

// f:R -> R x R x R
void plot(double f1(double), double f2(double), double f3(double), double t_min,
          double t_max, unsigned int n);

// f:R -> R
void plot(double f(double), double t_min, double t_max, unsigned int n);
void plot(const Deriv&, double t_min, double t_max, unsigned int n);
void plot(const Integral&, double t_min, double t_max, unsigned int n);

// f: R -> R x R
void plot(double f1(double), double f2(double), double t_min, double t_max,
          unsigned int n);

void polarplot(double f(double), double t_min, double t_max, unsigned int);

//// surfaces ////
// f:R^2 -> R x R x R
void plot(double f1(double u1, double u2), double f2(double u1, double u2),
          double f3(double u1, double u2), const P& min, const P& max,
          const mesh& coarse, const mesh& fine);

void plot(double f1(double u1, double u2), double f2(double u1, double u2),
          double f3(double u1, double u2), const domain& R);

// f:R^2 -> R
void plot(double f(double u1, double u2), const P& p1, const P& p2,
          const mesh& coarse, const mesh& fine);

// f:R^2 -> R^3
void plot(P f(double, double), const domain&);
void plot(P f(double, double), const domain_list&);

// f:R^3 -> R^3
void plot(P f(double, double, double), const domain&);
void plot(P f(double, double, double), const domain_list&);

// Derivatives and integrals
void plot_deriv(double f(double), double a, double b, unsigned int n);

void plot_int(double f(double), double a, double b, unsigned int n);

void plot_int(double f(double), double x0, double a, double b, unsigned int);

void tan_line(double f1(double), double f2(double), double t0);
void tan_line(double f(double), double t0);
void tan_line(P f(double), double t0);

void envelope(double f1(double), double f2(double), double, double,
              unsigned int);
void envelope(double f(double), double, double, unsigned int);
void envelope(P f(double), double, double, unsigned int);

void tan_field(double f1(double), double f2(double), double, double,
               unsigned int);
void tan_field(P f(double), double, double, unsigned int);

// Slope, dart, and vector fields
// For slope and dart fields, the optional scale argument affects the
// (constant) drawn length of field elements. For vector fields, elements
// are drawn at true length, and "scale" affects the arrowhead size.

// planar fields
void slope_field(P F(double, double), const P& p, const P& q, unsigned int,
                 unsigned int, double scale = 1.0);
void dart_field(P F(double, double), const P& p, const P& q, unsigned int,
                unsigned int, double scale = 1.0);
void vector_field(P F(double, double), const P& p, const P& q, unsigned int,
                  unsigned int, double scale = 1.0);

void slope_field(P F(double, double), const domain& R, double scale = 1.0);
void dart_field(P F(double, double), const domain& R, double scale = 1.0);
void vector_field(P F(double, double), const domain& R, double scale = 1.0);

// spatial fields
void slope_field(P F(double, double, double), const P&, const P&, unsigned int,
                 unsigned int, double scale = 1.0);
void dart_field(P F(double, double, double), const P&, const P&, unsigned int,
                unsigned int, double scale = 1.0);
void vector_field(P F(double, double, double), const P&, const P&, unsigned int,
                  unsigned int, double scale = 1.0);

// spatial fields over a (3-D) domain
void slope_field(P F(double, double, double), const domain&,
                 double scale = 1.0);
void dart_field(P F(double, double, double), const domain&, double scale = 1.0);
void vector_field(P F(double, double, double), const domain&,
                  double scale = 1.0);

// Solutions of ODE systems

// start at time 0
void ode_plot(P F(double, double), const P& start, double t_max, unsigned int);

void ode_plot(P F(double, double, double), const P& start, double t_max,
              unsigned int);

// arbitrary start time
void ode_plot(P F(double, double), const P& start, double t_min, double t_max,
              unsigned int);

void ode_plot(P F(double, double, double), const P& start, double t_min,
              double t_max, unsigned int);

// flow x0 under field for specified time
P flow(P F(double, double), const P& start, double t_max, unsigned int n = 0);
P flow(P F(double, double, double), const P& start, double t_max,
       unsigned int n = 0);

void riemann_sum(double f(double), double a, double b, unsigned int n,
                 epix_integral_type TYPE);

// Jay Belanger's shaded plot functions -- December 1, 2002
void shadeplot(double f1(double), double f2(double), double, double,
               unsigned int);
void shadeplot(double f(double), double t_min, double t_max, unsigned int);

void blackplot(double f1(double), double f2(double), double t_min, double t_max,
               unsigned int);

void blackplot(double f(double), double t_min, double t_max, unsigned int);

void whiteplot(double f1(double), double f2(double), double t_min, double t_max,
               unsigned int);

void whiteplot(double f(double), double t_min, double t_max, unsigned int);

// wiremesh surface plotting
void plot(P Phi(double, double), const P&, const P&, const mesh& N,
          const mesh& num_pts);

/*
void clipplot(P Phi(double, double),
          const P&, const P&, const mesh& N, const mesh& num_pts);

void cropplot(P Phi(double, double),
              const P&, const P&, const mesh& N, const mesh& num_pts);
*/

}  // namespace ePiX

#endif /* EPIX_PLOTS */
