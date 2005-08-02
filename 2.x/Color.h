/***
 ***  Color.h -- epix2::Color class
 ***
 *** This file is part of ePiX, a preprocessor for creating high-quality 
 *** line figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: August 01, 2005
 ***
 *** 
 *** Copyright (C) 2001, 2002, 2003, 2004, 2005
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
 *** This file provides:
 ***   - Color: 4 densities, a color model, and (static) pallet
 ***      Functions to print a string representation, scale densities
 ***   - Style 
 ***      Line and fill colors
 ***/

#ifndef EPIX2_COLOR
#define EPIX2_COLOR

#include <string>
#include <set>

#include "Enums.h"

namespace ePiX2 {

  class Color {

  public:

    static epix2_color_model Model;

    Color(void) : dens_red(0), dens_green(0), dens_blue(0), my_model(rgb) { }

    // rgb, cmy, hsb
    Color(double red, double green, double blue,
	  epix2_color_model arg=rgb);

    // cmyk
    Color(double, double, double, double, epix2_color_model);

    Color operator* (const double c) const;

    std::string model(void) const; //  {rgb, cmy, cmyk, gray};
    std::string name(void) const;  // internal colorname
    std::string densities(void) const;

    // \xdefinecolor{name()}{model()}{densities()}
    std::string str(void) const;

    static void print_pallet(void);
    
  private:

    double dens_red;   // cyan in CMYK, etc.
    double dens_green;
    double dens_blue;

    double dens_black;

    epix2_color_model my_model;
    static std::set<std::string> pallet; // Colors seen so far

  }; /* end of class Color */


  class Style {

    friend class Shape;

  public:
    // set face color; TO DO: Independent color for backs of faces
    void rgb(const double r, const double g, const double b);
    void cmyk(const double c, const double m, const double y, const double k);

    // set edge color
    void rgb0(const double r, const double g, const double b);
    void cmyk0(const double c, const double m, const double y, const double k);

  protected:

    //    double    line_width;
    //    line_type line_style;
    // ? TO DO: vertex_color
    Color line_color;
    Color fill_color;
    Color back_color;
    //    std::string fill_style;

  }; // end of class Style

  std::string str_model(epix2_color_model);

} /* end of namespace */

#endif /* EPIX2_COLOR */
