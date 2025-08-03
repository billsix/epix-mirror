/* 
 * markers.h -- ePiX markers and labels
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.16
 * Last Change: September 09, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*
 * label -- put string constant <label_text> at Cartesian position <base>
 * translated by (offset.x1, offset.x2) true points (i.e., 3rd component
 * of <offset> is discarded).
 *
 * Accepts an optional LaTeX-style positioning argument.
 * If no offset is specified, the label is centered at the given Cartesian
 * location.
 * masklabel requires the "color" package, and places the text in 
 * a white box that masks whatever is underneath and earlier in the file.
 */
#ifndef EPIX_MARKERS
#define EPIX_MARKERS

#include <string>

#include "enums.h"

namespace ePiX {

  class P;

  // position, [offset], label text, [alignment]
  void label(const P&, const P&, const std::string&);
  void label(const P&, const std::string&);
  void label(const P&, const P&, const std::string&, epix_label_posn);

  void masklabel(const P&, const P&, const std::string&);
  void masklabel(const P&, const std::string&);
  void masklabel(const P&, const P&, const std::string&, epix_label_posn);


  // Empty and filled LaTeX circles of diameter get_dotsize() true pt
  void marker (const P&, epix_mark_type);

  void circ(const P& posn, const P& offset=P(0,0),
	    const std::string& label_text="",
	    epix_label_posn align=none); // filled white circ

  void ring(const P& posn, const P& offset=P(0,0),
	    const std::string& label_text="",
	    epix_label_posn align=none); // unfilled circ 

  void spot(const P& posn, const P& offset=P(0,0),
	    const std::string& label_text="",
	    epix_label_posn align=none);

  void dot(const P& posn, const P& offset=P(0,0),
	   const std::string& label_text="",
	   epix_label_posn align=none);

  void ddot(const P& posn, const P& offset=P(0,0),
	    const std::string& label_text="",
	    epix_label_posn align=none);

  void box(const P& posn, const P& offset=P(0,0),
	   const std::string& label_text="",
	   epix_label_posn align=none);

  void bbox(const P& posn, const P& offset=P(0,0),
	    const std::string& label_text="",
	    epix_label_posn align=none);

  void h_axis_tick(const P& location, epix_label_posn align=c);
  void v_axis_tick(const P& location, epix_label_posn align=c);

  // Arrow with aligned label at tail
  void arrow(const P& tail, const P& head, const P& offset,
	     const std::string& label_text,
	     epix_label_posn align, double scale=1);

  //// Path-like glyphs ////
  // angle indicators; default args are true sizes in pt
  void right_angle(const P& loc, P leg1, P leg2, double scale=8);
  void arc_measure(const P& loc, P leg1, P leg2,
		   const P& offset, const std::string&, epix_label_posn,
		   double scale=8);
  void arc_measure(const P& loc, P leg1, P leg2, double scale=8);

  void axis_break(const P& tail, const P& head, double scale=12);
  void h_error_bar(const P& loc, double err, epix_mark_type mk, double ht=6);
  void v_error_bar(const P& loc, double err, epix_mark_type mk, double wd=6);
} // end of namespace

#endif /* EPIX_MARKERS */
