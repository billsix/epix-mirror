/* 
 * Label.cc -- ePiX::Label class draw() functions
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc14
 * Last Change: July 16, 2004
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

#include "globals.h"
#include "Label.h"
#include "cropping.h"
#include "output.h"

namespace ePiX {

  // Constructor for function-specified labels
  Label::Label(P basepoint, double f(P), epix_label_posn alignment, 
	bool mask, P offset)
  {
    // Generate label text as function of location
    std::string label_string;
    std::stringstream label_contents;
    label_contents << "$" << f(basepoint) << "$";
    label_string = label_contents.str();

    the_basepoint = basepoint;
    the_label_text = label_string;
    the_label_type = TEXT;
    the_alignment = alignment;
    masked = mask;
    the_offset = offset;
  }

  void Label::draw()
  {
    if (is_visible(the_basepoint))
      {
	newl();
	if (the_label_type == CIRC) 
	  epix_whiten();

	epix_put();
	print(the_basepoint, the_offset);

	if (the_alignment != none || the_alignment == c)
	  {
	    lbrace();
	    epix_makebox();
	    print_alignment(the_alignment);
	  }

	if (the_label_type == TEXT && 
	    (fabs(epix::get_labelangle()) > EPIX_EPSILON))
	  {
	    lbrace();
	    epix_rotatebox();
	  }

	if (masked)
	  {
	    lbrace();
	    epix_colorbox();
	  }

	if (the_label_type == TEXT)
	  epix_grouping(the_label_text);

	else
	  print_marker(the_label_type);

	if (masked)
	  rbrace();

	if (the_label_type == TEXT && 
	    (fabs(epix::get_labelangle()) > EPIX_EPSILON))
	  {
	    rbrace();
	  }

	if (the_alignment != none || the_alignment == c)
	  rbrace();
      } // end of is_visible
  } // end of Label::draw()

} /* end of namespace */

