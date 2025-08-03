/*
 * Color_RGB.h -- ePiX::RGB_Color class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.17
 * Last Change: September 13, 2007
 *
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
#ifndef EPIX_COLOR_RGB
#define EPIX_COLOR_RGB
#include <string>
#include <vector>

#include "Color_Base.h"

namespace ePiX {
  class RGB_Color : public Color_Base {
  public:
    RGB_Color(double r=0, double g=0, double b=0);
    RGB_Color(const RGB_Densities&);

    RGB_Color* clone() const;

    RGB_Color& filter(const Color_Base&);

    RGB_Color& operator*= (double c);

    RGB_Color& blend(const Color_Base&, double);
    RGB_Color& superpose(const Color_Base&);
    RGB_Color& invert();

    std::string model() const;
    std::string name() const;
    std::vector<double> densities() const;

    RGB_Densities to_rgb() const;

  private:
    double red;
    double green;
    double blue;
  }; // end of class RGB_Color

} // end of namespace
#endif /* EPIX_COLOR_RGB */
