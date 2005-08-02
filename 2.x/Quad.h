/***
 ***  Quad.h -- epix2::Quad class
 ***
 *** This file is part of ePiX, a preprocessor for creating high-quality 
 *** line figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: July 28, 2005
 ***
 *** 
 *** Copyright (C) 2001, 2002, 2003, 2004, 2005
 *** Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
 *** Department of Mathematics and Computer Science
 *** College of the Holy Cross
 *** Worcester, MA, 01610-2395, USA
 ***
 ***
 *** ePiX is free software; you can redistribute it and/or modify it
 *** under the terms of the GNU General Public License as published by
 *** the Free Software Foundation; either version 2 of the License, or
 *** (at your option) any later version.
 ***
 *** ePiX is distributed in the hope that it will be useful, but WITHOUT
 *** ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *** or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 *** License for more details.
 ***
 *** You should have received a copy of the GNU General Public License
 *** along with ePiX; if not, write to the Free Software Foundation, Inc.,
 *** 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 ***
 ***/

#ifndef EPIX2_QUAD
#define EPIX2_QUAD

#include "Point.h"
#include "Basis.h"
#include "Object.h"

namespace ePiX2 {

  /* * * Quad.h * * */

  class Quad : public Shape {

  public:

    // TO DO: Test for coplanarity
    Quad(const Point& v0, const Point& v1, const Point& v2, const Point& v3)
      : vtx0(v0), vtx1(v1), vtx2(v2), vtx3(v3) { }

    bool hides(const Point vpt, const Point X);

    void shatter(void);

  private:

    Point vtx0;
    Point vtx1;
    Point vtx2;
    Point vtx3;

  }; // end of class Quad

} /* end of namespace */

#endif /* EPIX2_QUAD */
