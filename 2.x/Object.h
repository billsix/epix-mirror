/*** 
 ***  Object.h -- epix2::Object class
 ***
 *** This file is part of ePiX, a preprocessor for creating high-quality 
 *** line figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: August 06, 2005
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
 ***   This file provides:
 ***
 *** The Object class, the base class for all visible elements of a Picture.
 *** Objects have an orientation (Basis) and scale factor; all geometric
 *** operations are expressed by their *inverse* ops on the basis/scale.
 *** When an Object is "shatter()"ed (converted to one or more Shards for
 *** hidden object removal), the basis and scale are used to compute the
 *** transformed Object.
 ***
 *** shatter() is not virtual, allowing Objects (not just Object*s) to be
 *** stored in a Picture list.
 ***/

#ifndef EPIX2_OBJECT
#define EPIX2_OBJECT

#include <list>
#include <iostream>

#include "Enums.h"

namespace ePiX2 {

  class Point;  /***/
  class Vector; /***/
  class Basis;  /***/
  class Color;  /***/
  class Shard;  /***/

  /* * * Object.h * * */

  class Object   // Marker, Label, Shape, Clump
    {
      friend class Picture;

    public:

      Object(void) { closed_oriented=false; }
      virtual ~Object(void) { }

      Object& operator+= (const Vector&); // translate
      void move_to(const Point arg);      // set origin

      Object& operator*= (const double);  // scale
      void scale (const double);

      void reflect(const Vector& axis);
      void rotate(const double angle, const Vector& axis);

      virtual void shatter(void) { }

      // we block vpt's view of X?
      virtual bool hides(const Point vpt, const Point X) const {return false;}

    protected:

      Basis the_orient; // contains location data
      double the_scale;

      bool closed_oriented;

      std::list<Shard> fragments;
    }; // end of class Object

  Object operator+ (const Object&, const Vector&);
  Object operator* (const Object&, const double);


  class Shape : public Object {

    friend class Clump;

  public:

    Shape(void);

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

  protected:
    Style style;

    Color get_line_color(void);
    Color get_fill_color(void);
    Color get_back_color(void);

  }; // end of class Shape


  /*  
  class Clump : public Shape {

  public:

    // add an Object
    Clump& operator+= (const Object obj);
    void operator+ (const Object obj);

    void shatter(void);
    bool hides(const Point vpt, const Point X);

    std::list<Object*> parts;
    std::list<Shard>  fragments;

    //    double    line_width;
    //    line_type line_style;
   
    Color default_line_color;
    Color default_fill_color;
    //    std::string default_fill_style;

  private:

    void polygonize(void);

  }; // end of class Clump
  */

} /* end of namespace */

#endif /* EPIX2_OBJECT */
