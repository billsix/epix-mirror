/*
 * verbatim.cc -- ePiX screen implemenation for verbatim text
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.7
 * Last Change: July 14, 2007
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

#include <sstream>

#include "format.h"
#include "verbatim.h"

namespace ePiX {

class affine;
class screen_mask;

verbatim::verbatim(const std::string& text) : m_text(text) {}

verbatim& verbatim::map_by(const affine& f) { return *this; }

verbatim& verbatim::crop_to(const screen_mask& M) { return *this; }

verbatim* verbatim::clone() const { return new verbatim(*this); }

bool verbatim::is_empty() const { return (m_text == ""); }

std::string verbatim::print_to(const format& fmt, const std::string& L) const {
  if (is_empty()) return "";

  // else
  return fmt.print_verbatim(m_text);
}

void verbatim::add_to_palette() const {}
}  // namespace ePiX
