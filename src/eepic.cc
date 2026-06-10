/*
 * eepic.cc -- ePiX's eepic output format
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2.0-2
 * Last Change: September 26, 2007
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
#include <list>

#include <string>
#include <sstream>

#include <set>  // for palette

#include "constants.h"

#include "functions.h"

#include "pairs.h"
#include "edge_data.h"

#include "Color.h"

#include "path_style.h"
#include "pen_data.h"

#include "hatching.h"

#include "format.h"
#include "eepic.h"

namespace ePiX {

// rotate successive hatches this much to avoid parallelity
const double d_hatch_theta(111.24612);  // ~90*(sqrt(5)-1)

eepic::eepic() : m_ink(Black()), m_nib(PLAIN_WIDTH), m_hatch(0) {}

eepic* eepic::clone() const { return new eepic(*this); }

// Filled region with specified Cartesian edges, offset, and color
std::string eepic::print_fill(const std::list<edge2d>& edges,
                              const pair& offset, const Color& fc,
                              const pen_data& line,
                              const std::string& len) const {
  std::stringstream obuf;

  // set angle
  const double hatch_angle(m_hatch);
  m_hatch += d_hatch_theta;

  // draw hatch lines separated by pen.width(), actual line width
  // scaled by density
  double dens(fc.alpha());  // transparency
  if (dens < EPIX_EPSILON) return "";

  // else
  length hatch_width(length(PLAIN_WIDTH).to(len));

  path_state style;  // solid

  if (EPIX_EPSILON < dens * hatch_width.magnitude()) {
    // Use fill color, draw thin lines to simulate transparency.
    // Area covered by double-hatching is (supposed to be) dens.
    pen_data fill_pen(fc, (1 - sqrt(1 - dens)) * hatch_width);

    // draw hatch-filled edges
    hatch_data bd1(hatch_angle, hatch_width.magnitude(), edges);
    for (unsigned int i = 0; i < bd1.data().size(); ++i)
      obuf << format::print_line(bd1.data().at(i), offset, fill_pen, style,
                                 std::string(""), len);

    // re-draw hatch lines at right angles...
    hatch_data bd2(hatch_angle + 90, hatch_width.magnitude(), edges);
    for (unsigned int i = 0; i < bd2.data().size(); ++i)
      obuf << format::print_line(bd2.data().at(i), offset, fill_pen, style, "",
                                 len);
  }

  obuf << set_pen_state(line);
  obuf << print_paths(edges, offset, "", len);

  return obuf.str();
}  // end of eepic::print_fill()

// may assume line is visible
std::string eepic::print_line(const std::list<edge2d>& edges,
                              const pair& offset, const pen_data& line,
                              const pen_data& base, const path_state& style,
                              const std::string& len) const {
  std::string value;

  // draw *solid* base first if necessary
  if (line.width() < base.width() && !base.color().is_unset())
    value += format::print_line(edges, offset, base, path_state(), "", len);

  return value += format::print_line(edges, offset, line, style, "", len);
}

std::string eepic::print_color(const std::string& model,
                               const std::string& name, double d1, double d2,
                               double d3) const {
  return format::xdefinecolor(model, name, d1, d2, d3);
}

std::string eepic::print_color(const std::string& model,
                               const std::string& name, double d1, double d2,
                               double d3, double d4) const {
  return format::xdefinecolor(model, name, d1, d2, d3, d4);
}

// one-line comment
std::string eepic::print_comment(const std::string& msg) const {
  std::stringstream obuf;
  obuf << "%% " << msg << std::endl;

  return obuf.str();
}

// verbatim string, newline protected
std::string eepic::print_verbatim(const std::string& msg) const {
  std::stringstream obuf;
  obuf << msg << "%" << std::endl;

  return obuf.str();
}

void eepic::reset_state() const {
  m_ink = Neutral();
  m_nib = length(0);
}

//// private member functions ////
std::string eepic::start_picture(const pair& sz, const pair& offset) const {
  std::stringstream obuf;
  obuf << "\\begin{picture}" << format::print(sz) << format::print(-offset)
       << "%" << std::endl;

  return obuf.str();
}

std::string eepic::end_picture() const {
  std::stringstream obuf;
  obuf << "\\end{picture}%" << std::endl;
  return obuf.str();
}

std::string eepic::set_unitlength(const std::string& len) const {
  std::stringstream obuf;

  obuf << "\\setlength{\\unitlength}{1" << len << "}%" << std::endl;

  return obuf.str();
}

std::string eepic::usepackages() const {
  return "usepackages epic,eepic,xcolor";
}

// string argument for passing attributes local to this path/loop
std::string eepic::start_open_path(const std::string& attribs) const {
  return "\\path";
}

std::string eepic::end_open_path(const std::string& attribs) const {
  std::stringstream obuf;
  obuf << std::endl;
  return obuf.str();
}

std::string eepic::start_closed_path(const std::string& attribs) const {
  return "\\path";
}

std::string eepic::end_closed_path(const std::string& attribs) const {
  std::stringstream obuf;
  obuf << std::endl;
  return obuf.str();
}

std::string eepic::set_fill_state(const Color& col) const  // unused
{
  std::stringstream obuf;

  if (m_ink != col) {
    m_ink = col;
    obuf << "\\color{" << m_ink.name() << "}%" << std::endl;
  }

  return obuf.str();
}

std::string eepic::set_pen_state(const pen_data& pen) const {
  std::stringstream obuf;

  if (m_nib != pen.width()) {
    m_nib = pen.width();
    obuf << "\\allinethickness{" << m_nib.name() << "}%" << std::endl;
  }

  if (m_ink != pen.color()) {
    m_ink = pen.color();
    obuf << "\\color{" << m_ink.name() << "}%" << std::endl;
  }

  return obuf.str();
}

// place a LaTeX box of width zero (containing string) at location (pair)
std::string eepic::put_box(const pair& loc, const std::string& msg) const {
  std::stringstream obuf;
  obuf << "\\put" << print(loc) << "{" << msg << "}" << std::endl;

  return obuf.str();
}

std::string eepic::print_circle_marker(const pair& here, double diam, bool fill,
                                       const Color& color,
                                       const std::string& len) const {
  std::stringstream obuf;
  obuf << "\\put" << print(here) << "{\\color{" << print(color) << "}$";

  if (fill)
    obuf << "\\allinethickness{" << 0.5 * diam << len << "}"
         << "\\circle{" << 0.5 * diam << "}";

  else
    obuf << "\\circle{" << diam << "}";

  obuf << "$}%" << std::endl;

  return obuf.str();
}
}  // namespace ePiX
