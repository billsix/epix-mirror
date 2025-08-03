/* 
 * axis.cc -- ePiX axis class and global compatibility functions
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.2.5
 * Last Change: May 11, 2008
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008
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
#include <cmath>
#include <sstream>

#include "errors.h"

#include "constants.h"
#include "triples.h"

#include "pairs.h"
#include "frac.h"
#include "functions.h"
#include "utils.h" // for truncate

#include "length.h"
#include "picture.h"

#include "camera.h"

#include "screen.h"
#include "active_screen.h"
#include "curves.h"

#include "label_data.h"

#include "markers.h"
#include "axis.h"

namespace ePiX {

  // functions to get axis label from position
  double __epix_coord1(const P&);
  double __epix_coord2(const P&);
  double __epix_coord3(const P&);

  // string representations of double
  std::string __epix_coord_as_double(double x, unsigned int digits,
				     unsigned int base);
  std::string __epix_coord_as_frac(double x, unsigned int digits,
				   unsigned int base);
  std::string __epix_coord_as_trig_frac(double x, unsigned int digits,
					unsigned int base);
  std::string __epix_coord_as_log(double x, unsigned int digits,
				  unsigned int base);
  std::string __epix_coord_as_log_sci(double x, unsigned int digits,
				  unsigned int base);
  std::string __epix_coord_as_sci(double x, unsigned int digits,
				  unsigned int base);

  //// Member functions ////
  // Constructor automatically selects coordinate function from endpoints
  axis::axis(const P& tail, const P& head, unsigned int n,
	     const P& offset, epix_label_posn POSN)
    : m_tail(tail), m_head(head), m_major_segs(n), m_minor_segs(n),
      m_tick_ratio(0.5), m_align_ticks(c),
      m_offset(offset), m_align_labels(POSN),
      m_log_base(10), m_log(false), m_precision(DEF_PRECISION),
      m_coord(__epix_coord1), m_rep(__epix_coord_as_double)
  {
    m_log_tags.insert(1);
    int count(fabs(head.x1() - tail.x1()) < EPIX_EPSILON ? 0 : 1);
    if (EPIX_EPSILON < fabs(head.x2() - tail.x2()))
      {
	++count;
	m_coord = __epix_coord2;
      }

    if (EPIX_EPSILON < fabs(head.x3() - tail.x3()))
      {
	++count;
	m_coord = __epix_coord3;
      }

    if (1 < count)
      epix_warning("Axis not parallel to a Cartesian axis");

    else if (count == 0)
      epix_warning("Degenerate coordinate axis");
  }


  axis& axis::frac()
  {
    m_rep = __epix_coord_as_frac;
    return *this;
  }

  axis& axis::trig()
  {
    m_rep = __epix_coord_as_trig_frac;
    m_log = false;

    return *this;
  }

  axis& axis::sci()
  {
    if (m_log)
      m_rep = __epix_coord_as_log_sci;
    else
      m_rep = __epix_coord_as_sci;
    return *this;
  }

  // f(x, precision, base) returns a string representing x
  axis& axis::label_rep(std::string f(double, unsigned int, unsigned int))
  {
    m_rep = f;
    return *this;
  }

  axis& axis::unmark(double arg)
  {
    m_omitted.insert(arg);
    return *this;
  }

  axis& axis::log(unsigned int base)
  {
    if (m_rep == __epix_coord_as_sci) // preserve sci notation
      m_rep = __epix_coord_as_log_sci;
    else
      m_rep = __epix_coord_as_log;

    if (2 <= base && base <= MAX_LOG_BASE)
      m_log_base = base;
    else
      epix_warning("axis::log base out of range, ignoring");

    m_log = true;

    return *this;
  }

  // add logarithmic labels
  axis& axis::tag(double arg)
  {
    if (0 < arg && 2 < m_log_base)
      {
	int k((int) ceil(-std::log(arg)/std::log((double)m_log_base)));

	// arg != b^-k
	if (k == (int) floor(1 - std::log(arg)/std::log((double)m_log_base)))
	  m_log_tags.insert(arg*pow(m_log_base, k));
      }

    return *this;
  }

  axis& axis::tag235()
  {
    if (m_log_base == 10)
      {
	m_log_tags.insert(2);
	m_log_tags.insert(3);
	m_log_tags.insert(5);
      }

    return *this;
  }

  axis& axis::tags()
  {
    for (unsigned int i=1; i<m_log_base; ++i)
      m_log_tags.insert(i);

    return *this;
  }

  axis& axis::untag(double arg)
  {
    m_log_tags.erase(arg);
    return *this;
  }

  axis& axis::untag()
  {
    m_log_tags.clear();
    return *this;
  }

  axis& axis::align_labels(epix_label_posn POSN)
  {
    m_align_labels = POSN;
    return *this;
  }

  axis& axis::align(epix_label_posn POSN)
  {
    m_align_ticks = POSN;
    return *this;
  }


  // number of minor ticks per segment
  axis& axis::subdivide(unsigned int n)
  {
    m_minor_segs = m_major_segs*((unsigned int) snip_to(n, 1, MAX_LOG_BASE));
    return *this;
  }

  // set tick length; printed size governed by MIN/MAX_TICK, but 0 significant
  axis& axis::tick_ratio(double ratio)
  {
    m_tick_ratio = snip_to(ratio, 0, 1);
    return *this;
  }


  // set precision for printing axis labels in decimal
  axis& axis::precision(unsigned int digits)
  {
    if (digits == 0)
      m_precision = DEF_PRECISION;

    else
      m_precision = (unsigned int) min(digits, MAX_PRECISION);

    return *this;
  }

  void axis::draw_ticks() const
  {
    const P major_step((1.0/m_major_segs)*(m_head - m_tail));

    const pair screen_dir(cam()(m_head) - cam()(m_tail));

    if (norm(screen_dir) < EPIX_EPSILON) // poke in the eye
      return;

    // else, are we closer to horiz or vert?
    bool nearly_horiz(fabs(screen_dir.x2()) < fabs(screen_dir.x1()));

    // axis
    line(m_tail, m_head);

    // major ticks
    for (unsigned int i=0; i <= m_major_segs; ++i)
      {
	if (nearly_horiz)
	  h_axis_tick(m_tail + i*major_step, m_align_ticks);

	else
	  v_axis_tick(m_tail + i*major_step, m_align_ticks);
      }

    // Cartesian minor ticks
    if (!m_log && m_tick_ratio != 0 && m_major_segs != m_minor_segs)
      {
	// get tick size
	const double old_tick_len(the_mark_size().tick_size());

	the_mark_size().tick_size(m_tick_ratio*old_tick_len);
	const P minor_step((1.0/m_minor_segs)*(m_head - m_tail));

	for (unsigned int i=0; i <= m_minor_segs; ++i)
	  {
	    if (nearly_horiz)
	      h_axis_tick(m_tail + i*minor_step, m_align_ticks);

	    else
	      v_axis_tick(m_tail + i*minor_step, m_align_ticks);
	  }

	the_mark_size().tick_size(old_tick_len); // restore state
      }

    // logarithmic minor ticks
    else if (m_log && m_tick_ratio != 0)
      {
	// get tick size
	const double old_tick_len(the_mark_size().tick_size());

	the_mark_size().tick_size(m_tick_ratio*old_tick_len);

	const double denom(std::log((double)m_log_base));

	for (unsigned int i=0; i < m_major_segs; ++i)
	  for (unsigned int j=1; j<m_log_base; ++j)
	    {
	      if (nearly_horiz)
		h_axis_tick(m_tail + (i+std::log((double)j)/denom)*major_step,
			    m_align_ticks);

	      else
		v_axis_tick(m_tail + (i+std::log((double)j)/denom)*major_step,
			    m_align_ticks);
	    }

	the_mark_size().tick_size(old_tick_len); // restore state
      }
  }

  void axis::draw_labels() const
  {
    const P major_step((1.0/m_major_segs)*(m_head - m_tail));

    if (!m_log)
      for (unsigned int i=0; i <= m_major_segs; ++i)
	{
	  const P loc(m_tail + i*major_step);
	  const double tmp_lbl(truncate(m_coord(loc))); // round to 0 if nec.

	  std::set<double>::const_iterator mp(m_omitted.begin());
	  while (mp != m_omitted.end() && 
		 EPIX_EPSILON < (fabs(tmp_lbl - *mp))) // keep label?
	    ++mp;

	  if (mp != m_omitted.end())
	    continue;

	  // else
	  label_data mk(loc, m_offset,
			m_rep(tmp_lbl, m_precision, m_log_base));

	  mk.align_to(m_align_labels).draw();
	}

    else if (2 < m_log_base) // m_log
      {
	const double denom(std::log((double)m_log_base));

	for (unsigned int i=0; i < m_major_segs; ++i)
	  for (std::set<double>::const_iterator tp=m_log_tags.begin();
	       tp != m_log_tags.end(); ++tp)
	    {
	      double dx(std::log(*tp)/denom);
	      const P posn(m_tail + (i + dx)*major_step);

	      label_data mk(posn, m_offset,
			    m_rep(m_coord(posn), m_precision, m_log_base));

	      mk.align_to(m_align_labels).draw();
	    }

	// final label
	label_data mk(m_head, m_offset,
		      m_rep(m_coord(m_head), m_precision, m_log_base));

	mk.align_to(m_align_labels).draw();
      }
  }

  void axis::draw() const
  {
    draw_ticks();
    draw_labels();
  }


  //// Misc utilities for global functions ////
  // raw Cartesian coordinates
  std::string __epix_axis_label(double x)
  {
    std::stringstream obuf;
    obuf << "$" << x << "$";

    return obuf.str();
  }

  std::string __epix_axis1_label(double x, double y)
  {
    return __epix_axis_label(x);
  }

  std::string __epix_axis2_label(double x, double y)
  {
    return __epix_axis_label(y);
  }

  // log coordinates
  std::string __epix_log_label(double x, const unsigned int base)
  {
    std::stringstream obuf;

    int N((int) ceil(x));

    double k(pow(base, x - N));

    //    obuf.precision(2);
    obuf << "$";
    if (k != 1.0)
      {
	k *= base;
	N -= 1;
	obuf << k << "\\times";
      }

    obuf << base << "^{" << N << "}$";

    return obuf.str();
  }

  std::string __epix_log_axis1_label(double x, double y,
				     const unsigned int base=10)
  {
    return __epix_log_label(x, base);
  }

  std::string __epix_log_axis2_label(double x, double y,
				     const unsigned int base=10)
  {
    return __epix_log_label(y, base);
  }

  //// Global functions ////
  // Draws n+1 equally-spaced axis labels between <tail> and <head>.
  // Uses the string-valued function f to generate label text.
  void __epix_axis_labels(const P& tail, const P& head, unsigned int n,
			  const P& offset, epix_label_posn POSN,
			  std::string f(double, double), bool masked)
  {
    const P step((1.0/n)*(head - tail));

    // get mask color, if any
    Color my_mask(masked ? the_label_style().mask_color() : Neutral());

    if (masked && my_mask.is_unset())
      my_mask = White();

    for (unsigned int i=0; i<= n; ++i)
      {
	label_data mk(tail+i*step, offset, f);

	mk.mask_color(my_mask).align_to(POSN).draw();
      }
  }


  //// User-visible commands ////
  // Not implemented with the axis class because masking behavior differs.
  void h_axis_labels(const P& tail, const P& head, unsigned int n,
		     const P& offset, epix_label_posn POSN)
  {
    __epix_axis_labels(tail, head, n, offset, POSN, __epix_axis1_label, false);
  }

  void v_axis_labels(const P& tail, const P& head, unsigned int n,
		     const P& offset, epix_label_posn POSN)
  {
    __epix_axis_labels(tail, head, n, offset, POSN, __epix_axis2_label, false);
  }


  void h_axis_masklabels(const P& tail, const P& head, unsigned int n,
			 const P& offset, epix_label_posn POSN)
  {
    __epix_axis_labels(tail, head, n, offset, POSN, __epix_axis1_label, true);
  }

  void v_axis_masklabels(const P& tail, const P& head, unsigned int n,
			 const P& offset, epix_label_posn POSN)
  {
    __epix_axis_labels(tail, head, n, offset, POSN, __epix_axis2_label, true);
  }


  // automatic endpoints
  void h_axis_labels(unsigned int n, const P& offset, epix_label_posn POSN)
  {
    __epix_axis_labels(P(active_screen()->h_min(), 0),
		       P(active_screen()->h_max(), 0), n, offset, POSN,
		       __epix_axis1_label, false);
  }

  void h_axis_masklabels(unsigned int n, const P& offset, epix_label_posn POSN)
  {
    __epix_axis_labels(P(active_screen()->h_min(), 0),
		       P(active_screen()->h_max(), 0), n, offset, POSN,
		       __epix_axis1_label, true);
  }

  void v_axis_labels(unsigned int n, const P& offset, epix_label_posn POSN)
  {
    __epix_axis_labels(P(0, active_screen()->v_min()),
		       P(0, active_screen()->v_max()), n, offset, POSN,
		       __epix_axis2_label, false);
  }

  void v_axis_masklabels(unsigned int n, const P& offset, epix_label_posn POSN)
  {
    __epix_axis_labels(P(0, active_screen()->v_min()),
		       P(0, active_screen()->v_max()), n, offset, POSN,
		       __epix_axis2_label, true);
  }


  // log axis labels
  void __epix_log_axis_labels(const P& tail, const P& head, unsigned int n,
			      const P& offset, epix_label_posn POSN,
			      std::string f(double, double, unsigned int),
			      bool masked, unsigned int base=10)
  {
    const P step((1.0/n)*(head - tail));

    // get mask color, if any
    Color my_mask(masked ? the_label_style().mask_color() : Neutral());

    if (masked && my_mask.is_unset())
      my_mask = White();

    for (unsigned int i=0; i<= n; ++i)
      {
	P here(tail+i*step);
	std::string text(f(here.x1(), here.x2(), base));

	label_data mk(here, offset, text);

	mk.mask_color(my_mask).align_to(POSN).draw();
      }
  }

  // number of labels specified
  void h_axis_log_labels(const P& tail, const P& head, unsigned int n,
			 const P& offset, epix_label_posn POSN,
			 unsigned int base)
  {
    __epix_log_axis_labels(tail, head, n, offset, POSN, 
			   __epix_log_axis1_label, false, base);
  }

  void v_axis_log_labels(const P& tail, const P& head, unsigned int n,
			 const P& offset, epix_label_posn POSN,
			 unsigned int base)
  {
    __epix_log_axis_labels(tail, head, n, offset, POSN, 
			   __epix_log_axis2_label, false, base);
  }


  // put labels at integer coordinates
  void h_axis_log_labels(const P& tail, const P& head,
			 const P& offset, epix_label_posn POSN,
			 unsigned int base)
  {
    if (EPIX_EPSILON < (tail.x2() - head.x2()) ) // non-horizontal axis
      return;

    const double my_y(tail.x2());

    // get integers in horiz interval
    const int k_lo((int)  ceil(min(tail.x1(), head.x1())));
    const int k_hi((int) floor(max(tail.x1(), head.x1())));

    h_axis_log_labels(P(k_lo, my_y), P(k_hi, my_y), k_hi - k_lo,
		      offset, POSN, base);
  }

  void v_axis_log_labels(const P& tail, const P& head,
			 const P& offset, epix_label_posn POSN,
			 unsigned int base)
  {
    if (EPIX_EPSILON < (tail.x1() - head.x1()) ) // non-vertical axis
      return;

    const double my_x(tail.x1());

    // get integers in vert interval
    const int k_lo((int)  ceil(min(tail.x2(), head.x2())));
    const int k_hi((int) floor(max(tail.x2(), head.x2())));

    v_axis_log_labels(P(my_x, k_lo), P(my_x, k_hi), k_hi - k_lo,
		      offset, POSN, base);
  }


  //// masked versions
  void h_axis_log_masklabels(const P& tail, const P& head, unsigned int n,
			     const P& offset, epix_label_posn POSN,
			     unsigned int base)
  {
    __epix_log_axis_labels(tail, head, n, offset, POSN, 
			   __epix_log_axis1_label, true, base);
  }

  void v_axis_log_masklabels(const P& tail, const P& head, unsigned int n,
			     const P& offset, epix_label_posn POSN,
			     unsigned int base)
  {
    __epix_log_axis_labels(tail, head, n, offset, POSN, 
			   __epix_log_axis2_label, true, base);
  }


  // put labels at integer coordinates
  void h_axis_log_masklabels(const P& tail, const P& head,
			     const P& offset, epix_label_posn POSN,
			     unsigned int base)
  {
    if (EPIX_EPSILON < (tail.x2() - head.x2()) ) // non-horizontal axis
      return;

    const double my_y(tail.x2());

    // get integers in horiz interval
    const int k_lo((int)  ceil(min(tail.x1(), head.x1())));
    const int k_hi((int) floor(max(tail.x1(), head.x1())));

    h_axis_log_masklabels(P(k_lo, my_y), P(k_hi, my_y), k_hi - k_lo,
			  offset, POSN, base);
  }

  void v_axis_log_masklabels(const P& tail, const P& head,
			     const P& offset, epix_label_posn POSN,
			     unsigned int base)
  {
    if (EPIX_EPSILON < (tail.x1() - head.x1()) ) // non-vertical axis
      return;

    const double my_x(tail.x1());

    // get integers in vert interval
    const int k_lo((int)  ceil(min(tail.x2(), head.x2())));
    const int k_hi((int) floor(max(tail.x2(), head.x2())));

    v_axis_log_masklabels(P(my_x, k_lo), P(my_x, k_hi), k_hi - k_lo,
			  offset, POSN, base);
  }


  // Coordinate axes
  void h_axis(const P& tail, const P& head, unsigned int n, epix_label_posn AL)
  {
    line(tail, head);

    const P step((1.0/n)*(head - tail));

    for (unsigned int i=0; i <= n; ++i)
      h_axis_tick(tail + i*step, AL);
  }

  void v_axis(const P& tail, const P& head, unsigned int n, epix_label_posn AL)
  {
    line(tail, head);

    const P step((1.0/n)*(head - tail));

    for (unsigned int i=0; i <= n; ++i)
      v_axis_tick(tail + i*step, AL);
  }

  // n+1 = #ticks, num_pts = #segments used to draw
  void h_axis(unsigned int n, epix_label_posn AL)
  {
    h_axis(P(active_screen()->h_min(), 0),
	   P(active_screen()->h_max(), 0), n, AL);
  }

  void v_axis(unsigned int n, epix_label_posn AL)
  {
    v_axis(P(0, active_screen()->v_min()),
	   P(0, active_screen()->v_max()), n, AL);
  }


  void h_log_axis(const P& tail, const P& head,
		  unsigned int segs, epix_label_posn AL, unsigned int base)
  {
    line(tail, head, 0, segs);

    const P step((1.0/segs)*(head - tail));
    const double denom(log(base));

    for (unsigned int i=0; i < segs; ++i)
      for (unsigned int j=1; j<base; ++j)
	h_axis_tick(tail + (i+log(j)/denom)*step, AL);

    h_axis_tick(head, AL);
  }

  void v_log_axis(const P& tail, const P& head,
		  unsigned int segs, epix_label_posn AL, unsigned int base)
  {
    line(tail, head, 0, segs);

    const P step((1.0/segs)*(head - tail));
    const double denom(log(base));

    for (unsigned int i=0; i < segs; ++i)
      for (unsigned int j=1; j<base; ++j)
	v_axis_tick(tail + (i+log(j)/denom)*step, AL);

    v_axis_tick(head, AL);
  }


  //// New global functions ////
  axis top_axis(unsigned int n, const P& offset, epix_label_posn POSN)
  {
    axis A(active_screen()->tl(), active_screen()->tr(), n, offset, POSN);
    A.align(b);
    return A;
  }

  axis bottom_axis(unsigned int n, const P& offset, epix_label_posn POSN)
  {
    axis A(active_screen()->bl(), active_screen()->br(), n, offset, POSN);
    A.align(t);
    return A;
  }

  axis left_axis(unsigned int n, const P& offset, epix_label_posn POSN)
  {
    axis A(active_screen()->bl(), active_screen()->tl(), n, offset, POSN);
    A.align(r);
    return A;
  }

  axis right_axis(unsigned int n, const P& offset, epix_label_posn POSN)
  {
    axis A(active_screen()->br(), active_screen()->tr(), n, offset, POSN);
    A.align(l);
    return A;
  }


  //// Formatting functions for ePiX::axis ////
  // standard functions to get axis label from position
  double __epix_coord1(const P& arg)
  {
    return arg.x1();
  }

  double __epix_coord2(const P& arg)
  {
    return arg.x2();
  }

  double __epix_coord3(const P& arg)
  {
    return arg.x3();
  }


  // String representations of double
  // triples separated by commas
  std::string __epix_coord_as_double(double x, unsigned int digits,
				     unsigned int base)
  {
    std::stringstream obuf;
    obuf.precision(digits);

    obuf << "$" << x << "$";

    if (fabs(x) < 1000)
      return obuf.str();

    // else insert commas
    std::string val(obuf.str());

    std::string::size_type i(val.find_first_of("."));

    if (i == std::string::npos)
      i = val.size()-1;

    while (4 < i)
      {
	i -= 3;
	val.insert(i, ",");
      }

    return val;
  }

  // [-]\frac{p}{q}
  std::string __epix_coord_as_frac(double x, unsigned int digits,
				   unsigned int base)
  {
    ePiX::frac xfrac(x);

    std::stringstream obuf;
    obuf.precision(digits);

    obuf << "$";

    if (xfrac.is_int())
      obuf << xfrac.num() << "$";

    else
      {
	if (xfrac.num() < 0) // x may have rounded to zero
	  obuf << "-";

	obuf << "\\frac{" << abs(xfrac.num()) << "}"
	     << "{" << xfrac.denom() << "}$";
      }

    return obuf.str();
  }

  // [-]\frac{p\pi}{q}
  std::string __epix_coord_as_trig_frac(double x, unsigned int digits,
					unsigned int base)
  {
    ePiX::frac xfrac(x/M_PI);

    int xnum(xfrac.num());

    std::stringstream obuf;
    obuf.precision(digits);

    obuf << "$";

    if (xfrac.is_int())
      {
	if (xnum == 0)
	  obuf << 0 << "$";

	else if (xnum == 1)
	  obuf << "\\pi$";

	else if (xnum == -1)
	  obuf << "-\\pi$";

	else
	  obuf << xfrac.num() << "\\pi$";
      }

    else // xfrac not an int
      {
	if (xfrac.num() < 0)
	  obuf << "-";

	if (abs(xnum) == 1)
	  obuf << "\\frac{\\pi}{" << xfrac.denom() << "}$";

	else
	  obuf << "\\frac{" << abs(xnum) << "\\pi}"
	       << "{" << xfrac.denom() << "}$";
      }

    return obuf.str();
  }

  // b^x
  std::string __epix_coord_as_log(double x, unsigned int digits,
				  unsigned int base)
  {
    std::stringstream obuf;
    obuf.precision(digits);

    obuf << "$" << pow(base, x) << "$";

    return obuf.str();
  }

  // x = N + k, write as eval(b^k) \times b^N
  std::string __epix_coord_as_log_sci(double x, unsigned int digits,
				      unsigned int base)
  {
    std::stringstream obuf;
    obuf.precision(digits);

    int N((int) ceil(x));

    double k(pow(base, x - N));

    obuf << "$";
    if (k != 1.0)
      {
	k *= base;
	N -= 1;
	obuf << k << "\\times";
      }

    obuf << base << "^{" << N << "}$";

    return obuf.str();
  }

  std::string __epix_coord_as_sci(double x, unsigned int digits,
				  unsigned int base)
  {
    std::stringstream obuf;
    obuf.precision(digits);

    int sgn(x < 0 ? -1 : 1);
    x = fabs(x);
    if (x < EPIX_EPSILON)
      return "$0$";

    // else
    const double tmp(log10(x)/log10(base));

    int N((int) floor(tmp));

    double k(pow(base, tmp - N));

    obuf << "$";
    if (sgn < 0)
      obuf << "-";

    if (k != 1.0)
      obuf << k << "\\times";

    obuf << base << "^{" << N << "}$";

    return obuf.str();
  }
} // end of namespace
