/* 
 * Frame.h -- Orthonormal frames
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.9.0rc1
 * Last Change: August 27, 2003
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

/*
 * In geometry, a "frame" is a right-handed orthonormal basis, that is,
 * an ordered set of three mututally perpendicular unit vectors, oriented
 * according to the right-hand rule. A frame has nothing to do with
 * picture frames.
 *
 * This file provides:
 *   - Standard basis vectors (E_1, etc.), the Origin
 *   - the standard Cartesian Frame (based at the origin)
 *   - Functions for rotating a Frame about axes determined by elements
 *   - Functions to return elements (sea, sky, and eye)
 */

#ifndef _EPIX3d_FRAME
#define _EPIX3d_FRAME

#include "Position.h"
#include "Vector.h"
#include "functions.h"

namespace ePiX3d {

  // standard Cartesian frame at origin
  static const Position Origin = Position(0,0,0);

  const Vector E_1 = Vector(Origin,1,0,0);
  const Vector E_2 = Vector(Origin,0,1,0);
  const Vector E_3 = Vector(Origin,0,0,1);

  class Frame {
  private:
    Position base;
    // orthonormal triple
    Vector frame1;
    Vector frame2;
    Vector frame3;

  public:
    //constructor
    Frame(void) { base = Origin; frame1 = E_1; frame2 = E_2; frame3 = E_3; }

    // frame elements
    Vector sea() { return frame1; }
    Vector sky() { return frame2; }
    Vector eye() { return frame3; }

    // rotations about frame elements
    Frame rotate_frame1(double angle)
      {
	Vector temp2 = frame2;
	Vector temp3 = frame3;

	frame2 = 
	  (ePiX3d::cos(angle)*(temp2)) - (ePiX3d::sin(angle)*(temp3));
	frame3 = 
	  (ePiX3d::sin(angle)*(temp2)) + (ePiX3d::cos(angle)*(temp3));

	return *this;
      }


    Frame rotate_frame2(double angle)
      {
	Vector temp3 = frame3;
	Vector temp1 = frame1;

	frame3 = 
	  (ePiX3d::cos(angle)*(temp3)) - (ePiX3d::sin(angle)*(temp1));

	frame1 = 
	  (ePiX3d::sin(angle)*(temp3)) + (ePiX3d::cos(angle)*(temp1));

	return *this;
      }

    Frame rotate_frame3(double angle)
      {
	Vector temp1 = frame1;
	Vector temp2 = frame2;

	frame1 = 
	  (ePiX3d::cos(angle)*(temp1)) - (ePiX3d::sin(angle)*(temp2));

	frame2 = 
	  (ePiX3d::sin(angle)*(temp1)) + (ePiX3d::cos(angle)*(temp2));

	return *this;
      }

  }; // end of class Frame

  inline Vector sea(Frame arg) { return arg.sea(); }
  inline Vector sky(Frame arg) { return arg.sky(); }
  inline Vector eye(Frame arg) { return arg.eye(); }

} /* end of namespace */

#endif /* _EPIX3d_FRAME */
