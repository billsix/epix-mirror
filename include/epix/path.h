/* 
 * path.h -- ePiX user class for polygons and paths
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.2.5
 * Last Change: May 04, 2008
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
#ifndef EPIX_PATHS
#define EPIX_PATHS

#include <vector>

namespace ePiX {

  class Camera;
  class Color;
  class P;
  class path_data;
  class pen_data;
  class screen;
  class Sphere;

  class path {
    friend class facet;
  public:

    // path was visible to users, but is now implemented as a p_impl.
    path(const path&);
    path& operator= (const path&);
    ~path();

    // Interface dictated by backward compatibility
    path(const std::vector<P>& data, bool closed, bool filled);
    path();

    // line constructors
    path(const P& tail, const P& head, double expand=0);
    path(const P&, const P&, double expand, unsigned int num_pts);


    // Constructors that specify the number of points have two prototypes
    // rather than accepting a default argument. This hides the global
    // (but user-inaccessible) default number of points (see constants.h).

    // ellipse
    path(const P& center, const P& axis1, const P& axis2, 
	 double t_min, double t_max,
	 unsigned int num_pts);

    path(const P& center, const P& axis1, const P& axis2, 
	 double t_min, double t_max);


    // spline
    path(const P& p1, const P& p2, const P& p3, unsigned int n);
    path(const P& p1, const P& p2, const P& p3);

    path(const P& p1, const P& p2, const P& p3, const P& p4, unsigned int n);
    path(const P& p1, const P& p2, const P& p3, const P& p4);


    // parametric path
    path(P f(double), double t_min, double t_max, unsigned int num_pts);
    path(P f(double), double t_min, double t_max);

    // function graph
    path(double f(double), double t_min, double t_max, unsigned int num_pts);
    path(double f(double), double t_min, double t_max);

    // append a point
    path& pt(double, double, double=0);
    path& pt(const P&);

    // concatenate path segments
    path& operator+= (const path&);
    // concatenate, reversing second sequence
    path& operator-= (const path&);

    bool is_closed() const;
    bool is_filled() const;

    // set attributes
    path& close();
    path&  fill(const bool arg=true);


    void clip();

    // assumes path lies on sphere
    void clip_to(const Sphere&, const P& viewpt, bool back=false);

    std::vector<P> data() const;

    void draw() const;
    void draw(const Color&, const pen_data&) const;

  private:
    path_data*   m_segments;

  }; // end of class path

  /*
    Slated for removal
    // polygon/polyline with variable number of vertices
    path polygon(unsigned int num_pts ...);
    path polyline(unsigned int num_pts ...);
  */

} // end of namespace

#endif /* EPIX_PATHS */
