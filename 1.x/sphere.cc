/* 
 * sphere.cc -- ePiX::sphere class and mathematical operators
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.0
 * Last Change: September 04, 2004
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
#include <list>
#include <string>
#include <sstream>

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
  void dash_start(const P& arg1, const P& arg2);
  void print(std::ostringstream& s, const P location);

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
  } // end of sphere::draw()


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

  // Closely adapted from path::draw() in output.cc
  void path::draw(sphere S, bool front)
  {
    if (closed)
      vertices.push_back(vertices.at(0));

    if (epix::cropping)
      this->crop_to(crop_mask::Crop_Box);

    if (epix::clipping)
      this->clip_to(enclosure::Clip_Box);

    vertex prev, curr, next;
    bool prev_visible, curr_visible, next_visible;

    bool started = false; // in "draw" mode when we examined curr?
    epix_path_style STYLE = epix::path_style();

    std::list<path_pt> segments;

    // cull vertices, mark as start/end of path segments
    for (unsigned int i=0; i < vertices.size(); ++i)
      {
	// get prev, curr, next
	curr = vertices.at(i);
	curr_visible = (curr.is_onscreen() && curr.is_in_world() &&
			visible_on_sphere(curr.here(), front, S));

	if (0 < i)
	  {
	    prev = vertices.at(i-1);
	    prev_visible = (prev.is_onscreen() && prev.is_in_world());
	  }
	else 
	  {
	    prev = curr;
	    prev_visible = false;
	  }

	if (i < vertices.size()-1)
	  {
	    next = vertices.at(i+1);
	    next_visible = (next.is_onscreen() && next.is_in_world() &&
			    visible_on_sphere(next.here(), front, S));
	  }
	else
	  {
	    next = curr;
	    next_visible = false;
	  }

	// four cases: "started" is (un)set and curr is (in)visible
	if (curr_visible)
	  {
	    if (started)
	      {
		if (i < vertices.size() - 1)
		  segments.push_back(path_pt(curr, false, false));
		else
		  segments.push_back(path_pt(curr, false, true));
	      }
	    else // start path segment
	      {
		// loss of accuracy; don't seek edge of sphere
		segments.push_back(path_pt(prev.here(), true, false));
		if (curr.here() != prev.here())
		  segments.push_back(path_pt(curr, false, false));
		started = true;
	      }
	  } // end of curr_visible; started = true in all cases

	else
	  {
	    if (started) // end path
	      {
		// loss of accuracy; don't seek edge of sphere
		segments.push_back(path_pt(curr.here(), false, true));
		started = false;
	      }
	  } // if !started, do nothing

      } // end of for loop to cull vertices

    std::list<path_pt>::iterator p = ++segments.begin();
    std::list<path_pt>::iterator q, q2;

    if (p == segments.end())
      return; // empty path

    if (this->filled) // remove start/end flags except global first/last
      while (p++ != segments.end())
	p->unset();

    // Write fill \special if necessary; pstricks handles its own filling
    if (this->filled && SOLID == STYLE && !epix::using_pstricks)
      cout << "\n\\special{sh " << epix::get_gray() << "}%";

    // print path
    switch (STYLE) 
      {
      case DOTTED:

	for (p = segments.begin(); p != segments.end(); ++p)
	  {
	    newl();
	    box((p->here()));
	  }
	break;


      case DASHED:

	for (p = segments.begin(); p != segments.end(); ++p)
	  {
	    q = q2 = p;
	    {
	      if (p->is_start())
		{
		  dash_start(q2->here(), (++q)->here());
		}
	      else if (p->is_end())
		{
		  dash_start(q2->here(), (--q)->here());
		}
	      else
		{
		  dash_seg((--q2)->here(), p->here(), (++q)->here());
		}
	    }
	  }
	break;

      case SOLID: // fall through
      default:

	std::ostringstream outbuf;

	int temp_size;            // number of characters in current point
	int pt_count=0;           // points printed so far in path segment
	int curr_line_count=0;    // characters so far in this line

	for (p = segments.begin(); p != segments.end(); ++p)
	  {
	    std::ostringstream temp;         // empty buffer for point
	    print (temp, p->here());         // examine point
	    temp_size = temp.str().length(); // get length as a string
	    
	    if (p->is_start())
	      {
		outbuf << start_path_string();
		print (outbuf, p->here());
		curr_line_count=5+temp_size; // off by 2 if using_pstricks
	      }

	    else
	      {
		// reached maximum line length?
		if ((curr_line_count >= EPIX_FILE_WIDTH) || 
		    (curr_line_count + temp_size > EPIX_FILE_WIDTH + 5))
		  {
		    outbuf << "\n  ";
		    curr_line_count = 2; // reset number of characters
		  }

		outbuf << temp.str(); // "routine" outcome
		++pt_count;
		curr_line_count += temp_size;

		// break path segment to avoid TeX memory overflow?
		if (0 == (pt_count%EPIX_PATH_LENGTH) && (pt_count > 0)
		    && !(this->filled)) // don't break filled paths
		  {
		    outbuf << start_path_string() << temp.str();
		    pt_count = 1;      // reset count and number of characters
		    curr_line_count = 5 + temp_size; 
		  }
	      } // not start of path

	    if (p->is_end())
	      {
		outbuf << end_path_string();
	      }

	  } // end of for loop; outbuf contains formatted output
	std::cout << outbuf.str();

	break;

      } // end of switch(STYLE)

  } // end of path::draw(sphere S, bool front)


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
    P center = S.center() + (Sin(lat)*coords.eye());
    double radius = S.radius()*Cos(lat);

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

} /* end of namespace */
