/* 
 * sphere.h -- ePiX::sphere class
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.7
 * Last Change: March 06, 2006
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

/*
 *   This file provides:
 *
 *   The sphere class (center, radius),
 *   and operators:
 *    - sphere += P (translate by <P>)
 *    - sphere *= double (scale about center)
 *    - draw() (ePiX line)
 *    - center();
 *    - radius();
 *    - sphere*sphere (circle of intersection)
 *
 *   Hiding test, latitude and longitudes on a sphere.
 */

#ifndef EPIX_SPHERE
#define EPIX_SPHERE

#include "globals.h"
#include "triples.h"
#include "camera.h"
#include "circle.h"

namespace ePiX {

  class sphere
    {
    private:
      P ctr;
      double rad;

    public:
      sphere(const P& p1=P(0,0,0), double r=1) : ctr(p1), rad(r) { }
      sphere(const P& p1, const P& p2);

      P center() const { return ctr; }
      double radius() const { return rad; }

      // translation
      sphere& operator += (const P& arg)
	{
	  ctr += arg;
	  return *this;
	}

      // scaling
      sphere& operator *= (const double& arg)
	{
	  rad *= arg;
	  return *this;
	}

      // sphere
      void draw();

    }; /* end of sphere class */


  // translation
  inline sphere operator + (const sphere& sph, const P& arg)
    {
      sphere temp=sph;
      return temp += arg;
    }

  inline sphere operator + (const P& arg, const sphere& sph)
    {
      sphere temp=sph;
      return temp += arg;
    }

  // scaling about center
  inline sphere operator * (const double& arg, const sphere& sph)
    {
      sphere temp=sph;
      return temp *= arg;
    }

  // intersection
  circle operator * (const sphere& sph1, const sphere& sph2);


  // alternative constructor: specify pair of antipodal points
  sphere poles(const P& p1, const P& p2);


  // utility function for hidden line removal (hidden<0 reverses value)
  bool visible_on_sphere(const P& arg, bool front = true, 
			 const sphere& S=sphere());

  // {lat,long}itude lines with hiding
  void latitude(double lat, double long_min, double long_max,
		sphere S=sphere(), frame coords=frame());

  void longitude(double lngtd, double lat_min, double lat_max,
		 sphere S=sphere(), frame coords=frame());

  void back_latitude(double lat, double long_min, double long_max,
		     sphere S=sphere(), frame coords=frame());

  void back_longitude(double lngtd, double lat_min, double lat_max,
		      sphere S=sphere(), frame coords=frame());

} /* end of namespace */

#endif /* EPIX_SPHERE */
