/*
 * Color_Neutral.cc -- ePiX::Neutral_Color
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.18
 * Last Change: September 14, 2007
 */

/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
 * Department of Mathematics and Computer Science
 * College of the Holy Cross
 * Worcester, MA, 01610-2395, USA
 *
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

#include <string>

#include "Color.h"
#include "Color_Neutral.h"

namespace ePiX {

  Neutral_Color::Neutral_Color() { }

  Color_Base::RGB_Densities Neutral_Color::to_rgb() const
  {
    return Color_Base::RGB_Densities(1, 1, 1);
  }

  Neutral_Color* Neutral_Color::clone() const
  {
    return new Neutral_Color(*this);
  }


  Color_Base& Neutral_Color::filter(const Color_Base& color)
  {
    Color_Base* tmp(color.clone());
    return *tmp;
  }

  Neutral_Color& Neutral_Color::operator*= (double c)
  {
    return *this;
  }

  Neutral_Color& Neutral_Color::blend(const Color_Base& color, double d)
  {
    return *this;
  }

  Neutral_Color& Neutral_Color::superpose(const Color_Base& color)
  {
    return *this;
  }

  Neutral_Color& Neutral_Color::invert()
  {
    return *this;
  }

  // string identifier -- not output-specific
  std::string Neutral_Color::name() const
  {
    return "";
  }

  std::string Neutral_Color::model() const
  {
    return "";
  }

  std::vector<double> Neutral_Color::densities() const
  {
    std::vector<double> value(0);
    return value;
  }
} // end of namespace
