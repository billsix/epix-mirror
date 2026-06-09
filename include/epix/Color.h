/*
 * Color.h -- ePiX::Color class and helpers
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.2.2
 * Last Change: November 12, 2007
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

/*
 * This file defines the ePiX Color user interface:
 *       operator*=           Modify intensity
 *       blend(Color, double) Color mixing
 *       filter(Color)        Apply us as a filter
 *       superpose(Color)     Add colors, clip intensities
 *       invert()             Color negative
 *
 *   - Named primary constructors
 *
 * Scaling and filtering are implemented by converting to the filter's
 * color model. Color model conversions come from the LaTeX xcolor
 * package documentation, V 1.11 (2044/05/09) by Uwe Kern.
 */

#ifndef EPIX_COLOR
#define EPIX_COLOR

#include <string>
#include <vector>

namespace ePiX {

  class Color_Base;

  class Color {
  public:
    Color();
    Color(const Color_Base*);
    Color(const Color_Base&);
    Color(const Color&);

    Color& operator= (const Color&);

    ~Color();

    const Color_Base* operator->() const;
    double alpha() const;

    bool is_unset() const;

    Color filter(const Color&) const;

    Color& operator*= (double c);     // scale the intensity
    Color& blend(const Color&, double);
    Color& superpose(const Color&);
    Color& invert();
    Color& alpha(double);

    std::string model() const;
    std::string name() const;
    std::vector<double> densities() const;

  private:
    Color_Base* m_color;
    double m_alpha;

  }; // end of class Color

  Color operator* (double, const Color&);
  const bool operator== (const Color&, const Color&);
  const bool operator!= (const Color&, const Color&);

  // for palette ordering
  const bool operator<  (const Color&, const Color&);


  /*
   * * *  Primary color functions in the global namespace * * *
   *
   * A primary color constructor accepts a numerical argument, which is
   * reduced mod 4, then interpreted as a color density as follows:
   * d = -2 or 2 : white
   * d = -1: full anti-saturation
   * d =  0: black
   * d =  1: full saturation (default)
   *
   * The primary changes continuously, and linearly on each interval
   * [-2,-1], [-1,0], [0,1], and [1,2].
   *
   * Example:
   * Red(0.3) = rgb(0.3, 0, 0) = Red(-3.7)
   * Red(1.3) = rgb(1,0.3,0.3) = Red(-2.7)
   * Red(2.3) = rgb(0.7, 1, 1) = Red(-1.7)
   * Red(3.3) = rgb(0,0.7,0.7) = Red(-0.7)
   */
  Color RGB(double r=0, double g=0, double b=0);
  Color CMY(double c=1, double m=1, double y=1);
  Color CMYK(double c=1, double m=1, double y=1,
	     double k=0); // void same as CMYK(0,0,0,1);
  Color Gray(double d=0);

  Color Red(double d=1);
  Color Green(double d=1);
  Color Blue(double d=1);
  Color White(double d=1);
  Color Black(double d=1);

  Color Cyan(double d=1);
  Color Magenta(double d=1);
  Color Yellow(double d=1);
  Color CMY_White(double d=1);
  Color CMY_Black(double d=1);

  Color CyanK(double d=1);
  Color MagentaK(double d=1);
  Color YellowK(double d=1);
  Color CMYK_White(double d=1);
  Color CMYK_Black(double d=1);

  // color separation
  Color C_Process(double d=1);
  Color M_Process(double d=1);
  Color Y_Process(double d=1);
  Color K_Process(double d=1);

  // "conversion filters"
  Color Neutral();
  Color RGB_Neutral();
  Color CMY_Neutral();
  Color CMYK_Neutral();
  Color Gray_Neutral();

} // end of namespace

#endif /* EPIX_COLOR */
