/* 
 * circle.h -- ePiX::circle class and mathematical operators
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.10rc7
 * Last Change: April 17, 2003
 */

/* 
 * Copyright (C) 2001, 2002, 2003
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

#ifndef _EPIX_CIRCLE
#define _EPIX_CIRCLE

#include "../globals.h"
#include "../triples.h"
#include "segment.h"

namespace ePiX {

  class circle 
    {
    private:
      triple ctr;
      double rad;
      triple perp; // *unit* normal, even if rad = 0

    public:
      circle(triple center=P(0,0,0), double radius=1, triple normal = E_3)
	{
	  ctr = center;
	  rad = radius;
	  perp = normal;
	}
      circle(triple pt1, triple pt2, triple pt3); // three points

      triple center() { return ctr; }
      double radius() { return rad; }
      triple normal() { return perp; }
    }; /* end of circle class */

  // intersection operators
  /*
   * Throws the following exceptions:
   *   NON_COPLANAR if segment, circle are not coplanar
   *   SEPARATED if coplanar but line defined by segment misses circle
   *   TANGENT if coplanar but line defined by segment tangent to circle
   */   
  segment operator * (segment& arg_seg, circle& arg_circle);
  inline segment operator * (circle& arg_circle, segment& arg_seg)
    { return arg_seg*arg_circle; }

  /*
   * Throws the following exceptions:
   *   TANGENT if circles are tangent
   *   COINCIDENT if circles are identical
   *   NOT_COPLANAR if circles not coplanar
   *   SEPARATED if disjoint, neither contained in the other
   *   CONCENTRIC if disjoint, one contained in the other (not nec same ctr)
   */
  segment operator * (circle& arg1, circle& arg2);

  void draw(circle& arg);

} /* end of namespace */

#endif /* _EPIX_CIRCLE */
