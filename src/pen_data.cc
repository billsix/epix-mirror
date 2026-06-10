/*
 * pen_data.cc -- ePiX's pen class
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.20
 * Last Change: September 19, 2007
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

#include "constants.h"

#include "camera.h"
#include "pen_data.h"

namespace ePiX {

pen_data::pen_data(const Color& col, const length& wid)
    : m_color(col), m_width(wid) {}

pen_data::pen_data(const Color& col) : m_color(col), m_width(PLAIN_WIDTH) {}

// set
void pen_data::color(const Color& col) { m_color = col; }
void pen_data::width(const length& len) { m_width = len; }

// get
Color pen_data::color() const { return m_color; }
length pen_data::width() const { return m_width; }

bool pen_data::operator==(const pen_data& pen) const {
  return ((m_color == pen.m_color) && (m_width == pen.m_width));
}

bool pen_data::operator!=(const pen_data& pen) const { return !(*this == pen); }

pen_data pen_data::seen_through(const Camera& mycam) const {
  Color col(m_color);
  if (!col.is_unset()) col = mycam(col);

  return pen_data(col, m_width);
}

bool pen_data::is_unset() const {
  return m_color.is_unset() || (m_width < length(EPIX_EPSILON));
}

// undrawn
const pen_data& Xfine() {
  static const pen_data* const xfine_pen(new pen_data(Neutral(), length(0)));

  return *xfine_pen;
}
}  // namespace ePiX
