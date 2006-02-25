/***
 ***  Color.h -- epix2::Color class and helpers
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: February 5, 2006
 ***
 *** 
 *** Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 *** Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
 *** Department of Mathematics and Computer Science
 *** College of the Holy Cross
 *** Worcester, MA, 01610-2395, USA
 ***
 ***
 *** ePiX is free software; you can redistribute it and/or modify it
 *** under the terms of the GNU General Public License as published by
 *** the Free Software Foundation; either version 2 of the License, or
 *** (at your option) any later version.
 ***
 *** ePiX is distributed in the hope that it will be useful, but WITHOUT
 *** ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *** or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 *** License for more details.
 ***
 *** You should have received a copy of the GNU General Public License
 *** along with ePiX; if not, write to the Free Software Foundation, Inc.,
 *** 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 ***
 ***
 *** This file provides classes for defining and manipulating colors:
 ***   - Color_Base: The abstract interface
 ***   - RGB_Color, CMY_Color, CMYK_Color: Implementations
 ***   - Color: The user interface
 ***     *= Scale color by a double
 ***     operator(const Color& col) Apply us as a filter to col
 ***   - fog(d) Simulate color seen through fog (d=0 no effect, d=oo White)
 ***
 *** Scaling and filtering are implemented by converting to the filter's
 *** color model. Color model conversions come from the LaTeX xcolor
 *** package documentation, V 1.11 (2044/05/09) by Uwe Kern.
 ***/

#ifndef EPIX2_COLOR
#define EPIX2_COLOR

#include <string>

namespace ePiX2 {
  /* * * Color.h * * */
  struct RGB_Densities
  {
    RGB_Densities (const double r, const double g, const double b)
      : m_dens_red(r), m_dens_green(g), m_dens_blue(b) { }

    double m_dens_red, m_dens_green, m_dens_blue;
  };

  class Format;
  class Color_Base {
  public:
    virtual ~Color_Base(void) { }
    virtual Color_Base* clone(void) const = 0;

    // scale the intensity
    virtual Color_Base& operator*= (const double c) = 0;

    // apply us as filter
    virtual Color_Base* operator() (const Color_Base&) const = 0;

    // filter through fog as specified distance
    virtual Color_Base* fog(const double) const = 0;

    // internal colorname and densities
    virtual std::string name(void) const = 0;
    virtual std::string print_to(const Format&) const = 0;

    bool operator== (const Color_Base& col) const;
    virtual RGB_Densities to_rgb(void) const = 0;

  }; /* end of class Color_Base */


  // Color_Base handle
  class Color {
  public:
    static Color Unset;

    Color(void);
    Color(Color_Base*);
    Color(const Color&);
    Color& operator= (const Color&);

    ~Color(void);

    Color_Base* operator->() const;

    // re-bind a Color to another Color_Base*
    void bind(Color_Base*);

    bool operator== (const Color&) const;
    bool operator< (const Color&) const; // needed for Screen::pallet
    bool operator!= (const Color& col) const { return !((*this) == col); }
    bool is_unset(void) const;

    Color operator*= (const double c);     // scale the intensity
    Color operator() (const Color&) const; // apply us as a filter
    Color fog(const double) const;

    std::string name(void) const { return m_color->name(); }

    std::string print_to(const Format& fmt) const
      { return m_color->print_to(fmt); }

  private:
    Color_Base* m_color;
    int* c_pt;
  }; // end of class Color

  Color operator* (const double, const Color&);

  /* * * Primary color functions in the global namespace * * */

  /*
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
  Color RGB(const double r=0, const double g=0, const double b=0);
  Color CMY(const double c=1, const double m=1, const double y=1);
  Color CMYK(const double c=1, const double m=1, const double y=1,
	     const double k=1);

  Color Red(const double d=1);
  Color Green(const double d=1);
  Color Blue(const double d=1);
  Color White(const double d=1);
  Color Black(const double d=1);
  inline Color Neutral(void) { return RGB(1,1,1); }

  Color Cyan(const double d=1);
  Color Magenta(const double d=1);
  Color Yellow(const double d=1);
  Color CMY_White(const double d=1);
  Color CMY_Black(const double d=1);
  inline Color CMY_Neutral(void) { return CMY(1,1,1); }

  Color CyanK(const double d=1);
  Color MagentaK(const double d=1);
  Color YellowK(const double d=1);
  Color CMYK_White(const double d=1);
  Color CMYK_Black(const double d=1);
  inline Color CMYK_Neutral(void) { return CMYK(0,0,0,1); }


  /* * * Implementation classes * * */
  class RGB_Color : public Color_Base {
  public:
    RGB_Color(const double r=0, const double g=0, const double b=0);
    RGB_Color(const RGB_Densities);

    RGB_Color* clone(void) const;

    RGB_Color* operator() (const Color_Base&) const;
    RGB_Color* fog(const double) const;
    RGB_Color& operator*= (const double c);

    std::string name(void) const;
    std::string print_to(const Format&) const;

    RGB_Densities to_rgb(void) const;

  private:
    double red;
    double green;
    double blue;
  }; // end of class RGB_Color


  class CMY_Color : public Color_Base {
  public:
    CMY_Color(const double c=1, const double m=1, const double y=1);
    CMY_Color(const RGB_Densities);

    CMY_Color* clone(void) const;

    CMY_Color* operator() (const Color_Base&) const;
    CMY_Color* fog(const double) const;
    CMY_Color& operator*= (const double c);

    std::string name(void) const;
    std::string print_to(const Format&) const;

    RGB_Densities to_rgb(void) const;

  private:
    double cyan;
    double magenta;
    double yellow;
  }; // end of class CMY_Color


  class CMYK_Color : public Color_Base {
  public:
    CMYK_Color(const double c=1, const double m=1, const double y=1,
	       const double k=1);
    CMYK_Color(const RGB_Densities);

    CMYK_Color* clone(void) const;

    CMYK_Color* operator() (const Color_Base&) const;
    CMYK_Color* fog(const double) const;
    CMYK_Color& operator*= (const double c);

    std::string name(void) const;
    std::string print_to(const Format&) const;

    RGB_Densities to_rgb(void) const;

  private:
    double cyan;
    double magenta;
    double yellow;
    double black;
  }; // end of class CMYK_Color

} /* end of namespace */

#endif /* EPIX2_COLOR */
