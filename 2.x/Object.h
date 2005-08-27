/*** 
 ***  Object.h -- epix2::Object_Base and Object classes
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: August 07, 2005
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
 ***
 ***   The Object_Base class, the base class for all visible elements
 ***   of a Picture.  Object_Bases have an orientation (Basis) and scale
 ***   factor; all geometric operations are expressed by their *inverse*
 ***   ops on the basis/scale.  When an Object_Base is "shatter()"ed
 ***   (converted to one or more Shards for hidden object removal), the
 ***   basis and scale are used to compute the transformed Object_Base.
 ***
 ***   Though Object_Bases contain only orientation and scale data, they
 ***   can be created, so their functions cannot be pure virtual.
 ***/

#ifndef EPIX2_OBJECT
#define EPIX2_OBJECT

#include <list>
#include <iostream>

#include "Enums.h"
#include "Basis.h"
#include "Color.h"
#include "Hiding.h"

namespace ePiX2 {

  class Point;  /***/
  class Vector; /***/

  /* * * Object.h * * */

  class Object_Base   // Marker, Label, Object, Clump
    {
      friend class World;

    public:

      Object_Base(void) { closed_oriented=false; }
      virtual ~Object_Base(void) { }

      virtual Object_Base& operator+= (const Vector&); // translate
      virtual void move_to(const Point arg);      // set origin

      virtual Object_Base& operator*= (const double);  // scale
      virtual void scale (const double);

      virtual void reflect(const Vector& axis);
      virtual void rotate(const double angle, const Vector& axis);

      virtual void shatter(void) { }

      // we block vpt's view of X?
      virtual bool hides(const Point vpt, const Point X) const {return false;}

      virtual void set_line_color(const Color&) { }
      virtual void set_fill_color(const Color&) { }

    protected:

      Basis the_orient; // contains location data
      double the_scale;

      bool closed_oriented;

      std::list<Shard> fragments;

    }; // end of class Object_Base

  Object_Base operator+ (const Object_Base&, const Vector&);
  Object_Base operator* (const Object_Base&, const double);


  class Object : public Object_Base {

    friend class Clump;

  public:

    Object(void);

    // add an Object
    Object& operator<< (Object& obj);

    void shatter(void);
    bool hides(const Point vpt, const Point X);

    void rgb(const double r, const double g, const double b);
    void cmyk(const double c, const double m, const double y, 
	      const double k=0);

    // set edge color
    void rgb0(const double r, const double g, const double b);
    void cmyk0(const double c, const double m, const double y, 
	       const double k=0);

    void red(const double d=1)   { style.rgb(d,0,0); }
    void green(const double d=1) { style.rgb(0,d,0); }
    void blue(const double d=1)  { style.rgb(0,0,d); }

    void cyan(const double d=1)    { style.cmyk(d,0,0,0); }
    void magenta(const double d=1) { style.cmyk(0,d,0,0); }
    void yellow(const double d=1)  { style.cmyk(0,0,d,0); }

    void white(epix2_color_model cmod=ePiX2::rgb);
    void black(epix2_color_model cmod=ePiX2::rgb);

    void red0(const double d=1)   { style.rgb0(d,0,0); }
    void green0(const double d=1) { style.rgb0(0,d,0); }
    void blue0(const double d=1)  { style.rgb0(0,0,d); }

    void cyan0(const double d=1)    { style.cmyk0(d,0,0,0); }
    void magenta0(const double d=1) { style.cmyk0(0,d,0,0); }
    void yellow0(const double d=1)  { style.cmyk0(0,0,d,0); }

    void white0(epix2_color_model cmod=ePiX2::rgb);
    void black0(epix2_color_model cmod=ePiX2::rgb);

    // TO DO: Functions to set back_color

    void skeleton(bool T = true) { solid = !T; }

    void set_line_color(const Color& col) { style.line_color = col; }
    void set_fill_color(const Color& col) { style.fill_color = col; }
    void set_back_color(const Color& col) { style.back_color = col; }

  protected:

    std::list<Object*> parts;

    bool solid;

    Style style;

    Color get_line_color(void);
    Color get_fill_color(void);
    Color get_back_color(void);

  }; // end of class Object


  /*
  class Clump : public Object {

  public:

    // affine transformations
    Clump& operator+= (const Vector&); // translate
    void move_to(const Point arg);      // set origin

    Clump& operator*= (const double);  // scale
    void scale (const double);

    void reflect(const Vector& axis);
    void rotate(const double angle, const Vector& axis);

    // add an Object
    Clump& operator<< (Object_Base& obj);

    void shatter(void);
    bool hides(const Point vpt, const Point X);

  protected:

    std::list<Object_Base*> parts;

    Basis all_orient;
    double all_scale;

    //    double    line_width;
    //    line_type line_style;
   
    Color default_line_color;
    Color default_fill_color;
    //    std::string default_fill_style;

  }; // end of class Clump

  //  Clump operator+ (const Clump& clump, Object_Base& obj);
  */

} /* end of namespace */

#endif /* EPIX2_OBJECT */
