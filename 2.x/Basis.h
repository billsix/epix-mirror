/*** 
 ***  Basis.h -- epix2::Basis class
 ***
 *** This file is part of ePiX, a preprocessor for creating high-quality 
 *** line figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: July 31, 2005
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
 ***
 *** A Basis is guaranteed to be orthonormal.  
 *** This file provides:
 ***  - the Basis class
 ***  - translation, rotation, and reflection operators
 ***/

#ifndef EPIX2_BASIS
#define EPIX2_BASIS

#include "Point.h"
#include "Vector.h"

namespace ePiX2 {

  /* * * Basis.h * * */

  class Basis {
  private:
    Point location;
    // orthonormal triple
    Vector basis1;
    Vector basis2;
    Vector basis3;

  public:
    Basis() : location(), basis1(E_1()), basis2(E_2()), basis3(E_3()) {}

    // Gram-Schmidt {arg1, arg2, arg3} -> {sea, sky, eye}, with arg3 || eye
    Basis(Point loc, Vector arg1, Vector arg2, Vector arg3);

    // basis elements
    Vector sea(void) const { return basis1; }
    Vector sky(void) const { return basis2; }
    Vector eye(void) const { return basis3; }

    void rotate_sea(const double angle);
    void rotate_sky(const double angle);
    void rotate_eye(const double angle);

    void rotate(const double angle, const Vector& axis=E_3());
    void reflect(const Vector& axis=E_3());

    Point here(void) const { return location; }
    // coordinates of arg with respect to us
    Point coords(const Point arg) const;

    void move_to(const Point& arg)
      {
	location = arg;
	basis1.move_to(arg);
	basis2.move_to(arg);
	basis3.move_to(arg);
      }
  }; // end of class Basis

} /* end of namespace */

#endif /* EPIX2_BASIS */
