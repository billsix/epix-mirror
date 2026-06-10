/*
 * Color_CMYK.cc -- ePiX::CMYK_Color functions
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2.2
 *
 * Last Change: October 30, 2007
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

#include <sstream>
#include <string>
#include <vector>

#include <cmath>

#include "Color_Utils.h"
#include "Color.h"
#include "Color_CMYK.h"

namespace ePiX {

// Behaves "classically" if c+k, m+k, y+k are in [0,1]
// Except as noted, CMYK methods enforce/preserve this condition
CMYK_Color::CMYK_Color(double c, double m, double y, double k)
    : cyan(make_r(c + k, m + k, y + k)),
      magenta(make_g(c + k, m + k, y + k)),
      yellow(make_b(c + k, m + k, y + k)),
      black(std::min(std::min(cyan, magenta), yellow)) {
  /* conventional RGB densities
  double r(1 - std::min(1, c+k));
  double g(1 - std::min(1, m+k));
  double b(1 - std::min(1, y+k));
  */
  cyan -= black;
  magenta -= black;
  yellow -= black;
}

// private, no arg checking, bool unused
CMYK_Color::CMYK_Color(double c, double m, double y, double k, const bool arg)
    : cyan(c), magenta(m), yellow(y), black(k) {}

// black = min(c,m,y), following Uwe Kern's xcolor documentation
// V 1.11, 2004/05/09, p. 31
CMYK_Color::CMYK_Color(const RGB_Densities& s) {
  // may assume densities are in [0,1]
  double c(1 - s.m_dens_red), m(1 - s.m_dens_green), y(1 - s.m_dens_blue);

  black = std::min(std::min(c, m), y);
  cyan = c - black;
  magenta = m - black;
  yellow = y - black;
}

Color_Base::RGB_Densities CMYK_Color::to_rgb() const {
  // Use conventional formulas since our densities are already in [0,1];
  // "min" for safety, should be unnecessary.
  return Color_Base::RGB_Densities(1 - std::min(1.0, cyan + black),
                                   1 - std::min(1.0, magenta + black),
                                   1 - std::min(1.0, yellow + black));
}

CMYK_Color* CMYK_Color::clone() const { return new CMYK_Color(*this); }

CMYK_Color& CMYK_Color::filter(const Color_Base& color) {
  Color_Base::RGB_Densities s(color.to_rgb());  // arg
  Color_Base::RGB_Densities t(to_rgb());        // this

  // filter in RGB
  double c(1 - std::min(t.m_dens_red, s.m_dens_red));
  double m(1 - std::min(t.m_dens_green, s.m_dens_green));
  double y(1 - std::min(t.m_dens_blue, s.m_dens_blue));
  black = std::min(std::min(c, m), y);

  cyan = c - black;
  magenta = m - black;
  yellow = y - black;

  return *this;
}

CMYK_Color& CMYK_Color::operator*=(double sc) {
  Color_Base::RGB_Densities s(to_rgb());

  // scale in RGB
  double r(sc * (s.m_dens_red));
  double g(sc * (s.m_dens_green));
  double b(sc * (s.m_dens_blue));

  // Necessary b/c sc may not be in [0,1]
  double c(1 - make_r(r, g, b));
  double m(1 - make_g(r, g, b));
  double y(1 - make_b(r, g, b));

  black = std::min(std::min(c, m), y);
  cyan = c - black;
  magenta = m - black;
  yellow = y - black;

  return *this;
}

CMYK_Color& CMYK_Color::blend(const Color_Base& color, double d) {
  Color_Base::RGB_Densities s(color.to_rgb());
  double wt(clip_to_unit(d));

  double c((1 - wt) * std::min(1.0, cyan + black) + wt * (1 - s.m_dens_red));
  double m((1 - wt) * std::min(1.0, magenta + black) +
           wt * (1 - s.m_dens_green));
  double y((1 - wt) * std::min(1.0, yellow + black) + wt * (1 - s.m_dens_blue));

  black = std::min(std::min(c, m), y);
  cyan = c - black;
  magenta = m - black;
  yellow = y - black;

  return *this;
}

CMYK_Color& CMYK_Color::superpose(const Color_Base& color) {
  Color_Base::RGB_Densities s(color.to_rgb());
  Color_Base::RGB_Densities t(to_rgb());

  // add in RGB
  double c(1 - clip_to_unit(t.m_dens_red + s.m_dens_red));
  double m(1 - clip_to_unit(t.m_dens_green + s.m_dens_green));
  double y(1 - clip_to_unit(t.m_dens_blue + s.m_dens_blue));

  black = std::min(std::min(c, m), y);
  cyan = c - black;
  magenta = m - black;
  yellow = y - black;

  return *this;
}

CMYK_Color& CMYK_Color::invert() {
  Color_Base::RGB_Densities s(to_rgb());

  // invert in RGB
  double c(1 - s.m_dens_red);
  double m(1 - s.m_dens_green);
  double y(1 - s.m_dens_blue);

  black = std::min(std::min(c, m), y);
  cyan = c - black;
  magenta = m - black;
  yellow = y - black;

  return *this;
}

std::string CMYK_Color::model() const { return "cmyk"; }

std::string CMYK_Color::name() const {
  std::ostringstream nm;

  nm << "cmyk_" << dtohex(cyan) << dtohex(magenta) << dtohex(yellow)
     << dtohex(black);
  return nm.str();
}

std::vector<double> CMYK_Color::densities() const {
  std::vector<double> val(4);
  val.at(0) = rd(cyan);
  val.at(1) = rd(magenta);
  val.at(2) = rd(yellow);
  val.at(3) = rd(black);
  return val;
}
}  // namespace ePiX
