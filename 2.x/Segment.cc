/* 
 *  Segment.cc -- epix2::Segment class
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
 *     - The Segment class (derived from Shape) and operators
 */

#include "Functions.h"
#include "Edge.h"
#include "Hiding.h"
#include "Basis.h"
#include "Object.h"
#include "Segment.h"

namespace ePiX2 {

  Point Segment::midpoint(const double t) const
  {
    return vtx0 + t*(vtx1-vtx0);
  }

  void Segment::shatter(void)
  {
    // compute transformed vertices
    Point V0=the_orient.coords(vtx0);
    Point V1=the_orient.coords(vtx1);

    Shard seg;

    seg.add_edge(Edge(V0, V1, get_line_color()));

    // N.B. Normal arbitrarily located at V0
    Vector N=(V1-V0);
    N *= recip(norm(N));
    if (norm(N%E_3(V0))< 0.7)
      N *= E_1(V0);
    else
      N *= E_3(V0);

    N *= recip(norm(N));
    seg.set_normal(N);

    seg.set_solid(false);
    seg.set_line_color(get_line_color());

    fragments.push_back(seg);

  } // end of Segment::shatter

} /* end of namespace */


