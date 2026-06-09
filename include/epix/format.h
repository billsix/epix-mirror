/*
 * format.h -- ePiX::output format interface
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.17
 * Last Change: September 13, 2007
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

/*
 * This file defines ePiX's output interface. The fmt_template
 * files contain skeleton code for a new format. The command
 *
 *   for suff in cc h; do
 *       sed 's/fmt/foo/g' fmt_template.$suff > foo.$suff
 *   done
 *
 * (or manual search-and-replace) creates "blank" files containing
 * declarations and pseudocode. (The include guards should also be
 * changed.) Please consult pst.cc and eepic.cc for complete examples.
 *
 * The pure virtual functions in this file *must* be implemented by
 * derived classes. They are signified by the keyword "virtual" and
 * the definition "= 0" after their declaration. They are also found
 * in the skeleton class mentioned above.
 *
 * The seven ordinary virtual functions (the destructor, pic_header and
 * pic_footer, and four low-level functions for printing a palette, pair,
 * Color, or length) _may_ be overridden by derived classes, but will
 * not be (unless the format has unusual conventions).
 *
 * No other functions should be re-implemented by derived classes.
 */

#ifndef EPIX_FORMAT
#define EPIX_FORMAT

#include <set>
#include <list>
#include <vector>
#include <string>

#include "enums.h"
#include "edge_data.h"

namespace ePiX {

  class Color;
  class label_state;
  class length;

  class marker_sizes;
  class pair;
  class path_state;
  class pen_data;

  class format {
  public:
    // Free resources allocated by the constructor (usually unnecessary)
    virtual ~format() = default;

    // Return a copy of this
    virtual format* clone() const = 0;


    // LaTeX symbol with specified location, offset and alignment
    std::string print_mark(const pair& location,
			   const pair& offset,
			   const epix_mark_type mark,
			   const marker_sizes& sizes,
			   const label_state& style,
			   const std::string& len) const;

    // Label text with specified location, offset, and alignment
    std::string print_text(const pair& location,
			   const pair& offset,
			   const std::string& text,
			   const label_state& style,
			   const std::string& len) const;


    // Filled region with specified Cartesian edges, offset, and color
    virtual std::string print_fill(const std::list<edge2d>& edges,
				   const pair& offset,
				   const Color& fill,
				   const pen_data& line,
				   const std::string& len) const = 0;


    // Unfilled region, specified Cartesian edges. Should generally be
    // trivially implemented using one-pen function (supplied).
    virtual std::string print_line(const std::list<edge2d>& edges,
				   const pair& offset,
				   const pen_data& line_pen,
				   const pen_data& base_pen,
				   const path_state& style,
				   const std::string& len) const = 0;

    // implemented using previous function
    std::string print_line(const edge2d&,
			   const pair&,
			   const pen_data&,
			   const pen_data&,
			   const path_state&,
			   const std::string&) const;


    // File comment and information block
    std::string file_comments(const std::string& units,
			      const length& hoff,
			      const length& voff,
			      double Xmin, double Xmax,
			      double Ymin, double Ymax,
			      double Hsize, double Vsize) const;


    // Picture start and finish:
    // Size and offset in page units,
    // palette (set of colors contained in the screen),
    // unitlength
    virtual std::string pic_header(const pair& sizes,
				   const pair& offsets,
				   const std::set<Color>& palette,
				   const std::string& len) const;

    virtual std::string pic_footer() const;


    // Print color declaration strings: model, name, densities
    std::string print_color(const std::string&,
			    const std::string&,
			    const std::vector<double>&) const;

    // One-line comment
    virtual std::string print_comment(const std::string&) const = 0;

    // Verbatim output
    virtual std::string print_verbatim(const std::string&) const = 0;


    // print text with color, font in an hbox
    std::string print_latex_hbox(const label_state& style,
				 const std::string& label) const;

    // Derived classes use functions below to implement the public interface.
    // The first two are needed to print screen_data for legend items.
    virtual std::string start_picture(const pair&, const pair&) const = 0;
    virtual std::string end_picture() const = 0;

