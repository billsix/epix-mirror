/* 
 * objects.h -- Simple picture objects: axes, grids, markers, polygons
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc12
 * Last Change: July 01, 2004
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

#ifndef EPIX_OBJECTS
#define EPIX_OBJECTS

#include <cstdarg>
#include <sstream>

#include "globals.h"
#include "triples.h"
#include "cropping.h"

namespace ePiX {

  // Picture objects

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
  // location given as a <P>
  void label(P base, P offset, std::string label_text);
  void label(P base, std::string label_text);
  void label(P base, P offset, std::string label_text,
	     epix_label_posn);

  void masklabel(P base, P offset, std::string label_text);
  void masklabel(P base, std::string label_text);
  void masklabel(P base, P offset, std::string label_text,
		 epix_label_posn);

  // Empty and filled LaTeX circles of diameter get_dotsize() true pt
  void circ(P); // filled white circ
  void ring(P); // unfilled circ 

  void spot(P arg);
  void dot(P arg);
  void ddot(P arg);

  void box(P arg);
  void bbox(P arg);

  void marker (P, epix_mark_type);

  // Axes and coordinate grids.

  /* 
   * Coordinate axes, specified by initial and final points, number of
   * tick marks, and (optionally) number of points (for cameras that do
   * not map lines to lines). Generally num_pts should be a multiple of n.
   * h/v_axis are identical except for style of tick marks.
   */
  void h_axis_tick(P location);
  void v_axis_tick(P location);

  //  void h_axis(P tail, P head, int n);
  //  void v_axis(P tail, P head, int n);

  // n+1 = #ticks, num_pts = #segments used to draw
  void h_axis(P tail, P head, int n, int num_pts=1);
  void v_axis(P tail, P head, int n, int num_pts=1);

  // These versions may be used with gcc 2.96 and later
  inline void h_axis(int n=x_size) { h_axis(P(x_min,0), P(x_max,0), n); }
  inline void v_axis(int n=y_size) { v_axis(P(0,y_min), P(0,y_max), n); }

  // n+1 = #ticks, num_pts = #segments used to draw
  inline void h_axis(int n, int num_pts) 
    { h_axis(P(x_min,0), P(x_max,0), n, num_pts); }
  inline void v_axis(int n, int num_pts) 
    { v_axis(P(0,y_min), P(0,y_max), n, num_pts); }

  /*
   * h_axis_labels: Draws n+1 equally-spaced axis labels between 
   *    <tail> and <head>. Automatically generates label values from
   *    Cartesian coordinates, and offsets labels in true pt.
   */

  void h_axis_labels(P tail, P head, int n, P offset);
  void v_axis_labels(P tail, P head, int n, P offset);

  void h_axis_masklabels(P tail, P head, int n, P offset);
  void v_axis_masklabels(P tail, P head, int n, P offset);

  // Axis labels with LaTeX-style alignment option

  void h_axis_labels(P tail, P head, int n, 
		     P offset, epix_label_posn POSN);
  void v_axis_labels(P tail, P head, int n, 
		     P offset, epix_label_posn POSN);

  void h_axis_masklabels(P tail, P head, int n, 
			 P offset, epix_label_posn POSN);
  void v_axis_masklabels(P tail, P head, int n, 
			 P offset, epix_label_posn POSN);

  // Axis labels with default endpoints
  inline void h_axis_labels(int n, P offset)
    { h_axis_labels(P(x_min,0), P(x_max,0), n, offset); }
  inline void h_axis_masklabels(int n, P offset)
    { h_axis_masklabels(P(x_min,0), P(x_max,0), n, offset); }

  inline void h_axis_labels(int n, P offset, epix_label_posn POSN)
    { h_axis_labels(P(x_min,0), P(x_max,0), n, offset, POSN); }
  inline void h_axis_masklabels(int n, P offset, epix_label_posn POSN)
    { h_axis_masklabels(P(x_min,0), P(x_max,0), n, offset, POSN); }


  inline void v_axis_labels(int n, P offset)
    { v_axis_labels(P(0,y_min), P(0,y_max), n, offset); }
  inline void v_axis_masklabels(int n, P offset)
    { v_axis_masklabels(P(0,y_min), P(0,y_max), n, offset); }

  inline void v_axis_labels(int n, P offset, epix_label_posn POSN)
    { v_axis_labels(P(0,y_min), P(0,y_max), n, offset, POSN); }
  inline void v_axis_masklabels(int n, P offset, epix_label_posn POSN)
    { v_axis_masklabels(P(0,y_min), P(0,y_max), n, offset, POSN); }

} /* end of namespace */

#endif /* EPIX_OBJECTS */
