/*
 * plots.h -- parametrized plotting algorithms
 *
 * Andrew D. Hwang            <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
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

/*
 * This file provides templated plotting algorithms and common instantiations.
 */

#ifndef EPIX_PLOTS
#define EPIX_PLOTS

#include "triples.h"
#include "functions.h"
#include "path.h"
#include "map.h"
#include "domain.h"

namespace ePiX {

  // can't pass domain as const reference...
  template<class T> void plot(T map, domain R)
  {
    P curr;
    int i_max = (R.dx1() > 0) ? R.coarse.n1() : 0; // max summation index
    int j_max = (R.dx2() > 0) ? R.coarse.n2() : 0;
    int k_max = (R.dx3() > 0) ? R.coarse.n3() : 0;


    if (R.dx1() > 0) // there's something to draw
      {
	int count1 = R.fine.n1(); // number of intervals in subdivision
	std::vector<vertex> data1(1+count1);

	double st1 = R.dx1();
	double st2 = R.step2();
	double st3 = R.step3();

	for (int j=0; j <= j_max; ++j)
	  for (int k=0; k <= k_max; ++k)
	    {
	      curr = R.corner1 + (j*st2)*E_2 + (k*st3)*E_3;

	      for (int i=0; i <= count1; ++i, curr += st1*E_1)
		data1.at(i) = vertex(map(curr));

	      // close path if initial and terminal points coincide
	      path path1 = path(data1, false);
	      path1.draw();
	    }
      }

    if (R.dx2() > 0)
      {
	int count2 = R.fine.n2();
	std::vector<vertex> data2(1+count2);

	double st1 = R.step1();
	double st2 = R.dx2();
	double st3 = R.step3();

	for (int i=0; i <= i_max; ++i)
	  for (int k=0; k <= k_max; ++k)
	    {
	      curr = R.corner1 + (i*st1)*E_1 + (k*st3)*E_3;
	      
	      for (int j=0; j <= count2; ++j, curr += st2*E_2)
		data2.at(j) = vertex(map(curr));

	      // close path if initial and terminal points coincide
	      path path2 = path(data2, false);
	      path2.draw();
	    }
      }

    if (R.dx3() > 0)
      {
	int count3 = R.fine.n3();
	std::vector<vertex> data3(1+count3);

	double st1 = R.step1();
	double st2 = R.step2();
	double st3 = R.dx3();

	for (int i=0; i <= i_max; ++i)
	  for (int j=0; j <= j_max; ++j)
	    {
	      curr = R.corner1 + (i*st1)*E_1 + (j*st2)*E_2;

	      for (int k=0; k <= count3; ++k, curr += st3*E_3)
		data3.at(k) = vertex(map(curr));

	      // close path if initial and terminal points coincide
	      path path3 = path(data3, false);
	      path3.draw();
	    }
      }

  }; // end of (path) plot

  // plot over a list of domains
  template<class T> void plot(T map, std::vector<domain> R_list)
  {
    for (unsigned int i=0; i < R_list.size(); ++i)
      plot(map, R_list.at(i));
  }

  // paths
  template<class T>void plot (T f, double t_min, double t_max, int num_pts)
    {
      plot(f, domain(t_min, t_max, num_pts));
    }

  // f:R -> R^3
  inline void plot (P f(double), double t_min, double t_max, int num_pts)
    {
      plot(path_map(f), domain(t_min, t_max, num_pts));
    }

  // f:R -> R x R x R
  inline void plot (double f1(double), double f2(double), double f3(double),
		    double t_min, double t_max, int num_pts)
    {
      plot(column_1var(f1,f2,f3), domain(t_min, t_max, num_pts));
    }

  // f:R -> R
  inline void plot (double f(double), double t_min, double t_max, int num_pts)
    { 
      plot(id, f, zero, t_min, t_max, num_pts); 
    }

  // f: R -> R x R
  inline void plot (double f1(double), double f2(double), 
		    double t_min, double t_max, int num_pts)
    { 
      plot(f1, f2, zero, t_min, t_max, num_pts); 
    }

  inline void polarplot (double f(double), double t_min, double t_max, 
			 int num_pts)
    {
      plot(column_1var(f, id, zero, cylindrical), 
	   domain(t_min, t_max, num_pts));
    }
  
