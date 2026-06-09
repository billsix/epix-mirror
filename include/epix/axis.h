/* 
 * axis.h -- Decorable coordinate axis class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.13
 * Last Change: August 23, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
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
#ifndef EPIX_AXIS
#define EPIX_AXIS

#include <string>
#include <set>

#include "enums.h"
#include "triples.h"

namespace ePiX {

  class axis {
  public:
    axis(const P&, const P&, unsigned int n,
	 const P& offset = P(0,0), epix_label_posn POSN=none);

    // attribute-setting
    axis& dec();  // default
    axis& frac(); // fractional labels
    axis& trig(); // fractions times pi
    axis& sci();  // scientific notation

    // f(x, precision, base) returns a string representing x
    axis& label_rep(std::string f(double, unsigned int, unsigned int));

    // unmark selected location (non-log labels only)
    axis& unmark(double arg=0);

    // add or remove logarithmic tags ("sublabels")
    axis& log(unsigned int base=10); // log ticks and labels
    axis& tag(double);               // put label at arg in log mode
    axis& tag235();                  // tags at 2, 3, 5 in log10 mode
    axis& tags();                    // tags at 2, ..., b-1 in logb mode

    axis& untag();                   // remove all log tags
    axis& untag(double);             // remove selected tag

    axis& align_labels(epix_label_posn);
    axis& align(epix_label_posn); // align tick marks

    // number of minor ticks per segment
    axis& subdivide(unsigned int);

    // set length of minor ticks
    axis& tick_ratio(double);

    // set precision for axis labels
    axis& precision(unsigned int digits=0); // 0: reset to default

    void draw_ticks() const;
    void draw_labels() const;
    void draw() const;

  private:
    P m_tail;
    P m_head;

    unsigned int m_major_segs;
    unsigned int m_minor_segs;

    double m_tick_ratio;

    epix_label_posn m_align_ticks;

    P m_offset;
    epix_label_posn m_align_labels;

    unsigned int m_log_base;
    bool m_log;
    unsigned int m_precision;

    double (*m_coord)(const P&);  // value to place at arg

    // represent arg as string
    std::string (*m_rep)(double arg, unsigned int prec, unsigned int base);

    std::set<double> m_omitted;
    std::set<double> m_log_tags;
  }; // end of axis class

  // axes along sides of bounding box, ticks automatically aligned
  axis top_axis(unsigned int n, const P& offset=P(0,0),
		epix_label_posn POSN=t);
  axis bottom_axis(unsigned int n, const P& offset=P(0,0),
		   epix_label_posn POSN=b);
  axis left_axis(unsigned int n, const P& offset=P(0,0),
		 epix_label_posn POSN=l);
  axis right_axis(unsigned int n, const P& offset=P(0,0),
		  epix_label_posn POSN=r);


  // Global functions from Version 1.0
  void h_axis_labels(const P& tail, const P& head, unsigned int n, 
		     const P& offset, epix_label_posn POSN=none);

  void v_axis_labels(const P& tail, const P& head, unsigned int n, 
		     const P& offset, epix_label_posn POSN=none);

  void h_axis_masklabels(const P& tail, const P& head, unsigned int n, 
			 const P& offset, epix_label_posn POSN=none);

  void v_axis_masklabels(const P& tail, const P& head, unsigned int n, 
			 const P& offset, epix_label_posn POSN=none);

  // Axis labels with default endpoints
  void h_axis_labels(unsigned int n, const P& offset, 
		     epix_label_posn POSN=none);
  void h_axis_masklabels(unsigned int n, const P& offset,
			 epix_label_posn POSN=none);

  void v_axis_labels(unsigned int n, const P& offset,
		     epix_label_posn POSN=none);
  void v_axis_masklabels(unsigned int n, const P& offset,
			 epix_label_posn POSN=none);

  //// logarithmic labels ////
  // labels written $k x base^i$ at i + log_b(k)
  void h_axis_log_labels(const P& tail, const P& head, unsigned int n,
			 const P& offset, epix_label_posn POSN=none,
			 unsigned int base=10);

  void v_axis_log_labels(const P& tail, const P& head, unsigned int n,
			 const P& offset, epix_label_posn POSN=none,
			 unsigned int base=10);

  // labels written $base^i$ at coord i
  void h_axis_log_labels(const P& tail, const P& head,
			 const P& offset, epix_label_posn POSN=none,
			 unsigned int base=10);

  void v_axis_log_labels(const P& tail, const P& head,
			 const P& offset, epix_label_posn POSN=none,
			 unsigned int base=10);


  void h_axis_log_masklabels(const P& tail, const P& head, unsigned int n,
			     const P& offset, epix_label_posn POSN=none,
			     unsigned int base=10);

  void v_axis_log_masklabels(const P& tail, const P& head, unsigned int n,
			     const P& offset, epix_label_posn POSN=none,
			     unsigned int base=10);

  void h_axis_log_masklabels(const P& tail, const P& head,
			     const P& offset, epix_label_posn POSN=none,
			     unsigned int base=10);

  void v_axis_log_masklabels(const P& tail, const P& head,
			     const P& offset, epix_label_posn POSN=none,
			     unsigned int base=10);


  // Coordinate axes, specified by initial and final points, number of
  // tick marks. h/v_axis are identical except for style of tick marks.

  // n subintervals
  void h_axis(const P& tail, const P& head, unsigned int n,
	      epix_label_posn align=c);
  void v_axis(const P& tail, const P& head, unsigned int n,
	      epix_label_posn align=c);

  // Default endpoints (xmin(), 0), xmax(), 0), etc.
  void h_axis(unsigned int n = xsize(), epix_label_posn align=c);
  void v_axis(unsigned int n = ysize(), epix_label_posn align=c);

  void h_log_axis(const P& tail, const P& head, unsigned int segs,
		  epix_label_posn align=c, unsigned int base=10);

  void v_log_axis(const P& tail, const P& head, unsigned int segs,
		  epix_label_posn align=c, unsigned int base=10);

} // end of namespace

#endif /* EPIX_AXIS */
