/* 
 * paint_style.cc -- ePiX's current line and fill style.
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
#include "camera.h"
#include "length.h"
#include "pen_data.h"
#include "paint_style.h"

namespace ePiX {

  // Magic numbers: initialization constants
  paint_state::paint_state() 
    : m_line(pen_data()), m_base(Xfine()),
      m_tint(Neutral()), m_filling(false) { }

  paint_state* paint_state::clone() const
  {
    return new paint_state(*this);
  }

  // set
  void paint_state::line_pen(const pen_data& pen)
  {
    m_line = pen;
  }

  void paint_state::line_color(const Color& col)
  {
    m_line.color(col);
  }

  void paint_state::line_width(const length& len)
  {
    m_line.width(len);
  }

  void paint_state::base_pen(const pen_data& pen)
  {
    m_base = pen;
  }

  void paint_state::base_color(const Color& col)
  {
    m_base.color(col);
  }

  void paint_state::base_width(const length& len)
  {
    m_base.width(len);
  }

  void paint_state::fill_color(const Color& col)
  {
    m_tint = col;
  }

  void paint_state::fill_flag(bool fill)
  {
    m_filling = fill;
  }


  // get
  pen_data paint_state::line_pen() const
  {
    return m_line;
  }
  Color  paint_state::line_color() const
  {
    return m_line.color();
  }
  length paint_state::line_width() const
  {
    return m_line.width();
  }

  pen_data paint_state::base_pen() const
  {
    return m_base;
  }
  Color  paint_state::base_color() const
  {
    return m_base.color();
  }
  length paint_state::base_width() const
  {
    return m_base.width();
  }

  Color  paint_state::fill_color() const
  {
    return m_tint;
  }
  bool    paint_state::fill_flag() const
  {
    return m_filling;
  }

  paint_state paint_state::seen_through(const Camera& mycam) const
  {
    return paint_state(m_line.seen_through(mycam), m_base.seen_through(mycam),
		       mycam(m_tint), m_filling);
  }

  paint_state::paint_state(const pen_data& lp, const pen_data& bp,
			   const Color& fc, bool ff)
    : m_line(lp), m_base(bp), m_tint(fc), m_filling(ff) { }


  paint_state& the_paint_style()
  {
    static paint_state* the_paint_state(new paint_state());
    return *the_paint_state;
  }
} // end of namespace
