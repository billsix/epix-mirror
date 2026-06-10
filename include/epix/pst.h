/*
 * pst.h -- ePiX's pstricks output format
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.15
 * Last Change: September 07, 2007
 */

/*
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
#ifndef EPIX_PST
#define EPIX_PST

#include <string>
#include <list>
#include <set>

#include "Color.h"
#include "length.h"

#include "edge_data.h"
#include "format.h"

namespace ePiX {

class path_state;
class pen_data;

class pst : public format {
 public:
  pst();

  pst* clone() const override;

  std::string print_fill(const std::list<edge2d>& edges, const pair& offset,
                         const Color& fill, const pen_data& line,
                         const std::string& len) const override;

  std::string print_line(const std::list<edge2d>& edges, const pair& offset,
                         const pen_data& line, const pen_data& base,
                         const path_state& style,
                         const std::string& len) const override;

  // model, name, densities
  std::string print_color(const std::string&, const std::string&, double,
                          double, double) const override;

  std::string print_color(const std::string&, const std::string&, double,
                          double, double, double) const override;

  // output file comment
  std::string print_comment(const std::string&) const override;

  std::string print_verbatim(const std::string&) const override;

  void reset_state() const override;

 private:
  mutable Color m_fill;
  mutable Color m_line;
  mutable Color m_base;

  mutable length m_lwidth;
  mutable length m_bwidth;

  // start, end pspicture, set unit and default linewidth
  std::string start_picture(const pair&, const pair&) const override;
  std::string end_picture() const override;

  std::string set_unitlength(const std::string& len) const override;

  std::string usepackages() const override;

  // string argument for passing attributes local to this path/loop
  std::string start_open_path(const std::string&) const override;
  std::string end_open_path(const std::string&) const override;

  std::string start_closed_path(const std::string&) const override;
  std::string end_closed_path(const std::string&) const override;

  // print declarations to set state of output format
  std::string set_fill_state(const Color&) const override;
  std::string set_pen_state(const pen_data&) const override;

  // place a LaTeX box of width zero (containing string) at location (pair)
  std::string put_box(const pair&, const std::string&) const override;

  std::string print_circle_marker(const pair& here, double diam, bool fill,
                                  const Color& color,
                                  const std::string& len) const override;
};  // end of class pst
}  // namespace ePiX

#endif /* EPIX_PST */
