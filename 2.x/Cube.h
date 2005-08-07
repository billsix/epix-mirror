/***
 ***  Cube.h -- epix2::Cube class
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: August 06, 2005
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

#ifndef EPIX2_CUBE
#define EPIX2_CUBE

#include "Point.h"
#include "Object.h"

namespace ePiX2 {

  /* * * Cube.h * * */

  class Cube : public Shape {

  public:

    Cube(double s1, double s2, double s3) 
      : side1(s1), side2(s2), side3(s3), solid(true) { }

    Cube(double s) : side1(s), side2(s), side3(s), solid(true) { }

    Cube(void) : side1(1), side2(1), side3(1), solid(true) { }

    Cube (const Point& arg1, const Point& arg2);

    void skeleton(bool T=true) { solid = !T; }

    bool hides(const Point vpt, const Point X);

    void shatter(void);

  private:

    double side1, side2, side3;

    bool solid;
  }; // end of class Cube

} /* end of namespace */

#endif /* EPIX2_CUBE */
