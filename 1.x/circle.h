/* 
 * circle.h -- ePiX::circle class
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc9
 * Last Change: June 23, 2004
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
 *   This file provides:
 *
 *   The circle class (center, radius, unit normal) and operators. See
 *   function declarations for list of exceptions thrown
 *
 *    - circle() (defaults to origin, unit radius, in (x1,x2,0)-plane
 *    - circle(ctr, pt) (parallel to (x1,x2,0)-plane, center ctr, through pt)
 *    - circle(pt1, pt2, pt3) (arbitrary non-collinear points)
 *    - center(), radius(), perp()
 *    - circle += P (translate by <P>)
 *    - circle *= double (scale by <double>)
 *    - draw() (ePiX ellipse)
 *    - circle*segment, circle*circle (segment def'd by pts of intersection)
 */


#ifndef EPIX_CIRCLE
#define EPIX_CIRCLE

#include "globals.h"
#include "triples.h"
#include "segment.h"

namespace ePiX {

  class circle 
    {
    private:
      P ctr;
      double rad;
      P perp_to; // *unit* normal, even if rad = 0

    public:
      // defaults to unit circle in (x1,x2,0) plane
      circle(P arg1=P(0,0,0), double arg2=1, P arg3=E_3) 
	{
	  double temp=norm(arg3);
	  if (temp < EPIX_EPSILON)
	    throw constructor_error(MALFORMED);

	  else
	    {
	      ctr = arg1;
	      rad = arg2; 
	      perp_to = (1/temp)*arg3;
	    }
	}

      circle(P ctr, P pt); // center, and point, normal = E_3
      circle(P pt1, P pt2, P pt3); // three points

      P center() const { return ctr; }
      double radius() const { return rad; }
      P perp()   const { return perp_to; }

      // translation
      circle& operator += (const P& arg)
	{
	  ctr += arg;

	  return *this;
	}

      // scaling
      circle& operator *= (const double& arg)
	{
	  rad *= arg;

	  return *this;
	}

      // circle
      void draw();

    }; /* end of circle class */

  // intersection operators
  /*
   * Throws the following exceptions:
   *   NON_COPLANAR if segment, circle are not coplanar
   *   SEPARATED if coplanar but line defined by segment misses circle
   *   TANGENT if coplanar but line defined by segment tangent to circle
   */
  segment& operator * (const segment&, const circle&);

  /*
   * Throws the following exceptions:
   *   TANGENT if circles are tangent
   *   COINCIDENT if circles are identical
   *   NOT_COPLANAR if circles not coplanar
   *   SEPARATED if disjoint, neither contained in the other
   *   CONCENTRIC if disjoint, one contained in other (not nec same ctr)
   */
  segment& operator * (const circle&, const circle&);

  inline segment operator * (const circle& arg_circle, const segment& arg_seg)
    { return arg_seg*arg_circle; }

  // affine operations: translation by a triple...
  inline circle& operator + (const P& displace, const circle& circ)
    { 
      circle temp = circ;
      return temp += displace; 
    }
  inline circle& operator + (const circle& circ, const P& displace)
    { 
      circle temp = circ;
      return temp += displace; 
    }

  // and scaling by a double
  inline circle& operator * (const double& c, const circle& circ)
    { 
      circle temp = circ;
      return temp *= c; 
    }
  inline circle& operator * (const circle& circ, const double& c)
    { 
      circle temp = circ;
      return temp *= c; 
    }

} /* end of namespace */

#endif /* EPIX_CIRCLE */
