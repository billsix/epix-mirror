/*
 * legend_item.cc -- ePiX legend_item implementations
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.18
 * Last Change: September 16, 2007
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

#include <string>
#include <sstream>

#include "enums.h"
#include "Color.h"
#include "pen_data.h"

#include "pairs.h"
#include "edge_data.h"

#include "camera.h"

#include "format.h"

#include "label_data.h"
#include "markers.h"

#include "paint_style.h"

#include "screen_data.h"
#include "glyph.h"
#include "pen_line.h"

#include "legend_item.h"

namespace ePiX {

// Magic numbers
screen_data __epix_legend_key(double L) {
  return screen_data(pair(0, -0.075 * L), pair(L, 0.925 * L));
}

double legend_item::key_coord(double sz, const std::string& len) const {
  return sz * (length("1pt").to(len).magnitude());
}

// N.B. store raw fill color; "key" passes through camera
fill_item::fill_item(const std::string& text)
    : m_fill(the_paint_style().fill_color()), m_text(text) {}

fill_item* fill_item::clone() const { return new fill_item(*this); }

std::string fill_item::value() const { return m_text; }

std::string fill_item::key(const format& fmt, double sz, const pen_data& bpen,
                           const std::string& len) const {
  const double L(legend_item::key_coord(sz, len));

  screen_data tmp(__epix_legend_key(L));

  tmp.backing(m_fill).border(bpen.color(), bpen.width().name());

  return tmp.print_to(fmt, len, true);
}

// N.B. Store pens passed through camera
path_item::path_item(const std::string& text)
    : m_line(the_paint_style().line_pen().seen_through(cam())),
      m_base(the_paint_style().base_pen().seen_through(cam())),
      m_text(text) {}

path_item* path_item::clone() const { return new path_item(*this); }

std::string path_item::value() const { return m_text; }

std::string path_item::key(const format& fmt, double sz, const pen_data& bpen,
                           const std::string& len) const {
  const double L(legend_item::key_coord(sz, len));

  screen_data tmp(__epix_legend_key(L));
  tmp.border(bpen.color(), bpen.width().name());

  // draw a centered horizontal segment in the current line style
  // Magic numbers
  std::list<edge2d> edges;
  edges.push_back(edge2d(tmp.l(), tmp.r(), true));

  tmp.add_tile(pen_line(m_line, m_base, edges));
  return tmp.print_to(fmt, len, true);
}

// key is a marker in the current style
mark_item::mark_item(epix_mark_type mark, const std::string& text)
    : m_mark(mark),
      m_style(the_label_style().seen_through(cam())),
      m_sizes(the_mark_size()),
      m_text(text) {}

mark_item* mark_item::clone() const { return new mark_item(*this); }

std::string mark_item::value() const { return m_text; }

std::string mark_item::key(const format& fmt, double sz, const pen_data& bpen,
                           const std::string& len) const {
  const double L(legend_item::key_coord(sz, len));

  screen_data tmp(__epix_legend_key(L));
  tmp.border(bpen.color(), bpen.width().name());

  tmp.add_tile(glyph(tmp.c(), pair(0, 0), "", m_mark, m_sizes, m_style, true));

  return tmp.print_to(fmt, len, true);  // encapsulate in a picture env.
}
}  // namespace ePiX
