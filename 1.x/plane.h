/* 
 *  plane.h -- ePiX::plane class
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

/*
 *  This file provides:
 *
 *  The plane class (point, normal),
 *  and operators:
 *   - height function and friends
 *   - equality, parallelity test
 *   - plane*segment (point of intersection)
 *   - plane*plane (draw line of intersection)
 *   - plane*sphere (circle of intersection)
 *
 */
#ifndef EPIX_PLANE
#define EPIX_PLANE

#include <vector>

#include "globals.h"
#include "triples.h"

namespace ePiX {

  class circle;
  class segment;
  class sphere;

  class plane {
  public:
    plane(const P& point=P(0,0,0), const P& normal=E_3);
    plane(const P&, const P&, const P&);

    P normal() const;
    plane& reverse();
    plane& operator+= (const P&);

    // normal component of arg
    double height(const P&) const;
    bool contains(const P&) const;

    // pts separated by us, or we contain at least one point
    bool separates(const P&, const P&) const;
    bool parallel_to (const plane&) const;
    bool operator== (const plane&) const;

    // return circle of intersection
    circle operator* (const sphere&) const;
    // draw Line
    void  operator* (const plane&) const;

    // draw lines of intersection of *this with clip box faces
    void draw() const;

  private:
    P m_pt;
    P m_perp; // UNIT normal
  }; // end of plane class

  circle operator* (const sphere&, const plane&);

  P operator* (const plane&, const segment&);
  P operator* (const segment&, const plane&);

} // end of namespace

#endif /* EPIX_PLANE */
