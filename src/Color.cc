/*
 * Color.cc -- ePiX Color functions
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2.2
 *
 * Last Change: November 12, 2007
 */

/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
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

#include "Color_Base.h"
#include "Color_Utils.h"

#include "Color_CMY.h"
#include "Color_CMYK.h"
#include "Color_Gray.h"
#include "Color_Neutral.h"
#include "Color_RGB.h"
#include "Color_Sep.h"

#include "Color.h"

namespace ePiX {

  Color::Color() 
    : m_color(Neutral().m_color), m_alpha(1) { }

  Color::Color(const Color_Base& col)
    : m_color(col.clone()), m_alpha(1) { }

  Color::Color(const Color_Base* col)
    : m_color(col->clone()), m_alpha(1) { }


  Color::Color(const Color& col)
    : m_color(col.m_color->clone()), m_alpha(col.m_alpha) { }


  Color& Color::operator= (const Color& col)
  {
    if (*this != col)
      {
	Color_Base* tmp(col.m_color->clone());
	delete m_color;

	m_color = tmp;
	m_alpha = col.m_alpha;
      }

    return *this;
  }

  Color::~Color()
  {
    delete m_color;
  }

  const Color_Base* Color::operator->() const
  {
    return m_color;
  }

  double Color::alpha() const
  {
    return m_alpha;
  }

  bool Color::is_unset() const
  {
    // we're Neutral()
    return (name() == "");
  }


  // no effect if we or col are Neutral
  Color Color::filter(const Color& col) const
  {
    if (m_color->name() == "" || col.is_unset())
      return col;

    else
      {
	Color_Base* fil(m_color->clone());
	fil->filter(*(col.m_color));

	Color val(fil);
	delete fil;
	return val;
      }
  }

  Color& Color::operator*= (double c)
  {
    (*m_color) *= c;

    return *this;
  }

  // Blending with Unset has no effect
  Color& Color::blend(const Color& col, double d)
  {
    if (!col.is_unset())
      m_color->blend(*(col.m_color), d);

    return *this;
  }

  Color& Color::superpose(const Color& col)
  {
    if (!col.is_unset())
      m_color->superpose(*(col.m_color));

    return *this;
  }

  Color& Color::invert()
  {
    m_color->invert();

    return *this;
  }

  Color& Color::alpha(double t)
  {
    m_alpha = t;
    return *this;
  }


  std::string Color::model() const
  {
    return m_color->model();
  }

  std::string Color::name() const
  {
    return m_color->name();
  }

  std::vector<double> Color::densities() const
  {
    return m_color->densities();
  }


  // non-member operators
  Color operator* (double c, const Color& col)
  {
    Color tmp(col);
    return tmp *= c;
  }

  const bool operator== (const Color& col1, const Color& col2)
  {
    return col1.name() == col2.name();
  }

  const bool operator!= (const Color& col1, const Color& col2)
  {
    return col1.name() != col2.name();
  }

  const bool operator<  (const Color& col1, const Color& col2)
  {
    return col1.name() < col2.name();
  }



  // Named constructors
  Color RGB(double r, double g, double b)
  {
    return Color(RGB_Color(r, g, b));
  }

  Color CMY(double c, double m, double y)
  {
    return Color(CMY_Color(c, m, y));
  }

  Color CMYK(double c, double m, double y, double k)
  {
    return Color(CMYK_Color(c, m, y, k));
  }

  Color Gray(double d)
  {
    return Color(Gray_Color(d));
  }


  /* * * Extended primaries in the global namespace * * */
  Color Red(double d)
  {
    return RGB(clippy(d), clippy(-d), clippy(-d));
  }

  Color Green(double d)
  {
    return RGB(clippy(-d), clippy(d), clippy(-d));
  }

  Color Blue(double d)
  {
    return RGB(clippy(-d), clippy(-d), clippy(d));
  }

  Color White(double d)   
  {
    return RGB(clippy(d), clippy(d), clippy(d)); 
  }

  Color Black(double d)
  { 
    return RGB(clippy(1-d), clippy(1-d), clippy(1-d));
  }


  /* * * CMY * * */
  Color Cyan(double d)
  { 
    return CMY_Color(clippy(d), clippy(-d), clippy(-d), true);
  }

  Color Magenta(double d)
  { 
    return CMY_Color(clippy(-d), clippy(d), clippy(-d), true);
  }

  Color Yellow(double d)
  { 
    return CMY_Color(clippy(-d), clippy(-d), clippy(d), true);
  }

  Color CMY_White(double d)
  {
    return CMY_Color(clippy(1-d), clippy(1-d), clippy(1-d), true);
  }

  Color CMY_Black(double d)
  {
    return CMY_Color(clippy(d), clippy(d), clippy(d), true);
  }


  /* * * CMYK * * */
  Color CyanK(double d)
  {
    double c(clippy(d));
    double m(clippy(-d));
    double y(m);        // [sic]
    double k(std::min(c,m)); // y=m

    return CMYK_Color(c-k, m-k, y-k, k, true);
  }

  Color MagentaK(double d)
  { 
    double c(clippy(-d));
    double m(clippy(d));
    double y(c);
    double k(std::min(c,m)); // y=c

    return CMYK_Color(c-k, m-k, y-k, k, true);
  }

  Color YellowK(double d)
  { 
    double c(clippy(-d));
    double m(c);
    double y(clippy(d));
    double k(std::min(c,y)); // m=c

    return CMYK_Color(c-k, m-k, y-k, k, true);
  }

  Color CMYK_White(double d)   
  {
    return CMYK_Color(0, 0, 0, clippy(1-d), true);
  }

  Color CMYK_Black(double d)
  {
    return CMYK_Color(0, 0, 0, clippy(d), true);
  }


  Color C_Process(double d)
  {
    return Cyan_Layer(d);
  }

  Color M_Process(double d)
  {
    return Magenta_Layer(d);
  }

  Color Y_Process(double d)
  {
    return Yellow_Layer(d);
  }

  Color K_Process(double d)
  {
    return Black_Layer(d);
  }


  /* * * Neutral filters * * */
  Color Neutral()
  {
    static Neutral_Color* neutral(new Neutral_Color());
    return Color(*neutral);
  }

  Color RGB_Neutral()
  {
    static RGB_Color* rgb_neutral(new RGB_Color(1, 1, 1));
    return Color(*rgb_neutral);
  }

  Color CMY_Neutral()
  {
    static CMY_Color* cmy_neutral(new CMY_Color(0, 0, 0, true));
    return Color(*cmy_neutral);
  }

  Color CMYK_Neutral()
  {
    static CMYK_Color* cmyk_neutral(new CMYK_Color(0, 0, 0, 0, true));
    return Color(*cmyk_neutral);
  }

  Color Gray_Neutral()
  {
    static Gray_Color* gray_neutral(new Gray_Color(1));
    return Color(*gray_neutral);
  }
} // end of namespace
