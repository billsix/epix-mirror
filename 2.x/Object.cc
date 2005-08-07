/* 
 *  Object.h -- epix2::Object class
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: August 06, 2005
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005
 * Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
 * Department of Mathematics and Computer Science
 * College of the Holy Cross
 * Worcester, MA, 01610-2395, USA
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

#include <iostream>

#include "Enums.h"
#include "Functions.h"
#include "Vector.h"
#include "Basis.h"
#include "Edge.h"
#include "Color.h"
#include "Hiding.h"
#include "Object.h"

namespace ePiX2 {

  // translate
  Object& Object::operator+= (const Vector& dX)
  {    
    the_orient.move_to(the_orient.here()-dX); // N.B. apply *inverse* to basis
    return *this;
  }

  void Object::move_to(const Point arg)
  {
    the_orient.move_to(arg);
  }

  Object& Object::operator*= (const double c)
  {
    the_scale *= recip(c); // will detect *=0 in shatter()
    return *this;
  }

  void Object::scale (const double c)
  {
    the_scale *= recip(c);
  }

  void Object::rotate(const double angle, const Vector& axis)
  {
    the_orient.rotate(-angle, axis);
  }

  void Object::reflect(const Vector& axis)
  {
    the_orient.reflect(axis); // reflection same as inverse
  }

  Object operator+ (const Object& obj, const Vector& dX)
  {
    Object temp = obj;
    return temp += dX;
  }

  Object operator* (const Object& obj, const double c)
  {
    Object temp = obj;
    return temp *= c;
  }

  // Shape operations

  Shape::Shape(void) 
  {
    the_orient=Basis();
    the_scale=1;
    // set default face and edge colors 
    white(); 
    black0(); 
  }

  // set face colors
  void Shape::rgb(const double r, const double g, const double b)
  {
    style.rgb(r, g, b);
  }

  void Shape::cmyk(const double c, const double m, const double y, 
		   const double k)
  {
    style.cmyk(c, m, y, k);
  }


  void Shape::white(epix2_color_model cmod)
  {
    using ePiX2::rgb;
    using ePiX2::cmyk;

    switch(cmod) {
    case cmy:  style.fill_color=Color(1,1,1,cmy);    break;
    case cmyk: style.fill_color=Color(1,1,1,0,cmyk); break;
    default:   style.fill_color=Color(1,1,1,rgb);    break;
    }
  }

  void Shape::black(epix2_color_model cmod)
  {
    using ePiX2::rgb;
    using ePiX2::cmyk;

    switch(cmod) {
    case cmy:  style.fill_color=Color(0,0,0,cmy);    break;
    case cmyk: style.fill_color=Color(0,0,0,1,cmyk); break;
    default:   style.fill_color=Color(0,0,0,rgb);    break;
    }
  }

  // set edge colors
  void Shape::rgb0(const double r, const double g, const double b)
  {
    style.rgb0(r,g,b);
  }

  void Shape::cmyk0(const double c, const double m, const double y, 
		    const double k)
  {
    style.cmyk0(c, m, y, k);
  }

  void Shape::white0(epix2_color_model cmod)
  {
    using ePiX2::rgb;
    using ePiX2::cmyk;

    switch(cmod) {
    case cmy:  style.line_color=Color(1,1,1,cmy);    break;
    case cmyk: style.line_color=Color(1,1,1,0,cmyk); break;
    default:   style.line_color=Color(1,1,1,rgb);    break;
    }
  }

  void Shape::black0(epix2_color_model cmod)
  {
    using ePiX2::rgb;
    using ePiX2::cmyk;

    switch(cmod) {
    case cmy:  style.line_color=Color(0,0,0,cmy);    break;
    case cmyk: style.line_color=Color(0,0,0,1,cmyk); break;
    default:   style.line_color=Color(0,0,0,rgb);    break;
    }
  }

  // private functions
  Color Shape::get_line_color(void) { return style.line_color; }
  Color Shape::get_fill_color(void) { return style.fill_color; }
  Color Shape::get_back_color(void) { return style.back_color; }


  /*
  // Clump operations

  Clump& Clump::operator+= (const Object obj) 
  { 
    parts.push_back(obj); 
    return *this;
  }

  void Clump::operator+ (const Object obj) 
  { 
    parts.push_back(obj); 
  }

  void Clump::shatter(void)
  {
    std::list<Object>::iterator obj;
    for (obj=parts.begin(); obj!=parts.end(); ++obj)
      (*obj).shatter();
  }

  bool Clump::hides(const Point vpt, const Point X)
  {
    bool value=false;
    std::list<Object>::const_iterator obj=parts.begin();

    while(!value && obj!=parts.end())
      {
	value = (value || (*obj).hides(vpt, X));
	++obj;
      }

    return value;
  }
  */

} // end of namespace

