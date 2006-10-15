/* 
 * sphere.cc -- ePiX::sphere class and mathematical operators
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.15
 * Last Change: October 09, 2006
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
#include "globals.h"
#include "errors.h"

#include "triples.h"

#include "circle.h"
#include "path.h"

#include "output.h"

#include "frame.h"
#include "camera.h"
#include "sphere.h"

namespace ePiX {

  extern epix_camera camera;

  sphere::sphere(const P& p1, double r)
    : ctr(p1), rad(r) { }

  sphere::sphere(const P& p1, const P& p2)
    : ctr(p1), rad(norm(p2-p1)) { }

  P sphere::center() const
  {
    return ctr;
  }
  double sphere::radius() const
  {
    return rad;
  }

  // translation
  sphere& sphere::operator += (const P& arg)
  {
    ctr += arg;
    return *this;
  }

  // scaling
  sphere& sphere::operator *= (const double& arg)
  {
    rad *= arg;
    return *this;
  }


  // translation
  sphere operator+ (const sphere& sph, const P& arg)
  {
    sphere temp(sph);
    return temp += arg;
  }

  sphere operator+ (const P& arg, const sphere& sph)
  {
    sphere temp(sph);
    return temp += arg;
  }

  // scaling about center
  sphere operator* (const double& arg, const sphere& sph)
  {
    sphere temp(sph);
    return temp *= arg;
  }

  // intersection
  circle operator* (const sphere& sph1, const sphere& sph2)
  {
    double r1(sph1.radius()), r2(sph2.radius());
    double dist(norm(sph1.center()-sph2.center()));

    if (r1+r2 < dist)
      throw join_error(SEPARATED);

    else if  (r1+r2 == dist || fabs(r2-r1) == dist)
      throw join_error(TANGENT);

    else if  (dist < fabs(r2-r1))
      throw join_error(CONCENTRIC);

    else
      {
	double x(0.5*(dist + (r1-r2)*(r1+r2)/dist));

	P temp_norm((1/dist)*(sph2.center()-sph1.center()));
	P temp_ctr(sph1.center()+(x*temp_norm));
	double temp_rad(sqrt((r1-x)*(r1+x)));

	return circle(temp_ctr, temp_rad, temp_norm);
      }
    }

  void sphere::draw() const
  { 
    if (camera.get_range() == 0) // we're at infinite distance
      {
	circle circumf(ctr, rad, camera.eye());
	circumf.draw();
      }

    else // we're at finite distance
      {
	double r1(rad);

	P location(camera.get_viewpt());
    
	P temp_points(ctr - location);
	double dist(norm(temp_points));

	if (dist <= r1)
	  epix_warning("Cannot draw sphere enclosing camera");
	
	else
	  {
	    temp_points *= 1/dist;
	    double x(r1*r1/dist);
	
	    circle temp(ctr - x*temp_points, sqrt((r1-x)*(r1+x)), temp_points);
	    temp.draw();
	  }
      }
  } // end of sphere::draw()

  // end of member functions

  sphere poles(const P& p1, const P& p2) 
  { 
    return sphere(midpoint(p1,p2), p2); 
  }

  // Visibility test for plotting on unit sphere; assumes arg is on S
  bool visible_on_sphere(const P& arg, bool front, const sphere& S)
  {
    bool visible(false);
    if ( ((arg-S.center())|(camera.get_viewpt()-arg)) >= 0 )
      visible = true;

    if (!front)
      visible = !visible;

    return visible;
  }


  // point constructor in geographic coords on specified sphere/frame
  P sphere_posn(double lat, double lngtd, sphere S, frame coords)
  {
    double rad(S.radius());

    return S.center() + 
      rad*Cos(lat)*(Cos(lngtd)*coords.sea() + Sin(lngtd)*coords.sky()) +
      rad*Sin(lat)*coords.eye();
  }

  // latitudes and longitudes
  void draw_latitude(double lat, double lngtd_min, double lngtd_max,
		     bool hidden, sphere S, frame coords)
  {
    P center(S.center() + (S.radius()*Sin(lat)*coords.eye()));
    double radius(S.radius()*Cos(lat));

    path temp(center, radius*coords.sea(), radius*coords.sky(),
	      lngtd_min, lngtd_max);

    temp.draw(S, hidden);
    end_stanza();
  }

  // draw portion of longitude line
  void draw_longitude(double lngtd, double lat_min, double lat_max,
		      bool hidden, sphere S, frame coords)
  {
    P center(S.center());
    double radius(S.radius());

    path temp(center,
	      radius*(Cos(lngtd)*coords.sea()+Sin(lngtd)*coords.sky() ),
	      radius*coords.eye(), lat_min, lat_max);

    temp.draw(S, hidden);
    end_stanza();
  }

  void latitude(double lat, double lngtd_min, double lngtd_max,
		sphere S, frame coords)
  {
    draw_latitude(lat, lngtd_min, lngtd_max, true, S, coords);
  }


  void longitude(double lngtd, double lat_min, double lat_max,
		 sphere S, frame coords)
  {
    draw_longitude(lngtd, lat_min, lat_max, true, S, coords);
  }

  void back_latitude(double lat, double lngtd_min, double lngtd_max,
		     sphere S, frame coords)
  {
    draw_latitude(lat, lngtd_min, lngtd_max, false, S, coords);
  }

  void back_longitude(double lngtd, double lat_min, double lat_max,
		      sphere S, frame coords)
  {
    draw_longitude(lngtd, lat_min, lat_max, false, S, coords);
  }

  } // end of namespace
