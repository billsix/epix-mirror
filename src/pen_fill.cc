/*
 * pen_fill.cc -- ePiX::pen_fill class, for filled screen elements
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.20
 * Last Change: September 19, 2007
 *
 *
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

#include "functions.h"

#include "pairs.h"
#include "affine.h"
#include "screen_mask.h"

#include "length.h"

#include "pen_data.h"
#include "path_style.h"

#include "format.h"
#include "picture_data.h"
#include "picture.h"

#include "edge_data.h"

#include "tile.h"
#include "pen_fill.h"

namespace ePiX {

using ep2 = std::list<edge2d>::const_iterator;

pen_fill::pen_fill()
    : m_tint(Neutral()), m_line(Xfine()), m_style(the_path_style()) {}

pen_fill::pen_fill(const Color& fill, const pen_data& line,
                   const std::list<edge2d>& data)
    : m_tint(fill),
      m_line(line),
      m_style(the_path_style()),
      m_border(data),
      m_edges(data) {}

pen_fill::pen_fill(const Color& fill, const pen_data& line, double th,
                   const std::list<edge2d>& data)
    : m_tint(fill),
      m_line(line),
      m_style(the_path_style()),
      m_border(data),
      m_edges(data) {}

pen_fill* pen_fill::clone() const { return new pen_fill(*this); }

pen_fill& pen_fill::map_by(const affine& f) {
  if (m_border.size() == 0) return *this;

  // else map border
  std::list<edge2d> bord;

  for (auto p = m_border.begin(); p != m_border.end(); ++p)
    bord.push_back(edge2d(f(p->tail()), f(p->head()), (*p).is_seen()));

  swap(m_border, bord);

  // and edges
  std::list<edge2d> edges;

  for (auto p = m_edges.begin(); p != m_edges.end(); ++p)
    edges.push_back(edge2d(f(p->tail()), f(p->head()), (*p).is_seen()));

  swap(m_edges, edges);
  return *this;
}

pen_fill& pen_fill::crop_to(const screen_mask& M) {
  m_border = M.crop_loop(m_border);
  m_edges = M.crop_path(m_edges);
  return *this;
}

bool pen_fill::is_empty() const {
  return (m_border.size() == 0 || (m_tint.is_unset() && m_line.is_unset()));
}

std::string pen_fill::print_to(const format& fmt,
                               const std::string& len) const {
  if (is_empty()) return "";

  // else at least one is true
  bool filled(!m_tint.is_unset());
  bool edged(!m_line.is_unset());

  // if solid bd, try to draw/fill in one step
  if (m_style.is_solid() && filled && edged)
    return fmt.print_fill(m_border, pair(0, 0), m_tint, m_line, len);

  // else draw fill, border separately
  std::string val;

  if (filled)
    val += fmt.print_fill(m_border, pair(0, 0), m_tint,
                          pen_data(m_tint, m_line.width()), len);

  if (edged)
    val += fmt.print_line(m_edges, pair(0, 0), m_line, Xfine(), m_style, len);

  return val;
}

void pen_fill::add_to_palette() const {
  the_picture().add_to_palette(m_tint).add_to_palette(m_line.color());
}
}  // namespace ePiX
