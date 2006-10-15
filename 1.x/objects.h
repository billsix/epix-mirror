/* 
 * objects.h -- ePiX axes, grids, markers, and labels
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
#ifndef EPIX_OBJECTS
#define EPIX_OBJECTS

#include <string>

#include "globals.h"
#include "triples.h"
#include "cropping.h"

namespace ePiX {

  void label(const P& base, const P& offset, std::string label_text);
  void label(const P& base, std::string label_text);
  void label(const P& base, const P& offset, std::string label_text,
	     epix_label_posn);

  void masklabel(const P& base, const P& offset, std::string label_text);
  void masklabel(const P& base, std::string label_text);
  void masklabel(const P& base, const P& offset, std::string label_text,
		 epix_label_posn);

  // Empty and filled LaTeX circles of diameter get_dotsize() true pt
  void circ(const P& posn, const P& offset=P(0,0), std::string label_text="",
	    epix_label_posn align=none); // filled white circ

  void ring(const P& posn, const P& offset=P(0,0), std::string label_text="",
	    epix_label_posn align=none); // unfilled circ 

  void spot(const P& posn, const P& offset=P(0,0), std::string label_text="",
	    epix_label_posn align=none);

  void dot(const P& posn, const P& offset=P(0,0), std::string label_text="",
	    epix_label_posn align=none);

  void ddot(const P& posn, const P& offset=P(0,0), std::string label_text="",
	    epix_label_posn align=none);

  void box(const P& posn, const P& offset=P(0,0), std::string label_text="",
	    epix_label_posn align=none);

  void bbox(const P& posn, const P& offset=P(0,0), std::string label_text="",
	    epix_label_posn align=none);

  void arrow(const P& tail, const P& head, const P& offset,
	     std::string label_text,
	     epix_label_posn align, double scale=1);

  void marker (const P&, epix_mark_type);

  // Axes and coordinate grids.

  // Coordinate axes, specified by initial and final points, number of
  // tick marks, and (optionally) number of points (for cameras that do
  // not map lines to lines). Generally num_pts should be a multiple of n.
  // h/v_axis are identical except for style of tick marks.

  void h_axis_tick(const P& location);
  void v_axis_tick(const P& location);

  // n+1 = #ticks, num_pts = #segments used to draw
  void h_axis(const P& tail, const P& head, int n, int num_pts=1);
  void v_axis(const P& tail, const P& head, int n, int num_pts=1);

  // Endpoints (x_min,0), x_max,0), etc.
  void h_axis(int n=x_size);
  void v_axis(int n=y_size);

  // n+1 = #ticks, num_pts = #segments used to draw
  void h_axis(int n, int num_pts);
  void v_axis(int n, int num_pts);


  // h_axis_labels: Draws n+1 equally-spaced axis labels between 
  //   <tail> and <head>. Automatically generates label values from
  //   Cartesian coordinates, and offsets labels in true pt.

  void h_axis_labels(const P& tail, const P& head, int n, const P& offset);
  void v_axis_labels(const P& tail, const P& head, int n, const P& offset);

  void h_axis_masklabels(const P& tail, const P& head, int n, const P& offset);
  void v_axis_masklabels(const P& tail, const P& head, int n, const P& offset);

  // Axis labels with LaTeX-style alignment option

  void h_axis_labels(const P& tail, const P& head, int n, 
		     const P& offset, epix_label_posn POSN);
  void v_axis_labels(const P& tail, const P& head, int n, 
		     const P& offset, epix_label_posn POSN);

  void h_axis_masklabels(const P& tail, const P& head, int n, 
			 const P& offset, epix_label_posn POSN);
  void v_axis_masklabels(const P& tail, const P& head, int n, 
			 const P& offset, epix_label_posn POSN);

  // Axis labels with default endpoints
  void h_axis_labels(int n, const P& offset);
  void h_axis_masklabels(int n, const P& offset);

  void h_axis_labels(int n, const P& offset, epix_label_posn POSN);
  void h_axis_masklabels(int n, const P& offset, epix_label_posn POSN);

  void v_axis_labels(int n, const P& offset);
  void v_axis_masklabels(int n, const P& offset);

  void v_axis_labels(int n, const P& offset, epix_label_posn POSN);
  void v_axis_masklabels(int n, const P& offset, epix_label_posn POSN);

} // end of namespace

#endif /* EPIX_OBJECTS */
