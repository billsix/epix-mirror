/* 
 *  Sphere.h -- epix2::Sphere class
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: July 21, 2005
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

#include "Point.h"
#include "Picture.h"
#include "Hiding.h"
#include "Quad.h"
#include "Sphere.h"

namespace ePiX2 {

  Sphere::Sphere(const Point& ctr, double rad)
    : center(ctr), radius(rad)
  {
    my_orientation=Basis(ctr, E_1(ctr), E_2(ctr), E_3(ctr));
  }

  Sphere::Sphere(const Point& ctr, double rad, const Basis& orient)
    : center(ctr), radius(rad), my_orientation(orient) { }


  Sphere& Sphere::operator+= (const Vector& dX)
  {
    center = center+dX;
    my_orientation.move_to(center);

    return *this;
  }

  void Sphere::move_to(const Point& ctr) // place barycenter at ctr
  {
    center=ctr;
    my_orientation.move_to(center);
  }

  Sphere& Sphere::operator*= (const double c)  // scale
  {
    radius *= c;
    return *this;
  }

  void Sphere::scale (const double c)
  {
    radius *= c;
  }

  void Sphere::rotate(const double angle, const Vector& axis)
  {
    my_orientation.rotate(angle, axis); // significant for shattering
    center = my_orientation.here();
  }

  void Sphere::shatter(Picture& world, int num1, int num2) const 
  {
    int n1 = 3 < abs(num1) ? abs(num1) : 3; // longitudes
    int n2 = 2 < abs(num2) ? abs(num2) : 2; // latitudes

    Point vertices[n1+1][n2+1];

    Vector E1=my_orientation.sea();
    Vector E2=my_orientation.sky();
    Vector E3=my_orientation.eye();

    double th=0, phi=-M_PI_2, dt=2*M_PI/n1, dphi=M_PI/n2;

    // calculate vertices; N.B. boundary conditions
    for (int j=0; j<=n2; ++j, phi += dphi)
      {
	th=0;
	for (int i=0; i<=n1; ++i, th += dt)
	  vertices[i][j] = center + 
	    radius*(cos(phi)*(cos(th)*E1 + sin(th)*E2) + (sin(phi)*E3));
      }

    // compute facets; convex -> no cutting
    //    double tmp1, tmp2;
    for (int j=0; j<n2; ++j, phi += dphi)
      for (int i=0; i<n1; ++i, th += dt)
	{
	  Quad facet = Quad(vertices[i][j],     vertices[i+1][j],
			    vertices[i+1][j+1], vertices[i][j+1]);
	  facet.shatter(world);
	}
    //	  world.add_shard(facet);

  } // end of Sphere::shatter

} /* end of namespace */


/*
	{
	  tmp1=(i+0.5)*dt, tmp2=(j+0.5)*dphi;
	  Point temp = center + 
	    radius*(cos(tmp2)*(cos(tmp1)*E1+sin(tmp1)*E2)+(sin(tmp2)*E3));

	  if (((temp-center).move_to(temp)|world.camera.viewpt()-temp)>0)
	    {
	      Quad facet = Quad(vertices[i][j],     vertices[i+1][j],
				vertices[i+1][j+1], vertices[i][j+1]);
	      facet.shatter(world);
	    }
	  //	  world.add_shard(facet);
	}

*/
