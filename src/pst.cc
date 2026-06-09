/*
 * pst.cc -- ePiX's pstricks output format
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.20
 * Last Change: September 19, 2007
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
#include <list>

#include <string>
#include <sstream>

#include <set>

#include "constants.h"

#include "functions.h"

#include "pairs.h"
#include "edge_data.h"

#include "Color.h"

#include "path_style.h"
#include "pen_data.h"

#include "format.h"
#include "pst.h"

namespace ePiX {

  pst::pst()
    : m_fill(Neutral()), m_line(Black()), m_base(Neutral()),
      m_lwidth(PLAIN_WIDTH), m_bwidth("0pt") { }

  pst* pst::clone() const
  {
    return new pst(*this);
  }

  std::string pst::print_fill(const std::list<edge2d>& edges,
			      const pair& offset,
			      const Color& fc,
			      const pen_data& line,
			      const std::string& len) const
  {
    std::stringstream obuf;

    // compute attribute string
    std::string attribs("[fillstyle=");

    if (fc.is_unset())
      attribs += "none";
    else
      attribs += "solid";

    attribs += "]";

    obuf << set_fill_state(fc) << set_pen_state(line); // side effects
    obuf << print_paths(edges, offset, attribs, len);

    return obuf.str();
  }


  std::string pst::print_line(const std::list<edge2d>& edges,
			      const pair& offset,
			      const pen_data& line,
			      const pen_data& base,
			      const path_state& style,
			      const std::string& len) const
  {
    std::stringstream value, obuf;

    obuf << set_pen_state(line);

    // update base
    m_base = base.color();
    m_bwidth = base.width();

    std::string attribs;

    // Generate string to pass as third argument to print_paths if need border
    if (m_lwidth < m_bwidth && !m_base.is_unset())
      {
	length tmp_b(m_bwidth), tmp_l(m_lwidth);
	tmp_b += (tmp_l *= -1);
	tmp_b *= 0.5;

	std::stringstream bordbuf;
	bordbuf << "[border=" << print(tmp_b)
		<< ",bordercolor=" << m_base.name() << "]";

	attribs = bordbuf.str();
      }

    if (style.is_solid())
      obuf << print_paths(edges, offset, attribs, len);

    else // not solid
      {
	// Use base to draw solid underlayer, line to draw top layer.
	// Attempt to adjust PST pen alignment; m_bwidth too large.
	if (m_lwidth < m_bwidth && !base.color().is_unset())
	  obuf << set_pen_state(pen_data(base.color(),
					 0.5*(m_bwidth + m_lwidth)))
	       << print_paths(edges, offset, attribs, len);

	obuf << format::print_line(edges, offset, line, style, "", len);
      }

    return obuf.str();
  }


  std::string pst::print_color(const std::string& model,
			       const std::string& name,
			       double d1, double d2,
			       double d3) const
  {
    // PSTricks declaration
    std::stringstream obuf;
    obuf << "\\new" << model;
    if (model == "cmy")
      obuf << "k";

    obuf << "color{" << name << "}";

    if (model == "cmy")
      {
	double bk(min(min(d1,d2),d3));

	obuf << "{" 
	     << d1-bk << " " << d2-bk << " " << d3-bk << " " << bk
	     << "}%"  << std::endl;
      }

    else
      obuf << "{"
	   << d1 << " " << d2 << " " << d3
	   << "}%" << std::endl;

    return obuf.str();
  }

  std::string pst::print_color(const std::string& model,
			       const std::string& name,
			       double d1, double d2,
			       double d3, double d4) const
  {
    // model presumably "cmyk"
    std::stringstream obuf;
    obuf << "\\newcmykcolor{" << name << "}{"
	 << d1 << " " << d2 << " " << d3 << " " << d4 << "}%"
	 << std::endl;

    return obuf.str();
  }


  std::string pst::print_comment(const std::string& msg) const
  {
    std::stringstream obuf;
    obuf << "%% " << msg << std::endl;

    return obuf.str();
  }

  // verbatim text, %-protected newline
  std::string pst::print_verbatim(const std::string& msg) const
  {
    std::stringstream obuf;
    obuf << msg << "%" << std::endl;

    return obuf.str();
  }

  void pst::reset_state() const
  {
    m_fill = Neutral();
    m_line = Neutral();
    m_base = Neutral();

    m_lwidth = length(0);
    m_bwidth = length(0);
  }


  //// private member functions ////
  std::string pst::start_picture(const pair& sz, const pair& offset) const
  {
    std::stringstream obuf;
    obuf << "\\begin{pspicture}"
	 << format::print(sz) << format::print(-offset) << "%" << std::endl;

    return obuf.str();
  }

  std::string pst::end_picture() const
  {
    std::stringstream obuf;
    obuf << "\\end{pspicture}%" << std::endl;
    return obuf.str();
  }

  std::string pst::set_unitlength(const std::string& len) const
  {
    std::stringstream obuf;

    obuf << "\\psset{unit=1" << len
	 << ",linewidth=" << print(length(PLAIN_WIDTH)) << "}%"
	 << std::endl;

    return obuf.str();
  }


  std::string pst::usepackages() const
  {
    return "usepackages pstricks";
  }

  std::string pst::start_open_path(const std::string& attribs) const
  {
    return "\\psline" + attribs;
  }

  std::string pst::end_open_path(const std::string& attribs) const
  {
    std::stringstream obuf;
    obuf << std::endl;
    return obuf.str();
  }

  std::string pst::start_closed_path(const std::string& attribs) const
  {
    return "\\pspolygon" + attribs;
  }

  std::string pst::end_closed_path(const std::string& attribs) const
  {
    std::stringstream obuf;
    obuf << std::endl;
    return obuf.str();
  }

  std::string pst::set_fill_state(const Color& col) const
  {
    if (m_fill == col)
      return "";

    // else
    m_fill = col;

    std::stringstream obuf;
    obuf << "\\psset{";

    // filled paths set fillstyle
    if (m_fill.is_unset())
      obuf << "fillstyle=none";

    else
      obuf << "fillcolor=" << m_fill.name();

    obuf << "}%" << std::endl;

    return obuf.str();
  }

  std::string pst::set_pen_state(const pen_data& pen) const
  {
    std::stringstream obuf;

    if (pen.color().is_unset())
      {
	m_lwidth=length(0);
	return "\\psset{linewidth=0pt}";
      }

    // else
    bool change_width(m_lwidth != pen.width());
    bool change_color(m_line != pen.color());

    if (change_width || change_color)
      {
	obuf << "\\psset{";

	if (change_width)
	  {
	    m_lwidth = pen.width();
	    obuf << "linewidth=" << m_lwidth.name();

	    if (change_color) // both true, need separator
	      obuf << ",";
	  }

	if (change_color)
	  {
	    m_line = pen.color();
	    obuf << "linecolor=" << m_line.name();
	  }

	obuf << "}%" << std::endl;
      }

    return obuf.str();
  }

  // place a LaTeX box of width zero (containing string) at location (pair)
  std::string pst::put_box(const pair& loc, const std::string& msg) const
  {
    std::stringstream obuf;
    obuf << "\\rput" << print(loc) << "{" << msg << "}" << std::endl;

    return obuf.str();
  }

  std::string pst::print_circle_marker(const pair& here, double diam,
				       bool fill, const Color& color,
				       const std::string& len) const
  {
    std::stringstream obuf;
    obuf << "\\pscircle";

    if (fill)
      obuf << "*";

    obuf << "[linecolor=" << print(color) << "]" << print(here)
	 << "{" << 0.5*diam << "}%" << std::endl;

    return obuf.str();
  }
} // end of namespace
