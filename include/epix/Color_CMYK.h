/*
 * Color_CMYK.h -- ePiX::CMYK_Color class
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
#ifndef EPIX_COLOR_CMYK
#define EPIX_COLOR_CMYK
#include <string>
#include <vector>

#include "Color_Base.h"
#include "Color.h"

namespace ePiX {

class CMYK_Color : public Color_Base {
 public:
  CMYK_Color(double c = 1, double m = 1, double y = 1, double k = 1);
  CMYK_Color(const RGB_Densities&);

  CMYK_Color* clone() const override;

  CMYK_Color& filter(const Color_Base&) override;

  CMYK_Color& operator*=(double c) override;

  CMYK_Color& blend(const Color_Base&, double) override;
  CMYK_Color& superpose(const Color_Base&) override;
  CMYK_Color& invert() override;

  std::string model() const override;
  std::string name() const override;
  std::vector<double> densities() const override;

  RGB_Densities to_rgb() const override;

  // These need to call our private constructor
  friend Color CyanK(double);
  friend Color MagentaK(double);
  friend Color YellowK(double);
  friend Color CMYK_White(double);
  friend Color CMYK_Black(double);
  friend Color CMYK_Neutral();

 private:
  double cyan;
  double magenta;
  double yellow;
  double black;

  // Construct a CMYK color without checking arguments. Final argument
  // only changes call signature
  CMYK_Color(double c, double m, double y, double k, const bool);
};  // end of class CMYK_Color

}  // namespace ePiX
#endif /* EPIX_COLOR_CMYK */
