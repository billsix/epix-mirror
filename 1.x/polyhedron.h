/* 
 *  polyhedron.h -- ePiX:: convex polyhedron class
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.4
 * Last Change: March 13, 2005
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
 *  This file provides:
 *
 *  The one_skel (topological 1-skeleton) class (<vector> of segments) 
 *  and operators:
 *    transform() // apply mapping to each vertex
 *    section()   // draw planar cross section
 */

#ifndef EPIX_POLYHEDRON
#define EPIX_POLYHEDRON

#include <vector>

#include "triples.h"
#include "segment.h"
#include "plane.h"

namespace ePiX {

  class one_skel {
  private:
    // std::vector<P> vertices;
    std::vector<segment> edges;

  public:
    one_skel(void);
    one_skel(unsigned int num_edges ...); // list of edges

    one_skel(const one_skel& old);

    //    void add_vertex(const P arg); // add vertex, form convex hull
    void add_edge(const segment arg) { edges.push_back(arg); }

    void transform(P Phi(P)); // apply Phi to each vertex
    // void draw();
    void section(const plane& knife);
  }; // end of class one_skel

} /* end of namespace */

#endif /* EPIX_POLYHEDRON */
