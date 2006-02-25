/* 
 *  Visibility.h -- Auxiliary function for visibility tests
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: July 10, 2005
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005
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

#ifndef EPIX2_VISIBILITY
#define EPIX2_VISIBILITY

#include "Point.h"

namespace ePiX2 {

  // true iff q lies on Seg(p1, p2)
  bool is_between(const Point q, const Point p1, const Point p2);

  // positive iff q1, q2 on the same side of the Plane(p1, p2, p3)
  double on_same_side(const Point p1, const Point p2, const Point p3,
		    const Point q1, const Point q2);

  // true iff Seg(q1,q2) crosses Seg(p1, p2), viewed from O
  bool seems_to_cross(const Point O, const Point p1, const Point p2, 
		      const Point q1, const Point q2);

  // true iff Seg(q1,q2) goes behind Seg(p1, p2), viewed from O
  bool goes_behind(const Point O, const Point p1, const Point p2, 
		   const Point q1, const Point q2);

} /* end of namespace */

#endif /* EPIX2_VISIBILITY */
