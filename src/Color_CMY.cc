/*
 * Color_CMY.cc -- ePiX::CMY_Color functions
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2.2
 *
 * Last Change: October 30, 2007
 *
 *
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
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
#include "Color_CMY.h"

namespace ePiX {

CMY_Color::CMY_Color(double c, double m, double y)
    : cyan(make_r(c, m, y)),
      magenta(make_g(c, m, y)),
      yellow(make_b(c, m, y)) {}

// private, no arg checking, bool unused
CMY_Color::CMY_Color(double c, double m, double y, const bool arg)
    : cyan(c), magenta(m), yellow(y) {}

CMY_Color::CMY_Color(const RGB_Densities& s)
    : cyan(1 - s.m_dens_red),
      magenta(1 - s.m_dens_green),
      yellow(1 - s.m_dens_blue) {}

Color_Base::RGB_Densities CMY_Color::to_rgb() const {
  // Use conventional formulas since our densities are already in [0,1]
  return Color_Base::RGB_Densities(1 - cyan, 1 - magenta, 1 - yellow);
}

CMY_Color* CMY_Color::clone() const { return new CMY_Color(*this); }

CMY_Color& CMY_Color::filter(const Color_Base& color) {
  Color_Base::RGB_Densities s(color.to_rgb());

  // filter in RGB; 1 - min(1-x, 1-y) = max(x, y)
  cyan = std::max(cyan, 1 - s.m_dens_red);
  magenta = std::max(magenta, 1 - s.m_dens_green);
  yellow = std::max(yellow, 1 - s.m_dens_blue);

  return *this;
}

CMY_Color& CMY_Color::operator*=(double sc) {
  Color_Base::RGB_Densities s(to_rgb());

  // scale in RGB
  double r(sc * (s.m_dens_red));
  double g(sc * (s.m_dens_green));
  double b(sc * (s.m_dens_blue));

  cyan = 1 - make_r(r, g, b);
  magenta = 1 - make_g(r, g, b);
  yellow = 1 - make_b(r, g, b);

  return *this;
}

CMY_Color& CMY_Color::blend(const Color_Base& color, double d) {
  Color_Base::RGB_Densities s(color.to_rgb());
  double wt(clip_to_unit(d));

  // blending is a convex lin. comb., may as well do in CMY
  cyan += wt * (1 - cyan - s.m_dens_red);
  magenta += wt * (1 - magenta - s.m_dens_green);
  yellow += wt * (1 - yellow - s.m_dens_blue);

  return *this;
}

CMY_Color& CMY_Color::superpose(const Color_Base& color) {
  Color_Base::RGB_Densities s(color.to_rgb());

  // add in RGB
  cyan = 1 - clip_to_unit(1 - cyan + s.m_dens_red);
  magenta = 1 - clip_to_unit(1 - magenta + s.m_dens_green);
  yellow = 1 - clip_to_unit(1 - yellow + s.m_dens_blue);

  return *this;
}

CMY_Color& CMY_Color::invert() {
  cyan = 1 - cyan;
  magenta = 1 - magenta;
  yellow = 1 - yellow;

  return *this;
}

std::string CMY_Color::model() const { return "cmy"; }

std::string CMY_Color::name() const {
  std::ostringstream nm;

  nm << "cmy_" << dtohex(cyan) << dtohex(magenta) << dtohex(yellow);

  return nm.str();
}

std::vector<double> CMY_Color::densities() const {
  std::vector<double> val(3);
  val.at(0) = rd(cyan);
  val.at(1) = rd(magenta);
  val.at(2) = rd(yellow);
  return val;
}
}  // namespace ePiX
