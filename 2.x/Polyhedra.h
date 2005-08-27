/*** 
 ***  Polyhedra.h -- ePiX2 classes for convex polyhedra
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: August 17, 2005
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
 *** This file provides:
 ***
 ***   Polygon    - A *convex* planar polygon
 ***   Polyhedron - Vertex, edge, and face data
 ***
 ***/


#ifndef EPIX2_POLYHEDRA
#define EPIX2_POLYHEDRA

#include <list>
#include <vector>

#include "Enums.h"
#include "Color.h"
#include "Point.h"
#include "Edge.h"
#include "Vector.h"
#include "Object.h"

namespace ePiX2 {

  /* * * Polyhedra.h * * */

  class Knife {
    friend class Mesh_Edge;
    friend class Mesh_Face;
    friend class Polyhedron;

  private:
    Vector blade;
    Color paint;

  }; // end of class Knife


  class Polygon_Edge {
    friend class Polygon;

    Polygon_Edge(void) { }

    // endpoints and *orienting* (not outward) normal
    Polygon_Edge(const Point& p1, const Point& p2, const Vector& N);

    Point tail, head;
    Vector outward;
  }; // end of class Polygon_Edge


  // cf. Shard
  class Polygon : public Object {

  public:
    Polygon(const Vector& N=E_3(Origin));

    Polygon(const Point&, const Point&, const Point&);
    Polygon(const Point&, const Point&, const Point&, const Point&);

    Polygon(const Point&, const Point&, const Point&, 
	    const Point&, const Point&);

    Polygon(const Point&, const Point&, const Point&, 
	    const Point&, const Point&, const Point&);

    Polygon& clip_by(const Vector&);
    Polygon& add(const Point&); // add vertex, return new convex hull

    void shatter(void);

  private:

    Vector normal;
    std::list<Polygon_Edge> boundary;

  }; // end of class Polygon


  class Polyhedron : public Object {

  public:
    Polyhedron(void);

    Polyhedron& operator<< (const Polygon);

    //Polyhedron& clip_by(const Knife&);  // remove half-space pointed to by N
    Polyhedron& clip_by(const Vector&);  // remove half-space pointed to by N

    /*
      Polyhedron& carve_by(const Knife&); // half-space without cutting plane
      Polygon& slice_by(const Knife&); // planar slice
    */

    //    Polyhedron& add_vertex(const Point&);
    void shatter(void);

  private:

    std::list<Polygon>   faces;

  }; // end of Polyhedron

} /* end of namespace */

#endif /* EPIX2_POLYHEDRA */
