/*
 * fmt.cc -- ePiX::fmt output format
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.15
 * Last Change: September 08, 2007
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

#include <list>

#include <string>
#include <sstream>

#include <set>

#include "pairs.h"
#include "edge_data.h"

#include "Color.h"

#include "path_style.h"
#include "pen_data.h"

#include "format.h"
#include "fmt.h"

namespace ePiX {

  fmt::fmt() { } // modify as necessary

  fmt* fmt::clone() const
  {
    return new fmt(*this);
  }

  // Filled region with specified Cartesian edges, offset, and color
  std::string fmt::print_fill(const std::list<edge2d>& edges,
			      const pair& offset,
			      const Color& fill,
			      const pen_data& line,
			      const std::string& len) const
  {
    // Use print_paths to represent a closed polygon bounded by
    // "edges", drawn with "line" and solid "fill"-ed.
  }

  // Unfilled region, specified Cartesian edges. Should generally be
  // trivially implemented using one-pen function (supplied).
  std::string fmt::print_line(const std::list<edge2d>& edges,
			      const pair& offset,
			      const pen_data& line_pen,
			      const pen_data& base_pen,
			      const path_state& style,
			      const std::string& len) const
  {
    // Use print_paths (solid) or format::print_edges (otherwise) to
    // represent a (closed or open) polygon bounded by "edges", drawn
    // with "line" and "base" pens in the line "style".
  }

  /*
   * Default implementations provided, see format.cc. If default works
   * for the new format, remove these declarations.
  // Picture start and finish:
  // Size and offset in page units,
  // palette (set of colors contained in the screen),
  // unitlength
  std::string fmt::pic_header(const pair& sizes,
			      const pair& offsets,
			      const std::set<Color>& palette,
			      const std::string& len) const
  {
    // Print commands to draw a picture of specified true size and offset
  }

  std::string fmt::pic_footer() const
  {
    // Close/end the picture
  }
  */

  // Print color declaration strings: model, name, densities;
  // may simply return xdefinecolor(...) if appropriate
  std::string fmt::print_color(const std::string&,
			       const std::string&,
			       double,
			       double,
			       double) const;

  std::string fmt::print_color(const std::string&,
			       const std::string&,
			       double,
			       double,
			       double,
			       double) const;

  // One-line comment
  std::string fmt::print_comment(const std::string&) const;

  // Verbatim output
  std::string fmt::print_verbatim(const std::string&) const;


  // start/end a picture-like environment, set unit length
  std::string fmt::start_picture(const pair&, const pair&) const;
  std::string fmt::end_picture() const;

  std::string fmt::set_unitlength(const std::string& len) const;

  // reset internal state date to null
  void fmt::reset_state() const;

  //// private member functions ////

  // returns "usepackages" + [SPACE] + "comma-separated LaTeX packages"
  std::string fmt::usepackages() const
  {
    return "usepackages fmt";
  }

  // string argument for passing attributes local to this path/loop
  std::string fmt::start_open_path(const std::string&) const;
  std::string fmt::end_open_path(const std::string&) const;

  std::string fmt::start_closed_path(const std::string&) const;
  std::string fmt::end_closed_path(const std::string&) const;

  // print declarations to set state of output format
  std::string fmt::set_fill_state(const Color&) const;
  std::string fmt::set_pen_state(const pen_data&) const;

  // place a LaTeX box of width zero (containing string) at location (pair)
  std::string fmt::put_box(const pair&, const std::string&) const;

  // draw a filled or hollow circle of specified diam, color
  std::string fmt::print_circle_marker(const pair& here, double diam,
				       bool fill, const Color& color,
				       const std::string& len) const;

  //// "Optional" (implemented, but reimplementable) functions ////
  // Used to format line breaks in long paths (e.g. newline and two spaces)
  // std::string fmt::path_break() const;

  // Used between points in paths (e.g. "" or "--")
  // std::string fmt::path_connector() const;

  // std::string fmt::print_palette(const std::set<Color>& palette) const;

  // Represent a pair, length, or Color as a string
  // std::string fmt::print(const pair&) const;   // Default: "(a,b)"
  // std::string fmt::print(const length&) const; // Default: "3.5cm", e.g.
  // std::string fmt::print(const Color&) const;  // Default: color name

} // end of namespace
