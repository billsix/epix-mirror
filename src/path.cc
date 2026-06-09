/* 
 * path.cc -- ePiX user class for polygons and paths
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

#include <vector>
#include <cmath>

#include "errors.h"
#include "triples.h"
#include "constants.h"

#include "functions.h"

#include "Color.h"
#include "pen_data.h"
#include "state.h"

#include "camera.h"
#include "active_screen.h"

#include "spline_data.h"
#include "path_data.h"

#include "path.h"

namespace ePiX {

  class screen;
  class Sphere;

  path::path(const path& C)
    : m_segments(C.m_segments->clone()) { }

  path& path::operator= (const path& C)
  {
    if (this != &C)
      {
	path_data*   tmp_segs(C.m_segments->clone());

	delete m_segments;

	m_segments=tmp_segs;
      }

    return *this;
  }

  path::~path()
  {
    delete m_segments;
  }


  path::path()
    : m_segments(new path_data()) { }

  path::path(const std::vector<P>& data, bool closed, bool filled)
    : m_segments(new path_data(data, closed, filled)) { }


  // path constructors *do not* close/fill; this must be done by client code

  // lines, without and with specified number of points
  path::path(const P& tail, const P& head, double expand)
    : m_segments(new path_data())
  {
    double c(expm1(M_LN2*expand/100.0)); // 2^{expand/100} - 1
    P dir(head - tail);

    m_segments->pt(tail - (0.5*c)*dir);
    m_segments->pt(head + (0.5*c)*dir);
  } // end of line constructor


  path::path(const P& tail, const P& head, 
	     double expand, unsigned int num_pts)
    : m_segments(new path_data())
  {
    if (num_pts < 1)
      num_pts=1;

    double c(expm1(M_LN2*expand/100.0)); // 2^{expand/100} - 1

    // direction and starting location
    P dir(head - tail);
    P start(tail - (0.5*c)*dir);

    dir *= ((1+c)/num_pts);

    for (unsigned int i=0; i <= num_pts; ++i)
      m_segments->pt(start+i*dir);

  } // end of line constructor


  // ellipse drawn with N edges
  path::path(const P& center, const P& axis1, const P& axis2, 
	     double t_min, double t_max,
	     unsigned int N)
    : m_segments(new path_data())
  {
    double T_MAX(t_max);
    if (full_turn() < fabs(t_max-t_min))
      {
	epix_warning("Angle bounds in ellipse span more than one turn");
	T_MAX = t_min + full_turn();
      }

    const unsigned int num_pts(1 < N ? N : 1);
    const double dt((T_MAX - t_min)/num_pts);

    for (unsigned int i=0; i <= num_pts; ++i)
      {
	double t(t_min + i*dt);
	m_segments->pt(center + ((Cos(t)*axis1)+(Sin(t)*axis2)));
      }
  } // end of ellipse constructor

  // ellipse with default number of points
  path::path(const P& center, const P& axis1, const P& axis2, 
	     double t_min, double t_max)
    : m_segments(new path_data())
  {
    double frac(min(fabs(t_max-t_min)/full_turn(), 1));
    auto num_pts((unsigned int) max(2, ceil(frac*EPIX_NUM_PTS)));

    const double dt((t_max - t_min)/num_pts);

    for (unsigned int i=0; i <= num_pts; ++i)
      {
	double t(t_min + i*dt);
	m_segments->pt(center + ((Cos(t)*axis1)+(Sin(t)*axis2)));
      }
  } // end of ellipse EPIX_NUM_PTS constructor


  // Splines
  path::path(const P& p1, const P& p2, const P& p3, unsigned int num_pts)
    : m_segments(new path_data())
  {
    if (num_pts < 2)
      num_pts=2; // draw at least V

    const double dt(1.0/num_pts);

    for (unsigned int i=0; i <= num_pts; ++i)
      m_segments->pt(spl_pt(p1, p2, p3, i*dt));
  }


  path::path(const P& p1, const P& p2, const P& p3)
    : m_segments(new path_data())
  {
    const double dt(1.0/EPIX_NUM_PTS);

    for (unsigned int i=0; i <= EPIX_NUM_PTS; ++i)
      m_segments->pt(spl_pt(p1, p2, p3, i*dt));
  }


  path::path(const P& p1, const P& p2, const P& p3, const P& p4,
	     unsigned int num_pts)
    : m_segments(new path_data())
  {
    if (num_pts < 3)
      num_pts=3; // draw at least N

    const double dt(1.0/num_pts);

    for (unsigned int i=0; i <= num_pts; ++i)
      m_segments->pt(spl_pt(p1, p2, p3, p4, i*dt));
  }


  path::path(const P& p1, const P& p2, const P& p3, const P& p4)
    : m_segments(new path_data())
  {
    const double dt(1.0/EPIX_NUM_PTS);

    for (unsigned int i=0; i <= EPIX_NUM_PTS; ++i)
      m_segments->pt(spl_pt(p1, p2, p3, p4, i*dt));
  }


  // parametrized path
  path::path(P f(double), double t_min, double t_max, unsigned int num_pts)
    : m_segments(new path_data())
  {
    if (num_pts < 2)
      num_pts = 2;

    const double dt((t_max - t_min)/num_pts);

    for (unsigned int i=0; i <= num_pts; ++i)
      m_segments->pt(f(t_min + i*dt));
  }

  // with default number of points
  path::path(P f(double), double t_min, double t_max)
    : m_segments(new path_data())
  {
    const double dt((t_max - t_min)/EPIX_NUM_PTS);

    for (unsigned int i=0; i <= EPIX_NUM_PTS; ++i)
      m_segments->pt(f(t_min + i*dt));
  }


  // graphs
  path::path(double f(double), double t_min, double t_max,
	     unsigned int num_pts)
    : m_segments(new path_data())
  {
    if (num_pts < 2)
      num_pts = 2;

    const double dt((t_max - t_min)/num_pts);

    for (unsigned int i=0; i <= num_pts; ++i)
      {
	double t(t_min + i*dt);
	m_segments->pt(P(t, f(t)));
      }
  }

  path::path(double f(double), double t_min, double t_max)
    : m_segments(new path_data())
  {
    const double dt((t_max - t_min)/EPIX_NUM_PTS);

    for (unsigned int i=0; i <= EPIX_NUM_PTS; ++i)
      {
	double t(t_min + i*dt);
	m_segments->pt(P(t, f(t)));
      }
  }


  // append a point
  path& path::pt(double x, double y, double z)
  {
    m_segments->pt(P(x, y, z));
    return *this;
  }

  path& path::pt(const P& loc)
  {
    m_segments->pt(loc);
    return *this;
  }


  // concatenate
  path& path::operator+= (const path& data)
  {
    *m_segments += *(data.m_segments);

    return *this;
  }

  path& path::operator-= (const path& data)
  {
    *m_segments -= *(data.m_segments);

    return *this;
  }

  path& path::close()
  {
    m_segments->close();
    return *this;
  }
  path& path::fill(const bool arg)
  {
    m_segments->fill(arg);
    return *this;
  }


  void path::clip()
  {
    m_segments->clip();
  }

  void path::clip_to(const Sphere& S, const P& viewpt, bool back)
  {
    m_segments->clip_to(S, viewpt, back);
  }

  std::vector<P> path::data() const
  {
    return m_segments->data();
  }

  void path::draw() const
  {
    m_segments->draw();
  }

  void path::draw(const Color& col, const pen_data& pen) const
  {
    m_segments->photo(*active_screen(), cam(), col, pen, Xfine());
  }
} // end of namespace
