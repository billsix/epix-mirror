/*
 * Color_CMY.h -- ePiX::CMY_Color class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.17
 * Last Change: September 13, 2007
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
#ifndef EPIX_COLOR_CMY
#define EPIX_COLOR_CMY
#include <string>
#include <vector>

#include "Color_Base.h"
#include "Color.h"

namespace ePiX {

  class CMY_Color : public Color_Base {
  public:
    explicit CMY_Color(double c=1, double m=1, double y=1);
    CMY_Color(const RGB_Densities&);

    CMY_Color* clone() const override;

    CMY_Color& filter(const Color_Base&) override;

    CMY_Color& operator*= (double c) override;

    CMY_Color& blend(const Color_Base&, double) override;
    CMY_Color& superpose(const Color_Base&) override;
    CMY_Color& invert() override;

    std::string model() const override;
    std::string name() const override;
    std::vector<double> densities() const override;

    RGB_Densities to_rgb() const override;

    // These need to call our private constructor
    friend Color Cyan(double);
    friend Color Magenta(double);
    friend Color Yellow(double);
    friend Color CMY_White(double);
    friend Color CMY_Black(double);
    friend Color CMY_Neutral();

  private:
    double cyan;
    double magenta;
    double yellow;

    // Construct a CMY color without checking arguments. Final argument
    // only changes call signature
    CMY_Color(double c, double m, double y, const bool);
  }; // end of class CMY_Color

} // end of namespace
#endif /* EPIX_COLOR_CMY */
