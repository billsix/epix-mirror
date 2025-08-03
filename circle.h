/* 
 * circle.h -- ePiX::Circle class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.9
 * Last Change: July 30, 2007
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
 *   This file provides:
 *
 *   The Circle class (center, radius, unit normal).
 *
 *    - Circle() (defaults to origin, unit radius, in (x1,x2,0)-plane
 *    - Circle(ctr, pt) (parallel to (x1,x2,0)-plane, center ctr, through pt)
 *    - Circle(pt1, pt2, pt3) (arbitrary non-collinear points)
 *    - center(), radius(), perp()
 *    - affine operations: shift(P), move_to(P), scale(double)
 *    - draw() (ePiX ellipse)
 */
#ifndef EPIX_CIRCLE
#define EPIX_CIRCLE

#include "triples.h"

namespace ePiX {

  class Circle {
  public:
    // defaults to unit circle in (x1,x2,0) plane
    Circle(const P& ctr=P(0,0,0), double rad=1, const P& perp=E_3);

    Circle(const P& ctr, const P& pt); // center, and point, normal = E_3
    Circle(const P& pt1, const P& pt2, const P& pt3); // three points

    Circle(bool); // malformed Circle for intersection operators

    P center() const;
    double radius() const;
    P perp() const;

    bool malformed() const;

    // translate
    Circle& shift(const P&);
    Circle& move_to(const P&);
    // scale radius by c
    Circle& scale(double c);

    void draw() const;

  private:
    P m_center;
    double m_radius;
    P m_perp; // unit normal, even if rad = 0

    bool m_malformed;
  }; // end of Circle class

  // global object-drawing commands
  void circle(const P& ctr=P(0,0,0), double rad=1, const P& perp=E_3);

  void circle(const P& ctr, const P& pt); // center, and point, normal = E_3
  void circle(const P& pt1, const P& pt2, const P& pt3); // three points

} // end of namespace

#endif /* EPIX_CIRCLE */
