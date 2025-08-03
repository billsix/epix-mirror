/*
 * plot_algorithms.h -- plot function templates, for build use only
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Andrew D. Hwang   <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
 *
 * Version 1.1.9
 * Last Change: August 01, 2007
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

/*
 * This file provides plotting templates, with T a class
 *
 *  void plot_map_dom(const T&, const domain&)
 *  void plot_map_domlist(const T&, const domain_list&)
 *  void plot_function(const T&, double, double, int)
 *  void euler_plot(const T&, const P&, double, double, int)
 *  P    euler_flow(const T&, P start, double, int)
 */

#ifndef EPIX_PLOT_ALGO
#define EPIX_PLOT_ALGO

#include <cmath>
#include <list>

#include "constants.h"

#include "triples.h"
#include "functions.h"

#include "edge_data.h"

#include "paint_style.h"

#include "domain.h"
#include "path_data.h"

namespace ePiX {

  typedef std::list<domain>::const_iterator dolci;

  const double INF(EPIX_INFTY);

  // Are args endpoints of a plottable edge?
  static bool is_valid(const P& tail, const P& head)
    {
      return tail.is_valid() && head.is_valid();
    }

  template<class T> void plot_map_dom(const T& map, const domain& R)
    {
      // only attempt to fill if plotting a curve
      const bool is_curve(R.dim() == 1);
      const bool filling(is_curve && the_paint_style().fill_flag());

      // max summation indices
      unsigned int i_max((R.dx1() > 0) ? R.coarse_n1() : 0);
      unsigned int j_max((R.dx2() > 0) ? R.coarse_n2() : 0);
      unsigned int k_max((R.dx3() > 0) ? R.coarse_n3() : 0);

      if (R.dx1() > 0) // there's something to draw
	{
	  // number of intervals in subdivision
	  unsigned int count1(R.fine_n1());

	  const double st1(R.dx1());
	  const double st2(R.step2());
	  const double st3(R.step3());

	  for (unsigned int j=0; j <= j_max; ++j)
	    for (unsigned int k=0; k <= k_max; ++k)
	      {
		std::list<edge3d> data1;
		const P init(R.corner1() + (j*st2)*E_2 + (k*st3)*E_3);

		for (unsigned int i=0; i < count1; ++i)
		  {
		    P curr(map(init + P(i*st1,0,0)));
		    P next(map(init + P((i+1)*st1,0,0)));
		    data1.push_back(edge3d(curr, next, is_valid(curr, next)));
		  }

		// close path if initial and terminal points coincide
		bool closed(is_curve &&
			    map(init) == map(init + P(count1*st1,0,0)));

		path_data path1(data1, closed && filling, filling);
		path1.draw();
	      }
	}

      if (R.dx2() > 0)
	{
	  unsigned int count2(R.fine_n2());

	  const double st1(R.step1());
	  const double st2(R.dx2());
	  const double st3(R.step3());

	  for (unsigned int i=0; i <= i_max; ++i)
	    for (unsigned int k=0; k <= k_max; ++k)
	      {
		std::list<edge3d> data2;
		const P init(R.corner1() + (i*st1)*E_1 + (k*st3)*E_3);
	      
		for (unsigned int j=0; j < count2; ++j)
		  {
		    P curr(map(init + P(0,j*st2,0)));
		    P next(map(init + P(0,(j+1)*st2,0)));
		    data2.push_back(edge3d(curr, next, is_valid(curr, next)));
		  }

		// close path if initial and terminal points coincide
		bool closed(is_curve &&
			    map(init) == map(init + P(0,count2*st2,0)));

		path_data path2(data2, closed && filling, filling);
		path2.draw();
	      }
	}

      if (R.dx3() > 0)
	{
	  unsigned int count3(R.fine_n3());

	  const double st1(R.step1());
	  const double st2(R.step2());
	  const double st3(R.dx3());

	  for (unsigned int i=0; i <= i_max; ++i)
	    for (unsigned int j=0; j <= j_max; ++j)
	      {
		std::list<edge3d> data3;
		const P init(R.corner1() + (i*st1)*E_1 + (j*st2)*E_2);

		for (unsigned int k=0; k < count3; ++k)
		  {
		    P curr(map(init + P(0,0,k*st3)));
		    P next(map(init + P(0,0,(k+1)*st3)));
		    data3.push_back(edge3d(curr, next, is_valid(curr, next)));
		  }

		// close path if initial and terminal points coincide
		bool closed(is_curve &&
			    map(init) == map(init + P(0,0,count3*st3)));

		path_data path3(data3, closed && filling, filling);
		path3.draw();
	      }
	}
    }; // end of plot_map_dom

  // plot over a list of domains
  template<class T> void plot_map_domlist(const T& map, const domain_list& R)
    {
      for (dolci p=R.m_list.begin(); p != R.m_list.end(); ++p)
	plot_map_dom(map, *p);
    }

  // paths
  template<class T>void plot_function(const T& f, double t1, double t2,
				      unsigned int n)
    {
      plot_map_dom(f, domain(t1, t2, n));
    }


  // Solutions of ODE systems
  template<class VF> void euler_plot(const VF& field, P curr, 
				     double t_min, double t_max, 
				     unsigned int num_pts)
  {
    std::list<edge3d> data;

    const double dt(t_max/(num_pts*EPIX_ITERATIONS));
    const double dseek(t_min/(num_pts*EPIX_ITERATIONS));

    if (fabs(t_min/num_pts) > EPIX_EPSILON) // seek beginning of path
      for (unsigned int i=0; i <= num_pts*EPIX_ITERATIONS; ++i)
	curr += dseek*field(curr);

    // curr = "start, flowed by t_min"
    P next(curr);

    for (unsigned int i=0; i <= num_pts*EPIX_ITERATIONS; ++i)
      {
	next += dt*field(next);  // Euler's method

	if (i%EPIX_ITERATIONS == 0)
	  {
	    data.push_back(edge3d(curr, next, is_valid(curr, next)));
	    curr = next;
	  }
      }

    path_data temp(data, false, false);

    temp.draw();
  } // end of euler_plot


  // flow x0 under field for specified time; pass x0 by value
  template<class VF> P euler_flow(const VF& field, P x0,
				  double t_max, unsigned int num_pts=0)
  {
    if (num_pts == 0) // use "sensible" default; hardwired constant 4
      num_pts = 4*(1 + (unsigned int)ceil(fabs(t_max)));

    const double dt(t_max/(num_pts*EPIX_ITERATIONS));

    for (unsigned int i=0; i <= num_pts*EPIX_ITERATIONS; ++i)
      x0 += dt*field(x0);

    return x0;
  }
} // end of namespace

#endif /* EPIX_PLOT_ALGO */
