/*
 * Color_Sep.cc -- ePiX CMYK Color separation functions
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
#include "Color_Sep.h"

namespace ePiX {

  Cyan_Layer::Cyan_Layer(double arg)
    : m_dens(clip_to_unit(arg)) { }

  // Return Cyan channel of color
  Cyan_Layer& Cyan_Layer::filter(const Color_Base& color)
  {
    Color_Base::RGB_Densities s(color.to_rgb());
    if (Color(color).is_unset())
      s = Color_Base::RGB_Densities(1,1,1);

    m_dens = std::min(m_dens, 1-s.m_dens_red);
    return *this;
  }

  Cyan_Layer* Cyan_Layer::clone() const
  {
    return new Cyan_Layer(*this);
  }

  Color_Base::RGB_Densities Cyan_Layer::to_rgb() const
  {
    return Color_Base::RGB_Densities(1-m_dens, 1, 1);
  }


  std::string Cyan_Layer::model() const
  {
    return "cmyk";
  }

  std::string Cyan_Layer::name() const
  {
    std::ostringstream nm;
    nm << "c_proc_" << dtohex(m_dens);
    return nm.str();
  }

  std::vector<double> Cyan_Layer::densities() const
  {
    std::vector<double> val(4);
    val.at(0) = rd(m_dens);
    val.at(1) = 0;
    val.at(2) = 0;
    val.at(3) = 0;
    return val;
  }


  // Magenta_Layer
  Magenta_Layer::Magenta_Layer(double arg)
    : m_dens(clip_to_unit(arg)) { }

  // Return Magenta channel of color
  Magenta_Layer& Magenta_Layer::filter(const Color_Base& color)
  {
    Color_Base::RGB_Densities s(color.to_rgb());
    if (Color(color).is_unset())
      s = Color_Base::RGB_Densities(1,1,1);

    m_dens = std::min(m_dens, 1-s.m_dens_green);
    return *this;
  }

  Magenta_Layer* Magenta_Layer::clone() const
  {
    return new Magenta_Layer(*this);
  }

  Color_Base::RGB_Densities Magenta_Layer::to_rgb() const
  {
    return Color_Base::RGB_Densities(1, 1-m_dens, 1);
  }


  std::string Magenta_Layer::model() const
  {
    return "cmyk";
  }

  std::string Magenta_Layer::name() const
  {
    std::ostringstream nm;
    nm << "m_proc_"  << dtohex(m_dens);
    return nm.str();
  }

  std::vector<double> Magenta_Layer::densities() const
  {
    std::vector<double> val(4);
    val.at(0) = 0;
    val.at(1) = rd(m_dens);
    val.at(2) = 0;
    val.at(3) = 0;
    return val;
  }


  // Yellow_Layer
  Yellow_Layer::Yellow_Layer(double arg)
    : m_dens(clip_to_unit(arg)) { }

  // Return Yellow channel of color
  Yellow_Layer& Yellow_Layer::filter(const Color_Base& color)
  {
    Color_Base::RGB_Densities s(color.to_rgb());
    if (Color(color).is_unset())
      s = Color_Base::RGB_Densities(1,1,1);

    m_dens = std::min(m_dens, 1-s.m_dens_blue);
    return *this;
  }

  Yellow_Layer* Yellow_Layer::clone() const
  {
    return new Yellow_Layer(*this);
  }

  Color_Base::RGB_Densities Yellow_Layer::to_rgb() const
  {
    return Color_Base::RGB_Densities(1, 1, 1-m_dens);
  }


  std::string Yellow_Layer::model() const
  {
    return "cmyk";
  }

  std::string Yellow_Layer::name() const
  {
    std::ostringstream nm;
    nm << "y_proc_" << dtohex(m_dens);
    return nm.str();
  }

  std::vector<double> Yellow_Layer::densities() const
  {
    std::vector<double> val(4);
    val.at(0) = 0;
    val.at(1) = 0;
    val.at(2) = rd(m_dens);
    val.at(3) = 0;
    return val;
  }


  // Black_Layer
  Black_Layer::Black_Layer(double arg)
    : m_dens(clip_to_unit(arg)) { }

  // Return Black channel of color
  Black_Layer& Black_Layer::filter(const Color_Base& color)
  {
    Color_Base::RGB_Densities s(color.to_rgb());
    if (Color(color).is_unset())
      s = Color_Base::RGB_Densities(1,1,1);

    double c(1-s.m_dens_red), m(1-s.m_dens_green), y(1-s.m_dens_blue);
    double k(std::min(std::min(c, m), y));

    m_dens = std::min(m_dens, k);
    return *this;
  }

  Black_Layer* Black_Layer::clone() const
  {
    return new Black_Layer(*this);
  }

  Color_Base::RGB_Densities Black_Layer::to_rgb() const
  {
    return Color_Base::RGB_Densities(1-m_dens, 1-m_dens, 1-m_dens);
  }


  std::string Black_Layer::model() const
  {
    return "cmyk";
  }

  std::string Black_Layer::name() const
  {
    std::ostringstream nm;
    nm << "k_proc_" << dtohex(m_dens);
    return nm.str();
  }

  std::vector<double> Black_Layer::densities() const
  {
    std::vector<double> val(4);
    val.at(0) = 0;
    val.at(1) = 0;
    val.at(2) = 0;
    val.at(3) = rd(m_dens);
    return val;
  }


  // null operations
  Cyan_Layer& Cyan_Layer::operator*= (double x) { return *this; }
  Cyan_Layer& Cyan_Layer::blend(const Color_Base& col, double d)
  { return *this; }

  Cyan_Layer& Cyan_Layer::superpose(const Color_Base& col) { return *this; }
  Cyan_Layer& Cyan_Layer::invert() { return *this; }


  Magenta_Layer& Magenta_Layer::operator*= (double x) { return *this; }
  Magenta_Layer& Magenta_Layer::blend(const Color_Base& col, double d)
  { return *this; }

  Magenta_Layer& Magenta_Layer::superpose(const Color_Base& col) { return *this; }
  Magenta_Layer& Magenta_Layer::invert() { return *this; }


  Yellow_Layer& Yellow_Layer::operator*= (double x) { return *this; }
  Yellow_Layer& Yellow_Layer::blend(const Color_Base& col, double d)
  { return *this; }

  Yellow_Layer& Yellow_Layer::superpose(const Color_Base& col) { return *this; }
  Yellow_Layer& Yellow_Layer::invert() { return *this; }


  Black_Layer& Black_Layer::operator*= (double x) { return *this; }
  Black_Layer& Black_Layer::blend(const Color_Base& col, double d)
  { return *this; }

  Black_Layer& Black_Layer::superpose(const Color_Base& col) { return *this; }
  Black_Layer& Black_Layer::invert() { return *this; }

} // end of namespace
