/* 
 * output.cc -- output file formatting routines
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.15
 * Last Change: October 10, 2006
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
#include "errors.h"

#include "triples.h"
#include "pairs.h"

#include "camera.h"
#include "lengths.h"
// #include "path.h"

#include "output.h"
#include "objects.h"

namespace ePiX {

  using std::cout;

  extern const std::string epix_version;
  epix_camera camera;

  void unitlength(const char *units)
  {
    pic_size = strtod(units, &pic_unit);
  }

  // Sectioning Functions

  void begin()
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


  void raw_print(const pair& arg)
  {
    cout << "(" << arg.x1() << "," << arg.x2() << ")";
  }

  void print(const P& location)
  {
    raw_print(truncate(c2p(camera(location))));
  }

  void print(const pair& location)
  {
    raw_print(truncate(c2p(camera(P(location.x1(), location.x2(), 0)))));
  }

  // print command for label locations
  void print(const P& location, const P& offset)
  {
    raw_print(truncate(c2p(camera(location)) +
		       t2p(pair(offset.x1(), offset.x2()))));
  }

  // print command for spatial displacements
  void print_vector(const P& direction)
  {
    raw_print(truncate(c2s(camera(direction))));
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


  // Functions that write small pieces of a LaTeX \picture environment.
  // Aside from delimiters (braces and dollar), each command preserves
  // the LaTeX grouping level.
  void lbrace()
  {
    cout << '{';
  }
  void rbrace()
  {
    cout << '}';
  }
  void dollar()
  {
    cout << '$';
  }
  void newl()
  {
    cout << std::endl;
  }

  void epix_grouping(const double arg)
  {
    std::cout << "{" << arg << "}";
  }
  void epix_grouping(const std::string& arg)
  {
    std::cout << "{" << arg << "}";
  }
  // same, in math mode
  void epix_math_grouping(const double arg)
  {
    std::cout << "{$" << arg << "$}";
  }
  void epix_math_grouping(const std::string& arg)
  {
    std::cout << "{$" << arg << "$}";
  }


  // common \LaTeX commands

  // label txt, with font information
  void epix_label_text(const std::string& arg)
  {
    cout << '{';
    if ( epix::fontsize != "normalsize" )
      {
	cout << '\\' << epix::fontsize << " ";
      }

    if ( epix::fontface != "default" )
      cout << "\\text" << epix::fontface << '{';

    cout << arg;

    if ( epix::fontface != "default" )
      cout << '}';

    cout << '}'; 
  }

  void epix_neg()
  {
    cout << "\\phantom{-}";
  }

  void epix_put()
  {
    cout << "\\put";
  }
  void epix_multiput()
  {
    cout << "\\multiput";
  }
  void epix_makebox()
  {
    cout << "\\makebox(0,0)";
  }
  void epix_whiten()
  {
    cout << "\\whiten";
  }


  void start_path()
  {
    newl();
    if (epix::using_pstricks)
      cout << "\\psline";

    else
      cout << "\\path";
  }

  void end_path() { }


  // string-valued functions for path::draw()
  std::string start_path_string()
  {
    if (epix::using_pstricks)
      return "\n\\psline";

    else
      return "\n\\path";
  }
  std::string end_path_string() 
  {
    return "";
  }

  void psset(const std::string& arg) 
  {
    if (epix::using_pstricks)
      cout << "\n\\psset{" << arg << "}";
  }

  void fill_color(const std::string& color) 
  {
    if (epix::using_pstricks)
      {
	epix::fillcolor=color; 
	cout << "\n\\psset{fillcolor=" << epix::fillcolor << "}";
      }
  }

  void epix_rotatebox()
  {
    cout << "\\rotatebox";
    epix_grouping(epix::get_labelangle());
  }

  void epix_colorbox()
  {
    cout << "\\colorbox";
    if (epix::using_pstricks)
      epix_grouping(epix::fillcolor);
    else
      epix_grouping("white");
  }

  void epix_rule(const double x, const double y)
  {
    cout << "\\rule{" << x << "pt}{" << y << "pt}";
  }

  /*
  void epix_rule(const double x,  const double y,
                 const double dx, const double dy)
  {
    cout << "{\\kern " << dx << "pt \\rule[" << dy << "pt]{"
         << x << "pt}{" << y << "pt}}";
  }
  */

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


  void epix_newline()
  {
    cout << "\n%%";
  }
  void epix_newline(const std::string& msg)
  {
    cout << "\n%% " << msg;
  }

  void end_stanza()
  {
    epix_newline("---");
  }

  void end()
  {
    cout << "\n\\end{picture}\n";
  }

  void line_break(int i, int num_pts)
  {
    // Print only 3 points on first line of dashed/dotted path
    if (epix::path_style() == DASHED || epix::path_style() == DOTTED)
      ++i;
  
    if ((i+1) % EPIX_PAIRS_PER_LINE == 0 && i < num_pts)
      cout << "  \n";
  }


  // Misc Styles and output formatting
  // Path style declarations
  void pen(const std::string& p) // valid LaTeX length, e.g. "0.01in"
  {
    cout << "\n\\allinethickness{" << p << "}%";
  }

  void pen(double w) // true points
  {
    cout << "\n\\allinethickness{" << w << "pt}%";
  }

  // Output styles
  void bold()
  {
    cout << "\n\\thicklines";
  }
  void plain()
  {
    cout << "\n\\thinlines";
  }
} // end of namespace
