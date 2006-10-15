/* 
 * objects.cc -- ePiX axes, grids, markers, and labels
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

#include "globals.h"
#include "triples.h"

#include "output.h"

#include "curves.h"
#include "Label.h"
#include "functions.h"
#include "objects.h"

namespace ePiX {

  extern double pic_size;
  extern char *pic_unit;

  // Picture objects

  // label -- put basepoint-aligned string constant <label_text> 
  // at Cartesian position <base> translated by <offset> true points.
  void label(const P& base, const P& offset, std::string lbl)
  {
    Label(base, lbl, TEXT, none, false, offset).draw();
  }
  void masklabel(const P& base, const P& offset, std::string lbl)
  {
    Label(base, lbl, TEXT, none, true, offset).draw();
  }

  void label(const P& base, const P& offset, 
	     std::string lbl, epix_label_posn POSN)
  {
    Label(base, lbl, TEXT, POSN, false, offset).draw();
  }
  void masklabel(const P& base, const P& offset, 
		 std::string lbl, epix_label_posn POSN)
  {
    Label(base, lbl, TEXT, POSN, true, offset).draw();
  }

  // centered labels
  void label(const P& base, std::string lbl)
  {
    Label(base, lbl, TEXT, c).draw();
  }

  void masklabel(const P& base, std::string lbl)
  {
    Label(base, lbl, TEXT, c, true).draw();
  }

  // Marker aliases
  void marker(const P& arg, epix_mark_type TYPE)
  {
    Label(arg, "", TYPE, c).draw();
  }

  // Markers with labels
  void circ(const P& posn, const P& offset, std::string label_text,
	    epix_label_posn align)
  {
    if (label_text != "")
      label(posn, offset, label_text, align);
    Label(posn, "", CIRC, c).draw();
  }

  void ring(const P& posn, const P& offset, std::string label_text,
	    epix_label_posn align)
  {
    if (label_text != "")
      label(posn, offset, label_text, align);
    Label(posn, "", RING, c).draw();
  }

  void spot(const P& posn, const P& offset, std::string label_text,
	    epix_label_posn align)
  {
    if (label_text != "")
      label(posn, offset, label_text, align);
    Label(posn, "", SPOT, c).draw();
  }

  void dot(const P& posn, const P& offset, std::string label_text,
	   epix_label_posn align)
  {
    if (label_text != "")
      label(posn, offset, label_text, align);
    Label(posn, "", DOT, c).draw();
  }

  void ddot(const P& posn, const P& offset, std::string label_text,
	    epix_label_posn align)
  {
    if (label_text != "")
      label(posn, offset, label_text, align);
    Label(posn, "", DDOT, c).draw();
  }

  void box(const P& posn, const P& offset, std::string label_text,
	   epix_label_posn align)
  {
    if (label_text != "")
      label(posn, offset, label_text, align);
    Label(posn, "", BOX, c).draw();
  }

  void bbox(const P& posn, const P& offset, std::string label_text,
	    epix_label_posn align)
  {
    if (label_text != "")
      label(posn, offset, label_text, align);
    Label(posn, "", BBOX, c).draw();
  }

  // arrow with label
  void arrow(const P& tail, const P& head, const P& offset,
	     std::string label_text,
	     epix_label_posn align, double scale)
  {
    arrow(tail, head, scale);
    label(tail, offset, label_text, align);
  }

  // Axis ticks (h_tick = tall, thin rectangle, for a *horizontal* axis)
  void h_axis_tick(const P& arg) 
  { 
    Label(arg, "", H_TICK, c).draw();
  }
  void v_axis_tick(const P& arg) 
  { 
    Label(arg, "", V_TICK, c).draw();
  }

  // Coordinate axes
  void draw_axis(const P& tail, const P& head,
		 int n, int num_pts, epix_tick_type TICK)
  {
    line(tail, head, 0, num_pts);

    P location(tail);
    const P step((1.0/n)*(head - tail)); // print n tick marks

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
  void h_axis(const P& tail, const P& head, int n, int num_pts)
  {
    draw_axis(tail, head, n, num_pts, H_AXIS);
  }
  void v_axis(const P& tail, const P& head, int n, int num_pts)
  {
    draw_axis(tail, head, n, num_pts, V_AXIS);
  }

  void h_axis(int n)
  {
    h_axis(P(x_min,0), P(x_max,0), n);
  }
  void v_axis(int n)
  {
    v_axis(P(0,y_min), P(0,y_max), n);
  }

  // n+1 = #ticks, num_pts = #segments used to draw
  void h_axis(int n, int num_pts) 
  {
    h_axis(P(x_min,0), P(x_max,0), n, num_pts);
  }
  void v_axis(int n, int num_pts) 
  {
    v_axis(P(0,y_min), P(0,y_max), n, num_pts);
  }



  // h_axis_labels: Draws n+1 equally-spaced axis labels between 
  //    <tail> and <head>. Automatically generates label values from
  //    Cartesian coordinates, and has true-distance offsets.
  //
  // There is a separate version that takes a LaTeX-style alignment argument.
  // The "unaligned" version tries to do constant alignment with labels that
  // may or may not have a sign. It was easier to write a new function to
  // handle labels with an alignment option than to modify the existing one.

  void draw_axis_labels(const P& tail, const P& head, int n, const P& offset, 
			epix_tick_type TICK, bool mask)
  {
    bool neg_flag;
    double label(0);
    P current(tail);
    P step((1.0/n)*(head - tail));

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

  void h_axis_labels(const P& tail, const P& head, int n, const P& offset)
  {
    draw_axis_labels(tail, head, n, offset, H_AXIS, false);
  }
  void v_axis_labels(const P& tail, const P& head, int n, const P& offset)
  {
    draw_axis_labels(tail, head, n, offset, V_AXIS, false);
  }
  void h_axis_masklabels(const P& tail, const P& head, int n, const P& offset)
  {
    draw_axis_labels(tail, head, n, offset, H_AXIS, true);
  }
  void v_axis_masklabels(const P& tail, const P& head, int n, const P& offset)
  {
    draw_axis_labels(tail, head, n, offset, V_AXIS, true);
  }

  // Needed by draw_axis_labels to pass coordinates to Label constructor
  double coord1(P arg)
  {
    return arg.x1();
  }
  double coord2(P arg)
  {
    return arg.x2();
  }
  // axis labels with alignment option
  void draw_axis_labels(const P& tail, const P& head, int n, const P& offset, 
			epix_tick_type TICK, bool mask, epix_label_posn POSN)
  {
    P current(tail);
    P step((1.0/n)*(head-tail));

    for (int i=0; i<= n; ++i, current += step)
      {
	// Compute proper type of label
	if (TICK == H_AXIS)
	  Label(current, coord1, POSN, mask, offset).draw();
	else // V_AXIS
	  Label(current, coord2, POSN, mask, offset).draw();
      }
  }

  void h_axis_labels(const P& tail, const P& head, int n, const P& offset, 
		     epix_label_posn POSN)
  {
    draw_axis_labels(tail, head, n, offset, H_AXIS, false, POSN);
  }
  void v_axis_labels(const P& tail, const P& head, int n, const P& offset, 
		     epix_label_posn POSN)
  {
    draw_axis_labels(tail, head, n, offset, V_AXIS, false, POSN);
  }
  void h_axis_masklabels(const P& tail, const P& head, int n, const P& offset, 
			 epix_label_posn POSN)
  {
    draw_axis_labels(tail, head, n, offset, H_AXIS, true, POSN);
  }
  void v_axis_masklabels(const P& tail, const P& head, int n, const P& offset, 
			 epix_label_posn POSN)
  {
    draw_axis_labels(tail, head, n, offset, V_AXIS, true, POSN);
  }

  void h_axis_labels(int n, const P& offset)
  {
    h_axis_labels(P(x_min,0), P(x_max,0), n, offset);
  }
  void h_axis_masklabels(int n, const P& offset)
  {
    h_axis_masklabels(P(x_min,0), P(x_max,0), n, offset);
  }

  void h_axis_labels(int n, const P& offset, epix_label_posn POSN)
  {
    h_axis_labels(P(x_min,0), P(x_max,0), n, offset, POSN);
  }
  void h_axis_masklabels(int n, const P& offset, epix_label_posn POSN)
  {
    h_axis_masklabels(P(x_min,0), P(x_max,0), n, offset, POSN);
  }


  void v_axis_labels(int n, const P& offset)
  {
    v_axis_labels(P(0,y_min), P(0,y_max), n, offset);
  }
  void v_axis_masklabels(int n, const P& offset)
  {
    v_axis_masklabels(P(0,y_min), P(0,y_max), n, offset);
  }

  void v_axis_labels(int n, const P& offset, epix_label_posn POSN)
  {
    v_axis_labels(P(0,y_min), P(0,y_max), n, offset, POSN);
  }
  void v_axis_masklabels(int n, const P& offset, epix_label_posn POSN)
  {
    v_axis_masklabels(P(0,y_min), P(0,y_max), n, offset, POSN);
  }
} // end of namespace
