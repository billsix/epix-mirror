/* 
 * plane.cc -- ePiX::Plane class and mathematical operators
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.2.2
 * Last Change: December 06, 2007
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

#include "constants.h"
#include "errors.h"

#include "curves.h" // for draw()

#include "plane.h"

namespace ePiX {

  const double INF(2*EPIX_INFTY);

  Plane::Plane(const P& pt, const P& perp)
    : m_pt(pt), m_perp(perp), m_malformed(false)
  { 
    double temp(norm(perp));

    if (temp < EPIX_EPSILON)
      {
	epix_warning("Degenerate plane normal, using (0,0,1)");
	m_perp=E_3;
      }

    else
      m_perp *= 1.0/temp;
  }

  Plane::Plane(const P& p1, const P& p2, const P& p3)
    : m_pt(p1), m_malformed(false)
  {
    P perp((p2-p1)*(p3-p1));
    double norm_perp(norm(perp));

    if (norm_perp < EPIX_EPSILON)
      {
	epix_warning("Collinear points in Plane constructor");
	m_malformed = true;
      }

    else
      m_perp = (1/norm_perp)*perp;
  }


  P Plane::pt() const
  {
    return m_pt;
  }

  P Plane::perp() const
  {
    return m_perp;
  }

  bool Plane::malformed() const
  {
    return m_malformed;
  }

  // translate
  Plane& Plane::shift(const P& arg)
  {
    if (!m_malformed)
      m_pt += arg;
    return *this;
  }

  Plane& Plane::move_to(const P& arg)
  {
    if (!m_malformed)
      m_pt = arg;
    return *this;
  }

  void Plane::draw() const
  {
    if (m_malformed)
      return;

    // else
    const P ctr((m_pt|m_perp)*m_perp); // closest point to origin

    // construct an orthonormal basis
    P e1(m_perp*E_1);

    if (norm(e1) < 0.7) // ~sqrt(0.5)
      e1 = m_perp*E_2;

    e1 *= 1.0/norm(e1);
    P e2(m_perp*e1);

    // very large rectangle
    quad(ctr + INF*( e1 + e2), ctr + INF*(-e1 + e2),
	 ctr + INF*(-e1 - e2), ctr + INF*( e1 - e2));
  }


  //// global functions ////
  void plane(const P& point, const P& normal)
  {
    Plane pl(point, normal);
    pl.draw();
  }

  void plane(const P& p1, const P& p2, const P& p3)
  {
    Plane pl(p1, p2, p3);
    pl.draw();
  }
} // end of namespace
