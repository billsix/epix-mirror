/*
 * legend_tile.cc -- ePiX screen implemenation for legends
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2.1
 * Last Change: September 28, 2007
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
#include <list>

#include "enums.h"

#include "length.h"

#include "label_style.h"
#include "pen_data.h"

#include "functions.h"
#include "pairs.h"
#include "affine.h"
#include "screen_mask.h"

#include "picture.h"
#include "picture_data.h"

#include "format.h"

#include "legend_item.h"
#include "legend_tile.h"

namespace ePiX {

// location, offsets, contents, mark type, visibility
legend_tile::legend_tile(const pair& here, const pair& offset,
                         epix_label_posn align, const label_state& style,
                         const pen_data& pen, double key_size, double skip,
                         std::list<legend_item*> items)
    : m_here(here),
      m_offset(offset),
      m_align(align),
      m_style(style),
      m_item_border(pen),
      m_key_size(key_size),
      m_label_skip(skip) {
  for (auto li = items.begin(); li != items.end(); ++li)
    m_items.push_back((*li)->clone());
}

legend_tile::legend_tile(const legend_tile& L)
    : m_here(L.m_here),
      m_offset(L.m_offset),
      m_align(L.m_align),
      m_style(L.m_style),
      m_item_border(L.m_item_border),
      m_key_size(L.m_key_size),
      m_label_skip(L.m_label_skip) {
  for (auto m_item : L.m_items) m_items.push_back(m_item->clone());
}

legend_tile& legend_tile::operator=(const legend_tile& L) {
  if (&L != this) {
    std::list<legend_item*> tmp;
    for (auto m_item : L.m_items) tmp.push_back(m_item->clone());

    m_here = L.m_here;
    m_offset = L.m_offset;

    m_align = L.m_align;
    m_style = L.m_style;

    m_item_border = L.m_item_border;

    m_key_size = L.m_key_size;
    m_label_skip = L.m_label_skip;

    swap(m_items, tmp);
  }

  return *this;
}

legend_tile::~legend_tile() {
  for (auto& m_item : m_items) delete m_item;
}

legend_tile& legend_tile::map_by(const affine& f) {
  m_here = f(m_here);
  return *this;
}

// legends unaffected by cropping
legend_tile& legend_tile::crop_to(const screen_mask& M) {
  /*
  if (M.crops(m_here))
    m_seen = false;
  */
  return *this;
}

legend_tile* legend_tile::clone() const { return new legend_tile(*this); }

bool legend_tile::is_empty() const { return false; }

std::string legend_tile::print_to(const format& fmt,
                                  const std::string& len) const {
  std::stringstream obuf, item_buf;

  item_buf << "%" << std::endl << "\\begin{tabular}{l}%" << std::endl;

  for (auto m_item : m_items)
    item_buf << m_item->key(fmt, m_key_size, m_item_border, len) << "\\hskip "
             << m_label_skip << "pt "
             << fmt.print_latex_hbox(m_style, m_item->value()) << " \\\\"
             << std::endl;

  item_buf << "\\end{tabular}%" << std::endl;

  obuf << fmt.print_text(m_here, m_offset, item_buf.str(), m_style, len);

  return obuf.str();
}

void legend_tile::add_to_palette() const {
  the_picture()
      .add_to_palette(m_item_border.color())
      .add_to_palette(m_style.text_color())
      .add_to_palette(m_style.mask_color())
      .add_to_palette(m_style.label_border().color());
}
}  // namespace ePiX