  // surfaces
  // f:R^2 -> R x R x R
  inline void plot(double f1(double u1, double u2),
		   double f2(double u1, double u2),
		   double f3(double u1, double u2), 
		   P min, P max, mesh coarse, mesh fine)
    {
      column_2var map(f1,f2,f3);
      domain dom=domain(min, max, coarse, fine);
      plot(map, dom);
    }
  inline void plot(double f1(double u1, double u2),
		   double f2(double u1, double u2),
		   double f3(double u1, double u2), 
		   domain R)
    {
      column_2var map(f1,f2,f3);
      plot(map, R);
    }

  // f:R^2 -> R
  inline void plot(double f(double u1, double u2),
		   P p1, P p2, mesh coarse, mesh fine)
    {
      plot(proj1, proj2, f, p1, p2, coarse, fine);
    }
  inline void plot(double f(double u1, double u2), domain R)
    {
      plot(column_2var(proj1, proj2, f), R);
    }

  // f:R^2 -> R^3
  inline void plot(P f(double u1, double u2),
		   P p1, P p2, mesh coarse, mesh fine)
    {
      surface_map map(f);
      domain R=domain(p1, p2, coarse, fine);
      plot(map, R);
    }
  inline void plot(P f(double u1, double u2), domain R)
    {
      surface_map map(f);
      plot(map, R);
    }


  // Derivatives and integrals
  inline void plot_deriv(double f(double t), double a, double b, int num_pts)
  { plot(D(f), a, b, num_pts); }

  inline void plot_int(double f(double), double a, double b, int num_pts)
  { plot(I(f, a), a, b, num_pts); }
  inline void plot_int(double f(double), double x0, double a, double b, 
		       int num_pts)
  { plot(I(f, x0), a, b, num_pts); }

  void tan_line(double f1(double t), double f2(double t), double t0);
  void tan_line(double f(double t), double t0);
  void tan_line(P f(double t), double t0);

  void envelope(double f1(double), double f2(double), double, double, int);
  void envelope(double f(double), double, double, int);
  void envelope(P f(double), double, double, int);

  void tan_field     (double f1(double), double f2(double), 
		      double, double, double);
  void tan_field     (P f(double), double, double, double);


  // Slope, dart, and vector fields

  // planar fields
  void draw_field (P F(double, double),
		   P, P, int, int, epix_field_type);

  inline void slope_field(P F(double, double), 
			  P p, P q, int n1, int n2)
    { draw_field(F, p, q, n1, n2, SLOPE); }

  inline void dart_field(P F(double, double), 
			 P p, P q, int n1, int n2)
    { draw_field(F, p, q, n1, n2, DART); }

  inline void vector_field(P F(double, double), 
			   P p, P q, int n1, int n2)
    { draw_field(F, p, q, n1, n2, VECTOR); }

  // spatial fields
  void draw_field (P F(double, double, double),
		   P, P, int, int, epix_field_type);

  inline void slope_field(P F(double, double, double), 
			  P p, P q, int n1, int n2)
    { draw_field(F, p, q, n1, n2, SLOPE); }

  inline void dart_field(P F(double, double, double), 
			 P p, P q, int n1, int n2)
    { draw_field(F, p, q, n1, n2, DART); }

  inline void vector_field(P F(double, double, double), 
			   P p, P q, int n1, int n2)
    { draw_field(F, p, q, n1, n2, VECTOR); }


  // Solutions of ODE systems
  template<class VF> void euler_plot(VF field, P start, 
				     double t_min, double t_max, int num_pts)
  {
    std::vector<vertex> data(num_pts+1);

    P curr = start;
    const double dt = t_max/(num_pts*EPIX_ITERATIONS);
    const double dseek = t_min/(num_pts*EPIX_ITERATIONS);

    if (fabs(t_min/num_pts) > EPIX_EPSILON) // seek beginning of path
      for (int i=0; i <= num_pts*EPIX_ITERATIONS; ++i)
	curr += dseek*field(curr);

    //                             Euler's method ---v
    for (int i=0; i <= num_pts*EPIX_ITERATIONS; ++i, curr += dt*field(curr))
      if (i%EPIX_ITERATIONS == 0)
	data.at(i/EPIX_ITERATIONS) = vertex(curr); // N.B. integer division

    path temp(data, false);

    temp.draw();
    end_stanza();
  }


