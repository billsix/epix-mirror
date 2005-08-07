/* 
 *  Object.cc -- epix2::Object_Base and Object classes
 *
 * This file is part of ePiX, a program for creating high-quality 
 * figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: August 07, 2005
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
  Object_Base& Object_Base::operator+= (const Vector& dX)
  {    
    the_orient.move_to(the_orient.here()-dX); // N.B. apply *inverse* to basis
    return *this;
  }

  void Object_Base::move_to(const Point arg)
  {
    the_orient.move_to(arg);
  }

  Object_Base& Object_Base::operator*= (const double c)
  {
    the_scale *= recip(c); // will detect *=0 in shatter()
    return *this;
  }

  void Object_Base::scale (const double c)
  {
    the_scale *= recip(c);
  }

  void Object_Base::rotate(const double angle, const Vector& axis)
  {
    the_orient.rotate(-angle, axis);
  }

  void Object_Base::reflect(const Vector& axis)
  {
    the_orient.reflect(axis); // reflection same as inverse
  }

  Object_Base operator+ (const Object_Base& obj, const Vector& dX)
  {
    Object_Base temp = obj;
    return temp += dX;
  }

  Object_Base operator* (const Object_Base& obj, const double c)
  {
    Object_Base temp = obj;
    return temp *= c;
  }

  // Object operations

  Object::Object(void) 
  {
    the_orient=Basis();
    the_scale=1;
    // set default face and edge colors 
    white(); 
    black0(); 
  }

  // set face colors
  void Object::rgb(const double r, const double g, const double b)
  {
    style.rgb(r, g, b);
  }

  void Object::cmyk(const double c, const double m, const double y, 
		   const double k)
  {
    style.cmyk(c, m, y, k);
  }


  void Object::white(epix2_color_model cmod)
  {
    using ePiX2::rgb;
    using ePiX2::cmyk;

    switch(cmod) {
    case cmy:  style.fill_color=Color(1,1,1,cmy);    break;
    case cmyk: style.fill_color=Color(1,1,1,0,cmyk); break;
    default:   style.fill_color=Color(1,1,1,rgb);    break;
    }
  }

  void Object::black(epix2_color_model cmod)
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
  void Object::rgb0(const double r, const double g, const double b)
  {
    style.rgb0(r,g,b);
  }

  void Object::cmyk0(const double c, const double m, const double y, 
		    const double k)
  {
    style.cmyk0(c, m, y, k);
  }

  void Object::white0(epix2_color_model cmod)
  {
    using ePiX2::rgb;
    using ePiX2::cmyk;

    switch(cmod) {
    case cmy:  style.line_color=Color(1,1,1,cmy);    break;
    case cmyk: style.line_color=Color(1,1,1,0,cmyk); break;
    default:   style.line_color=Color(1,1,1,rgb);    break;
    }
  }

  void Object::black0(epix2_color_model cmod)
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
  Color Object::get_line_color(void) { return style.line_color; }
  Color Object::get_fill_color(void) { return style.fill_color; }
  Color Object::get_back_color(void) { return style.back_color; }


  // add an Object
  Object& Object::operator<< (Object& obj) 
  { 
    parts.push_back(&obj); 
    return *this;
  }

  void Object::shatter(void)
  {
    if (parts.size() > 0)
      {
	std::list<Object*>::iterator obj;
	for (obj=parts.begin(); obj!=parts.end(); ++obj)
	  {
	    (*obj)->the_orient = the_orient; // synchronize bases
	    (*obj)->the_scale  = the_scale;  // and length scales
	    (*obj)->shatter();               // child::shatter()
	    fragments.merge((*obj)->fragments);
	  }
      }
  }

  bool Object::hides(const Point vpt, const Point X)
  {
    bool value=false;
    if (parts.size() > 0)
      {
	std::list<Object*>::const_iterator obj=parts.begin();

	while(!value && obj!=parts.end())
	  {
	    value = (value || (*obj)->hides(vpt, X)); // child::hides()
	    ++obj;
	  }
      }

    return value; // children should define their own hides()
  }


  /*
  // Clump operations
  Clump& Clump::operator+= (const Vector& dX)
  {    
    all_orient.move_to(all_orient.here()-dX); // N.B. apply *inverse* to basis
    return *this;
  }

  void Clump::move_to(const Point arg)
  {
    all_orient.move_to(arg);
  }

  Clump& Clump::operator*= (const double c)
  {
    all_scale *= recip(c); // will detect *=0 in shatter()
    return *this;
  }

  void Clump::scale (const double c)
  {
    all_scale *= recip(c);
  }

  void Clump::rotate(const double angle, const Vector& axis)
  {
    all_orient.rotate(-angle, axis);
  }

  void Clump::reflect(const Vector& axis)
  {
    all_orient.reflect(axis); // reflection same as inverse
  }

  Clump operator+ (const Clump& obj, const Vector& dX)
  {
    Clump temp = obj;
    return temp += dX;
  }

  Clump operator* (const Clump& obj, const double c)
  {
    Clump temp = obj;
    return temp *= c;
  }

  Clump operator+ (const Clump& clump, Object_Base& obj)
  {
    Clump temp = clump;
    return temp << obj;
  }
  */

} // end of namespace

