/* 
 * objects.cc -- Simple picture objects: axes, grids, markers, polygons
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc13
 * Last Change: July 08, 2004
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
#include <sstream>
#include <cstdarg>

#include "globals.h"
#include "triples.h"

#include "camera.h"
#include "output.h"
#include "curves.h"
#include "Label.h"
#include "functions.h"
#include "objects.h"

namespace ePiX {

  extern double pic_size;
  extern char *pic_unit;

  extern epix_camera camera;

  // Picture objects

  // label -- put basepoint-aligned string constant <label_text> 
  // at Cartesian position <base> translated by <offset> true points.
  void label(P base, P offset, std::string lbl)
  {
    Label(base, lbl, TEXT, none, false, offset).draw();
  }
  void masklabel(P base, P offset, std::string lbl)
  {
    Label(base, lbl, TEXT, none, true, offset).draw();
  }

  void label(P base, P offset, 
	     std::string lbl, epix_label_posn POSN)
  {
    Label(base, lbl, TEXT, POSN, false, offset).draw();
  }
  void masklabel(P base, P offset, 
		 std::string lbl, epix_label_posn POSN)
  {
    Label(base, lbl, TEXT, POSN, true, offset).draw();
  }

  // centered labels
  void label(P base, std::string lbl)
  {
    Label(base, lbl, TEXT, c).draw();
  }

  void
  masklabel(P base, std::string lbl)
  {
    Label(base, lbl, TEXT, c, true).draw();
  }

  // Marker aliases
  void marker(P arg, epix_mark_type TYPE)
  {
    Label(arg, "", TYPE, c).draw();
  }

  void circ(P arg)
  {
    Label(arg, "", CIRC, c).draw();
  }  

  void ring(P arg)
  {
    Label(arg, "", RING, c).draw();
  }  

  void spot(P arg)
  {
    Label(arg, "", SPOT, c).draw();
  }  

  void dot(P arg)
  {
    Label(arg, "", DOT, c).draw();
  }

  void ddot(P arg)
  {
    Label(arg, "", DDOT, c).draw();
  }

  void box(P arg)
  {
    Label(arg, "", BOX, c).draw();
  }

  void bbox(P arg)
  {
    Label(arg, "", BBOX, c).draw();
  }

  // Axis ticks (h_tick = tall, thin rectangle, for a *horizontal* axis)
  void h_axis_tick(P arg) 
  { 
    Label(arg, "", H_TICK, c).draw();
  }
  void v_axis_tick(P arg) 
  { 
    Label(arg, "", V_TICK, c).draw();
  }

  // Coordinate axes
  static void 
  draw_axis(P tail, P head, int n, int num_pts, epix_tick_type TICK)
  {
    line(tail, head, 0, num_pts);

    P location = tail;
    P step  = (1.0/n)*(head - tail); // print n tick marks

    if (TICK == H_AXIS) // test axis type outside loop
      for (int i=0; i <= n; ++i, location += step)
	h_axis_tick(location);

    else if (TICK == V_AXIS)
      for (int i=0; i <= n; ++i, location += step)
	v_axis_tick(location);

    else // TICK == NULL or unknown
      ;

    end_stanza();
  }

  // Suitable for curved axes
  void h_axis(P tail, P head, int n, int num_pts)
  {
    draw_axis(tail, head, n, num_pts, H_AXIS);
  }
  void v_axis(P tail, P head, int n, int num_pts)
  {
    draw_axis(tail, head, n, num_pts, V_AXIS);
  }

  /*
   * h_axis_labels: Draws n+1 equally-spaced axis labels between 
   *    <tail> and <head>. Automatically generates label values from
   *    Cartesian coordinates, and has true-distance offsets.
   *
   * There is a separate version that takes a LaTeX-style alignment argument.
   * The "unaligned" version tries to do constant alignment with labels that
   * may or may not have a sign. It was easier to write a new function to
   * handle labels with an alignment option than to modify the existing one.
   */

  static void
  draw_axis_labels(P tail, P head, int n, P offset, 
		   epix_tick_type TICK, bool mask)
  {
    bool neg_flag;
    double label;
    P current = tail;
    P step = (1.0/n)*(head - tail);

    // determine if labels change sign (may be needed for alignment)
    if (TICK == H_AXIS && head.x1()*tail.x1() < 0)
      neg_flag=true;
    else if (TICK == V_AXIS && head.x2()*tail.x2() < 0)
      neg_flag=true;
    else
      neg_flag=false;

    for (int i=0; i<= n; ++i, current += step)
      if (is_visible(current))
	{
	  // Compute proper type of label
	  if (TICK == H_AXIS)
	    label = current.x1();

	  else if (TICK == V_AXIS)
	    label = current.x2();

	  else // stub for future "z-axis" labelling...
	    ;
	  newl();
	  epix_put();
	  print(current, offset);

	  // Put sign on label?
	  if (neg_flag && label >= 0)
	    {	      
	      lbrace();
	      dollar();
	      epix_neg(); // Let TeX worry about the non-existent "-"
	      // mask label?
	      if (mask)
		{
		  epix_colorbox();
		  epix_math_grouping(label);
		}
	      else
		epix_grouping(label);

	      dollar();
	      rbrace();
	    }
	  else
	    {
	      if (mask)
		{
		  lbrace();
		  epix_colorbox();
		}

	      epix_math_grouping(label);

	      if (mask)
		rbrace();
	    }
	} // end of is_visible(current)

    end_stanza();
  }

  void h_axis_labels(P tail, P head, int n, P offset)
  {
    draw_axis_labels(tail, head, n, offset, H_AXIS, false);
  }
  void v_axis_labels(P tail, P head, int n, P offset)
  {
    draw_axis_labels(tail, head, n, offset, V_AXIS, false);
  }
  void h_axis_masklabels(P tail, P head, int n, P offset)
  {
    draw_axis_labels(tail, head, n, offset, H_AXIS, true);
  }
  void v_axis_masklabels(P tail, P head, int n, P offset)
  {
    draw_axis_labels(tail, head, n, offset, V_AXIS, true);
  }

  static double coord1(P arg) { return arg.x1(); }
  static double coord2(P arg) { return arg.x2(); }
  // axis labels with alignment option
  static void
  draw_axis_labels(P tail, P head, int n, P offset, 
		   epix_tick_type TICK, bool mask, epix_label_posn POSN)
  {
    //    double label;
    P current = tail;
    P step = (1.0/n)*(head-tail);

    for (int i=0; i<= n; ++i, current += step)
      {
	// Compute proper type of label
	if (TICK == H_AXIS)
	  Label(current, coord1, POSN, mask, offset).draw();
	else // V_AXIS
	  Label(current, coord2, POSN, mask, offset).draw();
      }
  }

  void h_axis_labels(P tail, P head, int n, P offset, 
		     epix_label_posn POSN)
  {
    draw_axis_labels(tail, head, n, offset, H_AXIS, false, POSN);
  }
  void v_axis_labels(P tail, P head, int n, P offset, 
		     epix_label_posn POSN)
  {
    draw_axis_labels(tail, head, n, offset, V_AXIS, false, POSN);
  }
  void h_axis_masklabels(P tail, P head, int n, P offset, 
			 epix_label_posn POSN)
  {
    draw_axis_labels(tail, head, n, offset, H_AXIS, true, POSN);
  }
  void v_axis_masklabels(P tail, P head, int n, P offset, 
			 epix_label_posn POSN)
  {
    draw_axis_labels(tail, head, n, offset, V_AXIS, true, POSN);
  }

} /* end of namespace */
