/***
 ***  Tetra.h -- epix2::Tetrahedron class
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: August 08, 2005
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

#ifndef EPIX2_TETRA
#define EPIX2_TETRA

#include "Point.h"
#include "Object.h"

namespace ePiX2 {

  /* * * Tetra.h * * */

  class Tetrahedron : public Object {

  public:

    Tetrahedron(double s=1);

    Tetrahedron (const Point&, const Point&, const Point&, const Point&);

    void shatter(void);

  private:
    Point vtx0, vtx1, vtx2, vtx3;

  }; // end of class Tetrahedron

} /* end of namespace */

#endif /* EPIX2_TETRA */
