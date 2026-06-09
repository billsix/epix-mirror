/* 
 * pen_line.cc -- ePiX::pen_line class, for line-like objects
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
#include "functions.h"

#include "pairs.h"
#include "affine.h"
#include "screen_mask.h"

#include "length.h"

#include "pen_data.h"
#include "paint_style.h"

#include "format.h"
#include "picture_data.h" // for add_to_palette()
#include "picture.h"      // for the_picture()

#include "edge_data.h"

#include "tile.h"
#include "pen_line.h"

namespace ePiX {

  using ep2 = std::list<edge2d>::const_iterator;

  pen_line::pen_line()
    : m_line(the_paint_style().line_pen()),
      m_base(the_paint_style().base_pen()), m_style(the_path_style()) { }

  pen_line::pen_line(const pen_data& line, const pen_data& base,
		     const std::list<edge2d>& data)
    : m_line(line), m_base(base), m_style(the_path_style()), m_border(data) { }


  pen_line* pen_line::clone() const
  {
    return new pen_line(*this);
  }


  pen_line& pen_line::map_by(const affine& f)
  {
    if (m_border.size() == 0)
      return *this;

    std::list<edge2d> bord;

    for (auto p=m_border.begin(); p!=m_border.end(); ++p)
      bord.push_back(edge2d(f(p->tail()), f(p->head()), (*p).is_seen()));

    swap(m_border, bord);
    return *this;
  }


  pen_line& pen_line::crop_to(const screen_mask& M)
  {
    m_border = M.crop_path(m_border);
    return *this;
  }


  bool pen_line::is_empty() const
  {
    // N.B. line trumps base, even if base is otherwise visible
    return (m_border.size() == 0 ||
	    m_line.color().is_unset() ||
	    m_line.width() == length(0));
  }


  std::string pen_line::print_to(const format& fmt,
				 const std::string& len) const
  {
    if (is_empty())
      return "";

    // else
    return fmt.print_line(m_border, pair(0,0), m_line, m_base, m_style, len);
  }

  void pen_line::add_to_palette() const
  {
    // add_to_palette checks for unset
    the_picture().add_to_palette(m_line.color())
      .add_to_palette(m_base.color());
  }

  void pen_line::add(const edge2d& E)
  {
    m_border.push_back(E);
  }
} // end of namespace

