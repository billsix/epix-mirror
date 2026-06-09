/* 
 * curves.cc -- polygons, ellipses, circular arcs, splines
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.2.6
 * Last Change: January 31, 2009
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009
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

#include <cmath>

#include "constants.h"
#include "triples.h"
#include "errors.h"

#include "functions.h"
#include "pairs.h"

#include "camera.h"

#include "active_screen.h"
#include "screen.h"

#include "paint_style.h"

#include "state.h"
#include "domain.h"

#include "arrow_data.h"
#include "path.h"
#include "picture.h"

#include "spline.h"
#include "spline_data.h"
#include "curves.h"

namespace ePiX {

  // Simple geometric objects

  // Lines take a stretch factor, roughly in percent
  void line(const P& tail, const P& head, double expand)
  {
    unsigned int num_pts(cam().is_linear() ? 1 : EPIX_NUM_PTS);

    path data(tail, head, expand, num_pts);
    data.draw();
  }

  void line(const P& tail, const P& head, double expand,
	    unsigned int num_pts)
  {
    if (!cam().is_linear())
      num_pts = (unsigned int) max(num_pts, EPIX_NUM_PTS);

    path data(tail, head, expand, num_pts);
    data.draw();
  }

  // Line(p1, p2) -- draw uncropped portion of long line through p1, p2
  void Line(const P& arg1, const P& arg2)
  {
    P dir(arg2-arg1);
    const double denom(norm(dir));

    if (EPIX_EPSILON < denom)
      {
	// CAUTION: Magic numbers
	dir *= 1/denom;
	line(arg1-100*dir, arg1+100*dir, 0, cam().is_linear() ? 1 : 400);
      }
  } // end of Line


  // point-slope form
  void Line(const P& tail, double slope)
  {
    Line(tail, tail+P(1, slope, 0));
  }


  void triangle(const P& p1, const P& p2, const P& p3)
  {
    path data;
    if (cam().is_linear())
      data.pt(p1).pt(p2).pt(p3);
    else
      {
	// Magic number 60
	const unsigned int N(60);
	const double dt(1.0/N);

	const P step12(dt*(p2-p1));
	const P step23(dt*(p3-p2));
	const P step31(dt*(p1-p3));

	for (unsigned int i=0; i<N; ++i)
	  data.pt(p1 + i*step12);

	for (unsigned int i=0; i<N; ++i)
	  data.pt(p2 + i*step23);

	for (unsigned int i=0; i<N; ++i)
	  data.pt(p3 + i*step31);
      }

    data.close().fill(the_paint_style().fill_flag());
    data.draw();
  }

  void quad(const P& p1, const P& p2, const P& p3, const P& p4)
  {
    path data;
    if (cam().is_linear())
      data.pt(p1).pt(p2).pt(p3).pt(p4);
    else
      {
	// Magic number 60 -> quad has 240 pts, is printed in one segment
	const unsigned int N(60);
	const double dt(1.0/N);

	const P step12(dt*(p2-p1));
	const P step23(dt*(p3-p2));
	const P step34(dt*(p4-p3));
	const P step41(dt*(p1-p4));

	for (unsigned int i=0; i<N; ++i)
	  data.pt(p1 + i*step12);

	for (unsigned int i=0; i<N; ++i)
	  data.pt(p2 + i*step23);

	for (unsigned int i=0; i<N; ++i)
	  data.pt(p3 + i*step34);

	for (unsigned int i=0; i<N; ++i)
	  data.pt(p4 + i*step41);
      }

    data.close().fill(the_paint_style().fill_flag());
    data.draw();
  }

  // Draw coordinate rectangle with opposite corners as given. Arguments
  // must lie is a plane parallel to a coordinate plane, but not on a 
  // line parallel to a coordinate axis.

  void rect(const P& p1, const P& p2)
  {
    P diagonal(p2 - p1);
    P jump;
    int perp_count(0);

    // count coordinate axes perp to diagonal and flag normal
    if (fabs(diagonal|E_1) < EPIX_EPSILON)
      {
	++perp_count;
	jump = E_2&(diagonal);
      }
    if (fabs(diagonal|E_2) < EPIX_EPSILON)
      {
	++perp_count;
	jump = E_3&(diagonal);
      }
    if (fabs(diagonal|E_3) < EPIX_EPSILON)
      {
	++perp_count;
	jump = E_1&(diagonal);
      }

    quad(p1, p1+jump, p2, p2-jump);
  } // end rect

  void dart(const P& tail, const P& head)
  { 
    arrow(tail, head, 0.5); 
  }

  void aarrow(const P& tail, const P& head, double scale)
  {
    P midpt(0.5*(tail+head));
    arrow(midpt, tail, scale);
    arrow(midpt, head, scale);
  }

  void ellipse(const P& center, const P& axis1, const P& axis2,  
	       double t_min, double t_max, unsigned int num_pts)
  {
    path data(center, axis1, axis2, t_min, t_max, num_pts);


    if (min(fabs(t_max-t_min)/full_turn(), 1) == 1)
      {
	data.close();
	if (the_paint_style().fill_flag())
	  data.fill();
      }
    data.draw();
  }  

  void ellipse(const P& center, const P& axis1, const P& axis2,  
	       double t_min, double t_max)
  {
    ellipse(center, axis1, axis2, t_min, t_max, EPIX_NUM_PTS);
  }


  void ellipse(const P& center, const P& axis1, const P& axis2)
  {
    ellipse(center, axis1, axis2, 0, full_turn());
  }


  void ellipse_arc(const P& center, const P& axis1, const P& axis2,
		   double t_min, double t_max)
  {
    ellipse(center, axis1, axis2, t_min, t_max);
  }


  void arrow(const P& tail, const P& head, double scale)
  {
    std::vector<P> shaft(2);
    shaft.at(0) = tail;
    shaft.at(1) = head;

    arrow_data data(shaft, tail, head, scale);
    data.draw();
  }

  void ellipse(const P& center, const P& radius)
  {
    ellipse(center, radius.x1()*E_1, radius.x2()*E_2);
  }

  // Standard half-ellipse functions
  void ellipse_left (const P& center, const P& radius)
  {
    ellipse(center, radius.x1()*E_1, radius.x2()*E_2, 
	    0.25*full_turn(), 0.75*full_turn());
  }

  void ellipse_right (const P& center, const P& radius)
  {
    ellipse(center, radius.x1()*E_1, radius.x2()*E_2, 
	    -0.25*full_turn(), 0.25*full_turn());
  }

  void ellipse_top (const P& center, const P& radius)
  {
    ellipse(center, radius.x1()*E_1, radius.x2()*E_2, 0, 0.5*full_turn());
  }

  void ellipse_bottom (const P& center, const P& radius)
  {
    ellipse(center, radius.x1()*E_1, radius.x2()*E_2, -0.5*full_turn(), 0);
  }

  void arc(const P& center, double r, 
	   double start,  double finish)
  { 
    ellipse(center, r*E_1, r*E_2, start, finish); 
  }

  void arrow(const P& center, const P& axis1, const P& axis2,
	     double t_min, double t_max, double scale)
  {
    // EPIX_NUM_PTS pts = one full turn; scale accordingly
    double frac(fabs(t_max-t_min)/full_turn());
    unsigned int num_pts((unsigned int) max(2, ceil(frac*EPIX_NUM_PTS)));

    const double dt((t_max - t_min)/num_pts);

    std::vector<P> shaft(num_pts+1);

    for (unsigned int i=0; i <= num_pts; ++i)
      {
	double t(t_min + i*dt);
	shaft.at(i) = center + ((Cos(t)*axis1)+(Sin(t)*axis2));
      }

    arrow_data data(shaft, shaft.at(num_pts-1), shaft.at(num_pts), scale);
    data.draw();
  }

  // circular arcs parallel to (x,y)-plane

  void arc_arrow(const P& center, double r, 
		 double start, double finish, double scale)
  {
    arrow(center, r*E_1, r*E_2, start, finish, scale);
  }


  // quadratic spline
  void spline(const P& p1, const P& p2, const P& p3, unsigned int num_pts)
  {
    path data(p1, p2, p3, num_pts);
    data.draw();
  }

  void spline(const P& p1, const P& p2, const P& p3)
  {
    spline(p1, p2, p3, EPIX_NUM_PTS);
  }

  void arrow(const P& p1, const P& p2, const P& p3, double scale)
  {
    const unsigned int num_pts(EPIX_NUM_PTS);
    const double dt(1.0/num_pts);
    std::vector<P> shaft(num_pts+1);

    for (unsigned int i=0; i <= num_pts; ++i)
      shaft.at(i) = spl_pt(p1, p2, p3, i*dt);

    arrow_data data(shaft, shaft.at(num_pts-1), shaft.at(num_pts), scale);
    data.draw();
  }

  // cubic spline
  void spline(const P& p1, const P& p2, 
	      const P& p3, const P& p4, unsigned int num_pts)
  {
    path data(p1, p2, p3, p4, num_pts);
    data.draw();
  }

  void spline(const P& p1, const P& p2, const P& p3, const P& p4)
  {
    spline(p1, p2, p3, p4, EPIX_NUM_PTS);
  }

  // natural spline through points
  void spline(const std::vector<P>& data, unsigned int num_pts)
  {
    n_spline tmp(data, data.at(0) == data.at(data.size()-1));
    path trace(tmp.data(num_pts));

    trace.draw();
  }

  void arrow(const P& p1, const P& p2, const P& p3, const P& p4, double scale)
  {
    const unsigned int num_pts(EPIX_NUM_PTS);
    const double dt(1.0/num_pts);
    std::vector<P> shaft(num_pts+1);

    for (unsigned int i=0; i <= num_pts; ++i)
      shaft.at(i) = spl_pt(p1, p2, p3, p4, i*dt);

    arrow_data data(shaft, shaft.at(num_pts-1), shaft.at(num_pts), scale);
    data.draw();
  }


  // n1 x n2 Cartesian grid, where coarse = (n1, n2)
  void grid(const P& p1, const P& p2, mesh coarse, mesh fine)
  {
    P diagonal(p2 - p1);
    P jump1, jump2; // sides of grid

    int perp_count(0);

    int N1(coarse.n1());
    int N2(coarse.n2());

    // count coordinate axes diagonal is perp to and flag normal
    if (fabs(diagonal|E_1) < EPIX_EPSILON)
      {
	++perp_count;
	jump1 = E_2&diagonal;
	jump2 = E_3&diagonal;

      }
    if (fabs(diagonal|E_2) < EPIX_EPSILON)
      {
	++perp_count;
	jump1 = E_3&diagonal;
	jump2 = E_1&diagonal;
      }
    if (fabs(diagonal|E_3) < EPIX_EPSILON)
      {
	++perp_count;
	jump1 = E_1&diagonal;
	jump2 = E_2&diagonal;
      }

    if (perp_count != 1)
      epix_warning("Ignoring degenerate coordinate grid");

    else
      {
	// grid line spacing
	P grid_step1((1.0/N1)*jump1);
	P grid_step2((1.0/N2)*jump2);

	// makes grid subject to filling
	rect(p1, p1 + jump1 + jump2);

	for (int i=1; i < N1; ++i)
	  line(p1+i*grid_step1, p1+i*grid_step1+jump2, 0, fine.n2());

	for (int j=1; j < N2; ++j)
	  line(p1+j*grid_step2, p1+j*grid_step2+jump1, 0, fine.n1());
      }
  }

  // Grids that fill bounding_box with default camera
  void grid(const P& p1, const P& p2, unsigned int n1, unsigned int n2)
  {
    grid(p1, p2, mesh(n1, n2), mesh(1,1));
  }

  void grid(unsigned int n1, unsigned int n2)
  {
    grid(active_screen()->bl(), active_screen()->tr(), n1, n2);
  }


  // polar grid with specified radius, mesh (rings and sectors), and resolution
  void polar_grid(double radius, mesh coarse, mesh fine)
  {
    for (int i=1; i <= coarse.n1(); ++i) 
      ellipse(P(0,0,0), 
	      (i*radius/coarse.n1())*E_1, (i*radius/coarse.n1())*E_2, 
	      0, full_turn(), fine.n2());

    for (int j=0; j < coarse.n2(); ++j)
      line(P(0,0,0), polar(radius, j*(full_turn())/coarse.n2()), 
	   0, 2*fine.n1());
  }

  void polar_grid(double radius, unsigned int n1, unsigned int n2)
  {
    polar_grid(radius, mesh(n1,n2), mesh(n1,EPIX_NUM_PTS));
  }


  // logarithmic grids

  // local helpers
  void grid_lines1_log(double x_lo, double x_hi, double y_lo, double y_hi,
		       unsigned int segs, unsigned int base)
  {
    if (segs == 0)
      return;

    const double dx((x_hi-x_lo)/segs); // "major grid" steps
    const double denom(log(base));  // "minor"/log grid scale factor

    for (unsigned int i=0; i < segs; ++i)
      for (unsigned int j=1; j<base; ++j)
	{
	  double x_tmp(x_lo + dx*(i+log(j)/denom));

	  line(P(x_tmp, y_lo), P(x_tmp, y_hi));
	}

    line(P(x_hi,y_lo), P(x_hi, y_hi)); // draw rightmost line manually
  }

  void grid_lines2_log(double x_lo, double x_hi, double y_lo, double y_hi,
		       unsigned int segs, unsigned int base)
  {
    if (segs == 0)
      return;

    const double dy((y_hi-y_lo)/segs);
    const double denom(log(base));

    for (unsigned int i=0; i < segs; ++i)
      for (unsigned int j=1; j<base; ++j)
	{
	  double y_tmp(y_lo + dy*(i+log(j)/denom));

	  line(P(x_lo, y_tmp), P(x_hi, y_tmp));
	}

    line(P(x_hi,y_lo), P(x_hi, y_hi));
  }

  // global functions
  void log_grid(const P& p1, const P& p2,
		unsigned int segs1, unsigned int segs2,
		unsigned int base1, unsigned int base2)
  {
    grid_lines1_log(min(p1.x1(), p2.x1()), max(p1.x1(), p2.x1()),
		    min(p1.x2(), p2.x2()), max(p1.x2(), p2.x2()),
		    segs1, base1);

    grid_lines2_log(min(p1.x1(), p2.x1()), max(p1.x1(), p2.x1()),
		    min(p1.x2(), p2.x2()), max(p1.x2(), p2.x2()),
		    segs2, base2);
  }

  void log1_grid(const P& p1, const P& p2,
		 unsigned int segs1, unsigned int segs2,
		 unsigned int base1)
  {
    grid_lines1_log(min(p1.x1(), p2.x1()), max(p1.x1(), p2.x1()),
		    min(p1.x2(), p2.x2()), max(p1.x2(), p2.x2()),
		    segs1, base1);

    grid_lines2_log(min(p1.x1(), p2.x1()), max(p1.x1(), p2.x1()),
		    min(p1.x2(), p2.x2()), max(p1.x2(), p2.x2()),
		    segs2, 2);
  }

  void log2_grid(const P& p1, const P& p2,
		 unsigned int segs1, unsigned int segs2,
		 unsigned int base2)
  {
    grid_lines1_log(min(p1.x1(), p2.x1()),
		    max(p1.x1(), p2.x1()),
		    min(p1.x2(), p2.x2()),
		    max(p1.x2(), p2.x2()),
		    segs1, 2);

    grid_lines2_log(min(p1.x1(), p2.x1()),
		    max(p1.x1(), p2.x1()),
		    min(p1.x2(), p2.x2()),
		    max(p1.x2(), p2.x2()),
		    segs2, base2);
  }


  // grids fill current bounding box
  void log_grid(unsigned int segs1, unsigned int segs2,
		unsigned int base1, unsigned int base2)
  {
    grid_lines1_log(active_screen()->h_min(), active_screen()->h_max(),
		    active_screen()->v_min(), active_screen()->v_max(),
		    segs1, base1);

    grid_lines2_log(active_screen()->h_min(), active_screen()->h_max(),
		    active_screen()->v_min(), active_screen()->v_max(),
		    segs2, base2);
  }

  void log1_grid(unsigned int segs1, unsigned int segs2,
		 unsigned int base1)
  {
    grid_lines1_log(active_screen()->h_min(), active_screen()->h_max(),
		    active_screen()->v_min(), active_screen()->v_max(),
		    segs1, base1);

    grid_lines2_log(active_screen()->h_min(), active_screen()->h_max(),
		    active_screen()->v_min(), active_screen()->v_max(),
		    segs2, 2);
  }

  void log2_grid(unsigned int segs1, unsigned int segs2,
		 unsigned int base2)
  {
    grid_lines1_log(active_screen()->h_min(), active_screen()->h_max(),
		    active_screen()->v_min(), active_screen()->v_max(),
		    segs1, 2);

    grid_lines2_log(active_screen()->h_min(), active_screen()->h_max(),
		    active_screen()->v_min(), active_screen()->v_max(),
		    segs2, base2);
  }


  // fractal generation
  //
  // The basic recursion unit is a piecewise-linear path whose segments
  // are parallel to spokes on a wheel, labelled modulo <spokes>.
  // Recursively up to <depth>, each segment is replaced by a copy of the
  // recursion unit, scaled and rotated in order to join p to q.
  //
  // Kludge: pre_seed[0] = spokes, pre_seed[1] = length of seed;
  //
  // Sample data for _/\_ standard Koch snowflake:
  // const int seed[] = {6, 4, 0, 1, -1, 0};

  P jump(int spokes, int length, const std::vector<int>& seed)
  {
    P sum(P(0,0));

    for (int i=0; i< length; ++i)
      sum += cis(seed.at(i)*(full_turn())/spokes);

    return sum;
  }

  void fractal(const P& p, const P& q, const int depth, const int *pre_seed)
  {
    int spokes(pre_seed[0]);
    int seed_length(pre_seed[1]);
    std::vector<int> seed(seed_length);

    // extract seed from pre_seed
    for (int i=0; i<seed_length; ++i)
      seed.at(i) = pre_seed[i+2];

    // Unit-length steps in <seed> sequence add up to <scale>
    P scale(jump(spokes, seed_length, seed));

    // Number of points in final fractal
    int length(1+(int)pow(seed_length, depth));
    std::vector<int> dir(length); // stepping information
    std::vector<P> data(length);  // vertices

    // dir[] starts out [0, 1, -1, 0, ..., 0] (seed_length^depth entries)
    // then take repeated "Kronecker sum" with seed = [0, 1, -1, 0]

    for(int i=0; i<seed_length; ++i)
      dir.at(i) = seed.at(i);

    for(int i=1; i<depth; ++i) // recursively fill dir array
      for(int j=0; j < pow(seed_length,i); ++j)
	for(int k=seed_length-1; 0 < k; --k)
	  dir.at(k*(int)pow(seed_length,i) + j) = dir.at(j) + seed.at(k);

    P curr(p);
    // 10/09/06: -depth -> 1-depth
    double radius(pow(norm(scale), 1-depth));

    for(int i=0; i<length; ++i)
      {
	data.at(i) = curr;
	// increment between successive points as a pair
	pair temp((polar(radius, dir.at(i)*(full_turn())/spokes))*pair(q-p));

	// complex arithmetic
	temp /= pair(scale); // homothety to join p to q

	curr += P(temp.x1(), temp.x2());
      }

    path fractal(data, false, false);
    fractal.draw();
  }

} // end of namespace
