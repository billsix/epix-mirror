/* 
 *  Segment.h -- epix2::Segment class
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: August 01, 2005
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

/*
 *   This file provides:
 *     - The Segment class (derived from Shape) and operators
 */


#ifndef EPIX2_SEGMENT
#define EPIX2_SEGMENT

#include "Point.h"
#include "Object.h"

namespace ePiX2 {

  class Segment : public Shape {

  public:
    Segment(const Point arg=Origin) : vtx0(Origin), vtx1(arg) { }
    Segment(const Point arg0, const Point arg1) : vtx0(arg0), vtx1(arg1) { }

    Point midpoint(const double t=0.5) const;

    void shatter(void);

  private:
    Point vtx0;
    Point vtx1;

  }; // end of class Segment

} /* end of namespace */

#endif /* EPIX2_SEGMENT */