    virtual std::string set_unitlength(const std::string& len) const = 0;

    // If child maintains drawing state, this function clears it.
    virtual void reset_state() const = 0;

  protected:
    // inserted between points when breaking lines
    virtual std::string path_break() const;
    // inserted between points of a path
    virtual std::string path_connector() const;

    // names of LaTeX packages required to process our output
    virtual std::string usepackages() const = 0;

    // string argument for passing attributes local to this path/loop
    virtual std::string start_open_path(const std::string&) const = 0;
    virtual std::string end_open_path(const std::string&) const = 0;

    virtual std::string start_closed_path(const std::string&) const = 0;
    virtual std::string end_closed_path(const std::string&) const = 0;

    // print declarations to set state of output format
    virtual std::string set_fill_state(const Color&) const = 0;
    virtual std::string set_pen_state(const pen_data&) const = 0;

    // place a LaTeX box of width zero (containing string) at location (pair)
    virtual std::string put_box(const pair&, const std::string&) const = 0;

    // too idiosyncratic to handle format-independently
    virtual std::string print_circle_marker(const pair& here, double diam,
					    bool fill, const Color& color,
					    const std::string& len) const = 0;

    // xcolor definition strings
    std::string xdefinecolor(const std::string&,
			     const std::string&,
			     double,
			     double,
			     double) const;

    std::string xdefinecolor(const std::string&,
			     const std::string&,
			     double,
			     double,
			     double,
			     double) const;

    virtual std::string print_color(const std::string&,
				    const std::string&,
				    double,
				    double,
				    double) const = 0;

    virtual std::string print_color(const std::string&,
				    const std::string&,
				    double,
				    double,
				    double,
				    double) const = 0;

    // Define all colors in the picture
    virtual std::string print_palette(const std::set<Color>& palette) const;

    // Represent a pair, length, or Color as a string
    virtual std::string print(const pair&) const;   // Default: "(a,b)"
    virtual std::string print(const length&) const; // Default: "3.5cm", e.g.
    virtual std::string print(const Color&) const;  // Default: color name


    // one-pen version
    std::string print_line(const std::list<edge2d>& edges,
			   const pair& offset,
			   const pen_data& line,
			   const path_state& style,
			   const std::string& attribs,
			   const std::string& len) const;

    // for single edge; wrap in list and pass to previous function
    std::string print_line(const edge2d& e,
			   const pair& offset,
			   const pen_data& line,
			   const path_state& style,
			   const std::string& attribs,
			   const std::string& len) const;

    // Print a non-solid edge, using the virtual interface
    std::string print_edge(const edge2d& edge, const pair& offset,
			   const std::vector<double> bkpts,
			   double diam,
			   const Color& col,
			   const std::string& attribs,
			   const std::string& len) const;

    // Print an edge list as offset path segments, or as a loop
    // if all edges are visible and path is closed.
    std::string print_paths(const std::list<edge2d>& edges,
			    const pair& offset,
			    const std::string& attribs,
			    const std::string& len) const;

    /*
     * Text in an ePiX output file is assumed to be typeset with
     * LaTeX.  The functions below perform LaTeX-specific low-level
     * formatting.  For uniformity and simplicity, these functions are
     * format-independent: Implementations are provided and these
     * functions are non-virtual. Format-specific masking, padding,
     * and bordering capabilities (e.g. in PSTricks) are not used.
     */
    // label style, text, masking flag
    std::string print_latex_label(const label_state&,
				  const std::string&,
				  bool) const;

    // Print LaTeX command for marker symbol; 3rd (len) argument for scaling
    std::string print_latex_marker(const epix_mark_type,
				   const marker_sizes&,
				   const std::string&) const;

  private:
    // convert an epix_label_posn to string
    std::string print(const epix_label_posn arg) const;

    // convert true pt offset to len
    pair pt_to_len(pair offset, const std::string& len) const;

  }; // end of base class format
} // end of namespace

#endif /* EPIX_FORMAT */
