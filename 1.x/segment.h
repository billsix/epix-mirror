/* 
 * segment.h -- ePiX::segment class
 * 
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc16
 * Last Change: August 24, 2004
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
 *   The segment class (unordered pair of points),
 *   and operators:
 *    - join() (alternative constructor)
 *    - endpt1(), endpt2() (no guaranteed order)
 *    - segment += P (translate by <P>)
 *    - draw() (ePiX line)
 *    - midpoint(); (also callable on pair of Ps)
 *    - segment*segment (pt of intersection of _lines_ determined by segments)
 */

#ifndef EPIX_SEGMENT
#define EPIX_SEGMENT

#include "globals.h"
#include "triples.h"

namespace ePiX {

  class segment
    {
    private:
      P endpt1;
      P endpt2;

    public:
      segment() {};
      segment(P p1, P p2) { endpt1 = p1; endpt2 = p2; }

      // Note: Ends of segment are not "ordered" meaningfully
      P end1() const { return endpt1; }
      P end2() const { return endpt2; }

      // translation
      segment& operator += (const P& arg)
	{
	  endpt1 += arg;
	  endpt2 += arg;

	  return *this;
	}

      P midpoint() { return 0.5*(endpt1 + endpt2); }

      // segment
      void draw();

    }; /* end of segment class */

  //  inline void draw(segment arg) { line(arg.end1(), arg.end2()); }

  // translation
  inline segment& operator + (const segment& seg, const P& arg)
    {
      segment temp=seg;
      return temp += arg;
    }

  inline segment& operator + (const P& arg, const segment& seg)
    {
      segment temp=seg;
      return temp += arg;
    }

  // intersection
  P& operator * (const segment& seg1, const segment& seg2);


  // alternative constructor
  inline segment join(P p1, P p2) { return segment(p1,p2); }

} /* end of namespace */

#endif /* EPIX_SEGMENT */
