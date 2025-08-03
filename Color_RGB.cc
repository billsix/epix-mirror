/*
 * Color_RGB.cc -- ePiX::RGB_Color
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2.2
 *
 * Last Change: October 30, 2007
 */

/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
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

#include <sstream>
#include <string>
#include <vector>

#include <cmath>

#include "Color_Utils.h"
#include "Color_RGB.h"

namespace ePiX {

  RGB_Color::RGB_Color(double r, double g, double b)
    : red(make_r(r, g, b)), green(make_g(r, g, b)), blue(make_b(r, g, b)) { }


  RGB_Color::RGB_Color(const RGB_Densities& s)
    : red(s.m_dens_red), green(s.m_dens_green), blue(s.m_dens_blue) { }

  Color_Base::RGB_Densities RGB_Color::to_rgb() const
  {
    return Color_Base::RGB_Densities(red, green, blue);
  }

  RGB_Color* RGB_Color::clone() const
  {
    return new RGB_Color(*this);
  }


  RGB_Color& RGB_Color::filter(const Color_Base& color)
  {
    Color_Base::RGB_Densities s(color.to_rgb());

    red   = std::min(red,   s.m_dens_red);
    green = std::min(green, s.m_dens_green);
    blue  = std::min(blue,  s.m_dens_blue);

    return *this;
  }

  RGB_Color& RGB_Color::operator*= (double c)
  {
    double r(red*c);
    double g(green*c);
    double b(blue*c);

    red   = make_r(r, g, b);
    green = make_g(r, g, b);
    blue  = make_b(r, g, b);

    return *this;
  }

  RGB_Color& RGB_Color::blend(const Color_Base& color, double d)
  {
    Color_Base::RGB_Densities s(color.to_rgb());
    double wt(clip_to_unit(d));

    red   += wt*(s.m_dens_red   - red);
    green += wt*(s.m_dens_green - green);
    blue  += wt*(s.m_dens_blue  - blue);

    return *this;
  }

  RGB_Color& RGB_Color::superpose(const Color_Base& color)
  {
    Color_Base::RGB_Densities s(color.to_rgb());

    red   = clip_to_unit(red   + s.m_dens_red);
    green = clip_to_unit(green + s.m_dens_green);
    blue  = clip_to_unit(blue  + s.m_dens_blue);

    return *this;
  }

  RGB_Color& RGB_Color::invert()
  {
    red   = 1 - red;
    green = 1 - green;
    blue  = 1 - blue;

    return *this;
  }

  std::string RGB_Color::model() const
  {
    return "rgb";
  }

  std::string RGB_Color::name() const
  {
    std::ostringstream nm;

    nm << "rgb_" << dtohex(red) << dtohex(green) << dtohex(blue);

    return nm.str();
  }

  std::vector<double> RGB_Color::densities() const
  {
    std::vector<double> val(3);
    val.at(0) = rd(red);
    val.at(1) = rd(green);
    val.at(2) = rd(blue);
    return val;
  }
} // end of namespace
