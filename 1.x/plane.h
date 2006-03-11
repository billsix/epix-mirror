/* 
 *  plane.h -- ePiX::plane class
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.6
 * Last Change: March 06, 2006
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
#include "functions.h"
#include "sphere.h"
#include "circle.h"

namespace ePiX {

  class plane
    {
    public:
      plane(const P& point=P(0,0,0), const P& normal=E_3);

      plane(const P& p1, const P& p2, const P& p3);

      P normal() const { return N; }

      plane& reverse(void) 
	{ 
	  N *= -1;
	  return *this;
	}

      plane& operator += (const P& arg)
        {
          pt += arg;
          return *this;
        }

      // normal component of arg
      double height(const P& arg) const;
      bool contains(const P& arg) const;

      // pts separated by us, or we contain at least one point
      bool separates(const P arg1, const P arg2) const;
      bool parallel_to (const plane& arg) const;
      bool operator== (const plane& arg) const;

      // return circle of intersection
      circle operator* (const sphere& S) const;
      // draw Line
      void  operator* (const plane P1) const;

      // draw lines of intersection of *this with clip box faces
      void draw() const;

    private:
      P pt;
      P N; // UNIT normal

  }; /* end of plane class */

  inline circle operator* (const sphere& S, const plane& P) { return P*S; }

  inline void draw(plane arg) { arg.draw(); }

  P operator* (const plane knife, const segment seg);
  inline P operator* (const segment seg, const plane knife)
    {
      return knife*seg;
    }

} /* end of namespace */

#endif /* EPIX_PLANE */
