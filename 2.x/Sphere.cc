/* 
 *  Sphere.h -- epix2::Sphere class
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
 *     - The Sphere class (derived from Shape) and operators
 */

#include <vector>

#include "Functions.h"
#include "Edge.h"
#include "Hiding.h"
#include "Basis.h"
#include "Object.h"
#include "Visibility.h"
#include "Quad.h"
#include "Triangle.h"
#include "Sphere.h"

namespace ePiX2 {

  Sphere::Sphere(const Point& ctr, double rad, int n1, int n2)
    : center(ctr), radius(rad), longitudes(n1), latitudes(n2)
  {
    // quasi-sane minimum values
    longitudes = (int)max(3, abs(longitudes));
    latitudes  = (int)max(2, abs(latitudes));
  }

  bool Sphere::hides(const Point vpt, const Point X)
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


  void Sphere::shatter(void)
  {
    if (the_scale < EPIX2_EPSILON)
      return;

    // else
    closed_oriented=true;
    Point vertices[longitudes+1][latitudes+1];

    double rad=radius*recip(the_scale);

    Vector E1=rad*the_orient.sea();
    Vector E2=rad*the_orient.sky();
    Vector E3=rad*the_orient.eye();

    double th=0, phi=-M_PI_2, dt=2*M_PI/longitudes, dphi=M_PI/latitudes;

    // calculate vertices; N.B. boundary conditions
    for (int j=0; j<=latitudes; ++j)
      {
	phi = -M_PI_2 + j*dphi;
	th=0;
	for (int i=0; i<=longitudes; ++i)
	  {
	    th = i*dt;
	    vertices[i][j] = center + 
	      (cos(phi)*(cos(th)*E1 + sin(th)*E2) + (sin(phi)*E3));
	  }
      }

    // compute facets; convex -> no cutting
    Vector temp_N;
    Point v00, v10, v11, v01;

    for (int j=0; j<latitudes; ++j)
      for (int i=0; i<longitudes; ++i)
	{
	  Shard face;

	  v00 = vertices[i][j],     v10 = vertices[i+1][j];
	  v11 = vertices[i+1][j+1], v01 = vertices[i][j+1];

	  // N.B. Normal arbitrarily located at v00
	  temp_N=(v10-v00)*(v01-v00);
	  temp_N *= recip(norm(temp_N));
	  face.set_normal(temp_N);

	  Edge e0(v00, v10, get_line_color());
	  Edge e1(v10, v11, get_line_color());
	  Edge e2(v11, v01, get_line_color());
	  Edge e3(v01, v00, get_line_color());

	  if (j == 0) // at south pole
	    {
	      face.add_edge(e1);
	      face.add_edge(e2);
	      face.add_edge(e3);
	    }

	  else if (j == latitudes-1) // north pole
	    {
	      face.add_edge(e0);
	      face.add_edge(e1);
	      face.add_edge(e3);
	    }

	  else
	    {
	      face.add_edge(e0);
	      face.add_edge(e1);
	      face.add_edge(e2);
	      face.add_edge(e3);
	    }

	  face.set_solid(true);
	  face.set_line_color(get_line_color());
	  face.set_fill_color(get_fill_color());

	  fragments.push_back(face);
	}
  } // end of Sphere::shatter

} /* end of namespace */
