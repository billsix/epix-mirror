/* 
 * label_data.cc -- ePiX::label_data class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.21
 * Last Change: September 22, 2007
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

#include <sstream>

#include "enums.h"

#include "triples.h"
#include "pairs.h"

#include "label_data.h"
#include "camera.h"
#include "screen_data.h"
#include "screen.h"

#include "clipping.h"

#include "paint_style.h"

#include "active_screen.h"
#include "glyph.h"

namespace ePiX {

  label_data::label_data(const P& here, const P& offset,
			 const std::string& text, epix_mark_type mark)
    : m_here(here), m_offset(offset.x1(), offset.x2()),
      m_text(text), m_mark(mark), m_sizes(the_mark_size()),
      m_style(the_label_style()), m_seen(!the_clip_box().clips(m_here))
  {
    // set axis tick color to line color; rotation ok if requested
    if (m_mark == HTICK || m_mark == VTICK)
      m_style.text_color(the_paint_style().line_color());
  }

    // generate label text from user-specified function of 2 or 3 variables
  label_data::label_data(const P& here, const P& offset,
			 std::string f(double,double), epix_mark_type mark)
    : m_here(here), m_offset(offset.x1(), offset.x2()),
      m_text(f(here.x1(), here.x2())),
      m_mark(mark),  m_sizes(the_mark_size()), m_style(the_label_style()),
      m_seen(!the_clip_box().clips(m_here))
  {
    if (m_mark == HTICK || m_mark == VTICK)
      m_style.text_color(the_paint_style().line_color());
  }

  label_data::label_data(const P& here, const P& offset,
			 std::string f(double,double,double),
			 epix_mark_type mark)
    : m_here(here), m_offset(offset.x1(), offset.x2()),
      m_text(f(here.x1(), here.x2(), here.x3())),
      m_mark(mark),  m_sizes(the_mark_size()), m_style(the_label_style()),
      m_seen(!the_clip_box().clips(m_here))
  {
    if (m_mark == HTICK || m_mark == VTICK)
      m_style.text_color(the_paint_style().line_color());
  }

  // markers subject to masking, border
  // Only this constructor aligns TICK marks, prevents their rotation
  label_data::label_data(const P& here, epix_mark_type mark, epix_label_posn A)
    : m_here(here), m_offset(0, 0),
      m_text(""), m_mark(mark),  m_sizes(the_mark_size()),
      m_style(the_label_style()), m_seen(!the_clip_box().clips(m_here))
  {
    if (m_mark == HTICK || m_mark == VTICK)
      {
	m_style.text_color(the_paint_style().line_color());
	m_style.label_angle(0); // no initial rotation

	// prevent unseemly alignment; default is c, so needn't check
	if (m_mark == HTICK && A == t || A == b)
	  m_style.align_to(A);

	else if (A == l || A == r)
	  m_style.align_to(A);
      }
  }


  label_data& label_data::text_color(const Color& col)
  {
    m_style.text_color(col);
    return *this;
  }

  label_data& label_data::mask_color(const Color& col)
  {
    m_style.mask_color(col);
    return *this;
  }

  label_data& label_data::align_to(epix_label_posn align)
  {
    m_style.align_to(align);
    return *this;
  }

  // send colors through cam
  void label_data::draw() const
  {
    if (m_seen)
      (*active_screen()).m_screen->add_tile(glyph(cam()(m_here), m_offset,
						  m_text, m_mark, m_sizes,
						  m_style.seen_through(cam()),
						  true));
  }
} // end of namespace
