/* 
 * segment.cc -- ePiX::segment class and mathematical operators
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.10rc7
 * Last Change: April 18, 2003
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

#include <iostream>

#include "exceptions.h"
#include "segment.h"
#include "../lengths.h"

namespace ePiX {

  triple operator * (segment seg1, segment seg2)
  {
    triple p1 = seg1.end1();
    triple p2 = seg1.end2();
    triple p3 = seg2.end1();
    triple p4 = seg2.end2();

    triple dir1 = p2 - p1;
    triple dir2 = p4 - p3;
    triple normal = dir1*dir2; // cross product
    double normal_length = raw_len(normal);

    if (fabs( ((p2 - p1)*(p3 - p1)) | (p4 - p1) ) != 0)
      throw join_error(NON_COPLANAR);

    else if ( normal_length < EPIX_EPSILON)
      throw join_error(PARALLEL);

    else
      {
	triple unit_normal = (1.0/normal_length)*normal;
	triple dir3 = dir2*unit_normal;

	double s = (dir3|(p3-p1))/(dir3|dir1);

	if (s < 0 || 1 < s)
	  throw join_error(DISJOINT);

	else
	  return p1 + s*dir1;
      }
  }

} /* end of namespace */
