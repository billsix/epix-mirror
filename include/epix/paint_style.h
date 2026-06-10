/*
 * paint_style.h -- ePiX's current line and fill style.
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.3
 * Last Change: March 04, 2007
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

/*
 * This file implements ePiX's "paint" style:
 *
 *  [] Front pen, base pen (Color and width x2)
 *  [] Filling and fill color (bool, Color)
 *
 * The class in this file implements backend for user-level global
 * functions defined in state.h, which modify the_paint_style().
 *
 * This header is not part of epix.h.
 */
#ifndef EPIX_PAINT_STYLE
#define EPIX_PAINT_STYLE

#include "Color.h"
#include "pen_data.h"

namespace ePiX {

class Camera;
class length;

class paint_state {
 public:
  paint_state();

  paint_state* clone() const;

  // set
  void line_pen(const pen_data& pen);
  void line_color(const Color& col);
  void line_width(const length& len);

  void base_pen(const pen_data& pen);
  void base_color(const Color& col);
  void base_width(const length& len);

  void fill_color(const Color& col);
  void fill_flag(bool fill);

  // get
  pen_data line_pen() const;
  Color line_color() const;
  length line_width() const;

  pen_data base_pen() const;
  Color base_color() const;
  length base_width() const;

  Color fill_color() const;
  bool fill_flag() const;

  paint_state seen_through(const Camera& cam) const;

 private:
  pen_data m_line;
  pen_data m_base;

  Color m_tint;
  bool m_filling;

  paint_state(const pen_data&, const pen_data&, const Color&, bool fill);
};  // end of class paint_state

paint_state& the_paint_style();

}  // namespace ePiX

#endif /* EPIX_PAINT_STYLE */
