/* 
 *  plane.h -- ePiX::plane class
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc14
 * Last Change: July 26, 2004
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004
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
 *   - equality, parallelity test
 *   - plane*plane (draw line of intersection)
 *   - plane*sphere (circle of intersection)
 */

#ifndef EPIX_PLANE
#define EPIX_PLANE

#include "globals.h"
#include "triples.h"
#include "sphere.h"
#include "circle.h"

namespace ePiX {

  class plane
    {
    private:
      P pt;
      P N; // UNIT normal

    public:
      plane(P point=P(0,0,0), P normal=E_3)
	{ 
	  double temp=norm(normal);
	  if (temp < EPIX_EPSILON)
	    throw constructor_error(MALFORMED);

	  pt = point;
	  N = (1.0/temp)*normal;
	}

      plane(P p1, P p2, P p3);

      bool parallel_to (const plane& arg)
	{ return (norm((this->N)*arg.N)<EPIX_EPSILON); }

      bool operator== (const plane& arg)
	{
	  // normals parallel and arg.pt lies in *this
	  return ( (*this).parallel_to(arg) &&
		   (((this->pt) - arg.pt)|(this->N)) < EPIX_EPSILON);
	}

      // return circle of intersection
      circle operator* (const sphere& S) const;
      // draw Line
      void  operator* (const plane P1);

      // draw lines of intersection of *this with clip box faces
      void draw();

  }; /* end of plane class */

  inline circle operator* (const sphere& S, const plane& P) { return P*S; }

  inline void draw(plane arg) { arg.draw(); }

} /* end of namespace */

#endif /* EPIX_PLANE */
