/* 
 * Position.h -- Cartesian locations in object space
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
 * A Position is a raw Cartesian location (carrying no typographical data)
 * in 3-dimensional space.  This file provides:
 *     - The Position class, and Cartesian constructors
 *     - Projection mappings pr[i], pr[ij] (i,j = 1,2,3)
 *     - (In)equality operators
 */

#ifndef _EPIX3d_POSITION
#define _EPIX3d_POSITION

#include <cstdlib>
#include <math.h>

#include "constants.h"

namespace ePiX3d {

  class Position {

  private:
    // coordinates
    double X1, X2, X3;

  public:  

    // constructor (in Cartesian coordinates)
    Position(double x1=0, double x2=0, double x3=0) 
      { 
	X1 = x1; X2 = x2; X3 = x3; 
      }

    // Cartesian coordinate functions
    double x1() const { return X1; }
    double x2() const { return X2; }
    double x3() const { return X3; }

  }; // end of class Position
  
  // alternate notation for coordinates
  inline double x1(Position arg) { return arg.x1(); }
  inline double x2(Position arg) { return arg.x2(); }
  inline double x3(Position arg) { return arg.x3(); }

  //  projection to Cartesian coordinate axes
  inline Position pr1(Position arg) { return Position(arg.x1(), 0, 0); }
  inline Position pr2(Position arg) { return Position(0, arg.x2(), 0); }
  inline Position pr3(Position arg) { return Position(0, 0, arg.x3()); }

  // to Cartesian coordinate planes
  inline Position pr12(Position arg) { return Position(arg.x1(), arg.x2(), 0);}
  inline Position pr13(Position arg) { return Position(arg.x1(), 0, arg.x3());}
  inline Position pr23(Position arg) { return Position(0, arg.x2(), arg.x3());}

  // (in)equality
  inline bool operator == (Position arg1, Position arg2)
  {
    return ((arg1.x1() == arg2.x1()) && 
	    (arg1.x2() == arg2.x2()) && 
	    (arg1.x3() == arg2.x3()));
  }

  inline bool operator != (Position arg1, Position arg2)
  {
    return ((arg1.x1() != arg2.x1()) || 
	    (arg1.x2() != arg2.x2()) ||
	    (arg1.x3() != arg2.x3()));
  }

  // ePiX's default clip mask (others "and"-ed with this one)
  inline bool is_finite(Position arg)
  {
    return ((fabs(arg.x1()) < EPIX_INFTY) &&
	    (fabs(arg.x2()) < EPIX_INFTY) &&
	    (fabs(arg.x3()) < EPIX_INFTY));
  }

} /* end of namespace ePiX3d */

#endif /* _EPIX_POSITION */
