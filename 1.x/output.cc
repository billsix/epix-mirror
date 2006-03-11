/* 
 * output.cc -- output file formatting routines
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

#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <list>

#define EPIX_COMPILE_OUTPUT

// #include <stdexcept>

#include "globals.h"
#include "triples.h"
#include "pairs.h"

#include "camera.h"
#include "lengths.h"
#include "path.h"

#include "output.h"
#include "objects.h"

#include "segment.h"
#include "circle.h"
#include "plane.h"
#include "sphere.h"

namespace ePiX {

  extern std::string epix_version;
  epix_camera camera;

  // define global variables (for backward compatibility, not in epix::)
  double x_min, x_max, x_size, y_min, y_max, y_size;
  double tix=0;

  double pic_size;
  char *pic_unit;

  // define static variables
  double epix::dashfill=0.5;
  double epix::separation=12;
  double epix::gray_depth=0.3;
  double epix::dotsize=3.0;

  double epix::angle_units=1.0;
  double epix::labelangle=0;

  bool epix::clipping=false;
  bool epix::cropping=false;
  bool epix::fill_paths=false;

  bool epix::using_pstricks=false;
  std::string epix::fillcolor="white";

  std::string epix::fontsize="normalsize";
  std::string epix::fontface="default";

  double epix::arrowwidth=1.5;
  double epix::arrowratio=5.5;
  double epix::arrowcamber=0.0;
  double epix::arrowfill=0.0;

    //     epix_output_type epix::OUTPUT_TYPE;
    //     std::string epix::font;
  epix_path_style epix::PATH_STYLE=SOLID;

  int epix::begin_count=0; // number of times begin() has been called

  void unitlength(const char *units)
  {
    pic_size = strtod(units, &pic_unit);
  }

  // Sectioning Functions

  void begin(void)
  {
    camera=epix_camera();

    // initialize size variables; must be done after bounding_box()
    x_min = crop_mask::Bounding_Box.bl().x1();
    y_min = crop_mask::Bounding_Box.bl().x2();

    x_max = crop_mask::Bounding_Box.tr().x1();
    y_max = crop_mask::Bounding_Box.tr().x2();

    x_size = x_max - x_min;
    y_size = y_max - y_min;

    // Write eepic header
    if (epix::begin_count == 0)
      cout << "%% ePiX-" << epix_version;
    ++epix::begin_count;

    epix_newline();
    epix_newline();

    fprintf (stdout, "   Cartesian bounding box: [%g,%g] x [%g,%g]",
	     x_min, x_max, y_min, y_max);

    epix_newline();
    fprintf (stdout, "   Actual size: %g%s x %g%s",
	     h_size()*pic_size, pic_unit, v_size()*pic_size, pic_unit);

    epix_newline();
    fprintf (stdout, "   Figure offset: %s by %g%s, %s by %g%s",
	     (h_offset() >= 0) ? "right" : "left", 
	     fabs(h_offset())*pic_size, pic_unit, 
	     (v_offset() >= 0) ? "up"    : "down", 
	     fabs(v_offset())*pic_size, pic_unit);

    epix_newline();
    fprintf (stdout, "\n\\setlength{\\unitlength}{%g%s}", pic_size, pic_unit);

    fprintf (stdout, "\n\\begin{picture}(%g,%g)(%g,%g)", // reverse offset
	     h_size(), v_size(), -h_offset(), -v_offset());

    if (epix::using_pstricks)
      {
	epix_newline();
	fprintf (stdout, "\n\\psset{unit=%g%s,"
		 "linewidth=0.4pt,fillstyle=solid}", 
		 pic_size, pic_unit);
      }

    end_stanza();
  }


  void print(const P location)
  {
    pair out = truncate(c2p(camera(location)));
    raw_print(out);
  }

  void print(std::ostringstream& s, const P location)
  {
    pair out = truncate(c2p(camera(location)));
    s << '(' << out.x1() << ',' << out.x2() << ')';
  }

  void print(const pair location)
  {
    pair out = truncate(c2p(camera(P(location.x1(), location.x2(), 0))));
    raw_print(out);
  }

  // print command for label locations
  void print(const P location, const P offset)
  {
    pair temp = pair(offset.x1(), offset.x2());
    pair loc = truncate(c2p(camera(location)) + t2p(temp));
    raw_print(loc);
  }
  // print command for spatial displacements
  void print_vector(const P direction)
  {
    pair out = truncate(c2s(camera(direction)));
    raw_print(out);
  }

  // print label elements

  // labels with LaTeX-style alignment
  void print_alignment(epix_label_posn POSN)
  {
    cout << "[";

    switch(POSN) {
    case c:
      cout << "c";  break;

    case t:
      cout << "b";  break;  // N.B. Reversal of LaTeX meaning

    case tr: case rt:
      cout << "bl"; break;

    case r:
      cout << "l";  break;

    case br: case rb:
      cout << "tl"; break;

    case b: 
      cout << "t";  break;

    case bl: case lb:
      cout << "tr"; break;

    case l: 
      cout << "r";  break;

    case tl: case lt: 
      cout << "br"; break;

    default:
      epix_warning("Unrecognized label alignment option");   
    } // end of switch POSN
    cout << ']';
  } // end of print_alignment

  void print_marker(epix_mark_type MARK)
  {
    lbrace();

    switch(MARK) {
    case PATH:
      epix_warning("Label cannot be of PATH type");
      break;

    case CIRC:
      // white filled
      epix_circle(epix::get_dotsize());
      break;

    case RING:
      // double diameter
      epix_circle(1.5*epix::get_dotsize());
      break;

    case SPOT:
      // double diameter
      epix_disk(1.5*epix::get_dotsize());
      break;

    case DOT:
      epix_disk(epix::get_dotsize());
      break;

    case DDOT:
      // half diameter
      epix_disk(epix::get_dotsize()/1.5);
      break;

    case PLUS:
      cout << '+';
      break;

    case OPLUS:
      cout << "$\\oplus$";
      break;

    case TIMES:
      cout << "$\\times$";
      break;

    case OTIMES:
      cout << "$\\otimes$";
      break;

    case DIAMOND:
      cout << "$\\diamond$";
      break;

    case UP:
      cout << "$\\bigtriangleup$";
      break;

    case DOWN:
      cout << "$\\bigtriangledown$";
      break;

    case BOX:
      {
	double S=epix::get_dotsize();
	epix_rule(S,S);
	break;
      }

    case BBOX:
      {
	double S=epix::get_dotsize()/1.5;
	epix_rule(S,S);
	break;
      }

    case H_TICK:
      epix_rule(0.5, 4);
      break;

    case V_TICK:
      epix_rule(4, 0.5);
      break;

    default: 
      epix_warning("Ignoring unrecognized Label type");
      break;
    } // end of switch(MARK)

    rbrace();
  } // end of print_marker


  void start_path(void)
  {
    newl();
    if (epix::using_pstricks)
      cout << "\\psline";

    else
      cout << "\\path";
  }

  // small (un)filled circle
  void epix_circle(const double r_pt) // radius in pt
  {
    lbrace();
    cout << "\\circle";
    epix_grouping(t2p(r_pt)); // convert radius to picture units
    rbrace();
  }

  void epix_disk(const double r_pt)
  {
    lbrace();
    cout << "\\circle*";
    epix_grouping(t2p(r_pt));
    rbrace();
  }


  // Path style declarations
  void pen(std::string p) // valid LaTeX length, e.g. "0.01in"
  {
    cout << "\n\\allinethickness{" << p << "}%";
  }

  void pen(double w) // true points
  {
    cout << "\n\\allinethickness{" << w << "pt}%";
  }

  void dash_start(const P& arg1, const P& arg2)
  {
    P temp =  (0.5*epix::get_dashfill())*(arg2-arg1);
    start_path();
    print(arg1);
    print(arg1 + temp);
  }

  void dash_seg(const P& arg0, const P& arg1, const P& arg2)
  {
    double fill=0.5*epix::get_dashfill();
    P temp0 =  fill*(arg0-arg1);
    P temp1 =  fill*(arg2-arg1);

    start_path();
    print(arg1 + temp0);
    print(arg1);
    print(arg1 + temp1);
  }

  void line_break(int i, int num_pts)
  {
    // Print only 3 points on first line of dashed/dotted path
    if (epix::path_style() == DASHED || epix::path_style() == DOTTED)
      ++i;
  
    if ((i+1) % EPIX_PAIRS_PER_LINE == 0 && i < num_pts)
      cout << "  \n";
  }

  // Variable color handling, requires "pstcol" package

  // Red-Blue-Green
  void rgb(double r, double g, double b)
  {
    r = clip_to_unit(r);
    g = clip_to_unit(g);
    b = clip_to_unit(b);
    fprintf (stdout, "\n\\color[rgb]{%.2f,%.2f,%.2f}", r, g, b);
  }

  // Cyan-Magenta-Yellow-Black
  void
  cmyk(double c, double m, double y, double k)
  {
    c = clip_to_unit(c);
    m = clip_to_unit(m);
    y = clip_to_unit(y);
    k = clip_to_unit(k);
    fprintf (stdout, "\n\\color[cmyk]{%.2f,%.2f,%.2f,%.2f}", c, m, y, k);
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
  void path::draw(void)
  {
    if(closed)
      vertices.push_back(vertices.at(0));

    // mark vertices for removal
    if (epix::cropping)
      this->crop_to(crop_mask::Crop_Box);

    if (epix::clipping)
      this->clip_to(enclosure::Clip_Box);

    vertex prev, curr; //, next;
    P temp_path_end;

    bool prev_visible, curr_visible; //, next_visible;

    bool started = false; // in "draw" mode when we examined curr?
    epix_path_style STYLE = epix::path_style();

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

	/*
	if (i < vertices.size() - 1)
	  {
	    next = vertices.at(i+1);
	    next_visible = (next.is_onscreen() && next.is_in_world());
	  }
	else next = curr;
	*/

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

	int temp_size;             // number of characters in current point
	int pt_count=0;            // points printed so far in path segment
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

  } // end of path::draw()

} /* end of namespace */
