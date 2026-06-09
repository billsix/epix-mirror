/* 
 * glyph.cc -- ePiX screen implemenation for labels and markers
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.22
 * Last Change: September 24, 2007
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

#include <cmath>
#include <sstream>

#include "constants.h"
#include "angle_units.h"

#include "functions.h"

#include "pairs.h"
#include "affine.h"
#include "screen_mask.h"

#include "length.h"

#include "marker_style.h"
#include "label_style.h"

#include "format.h"
#include "state.h"

#include "picture_data.h"
#include "picture.h"

#include "glyph.h"

namespace ePiX {

  glyph::glyph(const pair& here, const pair& offset,
	       const std::string& label, const epix_mark_type mark,
	       const marker_sizes& sizes,
	       const label_state& style, const bool seen)
    : m_here(here), m_offset(offset),
      m_label(label), m_mark(mark), m_sizes(sizes),
      m_style(style), m_seen(seen) { }

  glyph& glyph::map_by(const affine& f)
  {
    m_here = f(m_here); // new location

    // map offset, preserving length
    const double old_norm(norm(m_offset));
    m_offset = f(m_offset) - f(pair(0,0)); // new displacement

    const double new_norm(norm(m_offset));
    if (EPIX_EPSILON < new_norm)
      m_offset *= old_norm/new_norm; // scale to preserve page length

    // compute new label angle; work in radians for safety/efficiency
    const double th(PI_180*m_style.label_angle()); // old angle in radians

    // new baseline direction
    const pair tmp_dir(f(pair(std::cos(th), std::sin(th))) - f(pair(0,0)));

    // no change if old baseline maps to zero
    if (EPIX_EPSILON < norm(tmp_dir))
      {
	// new angle in current units
	double theta(Atan2(tmp_dir.x2(), tmp_dir.x1()));

	// effectively, reflect label; works well stylistically
	if (f.reverses_orientation())
	  theta -= 0.5*full_turn();

	m_style.label_angle(theta);
      }

    return *this;
  }

  glyph& glyph::crop_to(const screen_mask& M)
  {
    if (M.crops(m_here))
      m_seen = false;

    return *this;
  }

  glyph* glyph::clone() const
  {
    return new glyph(*this);
  }

  bool glyph::is_empty() const
  {
    return !m_seen;
  }


  std::string glyph::print_to(const format& fmt, const std::string& len) const
  {
    if (is_empty())
      return "";

    std::stringstream obuf;
    // print (masked) label, if any, before mark.
    if (m_label != "")
      obuf << fmt.print_text(m_here, m_offset, m_label, m_style, len);

    if (m_mark != TEXT && m_mark != PATH)
      obuf << fmt.print_mark(m_here, m_offset, m_mark, m_sizes, m_style, len);

    return obuf.str();
  }

  void glyph::add_to_palette() const
  {
    the_picture().add_to_palette(m_style.text_color())
      .add_to_palette(m_style.mask_color())
      .add_to_palette(m_style.label_border().color());
  }
} // end of namespace
