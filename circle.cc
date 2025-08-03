/* 
 * circle.cc -- ePiX::Circle class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.9
 * Last Change: July 30, 2007
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
#include <cmath>

#include "errors.h"

#include "constants.h"

#include "triples.h"
#include "state.h"

#include "segment.h"
#include "frame.h"
#include "camera.h"

#include "path.h"

#include "paint_style.h"

#include "intersections.h"

#include "circle.h"

namespace ePiX {

  static const double EPS(EPIX_EPSILON);

  Circle::Circle(const P& ctr, const double rad, const P& perp)
    : m_center(ctr), m_radius(rad), m_perp(perp), m_malformed(false)
  {
    double temp(norm(perp));
    if (temp < EPS)
      {
	epix_warning("Degenerate circle normal, using (0,0,1)");
	m_perp=E_3;
      }

    else
      m_perp *= (1.0/temp); // normalize
  }

  // point-and-center constructor -- parallel to (x1,x2,0)-plane
  Circle::Circle(const P& ctr, const P& pt)
    : m_center(ctr), m_radius(norm(pt - ctr)), m_perp(E_3), m_malformed(false)
  { 
    if (fabs(E_3|(pt - ctr)) > EPS)
      epix_warning("Circle requested with point not in (x,y) plane");
  }

  // three-point circle constructor
  Circle::Circle(const P& pt1, const P& pt2, const P& pt3)
    : m_malformed(false)
  {
    P D21(pt2-pt1), D31(pt3-pt1), D32(pt3-pt2), N(D21*D31);

    if (norm(D21) < EPS ||
	norm(D31) < EPS ||
	norm(D32) < EPS ||
	norm(N) < EPS)
      {
	epix_warning("Collinear points in Circle constructor");
	m_malformed = true;
      }

    else
      {
	m_perp = (1.0/norm(N))*N;

	P q2(0.5*(pt1+pt2));
	P dir2(m_perp*(q2-pt1));

	P q3(0.5*(pt1+pt3));
	P dir3(m_perp*(q3-pt1));

	m_center = Segment(q2, q2+dir2)*Segment(q3, q3+dir3);
	m_radius = norm(m_center - pt1);
      }
  }


  Circle::Circle(bool tag) : m_malformed(true) { }


  P Circle::center() const
  {
    return m_center;
  }

  double Circle::radius() const
  {
    return m_radius;
  }

  P Circle::perp() const
  {
    return m_perp;
  }

  bool Circle::malformed() const
  {
    return m_malformed;
  }

  // translation
  Circle& Circle::shift(const P& arg)
  {
    if (!m_malformed)
      m_center += arg;
    return *this;
  }

  Circle& Circle::move_to(const P& arg)
  {
    if (!m_malformed)
      m_center = arg;
    return *this;
  }

  // scale radius
  Circle& Circle::scale(const double c)
  {
    if (!m_malformed)
      m_radius *= c;
    return *this;
  }


  // project to screen by camera
  void Circle::draw() const
  {
    if (m_malformed)
      return;

    // else
    double r(m_radius);
    P N(m_perp);
    frame axes; // standard basis

    if (EPS <= norm(N*E_3))
      axes = frame(E_3*N, E_3, N);

    path data(m_center, r*(axes.sea()), r*(axes.sky()), 0, full_turn());
    data.close().fill(the_paint_style().fill_flag());
    data.draw();
  }


  //// global functions ////
  void circle(const P& ctr, const double rad, const P& perp)
  {
    Circle C(ctr, rad, perp);
    C.draw();
  }

  void circle(const P& ctr, const P& pt)
  {
    Circle C(ctr, pt);
    C.draw();
  }

  void circle(const P& pt1, const P& pt2, const P& pt3)
  {
    Circle C(pt1, pt2, pt3);
    C.draw();
  }
} // end of namespace
