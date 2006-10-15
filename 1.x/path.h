/* 
 *  path.h -- ePiX classes for polygons and paths
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.15
 * Last Change: October 09, 2006
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
#ifndef EPIX_PATHS
#define EPIX_PATHS

#include <vector>

#include "globals.h"
#include "triples.h"

namespace ePiX {

  class crop_mask;
  class enclosure;
  class sphere;

  class vertex {

  public:
    vertex(const double a1=0, const double a2=0, const double a3=0);
    vertex(const P& arg);

    P here() const;
    bool is_onscreen() const;
    bool is_in_world() const;

    void set_crop(const bool);
    void set_clip(const bool);

  private:
    P location;
    bool onscreen;
    bool in_world;
  }; //end of class vertex


  class path {
  public:

    path(const std::vector<vertex>& data, bool loop);
    path(int num_pts=0);

    // line constructors
    path(const P& tail, const P& head, const double expand=0);
    path(const P& tail, const P& head, const double expand, int num_pts);

    // finite-data paths (ellipse, spline)
    path(const P& center, const P& axis1, const P& axis2, 
	 const double t_min, const double t_max,
	 int num_pts=EPIX_NUM_PTS);

    path(const P& p1, const P& p2, const P& p3, 
	 int num_pts=EPIX_NUM_PTS);

    path(const P& p1, const P& p2, const P& p3, const P& p4, 
	 int num_pts=EPIX_NUM_PTS);


    path(P f(double), double t_min, double t_max, int num_pts=EPIX_NUM_PTS);
    path(double f(double), double t_min, double t_max, 
	 int num_pts=EPIX_NUM_PTS);

    // append a point
    path& pt(const double, const double, const double=0);
    path& pt(const P&);

    // concatenate path segments
    path& operator+= (const path& vertices);
    // concatenate, reversing second sequence
    path& operator-= (const path& vertices);

    bool is_closed(void) const { return closed; }
    bool is_filled(void) const { return filled; }

    // set attributes
    path& close(const bool arg=true);
    path& fill(const bool arg=true);
    path& set_fill(const bool arg=true); // deprecated

    void set_crop_all(const bool arg);
    void set_clip_all(const bool arg);

    // defined in output.cc
    void crop_to(const crop_mask& screen, const bool arg=true);
    void clip_to(const enclosure& world, const bool arg=true);
    //    void hide_by(bool hiding(const P));
    void draw(void);

    void draw(sphere, bool); // spherical plotting (front/back), in sphere.cc

  private:
    std::vector<vertex> vertices;

    bool closed;
    bool filled;
  }; // end of class path

  // polygon/polyline with variable number of vertices
  path polygon(int num_pts ...);
  path polyline(int num_pts ...);


  class path_pt {
  public:
    path_pt(const P& arg, bool is_start, bool is_end);
    path_pt(const vertex& arg, bool is_start, bool is_end);
    void unset();

    P here() const;
    bool is_start() const;
    bool is_end() const;

  private:
    P location;
    bool start; // start/end of path segment
    bool end;
  }; //end of class path_pt

} // end of namespace

#endif /* EPIX_PATHS */
