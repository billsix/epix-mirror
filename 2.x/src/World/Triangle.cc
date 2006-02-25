/* 
 * Triangle.cc -- epix2::Triangle class
 *
 * This file is part of ePiX, a program for creating high-quality 
 * figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: January 29, 2006
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
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

#include "Functions.h"
#include "Point.h"
#include "Visibility.h"
#include "Vector.h"
#include "Object.h"
#include "Triangle.h"

namespace ePiX2 {

  Triangle::Triangle(const Point& v1, const Point& v2, const Point& v3)
    : vtx1(v1), vtx2(v2), vtx3(v3)
  {
    Point ctr = vtx1 + (1.0/3)*((vtx2-vtx1) + (vtx3-vtx1));
    Vector N=(vtx1-ctr)*(vtx2-ctr);
    N *= recip(norm(N)); // normalize, returning (0,0,0) if N=0

    // Allocate
    Plate* face = new Plate(N);

    face->add_edge(vtx1, vtx2);
    face->add_edge(vtx2, vtx3);
    face->add_edge(vtx3, vtx1);

    add_shard(face);
  }

  Triangle* Triangle::clone(void) const
  {
    return new Triangle(*this);
  }

  bool Triangle::hides(const Point vpt, const Point X) const
  {
    return ( (!on_same_side(vtx1, vtx2, vtx3, vpt, X)) &&
	       on_same_side(vtx1, vtx2, vpt, X,  vtx3) &&
               on_same_side(vtx2, vtx3, vpt, X,  vtx1) &&
               on_same_side(vtx3, vtx1, vpt, X,  vtx2) );
  }


  /*
  void Triangle::shatter(void)
  {
    Point V1 = coords(vtx1);
    Point V2 = coords(vtx2);
    Point V3 = coords(vtx3);

    Point ctr = V1 + (1.0/3)*((V2-V1) + (V3-V1));
    Vector N=(V1-ctr)*(V2-ctr);
    N *= recip(norm(N)); // normalize, returning (0,0,0) if N=0

    // Allocate
    Plate* face = new Plate(N);

    face->add_edge(V1, V2);
    face->add_edge(V2, V3);
    face->add_edge(V3, V1);

    add_shard(face);
  } // end of Triangle::make_shard_list
  */

} /* end of namespace */