  // start at time 0
  inline void ode_plot (P F(double, double), P start, 
			double t_max, int num_pts)
    { 
      surface_map vf(F); // convert to map:P -> P
      euler_plot(vf, start, 0, t_max, num_pts);
    }

  inline void ode_plot (P F(double, double, double), P start, 
			double t_max, int num_pts)
    { 
      space_map vf(F);
      euler_plot(vf, start, 0, t_max, num_pts);
    }

  // arbitrary start time
  inline void ode_plot (P F(double, double), P start, 
			double t_min, double t_max, int num_pts)
    { 
      surface_map vf(F); // convert to map:P -> P
      euler_plot(vf, start, t_min, t_max, num_pts);
    }

  inline void ode_plot (P F(double, double, double), P start, 
			double t_min, double t_max, int num_pts)
    { 
      space_map vf(F);
      euler_plot(vf, start, t_min, t_max, num_pts);
    }


  // flow x0 under field for specified time
  template<class VF> P flow(VF field, P x0, double t_max, int num_pts=0)
  {
    P curr = x0;

    if (num_pts == 0) // use "sensible" default; hardwired constant 4
      num_pts = 4*(1 + (int)ceil(fabs(t_max)));

    const double dt = t_max/(num_pts*EPIX_ITERATIONS);

    for (int i=0; i <= num_pts*EPIX_ITERATIONS; ++i)
      curr += dt*field(curr);

    return curr;
  }

  inline P flow (P F(double, double), P start, double t_max,
		   int num_pts = 0)
  {
    surface_map vf(F);
    return flow(vf, start, t_max, num_pts);
  }

  inline P flow (P F(double, double, double), P start, double t_max,
		   int num_pts = 0)
  {
    space_map vf(F);
    return flow(vf, start, t_max, num_pts);
  }


  // Jay Belanger's shaded plot routines -- December 1, 2002

  void shadeplot(double f1(double), double f2(double), double, double, int);
  inline void shadeplot(double f(double), double t_min, double t_max, 
			int num_pts)
    {
      shadeplot(f, zero, t_min, t_max, num_pts);
    }

  inline void blackplot(double f1(double), double f2(double), 
			double t_min, double t_max, int num_pts)
    {
      double temp_gray = epix::get_gray();
      gray(1);
      shadeplot(f1, f2, t_min, t_max, num_pts);
      gray(temp_gray);
    }

  inline void blackplot(double f(double), double t_min, double t_max, 
			int num_pts)
    {
      blackplot(f, zero, t_min, t_max, num_pts);
    }

  inline void whiteplot(double f1(double), double f2(double), 
			double t_min, double t_max, int num_pts)
    {
      double temp_gray = epix::get_gray();
      gray(0);
      shadeplot(f1, f2, t_min, t_max, num_pts);
      gray(temp_gray);
    }

  inline void whiteplot(double f(double), double t_min, double t_max, 
			int num_pts)
    {
      whiteplot(f, zero, t_min, t_max, num_pts);
    }


  // Adaptive plotting
  void adplot (double f1(double), double f2(double), double, double, int);

  inline void adplot(double f(double), double t_min, double t_max, int num_pts)
  { adplot(id, f, t_min, t_max, num_pts); }

  // wiremesh surface plotting
  void plot(P Phi(double, double), P, P, mesh N, mesh num_pts);

  inline void clipplot(P Phi(double, double), P p1, P p2, 
		       mesh N, mesh num_pts)
    {
      bool temp_clip = epix::clipping;
      clip();
      plot(Phi, p1, p2, N, num_pts);
      clip(temp_clip);
    }

  inline void cropplot(P Phi(double, double), P p1, P p2, 
		       mesh N, mesh num_pts)
    {
      bool temp_crop = epix::cropping;
      crop();
      plot(Phi, p1, p2, N, num_pts);
      crop(temp_crop);
    }

} /* end of namespace */

#endif /* EPIX_PLOTS */
