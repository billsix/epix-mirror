/*
 * plot_algorithms.h -- plot function templates, for build use only
 *
 * Andrew D. Hwang   <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
 *
 * Version 1.0.16
 * Last Change: October 14, 2006
 *
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

/*
 * This file provides plotting templates, with T a class
 *
 *  void plot_map_dom(const T&, const domain&)
 *  void plot_map_domlist(const T&, const std::vector<domain>&)
 *  void plot_function(const T&, double, double, int)
 *  void euler_plot(const T&, const P&, double, double, int)
 *  P    euler_flow(const T&, P start, double, int)
 */

#ifndef EPIX_PLOT_ALGO
#define EPIX_PLOT_ALGO

#include "triples.h"
#include "functions.h"
#include "domain.h"

namespace ePiX {

  template<class T> void plot_map_dom(const T& map, const domain& R)
    {
      int i_max((R.dx1() > 0) ? R.coarse.n1() : 0); // max summation index
      int j_max((R.dx2() > 0) ? R.coarse.n2() : 0);
      int k_max((R.dx3() > 0) ? R.coarse.n3() : 0);

      if (R.dx1() > 0) // there's something to draw
	{
	  int count1(R.fine.n1()); // number of intervals in subdivision
	  std::vector<vertex> data1(1+count1);

	  double st1(R.dx1());
	  double st2(R.step2());
	  double st3(R.step3());

	  for (int j=0; j <= j_max; ++j)
	    for (int k=0; k <= k_max; ++k)
	      {
		P curr(R.corner1 + (j*st2)*E_2 + (k*st3)*E_3);

		for (int i=0; i <= count1; ++i, curr += st1*E_1)
		  data1.at(i) = vertex(map(curr));

		// close path if initial and terminal points coincide
		path path1(data1, false);
		path1.draw();
	      }
	}

      if (R.dx2() > 0)
	{
	  int count2(R.fine.n2());
	  std::vector<vertex> data2(1+count2);

	  double st1(R.step1());
	  double st2(R.dx2());
	  double st3(R.step3());

	  for (int i=0; i <= i_max; ++i)
	    for (int k=0; k <= k_max; ++k)
	      {
		P curr(R.corner1 + (i*st1)*E_1 + (k*st3)*E_3);
	      
		for (int j=0; j <= count2; ++j, curr += st2*E_2)
		  data2.at(j) = vertex(map(curr));

		// close path if initial and terminal points coincide
		path path2(data2, false);
		path2.draw();
	      }
	}

      if (R.dx3() > 0)
	{
	  int count3(R.fine.n3());
	  std::vector<vertex> data3(1+count3);

	  double st1(R.step1());
	  double st2(R.step2());
	  double st3(R.dx3());

	  for (int i=0; i <= i_max; ++i)
	    for (int j=0; j <= j_max; ++j)
	      {
		P curr(R.corner1 + (i*st1)*E_1 + (j*st2)*E_2);

		for (int k=0; k <= count3; ++k, curr += st3*E_3)
		  data3.at(k) = vertex(map(curr));

		// close path if initial and terminal points coincide
		path path3(data3, false);
		path3.draw();
	      }
	}
    }; // end of plot_map_dom

  // plot over a list of domains
  template<class T> void plot_map_domlist(const T& map,
					  const std::vector<domain>& R_list)
    {
      for (unsigned int i=0; i < R_list.size(); ++i)
	plot_map_dom(map, R_list.at(i));
    }

  // paths
  template<class T>void plot_function(const T& f, double t1, double t2, int n)
    {
      plot_map_dom(f, domain(t1, t2, n));
    }


  // Solutions of ODE systems
  template<class VF> void euler_plot(const VF& field, const P& start, 
				     double t_min, double t_max, int num_pts)
  {
    std::vector<vertex> data(num_pts+1);

    P curr(start);
    const double dt(t_max/(num_pts*EPIX_ITERATIONS));
    const double dseek(t_min/(num_pts*EPIX_ITERATIONS));

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
  } // end of euler_plot


  // flow x0 under field for specified time; pass x0 by value
  template<class VF> P euler_flow(const VF& field, P x0,
				  double t_max, int num_pts=0)
  {
    if (num_pts == 0) // use "sensible" default; hardwired constant 4
      num_pts = 4*(1 + (int)ceil(fabs(t_max)));

    const double dt(t_max/(num_pts*EPIX_ITERATIONS));

    for (int i=0; i <= num_pts*EPIX_ITERATIONS; ++i)
      x0 += dt*field(x0);

    return x0;
  }
} // end of namespace

#endif /* EPIX_PLOT_ALGO */
