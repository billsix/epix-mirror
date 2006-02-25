/*
 * Color.cc -- ePiX2 Color functions
 *
 * This file is part of ePiX, a program for creating high-quality
 * figures in LaTeX
 *
 * Version 2.0pre
 *
 * Last Change: February 5, 2006
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
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <sstream>
#include <ostream>
#include <string>

#include "Constants.h"
#include "Errors.h"
#include "Functions.h"

#include "Output.h"

#include "Color.h"

namespace ePiX2 {

  // Magic number: 8-bits per color channel
  static const int COLORS=0xFF;

  bool Color_Base::operator== (const Color_Base& col) const
  {
    return (name() == col.name());
  }

  /* * * Handle functions * * */
  Color::Color(void) 
    : m_color(0), c_pt(new int(1)) { }

  Color Color::Unset=Color();

  Color::Color(Color_Base* col)
    : m_color(col), c_pt(new int(1)) { }

  Color::Color(const Color& r)
    : m_color(r.m_color), c_pt(r.c_pt) 
  { 
    (*c_pt)++; 
  }

  Color& Color::operator= (const Color& r)
  {
    if (m_color == r.m_color)
      return *this;

    if (--(*c_pt) == 0)
      {
	delete m_color;
	delete c_pt;
      }

    m_color = r.m_color;
    c_pt = r.c_pt;
    (*c_pt)++;
    return *this;
  }

  Color::~Color(void)
  {
    if (--(*c_pt) == 0)
      {
	delete m_color;
	delete c_pt;
      }
  }

  Color_Base* Color::operator->() const { return m_color; }

  void Color::bind(Color_Base* col)
  {
    if (col != m_color)
      {
	if (--*c_pt == 0)
	  {
	    delete m_color;
	    *c_pt = 1;
	  }

	else
	  c_pt = new int(1);

	m_color = col;
      }
  }


  bool Color::operator== (const Color& col) const
  {
    if (is_unset() || col.is_unset())
      return m_color == col.m_color;
    else
      return m_color->name() == col.m_color->name();
  }

  bool Color::operator< (const Color& col) const
  {
    // N.B. add_colors_to() must check for Unset
    return m_color->name() < col.m_color->name(); 
  }

  bool Color::is_unset(void) const
  {
    return (m_color == 0);
  }


  Color Color::operator*= (const double c)
  {
    if (is_unset())
      return *this;

    Color_Base* val = m_color->clone();
    (*val) *= c;
    return Color(val);
  }

  Color Color::operator() (const Color& col) const
  {
    if (is_unset())
      return *this;

    Color_Base* val = col.m_color->clone();
    val = (*m_color)(*(col.m_color));
    return Color(val);
  }

  Color Color::fog(const double d) const
  {
    if (is_unset())
      return *this;

    return Color(m_color->fog(d));
  }


  // Global constructors and operator
  Color RGB(const double r, const double g, const double b)
  {
    // Allocate
    return Color(new RGB_Color(r, g, b));
  }

  Color CMY(const double c, const double m, const double y)
  {
    // Allocate
    return Color(new CMY_Color(c, m, y));
  }

  Color CMYK(const double c, const double m, const double y, const double k)
  {
    // Allocate
    return Color(new CMYK_Color(c, m, y, k));
  }

  Color operator* (const double c, const Color& col)
  {
    Color tmp(col);
    return tmp *= c;
  }


  /* * * Miscellaneous utility functions * * */
  // represent a double in [0,1] by a 16-bit hex string
  static std::string dtohex(double arg) // assumes arg>0
  {
    using std::ios_base;
    int dens=(int) floor(arg*COLORS);

    std::ostringstream value;
    value.setf(ios_base::hex, ios_base::basefield);

    if (dens <= 0xF) // pad
      value << "0";

    value << dens;
    return value.str();
  }

  // saw2th:R -> [-2,2] is (1) a sawtooth function; (2) the identity on [-2,2]
  static double saw2th(double x)
  {
    return x - 4*floor((x+2)/4);
  }

  /*
   *  clippy(x):
   *  1....          ____                   if (x >= 0)
   *      .\        /.  .                     return x <=  1 ? x :  1;
   *      . \      / .  .
   *  0......\____/  .  .                   else
   *      -2 -1   0  1  2 --> x               return x >= -1 ? 0 : -1-x;
   */
  static double clippy(double x)
  {
    x = saw2th(x);
    return (x >= 0) ? ( x <= 1 ? x : 1 ) : ( x >= -1 ? 0 : -1-x );
  }

  static double clip_0_1(const double arg)
  {
    //    return (arg <= 0) ? 0 : ( arg >= 1 ? 1 : floor(COLORS*arg)/COLORS );
    return (arg <= 0) ? 0 : ( arg >= 1 ? 1 : arg );
  }

  static double make_r(double r, double g, double b)
  {
    return clip_0_1(clippy( r) + clippy(-g) + clippy(-b));
  }

  static double make_g(double r, double g, double b)
  {
    return clip_0_1(clippy(-r) + clippy( g) + clippy(-b));
  }

  static double make_b(double r, double g, double b)
  {
    return clip_0_1(clippy(-r) + clippy(-g) + clippy( b));
  }


  /* * * RGB_Color functions * * */
  RGB_Color::RGB_Color(const double r, const double g, const double b)
  {
    red   = make_r(r, g, b);
    green = make_g(r, g, b);
    blue  = make_b(r, g, b);
  }

  RGB_Color::RGB_Color(const RGB_Densities s)
    : red(s.m_dens_red), green(s.m_dens_green), blue(s.m_dens_blue) { }

  RGB_Densities RGB_Color::to_rgb(void) const
  {
    return RGB_Densities(red, green, blue);
  }

  RGB_Color* RGB_Color::clone(void) const
  {
    // Allocate
    return new RGB_Color(*this);
  }

  // Apply us as filter
  RGB_Color* RGB_Color::operator() (const Color_Base& color) const
  {
    RGB_Densities s=color.to_rgb();

    double r = red   * s.m_dens_red;
    double g = green * s.m_dens_green;
    double b = blue  * s.m_dens_blue;

    // Allocate
    return new RGB_Color(r,g,b);
  }

  // Fog
  RGB_Color* RGB_Color::fog(const double d) const
  {
    double mult = exp(-d);

    double r = 1 + mult*(red-1);
    double g = 1 + mult*(green-1);
    double b = 1 + mult*(blue-1);

    // Allocate
    return new RGB_Color(r,g,b);
  }

  RGB_Color& RGB_Color::operator*= (const double c)
  {
    red *= c;
    green *= c;
    blue *= c;

    double r = make_r(red, green, blue);
    double g = make_g(red, green, blue);
    double b = make_b(red, green, blue);

    red=r;
    green=g;
    blue=b;

    return *this;
  }

  // string identifier -- not output-specific
  std::string RGB_Color::name(void) const
  {
    std::ostringstream nm;

    nm << dtohex(red) << dtohex(green) << dtohex(blue);

    return nm.str();
  }

  std::string RGB_Color::print_to(const Format& fmt) const
  {
    return fmt.print_color("rgb", name(), red, green, blue);
  }


  /* * * CMY_Color functions * * */
  CMY_Color::CMY_Color(const double c, const double m, const double y)
  {
    /* conventional RGB densities
    double r = 1 - min(1, c+k);
    double g = 1 - min(1, m+k);
    double b = 1 - min(1, y+k);
    */
    // extended RGB densities
    double r = clippy(1-c);
    double g = clippy(1-m);
    double b = clippy(1-y);

    cyan    = 1 - make_r(r, g, b);
    magenta = 1 - make_g(r, g, b);
    yellow  = 1 - make_b(r, g, b);
  }


  CMY_Color::CMY_Color(const RGB_Densities s)
    : cyan(1-s.m_dens_red), 
      magenta(1-s.m_dens_green), 
      yellow(1-s.m_dens_blue) { }

  RGB_Densities CMY_Color::to_rgb(void) const
  {
    // Use conventional formulas since our densities are already in [0,1]
    return RGB_Densities(1-cyan, 1-magenta, 1-yellow);
  }

  CMY_Color* CMY_Color::clone(void) const
  {
    // Allocate
    return new CMY_Color(*this);
  }

  // Apply us as filter
  CMY_Color* CMY_Color::operator() (const Color_Base& color) const
  {
    // convert input color model to RGB, then manually to CMY below
    RGB_Densities s = color.to_rgb();
    // RGB_Densities f = this->to_rgb();

    // filter in CMY
    double c = cyan    * (1 - s.m_dens_red);
    double m = magenta * (1 - s.m_dens_green);
    double y = yellow  * (1 - s.m_dens_blue);

    // Allocate
    return new CMY_Color(c,m,y);
  }

  // Fog
  CMY_Color* CMY_Color::fog(const double d) const
  {
    double mult = exp(-d);

    double c = mult*cyan;
    double m = mult*magenta;
    double y = mult*yellow;

    // Allocate
    return new CMY_Color(c,m,y);
  }

  CMY_Color& CMY_Color::operator*= (const double sc)
  {
    RGB_Color value(this->to_rgb()); // convert to RGB
    value *= sc;                      // scale
    CMY_Color filter(1,1,1);         // convert back

    (*this) = *(filter(value));
    return *this;
  }

  // string identifier -- not output-specific
  std::string CMY_Color::name(void) const
  {
    std::ostringstream nm;

    nm << dtohex(cyan) << dtohex(magenta) << dtohex(yellow);

    return nm.str();
  }

  std::string CMY_Color::print_to(const Format& fmt) const
  {
    return fmt.print_color("cmy", name(), cyan, magenta, yellow);
  }


  /* * * CMYK_Color functions * * */
  CMYK_Color::CMYK_Color(const double c, const double m, const double y,
			 const double k)
  {
    /* conventional RGB densities
    double r = 1 - min(1, c+k);
    double g = 1 - min(1, m+k);
    double b = 1 - min(1, y+k);
    */
    // extended RGB densities
    double r = 1 - (c+k);
    double g = 1 - (m+k);
    double b = 1 - (y+k);

    cyan    = 1 - make_r(r, g, b);
    magenta = 1 - make_g(r, g, b);
    yellow  = 1 - make_b(r, g, b);

    black = min(min(cyan, magenta), yellow);
  }


  CMYK_Color::CMYK_Color(const RGB_Densities s)
    : cyan(1-s.m_dens_red), 
      magenta(1-s.m_dens_green), 
      yellow(1-s.m_dens_blue)
  {
    // black = min(c,m,y), following Uwe Kern's xcolor documentation
    // V 1.11, 2004/05/09, p. 31
    black = min(min(cyan, magenta), yellow);
  }

  RGB_Densities CMYK_Color::to_rgb(void) const
  {
    // Use conventional formulas since our densities are already in [0,1]
    double c = 1 - min(1, cyan+black);
    double m = 1 - min(1, magenta+black);
    double y = 1 - min(1, yellow+black);

    return RGB_Densities(c, m, y);
  }

  CMYK_Color* CMYK_Color::clone(void) const
  {
    // Allocate
    return new CMYK_Color(*this);
  }

  // Apply us as filter
  CMYK_Color* CMYK_Color::operator() (const Color_Base& color) const
  {
    // convert input color model to RGB, then manually to CMY below
    RGB_Densities s = color.to_rgb();

    // filter in CMY
    double c = min(1, cyan+black)    * (1 - s.m_dens_red);
    double m = min(1, magenta+black) * (1 - s.m_dens_green);
    double y = min(1, yellow+black)  * (1 - s.m_dens_blue);

    // cf. CMYK_Color(RGB_Densities) above
    double k = min(min(c,m),y);

    // Allocate
    return new CMYK_Color(c,m,y,k);
  }

  // Fog
  CMYK_Color* CMYK_Color::fog(const double d) const
  {
    double mult = exp(-d);

    double c = mult*cyan;
    double m = mult*magenta;
    double y = mult*yellow;
    double k = mult*black;

    // Allocate
    return new CMYK_Color(c,m,y,k);
  }

  CMYK_Color& CMYK_Color::operator*= (const double sc)
  {
    RGB_Color value(this->to_rgb());
    value *= sc;
    CMYK_Color filter(1,1,1,1);

    (*this) = *(filter(value));
    return *this;
  }

  // string identifier -- not output-specific
  std::string CMYK_Color::name(void) const
  {
    std::ostringstream nm;

    nm << dtohex(cyan) << dtohex(magenta) << dtohex(yellow);

    return nm.str();
  }

  std::string CMYK_Color::print_to(const Format& fmt) const
  {
    return fmt.print_color("cmyk", name(), cyan, magenta, yellow, black);
  }


  /* * * Extended primaries in the global namespace * * */
  Color Red(const double d)
  {
    return RGB(clippy(d), clippy(-d), clippy(-d));
  }

  Color Green(const double d)
  {
    return RGB(clippy(-d), clippy(d), clippy(-d));
  }

  Color Blue(const double d)
  {
    return RGB(clippy(-d), clippy(-d), clippy(d));
  }

  Color White(const double d)   
  {
    return RGB(clippy(d), clippy(d), clippy(d)); 
  }

  Color Black(const double d)
  { 
    return White(1-d);
  }

  /* * * CMY * * */
  Color Cyan(const double d)
  { 
    return CMY(clippy(d), clippy(-d), clippy(-d));
  }

  Color Magenta(const double d)
  { 
    return CMY(clippy(-d), clippy(d), clippy(-d));
  }

  Color Yellow(const double d)
  { 
    return CMY(clippy(-d), clippy(-d), clippy(d));
  }

  Color CMY_White(const double d)   
  {
    return CMY(clippy(1-d), clippy(1-d), clippy(1-d)); 
  }

  Color CMY_Black(const double d)
  { 
    return CMY_White(1-d);
  }


  /* * * CMYK * * */
  Color CyanK(const double d)
  {
    double c=clippy(d);
    double m=clippy(-d);
    double y=m;
    double k=min(c,m); // y=m

    return CMYK(c,m,y,k);
  }

  Color MagentaK(const double d)
  { 
    double c=clippy(-d);
    double m=clippy(d);
    double y=c;
    double k=min(c,m); // y=c

    return CMYK(c,m,y,k);
  }

  Color YellowK(const double d)
  { 
    double c=clippy(-d);
    double m=c;
    double y=clippy(d);
    double k=min(c,y); // m=c

    return CMYK(c,m,y,k);
  }

  Color CMYK_White(const double d)   
  {
    // TO DO: Semantics OK?
    //    return CMYK(clippy(1-d), clippy(1-d), clippy(1-d), clippy(1-d)); 
    //    return CMYK(1-d, 1-d, 1-d, 0);
    return CMYK(clippy(1-d), clippy(1-d), clippy(1-d), 0); 
  }

  Color CMYK_Black(const double d)
  { 
    return CMYK(d, d, d, 0);
  }


} /* end of namespace */
