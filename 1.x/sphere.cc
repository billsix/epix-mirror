/* 
 * sphere.cc -- ePiX::sphere class and mathematical operators
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc14
 * Last Change: July 27, 2004
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004
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

#include <iostream>

#include "globals.h"
#include "triples.h"

#include "path.h"
#include "cropping.h"

#include "camera.h"
#include "sphere.h"
#include "output.h"

namespace ePiX {

  extern epix_camera camera;
  void box(P);

  // defined in output.cc
  void dash_seg(const P& arg0, const P& arg1, const P& arg2);

  // intersection
  circle operator * (const sphere& sph1, const sphere& sph2)
    {
      double r1=sph1.radius();
      double r2=sph2.radius();
      double dist=norm(sph1.center()-sph2.center());

      if (r1+r2<dist)
      throw join_error(SEPARATED);

      else if  (r1+r2==dist || fabs(r2-r1)==dist)
      throw join_error(TANGENT);

      else if  (dist<fabs(r2-r1))
      throw join_error(CONCENTRIC);

      else
	{
	  double temp1=r1*r1;
	  double temp2=r2*r2;
	  double x=0.5*(dist+(temp1-temp2)/dist);

	  P temp_norm=(1/dist)*(sph2.center()-sph1.center());
	  P temp_ctr=sph1.center()+(x*temp_norm);
	  double temp_rad=sqrt(r1*r1-x*x);

	  return circle(temp_ctr, temp_rad, temp_norm);
	}
    }

  void sphere::draw() 
  { 
    if (camera.get_range() == 0) // we're at infinite distance
      {
	circle circumf = circle(ctr, rad, camera.eye());
	circumf.draw();
      }

    else // we're at finite distance
      {
	double r1=rad;

	P location = camera.get_viewpt();
    
	P temp_points = ctr - location;
	double dist = norm(temp_points);

	if (dist <= r1)
	  epix_warning("Cannot draw sphere enclosing camera");
	
	else
	  {
	    temp_points *= 1/dist;

	    double x=r1*r1/dist;
	
	    P temp_ctr = ctr - x*temp_points;
	    double temp_rad = sqrt(r1*r1-x*x);
	    circle temp = circle(temp_ctr, temp_rad, temp_points);
	    temp.draw();
	  }
      }
  }


  // Visibility test for plotting on unit sphere; assumes arg is on S

  bool visible_on_sphere(const P& arg, bool front, const sphere& S)
  {
    bool visible = false;
    if ( ((arg-S.center())|(camera.get_viewpt()-arg)) >= 0 )
      visible = true;

    if (!front)
      visible = !visible;

    return visible;
  }

  void path::draw(sphere S, bool front)
  {
    if (epix::cropping)
      this->crop_to(crop_mask::Crop_Box);

    if (epix::clipping)
      this->clip_to(enclosure::Clip_Box);

    vertex prev, curr, next;
    bool prev_visible, curr_visible, next_visible;

    for (unsigned int i=0; i < vertices.size(); ++i)
      {
	curr = vertices.at(i);
	curr_visible = (curr.is_onscreen() && curr.is_in_world() &&
			visible_on_sphere(curr.here(), front, S));

	switch (epix::path_style()) {
	case SOLID:

	  if (i < vertices.size()-1)
	    {
	      next = vertices.at(i+1);
	      next_visible = (next.is_onscreen() && next.is_in_world() &&
			      visible_on_sphere(next.here(), front, S));
	    }
	  else
	    next_visible = false;

	  if (curr_visible)
	    {
	      if (next_visible)
		{
		  if (i == 0)
		    start_path();
		  print(curr.here());
		}
	      else // finish path
		{
		  print(curr.here());
		  print(midpoint(curr.here(), next.here()));
		}
	    }

	  else // !curr_visible
	    {
	      if (next_visible)
		{
		  start_path();
		  print(midpoint(curr.here(), next.here()));
		}
	      // else !next_visible, do nothing
	    }
	  break; // end of case(SOLID)

	case DOTTED:
	  curr = vertices.at(i);
	  if (curr_visible)
	    newl();
	    box(curr.here());

	  break;

	case DASHED:
	  if (curr_visible)
	    {
	      if (0 < i)
		{
		  prev = vertices.at(i-1);
		  prev_visible = (prev.is_onscreen() && prev.is_in_world() &&
				  visible_on_sphere(prev.here(), front, S));
		}
	      else // first point
		prev = curr;

	      if (i < vertices.size() - 1)
		{
		  next = vertices.at(i+1);
		  next_visible = (next.is_onscreen() && next.is_in_world() &&
				  visible_on_sphere(next.here(), front, S));
		}
	      else // last point
		next = curr;

	      dash_seg(prev.here(), curr.here(), next.here());
	    }
	  break;
	} // end of switch(PATH_STYLE)
      } // end of for loop

    end_stanza();
  } // end of path::draw(sphere, bool)


  // point constructor in geographic coords on specified sphere/frame
  P sphere_posn(double lat, double lngtd, sphere S, frame coords)
  {
    using ePiX::cos;
    using ePiX::sin;

    double rad=S.radius();
    P ctr=S.center();

    P equatorial 
      = rad*cos(lat)*(cos(lngtd)*coords.sea()+sin(lngtd)*coords.sky());
    P height = rad*sin(lat)*coords.eye();

    return ctr + equatorial + height;	
  }

  // latitudes and longitudes
  static
  void draw_latitude(double lat, double lngtd_min, double lngtd_max,
		     bool hidden, sphere S, frame coords)
  {
    P center = S.center() + (ePiX::sin(lat)*coords.eye());
    double radius = S.radius()*ePiX::cos(lat);

    path temp(center, radius*coords.sea(), radius*coords.sky(),
		   lngtd_min, lngtd_max);

    temp.draw(S, hidden);
    end_stanza();
  }

  // draw portion of longitude line
  static
  void draw_longitude(double lngtd, double lat_min, double lat_max,
		      bool hidden, sphere S, frame coords)
  {
    P center = S.center();
    double radius = S.radius();

    path temp(center, 
		   radius*(ePiX::cos(lngtd)*coords.sea() +
			   ePiX::sin(lngtd)*coords.sky() ),
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

} /* end of namespace */
