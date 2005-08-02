/***
 ***  Sphere.h -- epix2::Sphere class
 ***
 *** This file is part of ePiX, a preprocessor for creating high-quality 
 *** line figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: July 23, 2005
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

#ifndef EPIX2_SPHERE
#define EPIX2_SPHERE

#include <iostream>
#include "Point.h"
#include "Vector.h"
#include "Object.h"

namespace ePiX2 {

  /* * * Sphere.h * * */

  class Sphere : public Shape {

  public:

    Sphere(const Point& ctr=Origin, double rad=1);

    Sphere(const Point& ctr, double rad, const Basis& orient);

    // translation
    Sphere& operator+= (const Vector&);
    void move_to(const Point&);

    // scale
    Sphere& operator*= (const double);
    void scale (const double arg);

    void rotate(const double angle, const Vector& axis);

    void shatter(Picture& world, int n1, int n2) const;
    void shatter(Picture& world) const { shatter(world, 16, 16); }

  private:

    Point center;
    double radius;
    Basis my_orientation;

  }; // end of class Sphere

} /* end of namespace */

#endif /* EPIX2_SPHERE */
