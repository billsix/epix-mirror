/* 
 *  path.cc -- ePiX classes for polygons and paths
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.18
 * Last Change: October 21, 2006
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

#include <cstdarg>
#include <iostream>
#include <sstream>

#include <vector>
#include <list>
#include <algorithm>
#include <cmath>

#include "globals.h"
#include "triples.h"
#include "lengths.h"
#include "camera.h"

#include "cropping.h"
#include "sphere.h"
#include "objects.h"
#include "output.h"

#include "path.h"

namespace ePiX {

  extern epix_camera camera;

  // utility functions
  void dash_start(const P& arg1, const P& arg2)
  {
    start_path();
    print(arg1);
    print(arg1 + 0.5*epix::get_dashfill()*(arg2-arg1));
  }

  void dash_seg(const P& arg0, const P& arg1, const P& arg2)
  {
    double fill(0.5*epix::get_dashfill());

    start_path();
    print(arg1 + fill*(arg0-arg1));
    print(arg1);
    print(arg1 + fill*(arg2-arg1));
  }

  void print(std::ostringstream& s, const P location)
  {
    pair out(truncate(c2p(camera(location))));
    s << '(' << out.x1() << ',' << out.x2() << ')';
  }

  //// vertex functions ////
  vertex::vertex(const double a1, const double a2, const double a3)
    : location(a1,a2,a3), onscreen(true), in_world(true) { }

  vertex::vertex(const P& arg)
    : location(arg), onscreen(true), in_world(true) { }

  P vertex::here() const
  {
    return location;
  }
  bool vertex::is_onscreen() const
  {
    return onscreen;
  }
  bool vertex::is_in_world() const
  {
    return in_world;
  }

  void vertex::set_crop(const bool arg)
  {
    onscreen = !arg;
  }
  void vertex::set_clip(const bool arg)
  {
    in_world = !arg;
  }

  ////  path functions ////

  path::path(const std::vector<vertex>& data, bool loop)
    : vertices(data), closed(loop), filled(epix::fill_paths) { }

  path::path(unsigned int num_pts)
    : vertices(std::vector<vertex> (num_pts)),
      closed(false), filled(epix::fill_paths) { }

  // lines, without and with specified number of points
  path::path(const P& tail, const P& head, const double expand)
    : closed(false), filled(false)
  {
    double c(expm1(M_LN2*expand/100.0)); // 2^{expand/100} - 1
    P dir(head - tail);

    if (epix::path_style() == SOLID)
      {
	std::vector<vertex> data(2);
  
	data.at(0) = vertex(tail - (0.5*c)*dir);
	data.at(1) = vertex(head + (0.5*c)*dir);

	swap(vertices, data);
      }

    else // DASHED or DOTTED
      {
	// estimate number of points
	// screen distance from (expanded) tail to head, in true pt
	double dist(norm(p2t(c2s((1+c)*(camera(head) - camera(tail))))));

	unsigned int num_pts((unsigned int) ceil(dist/epix::get_dashlength()));

	std::vector<vertex> data(num_pts+1);
  
	// starting location
	P start(tail - (0.5*c)*dir);

	dir *= ((1+c)/num_pts);

	for (unsigned int i=0; i < data.size(); ++i, start += dir)
	  data.at(i) = vertex(start);

	swap(vertices, data);
      }
  } // end of line constructor

  
  path::path(const P& tail, const P& head, 
	     const double expand, unsigned int num_pts)
    : vertices(std::vector<vertex> (num_pts+1)),
      closed(false), filled(false)
  {
    double c(expm1(M_LN2*expand/100.0)); // 2^{expand/100} - 1

    // direction and starting location
    P dir(head - tail);
    P start(tail - (0.5*c)*dir);

    dir *= ((1+c)/num_pts);

    for (unsigned int i=0; i < num_pts+1; ++i, start += dir)
      vertices.at(i) = vertex(start);

  } // end of line constructor


  // finite-data paths (ellipse, spline)
  path::path(const P& center, const P& axis1, const P& axis2, 
	     const double t_min, const double t_max,
	     unsigned int default_num_pts)
    : closed(false), filled(epix::fill_paths)
  {
    // default_num_pts = one full turn
    double frac(fabs(t_max-t_min)/(epix::full_turn()));
    unsigned int num_pts((unsigned int) max(1, ceil(frac*default_num_pts)));

    std::vector<vertex> data(num_pts+1);

    const double dt((t_max - t_min)/num_pts);
    double t(t_min);

    for (unsigned int i=0; i < num_pts+1; ++i, t += dt) 
      data.at(i) = vertex(center + ((Cos(t)*axis1)+(Sin(t)*axis2)));

    if (fabs(frac - 1) < EPIX_EPSILON) // one full turn?
      {
	data.pop_back();
	closed = true;
      }

    swap(vertices, data);
  } // end of ellipse constructor


  // Splines
  P spl_pt(const P& p1, const P& p2, const P& p3, double t)
  {
    return t*t*p1 + 2*t*(1-t)*p2 + (1-t)*(1-t)*p3;
  }

  P spl_pt(const P& p1, const P& p2, const P& p3, const P& p4, double t)
  {
    double s(1-t);
    return t*t*t*p1 + 3*t*t*s*p2 + 3*t*s*s*p3 + s*s*s*p4;
  }


  path::path(const P& p1, const P& p2, const P& p3, unsigned int num_pts)
    : vertices(std::vector<vertex> (num_pts+1)),
      closed(false), filled(false)
  {
    const double dt(1.0/num_pts);

    for (unsigned int i=0; i < vertices.size(); ++i) 
      vertices.at(i) = spl_pt(p1, p2, p3, i*dt);
  }


  path::path(const P& p1, const P& p2, const P& p3, const P& p4,
	     unsigned int num_pts)
    : vertices(std::vector<vertex> (num_pts+1)),
      closed(false), filled(false)
  {
    const double dt(1.0/num_pts);

    for (unsigned int i=0; i < vertices.size(); ++i) 
      vertices.at(i) = spl_pt(p1, p2, p3, p4, i*dt);
  }

  path::path(P f(double), double t_min, double t_max, unsigned int num_pts)
    : vertices(std::vector<vertex> (num_pts+1)),
      closed(false), filled(false)
  {
    const double dt((t_max - t_min)/num_pts);
    double t(t_min);

    for (unsigned int i=0; i <= num_pts; ++i, t += dt) 
      vertices.at(i) = vertex(f(t));
  }

  path::path(double f(double), double t_min, double t_max,
	     unsigned int num_pts)
    : vertices(std::vector<vertex> (num_pts+1)),
      closed(false), filled(false)
  {
    const double dt((t_max - t_min)/num_pts);
    double t(t_min);

    for (unsigned int i=0; i <= num_pts; ++i, t += dt) 
      vertices.at(i) = vertex(P(t,f(t)));
  }


  // append a point
  path& path::pt(const double x, const double y, const double z)
  {
    vertices.push_back(vertex(P(x, y, z)));
    return *this;
  }

  path& path::pt(const P& loc)
  {
    vertices.push_back(vertex(loc));
    return *this;
  }


  // concatenate
  path& path::operator+= (const path& data)
  {
    unsigned int my_size(vertices.size());
    vertices.resize(my_size + data.vertices.size());

    for(unsigned int i=0; i < data.vertices.size(); ++i)
      vertices.at(my_size+i) = data.vertices.at(i);

    return *this;
  }

  path& path::operator-= (const path& data)
  {
    unsigned int my_size(vertices.size());
    vertices.resize(my_size+data.vertices.size());

    for(unsigned int i=0; i < data.vertices.size(); ++i)
      vertices.at(my_size + data.vertices.size()-1-i) = data.vertices.at(i);

    return *this;
  }

  path& path::close(const bool arg)
  {
    closed = arg;
    return *this;
  }
  path& path::fill(const bool arg)
  {
    filled = arg;
    return *this;
  }
  path& path::set_fill(const bool arg) // legacy name
  {
    filled = arg;
    return *this;
  }


  // path (un)cropping, (un)clipping, and printing
  void path::set_crop_all(const bool arg)
  {
    for (unsigned int i=0; i < vertices.size(); ++i)
      vertices.at(i).set_crop(arg);
  }

  void path::set_clip_all(const bool arg)
  {
    for (unsigned int i=0; i < vertices.size(); ++i)
      vertices.at(i).set_clip(arg);
  }


  void path::crop_to(const crop_mask& screen, const bool arg)
  {
    vertex curr;
    for (unsigned int i=0; i < vertices.size(); ++i)
      {
	curr = vertices.at(i);
	// if arg=true and curr is in screen, or arg=false and curr not in
	vertices.at(i).set_crop( (screen.is_onscreen(curr.here()) != arg) );
      }
  } // end of crop_to(screen)


  void path::clip_to(const enclosure& world, const bool arg)
  {
    vertex curr;
    for (unsigned int i=0; i < vertices.size(); ++i)
      {
	curr = vertices.at(i);
	vertices.at(i).set_clip( (world.is_inside(curr.here()) != arg) );
      }
  } // end of clip_to(world)


  // Algorithm to draw path:
  // 1. Close path, crop, and clip vector of vertices if necessary
  // 2. Find starts and ends of path segments by examining visibility of
  //    current and previous points and whether or not we're already in
  //    a path segment. End result is a linked list of vertices with ends
  //    of path segments marked.
  // 3. Check list to see if list is non-empty. If not, check for filling,
  //    and print data points to output.
  //
  void path::draw()
  {
    if(closed)
      vertices.push_back(vertices.at(0));

    // mark vertices for removal
    if (epix::cropping)
      crop_to(crop_mask::Crop_Box);

    if (epix::clipping)
      clip_to(enclosure::Clip_Box);

    vertex prev, curr; //, next;
    P temp_path_end;

    bool prev_visible, curr_visible; //, next_visible;

    bool started(false); // in "draw" mode when we examined curr?
    epix_path_style STYLE(epix::path_style());

    std::list<path_pt> segments;

    // cull vertices, mark as start/end of path segments
    for (unsigned int i=0; i < vertices.size(); ++i)
      {
	// get prev, curr, next
	curr = vertices.at(i);
	curr_visible = (curr.is_onscreen() && curr.is_in_world());

	if (0 < i)
	  {
	    prev = vertices.at(i-1);
	    prev_visible = (prev.is_onscreen() && prev.is_in_world());
	  }
	else prev = curr;

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
		temp_path_end = seek_path_end(curr.here(), prev.here());
		segments.push_back(path_pt(temp_path_end, true, false));
		if (curr.here() != prev.here())
		  segments.push_back(path_pt(curr, false, false));
		started = true;
	      }
	  } // end of curr_visible; started = true in all cases

	else
	  {
	    if (started) // end path
	      {
		temp_path_end = seek_path_end(prev.here(), curr.here());
		segments.push_back(path_pt(temp_path_end, false, true));
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
      std::cout << "\n\\special{sh " << epix::get_gray() << "}%";

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

	int temp_size;          // number of characters in current point
	int pt_count(0);        // points printed so far in path segment
	int curr_line_count(0); // characters so far in this line

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

  } // end of path::draw()


  // 10/09/06: You *really* don't want to look in here...
  void path::draw(sphere S, bool front)
  {
    if (closed)
      vertices.push_back(vertices.at(0));

    if (epix::cropping)
      crop_to(crop_mask::Crop_Box);

    if (epix::clipping)
      clip_to(enclosure::Clip_Box);

    vertex prev, curr, next;
    bool prev_visible, curr_visible, next_visible;

    bool started(false); // in "draw" mode when we examined curr?
    epix_path_style STYLE(epix::path_style());

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

    if (filled) // remove start/end flags except global first/last
      while (p++ != segments.end())
	p->unset();

    // Write fill \special if necessary; pstricks handles its own filling
    if (filled && SOLID == STYLE && !epix::using_pstricks)
      std::cout << "\n\\special{sh " << epix::get_gray() << "}%";

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
	int pt_count(0);          // points printed so far in path segment
	int curr_line_count(0);   // characters so far in this line

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


  // global functions
  // polygon/polyline
  path polygon(unsigned int num_pts, ...)
  {
    std::vector<vertex> data(num_pts);

    va_list ap;
    va_start(ap, num_pts);
  
    for (unsigned int i=0; i < num_pts; ++i) 
      data.at(i) = vertex(*va_arg(ap, P*));

    va_end(ap);

    return path(data, true); // closed
  }

  path polyline(unsigned int num_pts, ...)
  {
    std::vector<vertex> data(num_pts);

    va_list ap;
    va_start(ap, num_pts);
  
    for (unsigned int i=0; i < num_pts; ++i) 
      data.at(i) = vertex(*va_arg(ap, P*));

    va_end(ap);

    return path(data, false); // closed
  }


  path_pt::path_pt(const P& arg, bool is_start, bool is_end)
    : location(arg), start(is_start), end(is_end) { }

  path_pt::path_pt(const vertex& arg, bool is_start, bool is_end)
    : location(arg.here()), start(is_start), end(is_end) { }

  void path_pt::unset()
  {
    start = end = false;
  }

  P path_pt::here() const { return location; }
  bool path_pt::is_start() const { return start; }
  bool path_pt::is_end()   const { return end; }
} // end of namespace
