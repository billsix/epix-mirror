/* 
 * segment.h -- ePiX::segment class
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
 *   This file provides:
 *
 *   The segment class (unordered pair of points),
 *   and operators:
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

  class segment {
  public:
    segment() {};
    segment(const P&, const P&);

    // Segment ends are not distinguished geometrically,
    // so these functions should normally be used in tandem.
    P end1() const;
    P end2() const;

    // translate
    segment& operator += (const P&);

    P midpoint() const;

    void draw() const;

  private:
    P endpt1;
    P endpt2;
  }; // end of segment class


  // translate; "P + segment" form specifically omitted
  segment operator+ (const segment&, const P&);

  // intersect
  P operator* (const segment&, const segment&);

} // end of namespace
#endif /* EPIX_SEGMENT */
