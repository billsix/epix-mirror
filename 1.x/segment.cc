/* 
 * segment.cc -- ePiX::segment class and mathematical operators
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc12
 * Last Change: July 04, 2004
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

#include <iostream>

#include "globals.h"
#include "triples.h"
#include "cropping.h"
// #include "path.h"
#include "curves.h"
#include "segment.h"

namespace ePiX {

  void segment::draw() { line(this->endpt1, this->endpt2); }

  P& operator * (const segment& seg1, const segment& seg2)
  {
    P p1 = seg1.end1();
    P p2 = seg1.end2();
    P p3 = seg2.end1();
    P p4 = seg2.end2();

    P dir1 = p2 - p1;
    P dir2 = p4 - p3;
    P normal = dir1*dir2; // cross product
    double normal_length = norm(normal);

    if (fabs( ((p2 - p1)*(p3 - p1)) | (p4 - p1) ) > EPIX_EPSILON)
      throw join_error(NON_COPLANAR);

    else if ( normal_length < EPIX_EPSILON)
      throw join_error(PARALLEL);

    else
      {
	P unit_normal = (1.0/normal_length)*normal;
	P dir3 = dir2*unit_normal;

	// note: return value may not lie on either segment
	double s = (dir3|(p3-p1))/(dir3|dir1);
	return p1 += s*dir1;
      }
  }

} /* end of namespace */
