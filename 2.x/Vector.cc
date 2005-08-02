/* 
 * Vector.cc -- ePiX2::Vector class and operations
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: August 01, 2005
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005
 * Andrew D. Hwang <ahwang@mathcs.holycross.edu>
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

#include "Constants.h"
#include "Errors.h"
#include "Edge.h"
#include "Vector.h"

namespace ePiX2 {

  // projection to N
  Point Point::operator& (const Vector N)
  {
    Vector v = (*this) - N.tail();
    return (v - v%N).head();
  }

  // projection to N perp
  Point Point::operator% (const Vector N)
  {
    Vector v = (*this) - N.tail();
    return (v%N).head();
  }

  double operator| (const Vector arg1, const Vector arg2)
  {
    if (arg1.basepoint != arg2.basepoint)
      epix_warning("Implicit translation in dot product");

    return 
      (arg1.Vx1)*(arg2.Vx1) + 
      (arg1.Vx2)*(arg2.Vx2) + 
      (arg1.Vx3)*(arg2.Vx3);
  }

  // Linear dependence
  bool lin_dependent (const Vector& v1, const Vector& v2)
  {
    return (norm(v1*v2) <= EPIX2_EPSILON*norm(v1)*norm(v2));
  }

  bool lin_dependent (const Vector& v1, const Vector& v2, const Vector& v3)
  {
    return (fabs((v1*v2)|v3) <= EPIX2_EPSILON*norm(v1)*norm(v2)*norm(v3));
  }


  // intersection of perp with E
  Point Vector::perp_hits(const Point& q1, const Point& q2) const
  {
    Vector dir=q2-q1;
    dir.move_to(basepoint);
    double t=-((q1-basepoint)|(*this))/(dir|(*this));

    return q1 + t*dir;
  }

  Point Vector::perp_hits(const Edge& E) const
  {
    return perp_hits(E.first, E.second);
  }

}; /* end of namespace ePiX2 */
