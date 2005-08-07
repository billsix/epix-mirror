/* 
 *  Cube.h -- epix2::Cube class
 *
 * This file is part of ePiX, a program for creating high-quality 
 * figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: August 06, 2005
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
 *     - The Cube class (derived from Shape) and operators
 */

#include <vector>

#include "Functions.h"
#include "Edge.h"
#include "Hiding.h"
#include "Basis.h"
#include "Object.h"
#include "Visibility.h"
#include "Quad.h"
#include "Cube.h"

namespace ePiX2 {

  Cube::Cube(const Point& arg1, const Point& arg2) : solid(true)
  {
    side1=fabs(arg2.x1() - arg1.x1());
    side2=fabs(arg2.x2() - arg1.x2());
    side3=fabs(arg2.x3() - arg1.x3());

    Vector dX = midpoint(arg1, arg2) - Origin;
    (*this) += dX;
  }

  /*
  bool Cube::hides(const Point vpt, const Point X)
  {
    Vector dir_vpt = vpt - center;
    Vector dir_X   =   X - center;
    double dist_X   = norm(dir_X);
    double dist_vpt = norm(dir_vpt);

    if (dist_vpt <= radius) // vpt inside
      return (dist_X > radius);

    else
      return (dist_X < radius ||
	      ( (dir_vpt|dir_X) < 0 &&
		norm(dir_vpt.perp_hits(vpt,X) - center) <= radius));
  }
  */

  void Cube::shatter(void)
  {
    closed_oriented=true;

    // vertices
    Point O_nnn(-side1,-side2,-side3), O_pnn( side1,-side2,-side3);
    Point O_npn(-side1, side2,-side3), O_ppn( side1, side2,-side3);
    Point O_nnp(-side1,-side2, side3), O_pnp( side1,-side2, side3);
    Point O_npp(-side1, side2, side3), O_ppp( side1, side2, side3);

    // face centers, to set normals
    Point O_1a(-side1,     0,     0), O_1b( side1,     0,     0);
    Point O_2a(     0,-side2,     0), O_2b(     0, side2,     0);
    Point O_3a(     0,     0,-side3), O_3b(     0,     0, side3);


    Point v_nnn=the_orient.coords(O_nnn);
    Point v_pnn=the_orient.coords(O_pnn);

    Point v_npn=the_orient.coords(O_npn);
    Point v_ppn=the_orient.coords(O_ppn);

    Point v_nnp=the_orient.coords(O_nnp);
    Point v_pnp=the_orient.coords(O_pnp);

    Point v_npp=the_orient.coords(O_npp);
    Point v_ppp=the_orient.coords(O_ppp);

    // and outward-oriented faces
    Shard f1a; // (v_nnn, v_nnp, v_npp, v_npn)
    f1a.add_edge(Edge(v_nnn, v_nnp, get_line_color()));
    f1a.add_edge(Edge(v_nnp, v_npp, get_line_color()));
    f1a.add_edge(Edge(v_npp, v_npn, get_line_color()));
    f1a.add_edge(Edge(v_npn, v_nnn, get_line_color()));

    Shard f1b; // (v_ppp, v_pnp, v_pnn, v_ppn)
    f1b.add_edge(Edge(v_ppp, v_pnp, get_line_color()));
    f1b.add_edge(Edge(v_pnp, v_pnn, get_line_color()));
    f1b.add_edge(Edge(v_pnn, v_ppn, get_line_color()));
    f1b.add_edge(Edge(v_ppn, v_ppp, get_line_color()));


    Shard f2a; // (v_nnn, v_pnn, v_pnp, v_nnp)
    f2a.add_edge(Edge(v_nnn, v_pnn, get_line_color()));
    f2a.add_edge(Edge(v_pnn, v_pnp, get_line_color()));
    f2a.add_edge(Edge(v_pnp, v_nnp, get_line_color()));
    f2a.add_edge(Edge(v_nnp, v_nnn, get_line_color()));

    Shard f2b; // (v_ppp, v_ppn, v_npn, v_npp)
    f2b.add_edge(Edge(v_ppp, v_ppn, get_line_color()));
    f2b.add_edge(Edge(v_ppn, v_npn, get_line_color()));
    f2b.add_edge(Edge(v_npn, v_npp, get_line_color()));
    f2b.add_edge(Edge(v_npp, v_ppp, get_line_color()));

    Shard f3a; // (v_nnn, v_npn, v_ppn, v_pnn)
    f3a.add_edge(Edge(v_nnn, v_npn, get_line_color()));
    f3a.add_edge(Edge(v_npn, v_ppn, get_line_color()));
    f3a.add_edge(Edge(v_ppn, v_pnn, get_line_color()));
    f3a.add_edge(Edge(v_pnn, v_nnn, get_line_color()));

    Shard f3b; // (v_ppp, v_npp, v_nnp, v_pnp)
    f3b.add_edge(Edge(v_ppp, v_npp, get_line_color()));
    f3b.add_edge(Edge(v_npp, v_nnp, get_line_color()));
    f3b.add_edge(Edge(v_nnp, v_pnp, get_line_color()));
    f3b.add_edge(Edge(v_pnp, v_ppp, get_line_color()));


    f1a.set_normal(the_orient.coords(O_1a-E_1(O_1a))-the_orient.coords(O_1a));
    f1b.set_normal(the_orient.coords(O_1b+E_1(O_1b))-the_orient.coords(O_1b));

    f2a.set_normal(the_orient.coords(O_2a-E_2(O_2a))-the_orient.coords(O_2a));
    f2b.set_normal(the_orient.coords(O_2b+E_2(O_2b))-the_orient.coords(O_2b));

    f3a.set_normal(the_orient.coords(O_3a-E_3(O_3a))-the_orient.coords(O_3a));
    f3b.set_normal(the_orient.coords(O_3b+E_3(O_3b))-the_orient.coords(O_3b));

    fragments.push_back(f1a);
    fragments.push_back(f1b);

    fragments.push_back(f2a);
    fragments.push_back(f2b);

    fragments.push_back(f3a);
    fragments.push_back(f3b);

    std::list<Shard>::iterator face;

    for (face=fragments.begin(); face!=fragments.end(); ++face)
      {
	(*face).set_solid(solid);
	(*face).set_line_color(get_line_color());
	if (solid)
	  (*face).set_fill_color(get_fill_color());
      }

  } // end of Cube::shatter

} /* end of namespace */
