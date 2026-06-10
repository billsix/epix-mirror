/*
 *  plane.h -- ePiX::Plane class
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.17
 * Last Change: September 13, 2007
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

#ifndef EPIX_PLANE
#define EPIX_PLANE

#include "triples.h"

namespace ePiX {

class Circle;
class Plane;
class Segment;
class Sphere;

class Plane {
 public:
  Plane(const P& pt = P(0, 0, 0), const P& perp = E_3);
  Plane(const P&, const P&, const P&);

  P pt() const;
  P perp() const;
  bool malformed() const;

  // affine operations
  Plane& shift(const P&);
  Plane& move_to(const P&);

  void draw() const;

 private:
  P m_pt;
  P m_perp;  // unit normal

  bool m_malformed;
};  // end of Plane class

void plane(const P& pt = P(0, 0, 0), const P& perp = E_3);
void plane(const P&, const P&, const P&);

}  // namespace ePiX

#endif /* EPIX_PLANE */
