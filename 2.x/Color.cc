/*
 * Color.cc -- ePiX2 Color functions
 *
 * This file is part of ePiX, a program for creating high-quality
 * figures in LaTeX
 *
 * Version 2.0pre
 *
 * Last Change: August 07, 2005
 */

/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005
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

#include <set>
#include <sstream>
#include <string>

//#include "Enums.h"
#include "Errors.h"
#include "Color.h"

namespace ePiX2 {

  const int COLORS=1000; // Magic number; must be a power of 10

  epix2_color_model Color::Model=natural; // define global color model
  std::set<std::string> Color::pallet;    // Colors seen so far

  // Miscellaneous utility functions

  // represent a double in [0,1] by its first three digits
  static std::string dtoa(double arg) // assumes arg>0
  {
    int digits=(int) floor(COLORS*arg); // strip off 3 decimals
    if (digits == COLORS) // kludge to distinguish 0 from 1 :)
      digits=COLORS-1;

    std::ostringstream value;
    value << digits;

    return value.str();
  }

  static double clip_to_unit(const double arg)
  {
    if (arg >= 1 ) return 1;
    else if (arg <= 0) return 0;
    else return floor(COLORS*arg)/COLORS;
  }

  static std::string curly_braces(const std::string msg)
  {
    std::ostringstream buf;
    buf << '{' << msg << '}';

    return buf.str();
  }

  std::string str_model(epix2_color_model arg)
  {
    switch(arg) {

    case rgb:
      return "rgb";
      break;

    case cmy:
      return "cmy";
      break;

    case cmyk:
      return "cmyk";
      break;

    case hsb:
      return "hsb";
      break;

      // Color constructor guarantees these are not seen in color names
    case gray:
      return "gray";
      break;

    default:
      return "natural";
      break;
    }
  }

  // class functions
  Color::Color(double red, double green, double blue, epix2_color_model cmod)
  {
    dens_red=clip_to_unit(red);
    dens_green=clip_to_unit(green);
    dens_blue=clip_to_unit(blue);
    dens_black=0;

    // erroneous to specify as local models
    if ((cmod == gray) || (cmod == natural))
      my_model=rgb;

    else
      my_model=cmod;

    Color::pallet.insert(this->str()); // add string identifier to global list
  }

  Color::Color(double red, double green, double blue, double black,
	       epix2_color_model arg)
  {
    dens_red=clip_to_unit(red);
    dens_green=clip_to_unit(green);
    dens_blue=clip_to_unit(blue);
    dens_black=clip_to_unit(black);

    my_model=cmyk;
    if (arg != cmyk)
      epix_warning("Using cmyk in color spec containing four densities");

    Color::pallet.insert(this->str());
  }

  Color Color::operator* (const double c) const
  {
    double tmp=clip_to_unit(c);

    if (my_model == cmyk)
      return Color(tmp*dens_red, tmp*dens_green, tmp*dens_blue, 
		   tmp*dens_black, cmyk);
    else
      return Color(tmp*dens_red, tmp*dens_green, tmp*dens_blue, my_model);
  }

  std::string Color::model(void) const
  {
    return str_model(my_model);
  }

  std::string Color::name(void) const
  {
    std::ostringstream name_buffer;

    name_buffer << str_model(my_model) 
		<< "_" << dtoa(dens_red) 
		<< "_" << dtoa(dens_green)
		<< "_" << dtoa(dens_blue);

    if (my_model == cmyk)
      name_buffer << "_" << dtoa(dens_black);

    return name_buffer.str();
  }

  /* for xcolor
  std::string Color::densities(void) const
  {
    std::ostringstream buf;

    buf << dens_red   << "," << dens_green << "," << dens_blue;

    if (my_model == cmyk)
      buf << "," << dens_black;

    return buf.str();
  }

  std::string Color::str(void) const // for xcolor + pstricks
  {
    std::ostringstream buf;
    buf << "\\xdefinecolor" 
	<< curly_braces(name()) 
	<< curly_braces(model())
	<< curly_braces(densities());

    return buf.str();
  }
  */

  // for vanilla PSTricks
  std::string Color::densities(void) const
  {
    std::ostringstream buf;

    buf << dens_red   << " " << dens_green << " " << dens_blue;

    if (my_model == cmyk)
      buf << " " << dens_black;

    return buf.str();
  }

  std::string Color::str(void) const // for xcolor + pstricks
  {
    std::ostringstream buf;
    buf << "\\new" << model() << "color" 
	<< curly_braces(name()) 
	<< curly_braces(densities());

    return buf.str();
  }

  void Color::print_pallet(void)
  {
    std::set<std::string>::const_iterator s;
    for(s=Color::pallet.begin(); s!=Color::pallet.end(); ++s)
      std::cout << std::endl << (*s);
  }

  // Style functions

  // face color
  void Style::rgb(const double r, const double g, const double b)
  {
    fill_color=Color(r, g, b, ePiX2::rgb);
  }

  void Style::cmyk(const double c, const double m, const double y, 
		   const double k)
  {
    fill_color=Color(c, m, y, k, ePiX2::cmyk);
  }

  // border color
  void Style::rgb0(const double r, const double g, const double b)
  {
    line_color=Color(r, g, b, ePiX2::rgb);
  }

  void Style::cmyk0(const double c, const double m, const double y, 
		    const double k)
  {
    line_color=Color(c, m, y, k, ePiX2::cmyk);
  }

} /* end of namespace */
