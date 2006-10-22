/* 
 * segment.cc -- ePiX::segment class and mathematical operators
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.18
 * Last Change: October 21, 2006
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

#include <iostream>

#include "globals.h"
#include "errors.h"
#include "triples.h"

#include "curves.h"
#include "segment.h"

namespace ePiX {

  segment::segment(const P& p1, const P& p2)
    : endpt1(p1), endpt2(p2) { }

  P segment::end1() const
  {
    return endpt1;
  }
  P segment::end2() const
  {
    return endpt2;
  }

  // translate
  segment& segment::operator += (const P& arg)
  {
    endpt1 += arg;
    endpt2 += arg;

    return *this;
  }

  P segment::midpoint() const
  {
    return 0.5*(endpt1 + endpt2);
  }

  void segment::draw() const
  {
    line(endpt1, endpt2);
  }
  // end of class functions

  segment operator+ (const segment& seg, const P& arg)
  {
    segment temp(seg);
    return temp += arg;
  }

  // intersection
  P operator* (const segment& seg1, const segment& seg2)
  {
    P p1(seg1.end1());
    P dir1(seg1.end2() - p1);

    P p3(seg2.end1());
    P p4(seg2.end2());
    P dir2(p4 - p3);

    P normal(dir1*dir2); // cross product
    double normal_length(norm(normal));

    if (fabs( (dir1*(p3 - p1)) | (p4 - p1) ) > EPIX_EPSILON)
      throw join_error(NON_COPLANAR);

    else if ( normal_length < EPIX_EPSILON)
      throw join_error(PARALLEL);

    else
      {
	// normal lies in plane of segments, is perp to dir2
	normal *= dir2;

	// get t so that normal|(X - p3) = (normal|(p1 - p3 + t*dir1)) = 0.
	// note: X may not lie on either segment
	return p1 + ((normal|(p3-p1))/(normal|dir1))*dir1;
      }
  }
} // end of namespace
