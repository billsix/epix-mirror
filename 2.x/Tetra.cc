/* 
 * Tetra.h -- epix2::Tetrahedron class
 *
 * This file is part of ePiX, a program for creating high-quality 
 * figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: August 08, 2005
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
 *     - The Tetrahedron class (derived from Object) and operators
 */

#include <vector>

#include "Functions.h"
#include "Edge.h"
#include "Point.h"
#include "Basis.h"
#include "Object.h"
#include "Hiding.h"
#include "Triangle.h"
#include "Tetra.h"

namespace ePiX2 {

  Tetrahedron::Tetrahedron(const Point& arg0, const Point& arg1,
			   const Point& arg2, const Point& arg3)
    : vtx0(arg0), vtx1(arg1), vtx2(arg2), vtx3(arg3)
  {
    if (((vtx3-vtx0)|((vtx1-vtx0)*(vtx2-vtx0))) < 0) // wrong orientation
      {
	vtx2=arg3;
	vtx3=arg2;
      }
  }

  Tetrahedron::Tetrahedron(double s)
  {
    double ht=1.0/sqrt(8);
    vtx0 = Point(0.5, 0,  ht);
    vtx1 = Point(-0.5, 0, ht);
    vtx2 = Point(0, 0.5, -ht);
    vtx3 = Point(0,-0.5, -ht);
  }


  void Tetrahedron::shatter(void)
  {
    closed_oriented=true;

    // vertices

    Triangle f1(vtx0, vtx2, vtx1);
    Triangle f2(vtx0, vtx1, vtx3);
    Triangle f3(vtx0, vtx3, vtx2);
    Triangle f4(vtx1, vtx2, vtx3);

    (*this) << f1 << f2 << f3 << f4;

    std::list<Object*>::iterator faces;

    for (faces=parts.begin(); faces!=parts.end(); ++faces)
      {
	(*faces)->skeleton(!solid);
	(*faces)->set_line_color(get_line_color());
	(*faces)->set_fill_color(get_fill_color());
      }

    this->Object::shatter();

  } // end of Tetrahedron::shatter

} /* end of namespace */
