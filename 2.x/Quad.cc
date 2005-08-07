/* 
 *  Quad.h -- epix2::Quad class
 *
 * This file is part of ePiX, a program for creating high-quality 
 * figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: August 01, 2005
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
 *     - The Quad class (derived from Object) and operators
 */

#include "Functions.h"
#include "Edge.h"
#include "Hiding.h"
//#include "Visibility.h"
#include "Quad.h"

namespace ePiX2 {

  bool Quad::hides(const Point vpt, const Point X)
  {
    return false; // TO DO :)
    /*
    return ( (!on_same_side(vtx1, vtx2, vtx3, vpt, X)) &&
	       on_same_side(vtx0, vtx1, vpt, X,  vtx2) &&
               on_same_side(vtx1, vtx2, vpt, X,  vtx3) &&
               on_same_side(vtx2, vtx3, vpt, X,  vtx0) &&
               on_same_side(vtx3, vtx1, vpt, X,  vtx2) );
    */
  }

  void Quad::shatter(void)
  {
    // compute transformed vertices
    Point V0=the_orient.coords(vtx0);
    Point V1=the_orient.coords(vtx1);
    Point V2=the_orient.coords(vtx2);
    Point V3=the_orient.coords(vtx3);


    Edge e0(V0, V1, get_line_color());
    Edge e1(V1, V2, get_line_color());
    Edge e2(V2, V3, get_line_color());
    Edge e3(V3, V0, get_line_color());

    Shard face;

    face.add_edge(e0);
    face.add_edge(e1);
    face.add_edge(e2);
    face.add_edge(e3);

    // N.B. Normal arbitrarily located at V0
    Vector N=(V1-V0)*(V3-V0);
    N *= recip(norm(N)); // normalize, returning (0,0,0) if N=0
    face.set_normal(N);

    face.set_solid(solid);
    face.set_line_color(get_line_color());
    face.set_fill_color(get_fill_color());

    fragments.push_back(face);

  } // end of Quad::shatter

} /* end of namespace */


