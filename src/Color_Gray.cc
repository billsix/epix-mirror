/*
 * Color_Gray.cc -- ePiX::Gray_Color functions
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
#include "Color_Gray.h"

namespace ePiX {

Gray_Color::Gray_Color(double d) : m_gray(clip_to_unit(d)) {}

Gray_Color::Gray_Color(const RGB_Densities& s)
    : m_gray((1.0 / 3) * (s.m_dens_red + s.m_dens_green + s.m_dens_blue)) {}

Color_Base::RGB_Densities Gray_Color::to_rgb() const {
  return Color_Base::RGB_Densities(m_gray, m_gray, m_gray);
}

Gray_Color* Gray_Color::clone() const { return new Gray_Color(*this); }

Gray_Color& Gray_Color::filter(const Color_Base& color) {
  Color_Base::RGB_Densities s(color.to_rgb());

  m_gray = std::min(
      m_gray, (1.0 / 3) * (s.m_dens_red + s.m_dens_green + s.m_dens_blue));
  return *this;
}

Gray_Color& Gray_Color::operator*=(double c) {
  m_gray = clip_to_unit(c * m_gray);
  return *this;
}

Gray_Color& Gray_Color::blend(const Color_Base& color, double d) {
  Color_Base::RGB_Densities s(color.to_rgb());
  double wt(clip_to_unit(d));

  m_gray += wt * ((1.0 / 3) * (s.m_dens_red + s.m_dens_green + s.m_dens_blue) -
                  m_gray);

  return *this;
}

Gray_Color& Gray_Color::superpose(const Color_Base& color) {
  Color_Base::RGB_Densities s(color.to_rgb());

  m_gray = clip_to_unit(
      (1.0 / 3) * (s.m_dens_red + s.m_dens_green + s.m_dens_blue) + m_gray);

  return *this;
}

Gray_Color& Gray_Color::invert() {
  m_gray = 1 - m_gray;
  return *this;
}

std::string Gray_Color::model() const { return "rgb"; }

std::string Gray_Color::name() const {
  std::ostringstream nm;

  nm << "gray_" << dtohex(m_gray);

  return nm.str();
}

std::vector<double> Gray_Color::densities() const {
  std::vector<double> val(3);
  val.at(0) = rd(m_gray);
  val.at(1) = rd(m_gray);
  val.at(2) = rd(m_gray);
  return val;
}
}  // namespace ePiX
