/* 
 *  path.cc -- ePiX classes for polygons and paths
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc14
 * Last Change: July 16, 2004
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

#include <cstdarg>
#include <vector>
#include <algorithm>
#include <cmath>

#include "globals.h"
#include "triples.h"
#include "path.h"
#include "lengths.h"
#include "camera.h"

namespace ePiX {

  extern epix_camera camera;

  // lines, without and with specified number of points
  path::path(const P tail, const P head, const double expand)
  {
    double c = expm1(M_LN2*expand/100.0); // 2^{expand/100} - 1
    P dir = head - tail;

    if (epix::path_style() == SOLID)
      {
	std::vector<vertex> data(2);
  
	data.at(0) = vertex(tail - (0.5*c)*dir);
	data.at(1) = vertex(head + (0.5*c)*dir);

	vertices = data;
	closed = false;
	filled = false;
      }

    else // DASHED or DOTTED
      {
	// estimate number of points
	// screen distance from (expanded) tail to head, in true pt
	double dist = norm(p2t(c2s((1+c)*(camera(head) - camera(tail)))));

	int num_pts = (int) ceil(dist/epix::get_dashlength());

	std::vector<vertex> data(num_pts+1);
  
	// starting location
	P pt = tail - (0.5*c)*dir;

	dir *= ((1+c)/num_pts);

	for (unsigned int i=0; i < data.size(); ++i, pt += dir)
	  data.at(i) = vertex(pt);

	vertices = data;
	closed = false;
	filled = false;
      }
  } // end of line constructor

  
  path::path(const P tail, const P head, 
	     const double expand, int num_pts)
  {
    std::vector<vertex> data(num_pts+1);
  
    double c = expm1(M_LN2*expand/100.0); // 2^{expand/100} - 1

    // direction and starting location
    P dir = head - tail;
    P pt = tail - (0.5*c)*dir;

    dir *= ((1+c)/num_pts);

    for (unsigned int i=0; i < data.size(); ++i, pt += dir)
      data.at(i) = vertex(pt);

    vertices = data;
    closed = false;
    filled = false;

  } // end of line constructor


  // finite-data paths (ellipse, spline)
  path::path(const P center, const P axis1, const P axis2, 
	     const double t_min, const double t_max,
	     int default_num_pts)
  {
    using ePiX::cos;
    using ePiX::sin;

    // default_num_pts = one full turn
    double frac = fabs(t_max-t_min)/(epix::full_turn());
    int num_pts = (int) max(1, ceil(frac*default_num_pts));

    std::vector<vertex> data(num_pts+1);
    vertices = data;
  
    const double dt = (t_max - t_min)/num_pts;
    double t = t_min;

    for (int i=0; i <= num_pts; ++i, t += dt) 
      vertices.at(i) = vertex(center + ((cos(t)*axis1)+(sin(t)*axis2)));

    closed = false;
    filled = epix::fill_paths;

    if (fabs(frac - 1) < EPIX_EPSILON) // one full turn?
      {
	vertices.pop_back();
	closed = true;
      }
  } // end of ellipse constructor


  // Splines
  static inline P
  spl_pt(const P p1, const P p2, const P p3, double t)
  {
    return t*t*p1 + 2*t*(1-t)*p2 + (1-t)*(1-t)*p3;
  }

  static inline P
  spl_pt(const P p1, const P p2, const P p3, const P p4, 
	 double t)
  {
    return t*t*t*p1 + 3*t*t*(1-t)*p2 + 
      3*t*(1-t)*(1-t)*p3 + (1-t)*(1-t)*(1-t)*p4;
  }


  path::path(const P p1, const P p2, const P p3, const int num_pts)
  {
    std::vector<vertex> data(num_pts+1);
    vertices = data;
  
    const double dt = 1.0/num_pts;

    for (unsigned int i=0; i < vertices.size(); ++i) 
      vertices.at(i) = spl_pt(p1, p2, p3, i*dt);

    closed = false;
    filled = false;

  }


  path::path(const P p1, const P p2, const P p3, const P p4, const int num_pts)
  {
    std::vector<vertex> data(num_pts+1);
    vertices = data;

    const double dt = 1.0/num_pts;

    for (unsigned int i=0; i < vertices.size(); ++i) 
      vertices.at(i) = spl_pt(p1, p2, p3, p4, i*dt);

    closed = false;
    filled = false;

  }

  path::path(P f(double), double t_min, double t_max, int num_pts)
  {
    std::vector<vertex> data(num_pts+1);
    vertices = data;

    const double dt = (t_max - t_min)/num_pts;
    double t = t_min;

    for (int i=0; i <= num_pts; ++i, t += dt) 
      vertices.at(i) = vertex(f(t));

    closed = false; // result may be used as a summand
    filled = false;
  }

  path::path(double f(double), double t_min, double t_max, int num_pts)
  {
    std::vector<vertex> data(num_pts+1);
    vertices = data;

    const double dt = (t_max - t_min)/num_pts;
    double t = t_min;

    for (int i=0; i <= num_pts; ++i, t += dt) 
      vertices.at(i) = vertex(P(t,f(t)));

    closed = false;
    filled = false;
  }


  // polygon/polyline
  path polygon(int num_pts, ...)
  {
    std::vector<vertex> data(num_pts);

    va_list ap;
    va_start(ap, num_pts);
  
    for (int i=0; i < num_pts; ++i) 
      data.at(i) = vertex(*va_arg(ap, P*));

    va_end(ap);

    return path(data, true); // closed
  }

  path polyline(int num_pts, ...)
  {
    std::vector<vertex> data(num_pts);

    va_list ap;
    va_start(ap, num_pts);
  
    for (int i=0; i < num_pts; ++i) 
      data.at(i) = vertex(*va_arg(ap, P*));

    va_end(ap);

    return path(data, false); // closed
  }


  // concatenate
  path& path::operator+= (const path& data)
  {
    unsigned int my_size = vertices.size();
    vertices.resize(my_size + data.vertices.size());

    for(unsigned int i=0; i < data.vertices.size(); ++i)
      vertices.at(my_size+i) = data.vertices.at(i);

    return *this;
  }

  path& path::operator-= (const path& data)
  {
    unsigned int my_size = vertices.size();
    vertices.resize(my_size+data.vertices.size());

    for(unsigned int i=0; i < data.vertices.size(); ++i)
      vertices.at(my_size + data.vertices.size()-1-i) = data.vertices.at(i);

    return *this;
  }
} /* end of namespace */

