/* 
 * Mapping.cc -- epix2::Mapping::operator()
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: July 13, 2005
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

#include "Point.h"
#include "Mapping.h"

namespace ePiX2 {

  static Point Ident(Point arg) { return arg; }
  
  Mapping Mapping::Identity = Mapping(Ident);

  Point Mapping::operator() (const Point& arg)
  {
    Point value; // put return value here
    Point temp = arg;

    // precompose, avoiding infinite recursion
    if (this != &Mapping::Identity)
      temp = (*inner)(arg);

    // extract coordinates
    double x=temp.x1(), y=temp.x2();

    switch(flag) {

    case map_1_1:
      value = Point(x, f11(x), 0);
      break;

    case map_2_1:
      value = Point(x, y, f21(x,y));
      break;

    case map_1_3:
      value = f13(x);
      break;

    case map_2_3:
      value = f23(x,y);
      break;

    case map_3_3:
      value = f33(temp);
      break;
    }

    return value;

  } // end of evaluation operator

  // composition
  Mapping Mapping::operator() (Mapping& precomp)
  {
    if (this == &Mapping::Identity) // avoid infinite recursion
      return precomp;

    else
      {
	Mapping outer = (*this);
	outer.inner = &precomp;

	return outer;
      }
  } // end of composition operator

} // end of namespace
