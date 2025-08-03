/*
 * tikz.cc -- ePiX::tikz output format
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.15
 * Last Change: September 07, 2007
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

#include <list>

#include <string>
#include <sstream>

#include <set>

#include "constants.h"

#include "utils.h"

#include "pairs.h"
#include "edge_data.h"

#include "Color.h"

#include "path_style.h"
#include "pen_data.h"

// need to get units from the_picture
#include "picture_data.h"
#include "picture.h"

#include "format.h"
#include "tikz.h"

namespace ePiX {

  tikz::tikz()
    : m_fill(Neutral()), m_stroke(Black()), m_lwidth(PLAIN_WIDTH),
      m_units(the_picture().the_unitlength.units()) { }


  tikz* tikz::clone() const
  {
    return new tikz(*this);
  }

  // Filled region with specified Cartesian edges, offset, and color
  std::string tikz::print_fill(const std::list<edge2d>& edges,
			       const pair& offset,
			       const Color& fc,
			       const pen_data& line,
			       const std::string& len) const
  {
    std::stringstream obuf;
    obuf << set_fill_state(fc) << set_pen_state(line);

    // compute attribute string
    std::stringstream attribs;

    if ( !(fc.is_unset()) )
      {
	attribs << "[fill";

	if (fc.alpha() < 1)
	  attribs << ",opacity=" << fc.alpha();

	attribs << "]";
      }

    obuf << print_paths(edges, offset, attribs.str(), len);

    return obuf.str();
  }

  // Unfilled region, specified Cartesian edges
  std::string tikz::print_line(const std::list<edge2d>& edges,
			       const pair& offset,
			       const pen_data& line,
			       const pen_data& base,
			       const path_state& style,
			       const std::string& len) const
  {
    std::string value;

    // draw *solid* base first
    if (line.width() < base.width() && !base.color().is_unset())
      {
	std::stringstream battr;
	battr << "[color=" << format::print(base.color()) << ","
	      << "line width=" << format::print(base.width()) << "]";

	value += format::print_line(edges, offset, base, path_state(),
				    battr.str(), len);
      }

    value += set_pen_state(line);

    return value += format::print_line(edges, offset, line, style, "", len);
  }

  // Print color declaration strings: model, name, densities
  std::string tikz::print_color(const std::string& model,
				const std::string& name,
				double d1, double d2,
				double d3) const
  {
    return format::xdefinecolor(model, name, d1, d2, d3);
  }

  std::string tikz::print_color(const std::string& model,
				const std::string& name,
				double d1, double d2,
				double d3, double d4) const
  {
    return format::xdefinecolor(model, name, d1, d2, d3, d4);
  }


  // One-line comment
  std::string tikz::print_comment(const std::string& msg) const
  {
    std::stringstream obuf;
    obuf << "%% " << msg << std::endl;

    return obuf.str();
  }

  // verbatim string, newline protected
  std::string tikz::print_verbatim(const std::string& msg) const
  {
    std::stringstream obuf;
    obuf << msg << "%" << std::endl;

    return obuf.str();
  }


  // start/end a picture-like environment, set unit length
  std::string tikz::start_picture(const pair& sz, const pair& offset) const
  {
    std::stringstream obuf;
    obuf << "\\begin{tikzpicture}" << std::endl
	 << "\\pgfsetlinewidth{" << format::print(PLAIN_WIDTH) << "}"
	 << std::endl
	 << "\\useasboundingbox " << print(offset)
	 << " rectangle " << print(sz + offset) << ";"
	 << std::endl;

    return obuf.str();
  }

  std::string tikz::end_picture() const
  {
    std::stringstream obuf;
    obuf << "\\end{tikzpicture}" << std::endl;

    return obuf.str();
  }

  // Actual work done in start_picture
  std::string tikz::set_unitlength(const std::string& len) const
  {
    return "";
    /*
    std::stringstream obuf;

    obuf << "\\setlength{\\unitlength}{1" << len << "}%" << std::endl;

    return obuf.str();
    */
  }


  void tikz::reset_state() const
  {
    m_fill = Neutral();
    m_stroke = Black();
    m_lwidth = PLAIN_WIDTH;
  }

  //// private member functions ////
  std::string tikz::path_connector() const
  {
    return "--";
  }


  std::string tikz::usepackages() const
  {
    return "usepackages tikz";
  }

  // string argument for passing attributes local to this path/loop
  std::string tikz::start_open_path(const std::string& attribs) const
  {
    return "\\draw " + attribs;
  }

  std::string tikz::end_open_path(const std::string&) const
  {
    std::stringstream obuf;
    obuf << ";" << std::endl;

    return obuf.str();
  }

  std::string tikz::start_closed_path(const std::string& attribs) const
  {
    return "\\draw " + attribs;
  }

  std::string tikz::end_closed_path(const std::string&) const
  {
    std::stringstream obuf;
    obuf <<  "--cycle;" << std::endl;

    return obuf.str();
  }

  // print declarations to set state of output format
  std::string tikz::set_fill_state(const Color& col) const
  {
    std::stringstream buf;
    if (col != m_fill && !col.is_unset())
      buf << "\\pgfsetfillcolor{" << format::print(col) << "}" << std::endl;

    m_fill = col;
    return buf.str();
  }

  std::string tikz::set_pen_state(const pen_data& pen) const
  {
    std::stringstream buf;
    if (pen.color() != m_stroke)
      {
	buf << "\\pgfsetstrokecolor{" << format::print(pen.color()) << "}"
	    << std::endl;

	m_stroke = pen.color();
      }

    if (pen.width() != m_lwidth)
      {
	buf << "\\pgfsetlinewidth{" << format::print(pen.width()) << "}"
	    << std::endl;

	m_lwidth = pen.width();
      }

    return buf.str();
  }

  // place a LaTeX box of width zero (containing string) at location (pair)
  std::string tikz::put_box(const pair& loc, const std::string& msg) const
  {
    std::stringstream obuf;

    obuf << "\\pgftext[at={\\pgfpoint{"
	 << truncate(loc.x1()) << m_units << "}{"
	 << truncate(loc.x2()) << m_units
	 << "}}] {" << msg << "}" << std::endl;

    return obuf.str();
  }

  std::string tikz::print_circle_marker(const pair& here, double diam,
					bool fill, const Color& color,
					const std::string& len) const
  {
    std::stringstream obuf;
    obuf << "\\";

    if (fill)
      obuf << "fill";

    obuf << "draw[color=" << format::print(color) << "] "
	 << print(here) << " circle(" << 0.5*diam << len << ");" << std::endl;

    return obuf.str();
  }

  std::string tikz::print(const pair& arg) const
  {
    std::stringstream o;
    o << "(" << truncate(arg.x1()) << m_units
      << "," << truncate(arg.x2()) << m_units << ")";
    return o.str();
  }
} // end of namespace
