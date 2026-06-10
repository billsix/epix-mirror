/*
 * Color_Gray.h -- ePiX::Gray_Color class
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
#ifndef EPIX_COLOR_GRAY
#define EPIX_COLOR_GRAY
#include <string>
#include <vector>

#include "Color_Base.h"

namespace ePiX {

class Gray_Color : public Color_Base {
 public:
  Gray_Color(double dens = 0);
  Gray_Color(const RGB_Densities&);

  Gray_Color* clone() const override;

  Gray_Color& filter(const Color_Base&) override;

  Gray_Color& operator*=(double c) override;

  Gray_Color& blend(const Color_Base&, double) override;
  Gray_Color& superpose(const Color_Base&) override;
  Gray_Color& invert() override;

  std::string model() const override;
  std::string name() const override;
  std::vector<double> densities() const override;

  RGB_Densities to_rgb() const override;

 private:
  double m_gray;
};  // end of class Gray_Color

}  // namespace ePiX
#endif /* EPIX_COLOR_GRAY */
