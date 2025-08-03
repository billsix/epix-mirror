/*
 * fmt.h -- ePiX::fmt output format
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.15
 * Last Change: September 08, 2007
 *
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
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

/*
 * To create files foo.h, foo.cc for output format "foo", do
 *
 *   for suff in cc h; do
 *       sed 's/fmt/foo/g' format_template.$suff > foo.$suff
 *   done
 *
 * The include guards and initial comments should be updated manually.
 */

#ifndef EPIX_FMT
#define EPIX_FMT

#include <set>
#include <list>
#include <string>

#include "edge_data.h"

namespace ePiX {

  class Color;
  class pair;
  class path_state;
  class pen_data;

  class fmt : public format {
  public:

    fmt(); // some constructor mandatory, needn't have this signature

    // Return a copy of this
    fmt* clone() const;

    // Filled region with specified Cartesian edges, offset, and color
    std::string print_fill(const std::list<edge2d>& edges,
			   const pair& offset,
			   const Color& fill,
			   const pen_data& line,
			   const std::string& len) const;

    // Unfilled region, specified Cartesian edges. Should generally be
    // trivially implemented using one-pen function (supplied).
    std::string print_line(const std::list<edge2d>& edges,
			   const pair& offset,
			   const pen_data& line_pen,
			   const pen_data& base_pen,
			   const path_state& style,
			   const std::string& len) const;

    /*
     * Picture start and finish:
     * Size and offset in page units,
     * palette (set of colors contained in the screen),
     * unitlength
     *
     * These functions have default implementations, see format.cc.
     * If the new format can use the default implementation, these
     * functions declarations should be removed.
     *
    std::string pic_header(const pair& sizes,
			   const pair& offsets,
			   const std::set<Color>& palette,
			   const std::string& len) const;

    std::string pic_footer() const;
    */

    // Print color declaration strings: model, name, densities
    std::string print_color(const std::string&,
			    const std::string&,
			    double,
			    double,
			    double) const;

    std::string print_color(const std::string&,
			    const std::string&,
			    double,
			    double,
			    double,
			    double) const;

    // One-line comment
    std::string print_comment(const std::string&) const;

    // Verbatim output
    std::string print_verbatim(const std::string&) const;


    // begin and end a picture-like environment, set the unit length
    std::string start_picture(const pair&, const pair&) const;
    std::string end_picture() const;

    std::string set_unitlength(const std::string& len) const;

    // State data, if any, must be mutable
    void reset_state() const;

  private:

    std::string usepackages() const;

    // string argument for passing attributes local to this path/loop
    std::string start_open_path(const std::string&) const;
    std::string end_open_path(const std::string&) const;

    std::string start_closed_path(const std::string&) const;
    std::string end_closed_path(const std::string&) const;

    // print declarations to set state of output format
    std::string set_fill_state(const Color&) const;
    std::string set_pen_state(const pen_data&) const;

    // place a LaTeX box of width zero (containing string) at location (pair)
    std::string put_box(const pair&, const std::string&) const;

    // draw a filled or hollow circle of specified diam, color
    std::string print_circle_marker(const pair& here, double diam,
				    bool fill, const Color& color,
				    const std::string& len) const;

    //// "Optional" (implemented, but reimplementable) functions ////
    // inserted in long paths, defaults to <newline> and two spaces
    // std::string path_break() const;

    // inserted between path points, deaults to ""
    // std::string path_break() const;

    // std::string print_palette(const std::set<Color>&) const;

    // Represent a pair, length, or Color as a string
    // std::string print(const pair&) const;   // Default: "(a,b)"
    // std::string print(const length&) const; // Default: "3.5cm", e.g.
    // std::string print(const Color&) const;  // Default: color name

  }; // end of class fmt
} // end of namespace

#endif /* EPIX_FMT */
