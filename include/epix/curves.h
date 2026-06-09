/*
 * curves.h -- Ellipses, arcs, splines, coordinate grids
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.10
 * Last Change: August 08, 2007
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

#ifndef EPIX_CURVES
#define EPIX_CURVES

#include <vector>

#include "constants.h"

namespace ePiX {

  class P;
  class mesh;

  // lines can be "stretched" by double parameter
  void line(const P& tail, const P& head, double expand=0); 
  void line(const P& tail, const P& head, double expand,
	    unsigned int num_pts);

  // "Visible" portion of the line through p1, p2
  void Line(const P& tail, const P& head);

  // point-slope form
  void Line(const P&, double);

  void triangle(const P&, const P&, const P&);
  void quad(const P&, const P&, const P&, const P&);

  void rect(const P&, const P&);
  void rect(const P&, const P&, bool solid);

  // arrows
  void arrow(const P& tail, const P& head, double scale=1);
  void dart(const P& tail, const P& head);

  // double-tipped
  void aarrow(const P& tail, const P& head, double scale=1);

  // arbitrary elliptical arc
  void arrow(const P& center, const P& axis1, const P& axis2,
	     double t_min, double t_max, double scale=1);

  // Algebraic curves (elliptical and circular arcs, splines)
  void ellipse(const P& center, const P& axis1, const P& axis2); // full turn

  void ellipse(const P& center, const P& axis1, const P& axis2,  
	       double t_min, double t_max); // angle range

  void ellipse(const P& center, const P& axis1, const P& axis2,  
	       double t_min, double t_max, unsigned int num_pts);

  // for backward compatibility
  void ellipse_arc(const P& center, const P& axis1, const P& axis2,
		   double t_min, double t_max);

  // old style "center and polyradius"
  void ellipse (const P& center, const P& radius);

  // Standard half-ellipse functions
  void ellipse_left (const P&, const P&);
  void ellipse_right (const P&, const P&);
  void ellipse_top (const P&, const P&);
  void ellipse_bottom (const P&, const P&);

  // Circular arcs parallel to (x,y)-plane
  void arc(const P& center, double r, 
	   double start,  double finish);

  void arc_arrow (const P& center, double r, 
		  double start, double finish, 
		  double scale=1);


  // Quadratic and cubic splines/spline arrows
  void spline(const P& p1, const P& p2, const P& p3, unsigned int num_pts);
  void spline(const P& p1, const P& p2, const P& p3);

  void arrow(const P& p1, const P& p2, const P& p3, double scale=1);

  void spline (const P& p1, const P& p2, const P& p3, const P& p4,
	       unsigned int num_pts);
  void spline (const P& p1, const P& p2, const P& p3, const P& p4);

  void arrow(const P&, const P&, const P&, const P&, double scale=1);

  // natural spline
  void spline(const std::vector<P>&, unsigned int num_pts);


  // A "mesh" is an ordered pair of positive integers, and is used to
  // specify the "fineness" of a grid. Grids, like parametric surface
  // meshes, have a "coarse mesh" -- the numbers of grid intervals in
  // each direction, and a "fine mesh" -- the numbers of points used
  // to render the grid lines. Since an ePiX camera does not always
  // map lines in object space to lines on the screen, grid lines cannot
  // generally be drawn using only two points.
  // A grid may look strange unless each component of fine is a multiple
  // of the corresponding entry of coarse, of course. :)

  // Cartesian grid of specified size, mesh, and resolution
  void grid(const P& arg1, const P& arg2, mesh coarse, mesh fine);

  // coarse = fine = (n1,n2)
  void grid(const P& arg1, const P& arg2,
	    unsigned int n1=1, unsigned int n2=1);
  void grid(unsigned int n1=1, unsigned int n2=1);

  // polar grid of specified radius, mesh (rings and sectors), and resolution
  void polar_grid(double r, mesh coarse, mesh fine);

  // polar grid with n1 rings and n2 sectors
  void polar_grid(double r, unsigned int n1, unsigned int n2);


  // (semi-)logarithmic grids specified by corners and numbers of orders of
  // magnitude or grid divisions in each direction. Optional arguments
  // specify the log base (10 by default). If corners are omitted, the grid
  // fills the bounding box.
  void log_grid(const P& arg1, const P& arg2,
		unsigned int segs1, unsigned int segs2,
		unsigned int base1=10, unsigned int base2=10);

  void log1_grid(const P& arg1, const P& arg2,
		 unsigned int segs1, unsigned int segs2,
		 unsigned int base1=10);

  void log2_grid(const P& arg1, const P& arg2,
		 unsigned int segs1, unsigned int segs2,
		 unsigned int base2=10);

  void log_grid(unsigned int segs1, unsigned int segs2,
		unsigned int base1=10, unsigned int base2=10);

  void log1_grid(unsigned int segs1, unsigned int segs2,
		 unsigned int base1=10);

  void log2_grid(unsigned int segs1, unsigned int segs2,
		 unsigned int base2=10);

  // fractal generation
  //
  // The basic "level-1" recursion unit is a piecewise-linear path whose 
  // segments are parallel to spokes on a wheel, labelled modulo <spokes>.
  // Recursively up to <depth>, each segment is replaced by a copy of the
  // recursion unit.
  //
  // Sample data for _/\_ standard Koch snowflake:
  // const int pre_seed[] = {6, 4, 0, 1, -1, 0};
  // pre_seed[0] = spokes, pre_seed[1] = seed_length;

  void fractal (const P& p, const P& q, const int depth, const int *pre_seed);

} // end of namespace

#endif /* EPIX_CURVES */
