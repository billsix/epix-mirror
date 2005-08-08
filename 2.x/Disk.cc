/* 
 *  Disk.h -- epix2::Disk class
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
 *     - The Disk class (derived from Object) and operators
 */

#include "Functions.h"
#include "Edge.h"
#include "Hiding.h"
#include "Basis.h"
#include "Object.h"
#include "Disk.h"

namespace ePiX2 {

  Disk Disk::Circle(const Point& ctr, double rad, const Vector& N)
  {
    return Disk(ctr, rad, N, false);
  }

  bool Disk::hides(const Point vpt, const Point X)
  {
    // Circle, or we're edge-on
    if (!solid || fabs(normal|(vpt-center)) < EPIX2_EPSILON)
      return false;

    else // behind plane and line(vpt,x) intersects disk
      return ((normal|(vpt-normal.tail()))*(normal|(X-normal.tail())) < 0 &&
	      norm(normal.perp_hits(vpt,X) - center) <= radius);
  }


  void Disk::shatter(void) 
  {
    const int N=80; // Magic number

    // compute normal in transformed coordinates
    Point new_center=the_orient.coords(center, the_scale);
    Point new_N_tip=the_orient.coords(normal.head(), the_scale);

    normal=new_N_tip - new_center; 
    normal *= recip(norm(normal));

    Vector V1, V2; // basis for disk plane

    if (norm(normal%E_3(center)) < 0.7) // within ~45 degrees of E_3
      V1 = E_2(center)*normal;

    else
      V1 = normal*E_3(center);

    V1 *= recip(norm(V1));
    V2 = normal*V1;

    // now that we have a basis, compute edges
    double th, dt=2*M_PI/N;

    Shard face;

    Point curr = center+radius*V1, next;

    for (int i=0; i<N; ++i)
      {
	th=(i+1)*dt;
	next = center+radius*(cos(th)*V1 + sin(th)*V2);

	face.add_edge(Edge(curr, next, get_line_color()));
	curr=next;
      }

    face.set_normal(normal);
    face.set_solid(solid);
    face.set_fill_color(get_fill_color());

    fragments.push_back(face);

  } // end of Disk::shatter

} /* end of namespace */


