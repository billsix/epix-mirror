/*
 * glyph.h -- ePiX screen implemenation for labels and markers
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.18
 * Last Change: September 16, 2007
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

#ifndef EPIX_GLYPH
#define EPIX_GLYPH

#include <string>

#include "pairs.h"
#include "length.h"
#include "Color.h"

#include "marker_style.h"
#include "label_style.h"
#include "tile.h"

namespace ePiX {

class format;
class affine;
class screen_mask;

class glyph : public tile {
 public:
  // location, offsets, contents, mark type, visibility
  glyph(const pair& here, const pair& offset, const std::string& label,
        const epix_mark_type mark, const marker_sizes& sizes,
        const label_state& style, const bool seen);

  glyph& map_by(const affine&) override;
  glyph& crop_to(const screen_mask&) override;

  glyph* clone() const override;

  bool is_empty() const override;

  std::string print_to(const format&, const std::string&) const override;
  void add_to_palette() const override;

 private:
  pair m_here;
  pair m_offset;

  std::string m_label;
  epix_mark_type m_mark;

  marker_sizes m_sizes;
  label_state m_style;

  bool m_seen;
};  // end of class glyph

}  // namespace ePiX

#endif /* EPIX_GLYPH */
