/*
 * legend_tile.h -- ePiX screen implemenation for legends
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2.1
 * Last Change: September 28, 2007
 *
 *
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
 * Department of Mathematics and Computer Science
 * College of the Holy Cross
 * Worcester, MA, 01610-2395, USA
 *
 *
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

#ifndef EPIX_LEGEND_TILE
#define EPIX_LEGEND_TILE
#include <string>

#include "enums.h"

#include "pairs.h"
#include "length.h"
#include "Color.h"

#include "label_style.h"
#include "pen_data.h"

#include "tile.h"

namespace ePiX {

class format;
class legend_item;
class affine;
class screen_mask;

class legend_tile : public tile {
 public:
  legend_tile(const pair& here, const pair& offset, epix_label_posn align,
              const label_state& style, const pen_data& item_border,
              double key_size, double skip, std::list<legend_item*> items);

  legend_tile(const legend_tile&);
  legend_tile& operator=(const legend_tile&);
  ~legend_tile() override;

  legend_tile& map_by(const affine&) override;
  legend_tile& crop_to(const screen_mask&) override;

  legend_tile* clone() const override;

  bool is_empty() const override;

  std::string print_to(const format&, const std::string&) const override;
  void add_to_palette() const override;

 private:
  pair m_here;
  pair m_offset;

  epix_label_posn m_align;

  label_state m_style;

  pen_data m_item_border;

  double m_key_size;    // dims of key boxes in pt
  double m_label_skip;  // \hskip between box and item value

  std::list<legend_item*> m_items;
};  // end of class legend_tile

}  // namespace ePiX

#endif /* EPIX_LEGEND_TILE */
