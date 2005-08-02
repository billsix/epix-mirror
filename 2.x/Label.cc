/* 
 * Label.cc -- ePiX3d::Label class draw() functions
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.9,0rc1
 * Last Change: June 11, 2004
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004
 * Andrew D. Hwang <ahwang@mathcs.holycross.edu>
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
#include <sstream>

#include "Label.h"

namespace ePiX3d {

  // Constructor for function-specified labels
  Label::Label(Point locn, double f(double, double, double), 
	       epix3d_label_posn alignment, bool mask, Pair offset)
  {
    // Generate label text as function of location
    std::string label_string;
    std::stringstream label_contents;
    label_contents << "$" << f(locn.x1(), locn.x2(), locn.x3()) << "$";
    label_string = label_contents.str();

    the_basepoint = locn;
    the_label_text = label_string;

    the_alignment = alignment;
    masked = mask;
    the_offset = offset;
  }

  /*
  static void operator<< (epix3d_label_posn POSN)
  {
    switch(POSN) {
    case c:
      std::cout << "c";
      break;

    case t:
      std::cout << "b"; // N.B. Reversal of LaTeX meaning
      break;

    case tr:
      std::cout << "bl";
      break;

    case r:
      std::cout << "l";
      break;

    case br:
      std::cout << "tl";
      break;

    case b: 
      std::cout << "t";
      break;

    case bl:
      std::cout << "tr";
      break;

    case l: 
      std::cout << "r";
      break;

    case tl:
      std::cout << "br";
      break;

    default:
      epix_warning("Unrecognized label alignment option");
      std::cout << "c";
      break;
    }
  }

  // labels with LaTeX-style alignment
  static void print_alignment(epix_label_posn POSN)
  {
    std::cout << "[" << POSN << "]";
  }

  static void print_marker(epix_mark_type MARK)
  {
    switch(MARK) {

    case CIRC:
      fprintf (stdout, "{\\circle{%.3f}}", t2p(EPIX_SPOT_DIAM));
      break;

    case SPOT:   
      fprintf (stdout, "{$\\bullet$}");
      break;

    case RING:   
      fprintf (stdout, "{\\circle{%.3f}}", t2p(EPIX_SPOT_DIAM));
      break;

    case DOT:   
      fprintf (stdout, "{\\circle*{%.3f}}", t2p(EPIX_DOT_DIAM));
      break;

    case DDOT:
      fprintf (stdout, "{\\circle*{%.3f}}", t2p(EPIX_DDOT_DIAM));
      break;

    case PLUS:
      fprintf (stdout, "{+}");
      break;

    case OPLUS:
      fprintf (stdout, "{$\\oplus$}");
      break;

    case TIMES:
      fprintf (stdout, "{$\\times$}");
      break;

    case OTIMES:
      fprintf (stdout, "{$\\otimes$}");
      break;

    case DIAMOND:
      fprintf (stdout, "{$\\diamond$}");
      break;

    case UP:
      fprintf (stdout, "{$\\bigtriangleup$}");
      break;

    case DOWN:
      fprintf (stdout, "{$\\bigtriangledown$}");
      break;

    case BOX:   
      fprintf (stdout, "{\\rule{%gpt}{%gpt}}",
	       EPIX_BOX_SIZE, EPIX_BOX_SIZE);
      break;

    case BBOX:   
      fprintf (stdout, "{\\rule{%gpt}{%gpt}}",
	       EPIX_BBOX_SIZE, EPIX_BBOX_SIZE);
      break;

    case H_TICK:
      fprintf (stdout, "{\\rule{0.5pt}{4pt}}");
      break;

    case V_TICK:
      fprintf (stdout, "{\\rule{4pt}{0.5pt}}");
      break;

    default: 
      epix_warning("Ignoring unrecognized Label type");
      break;
    }
  }

  void Label::draw()
  {
    fprintf (stdout, "\n");    
    if (the_label_type == CIRC)
      fprintf (stdout, "\\whiten");

    fprintf (stdout, "\n\\put");    

    print(the_basepoint, the_offset);

    if (the_alignment != none)
      {
	fprintf (stdout, "{\\makebox(0,0)");
	print_alignment(the_alignment);
      }

    if (masked)
      fprintf (stdout, "{\\colorbox{white}");

    if (the_label_type == TEXT)
      //      fprintf (stdout, "{%s}", the_label_text);
      std::cout << '{' << the_label_text << '}';

    else
      print_marker(the_label_type);

    if (masked)
      fprintf (stdout, "}");

    if (the_alignment != none)
      fprintf (stdout, "}");
  }
  */

} /* end of namespace */

