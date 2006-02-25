/*** 
 ***  Object.h -- epix2::Object_Base and Object classes
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: February 12, 2006
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
 *** This file defines the virtual ePiX2 scene element hierarchy:
 ***
 *** Object_Base:
 *** 
 ***   Provides translation, scaling (homothety), rotatation, and reflection
 ***   operators, and can be chopped by a half-space or sliced by a plane.
 ***   Each Object_Base contains an orthonormal basis and scale factor, and
 ***   transformations act upon these data. When an Object_Base is drawn, its
 ***   pieces' coordinates are computed with respect to the *internal* basis
 ***   and placed into a *standard* Cartesian coordinate system.
 ***
 ***  [] +=(Vector), move_to(Point): Translate
 ***  [] *=(double), scale(double):  Homothety with respect to our origin
 ***  [] rotate(double angle, Vector axis): Rotate by angle about axis
 ***  [] reflect(Vector): Reflect in plane defined by Vector
 ***  [] chop(Vector), slice(Vector): Cut, using specified *outward* normal
 ***
 ***
 *** Object:
 *** 
 ***   A handle to Object_Base that provides direct access to Object_Base
 ***   operations and pointer-like access to other operations, e.g.
 ***   obj.reflect(Vector), obj->line_color(Color).
 ***
 ***/

#ifndef EPIX2_OBJECT
#define EPIX2_OBJECT

#include <vector>

#include "Point.h"
#include "Vector.h"
#include "Basis.h"

#include "Color.h"
#include "Length.h"
#include "Styles.h"

#include "Object_Rep.h"

namespace ePiX2 {

  // All scene elements derived from here
  class Object_Base
    {
      friend class Surface_Base;
    public:
      // pure virtuals
      virtual Object_Base* clone(void) const = 0;
      virtual void chop(const Vector&) = 0;  // half-space cut
      virtual void slice(const Vector&) = 0; // planar cut

      //      virtual void add_shard(const Shard_Base*) = 0;
      // add our shards to specified container
      virtual void contribute(std::vector<Shard>&) const = 0;

      virtual bool hides(const Point vpt, const Point X) const = 0;

      // non-virtuals
      Point  in_coords(const  Point& arg) const; // express w.r.t. basis/scale
      Vector in_coords(const Vector& arg) const;
      //      Point here(void) const;

      // virtuals
      virtual ~Object_Base(void) { }

      // next six functions re-defined by Compound
      virtual Object_Base& operator+= (const Vector&); // translate
      virtual void move_to(const Point arg);      // set origin

      virtual Object_Base& operator*= (const double);  // scale
      virtual void scale (const double);

      virtual void reflect(const Vector& axis);
      virtual void rotate(const double angle, const Vector& axis);


      virtual void update_by(const Path_Style& sty) { }
      virtual void update_by(const Surface_Style& sty) { }
      virtual void update_by(const Text_Style& sty) { }

      virtual void set_line(const Color&, const Length&, bool bind) { }
      virtual void set_base(const Color&, const Length&, bool bind) { }
      virtual void set_fill(const Color&, const Color&, bool bind,
			    const double shine, const double alpha) { }

      virtual void set_fill(const Color&, bool bind,
			    const double shine, const double alpha) { }

      virtual void shine(const double, bool) { }

      /*
      // paths and surfaces
      virtual void line_color(const Color&, bool override) { }
      virtual void line_width(const Length&, bool override) { }
      virtual void shine(const double, bool override) { }
      // paths
      virtual void base_color(const Color&, bool override) { }
      virtual void base_width(const Length&, bool override) { }
      // surfaces
      virtual void fill_color(const Color&, bool override) { }
      virtual void back_color(const Color&, bool override) { }

      virtual void default_line_color(const Color&) { }
      virtual void default_line_width(const Length&) { }

      virtual void default_base_color(const Color&) { }
      virtual void default_base_width(const Length&) { }

      virtual void default_fill_color(const Color&) { }
      virtual void default_back_color(const Color&) { }

      virtual void default_shine(const double) { }
      */

    private:
      Basis m_orient;
      double m_scale;

    }; // end of class Object_Base

  // Object_Base handle
  class Object 
    {
    public:
      Object(void);
      Object(Object_Base*);
      Object(const Object&);
      Object& operator= (const Object&);

      ~Object(void);

      //      Object clone(void) const;
      Object_Base* operator->() const;

      // re-bind a Object to another Object_Base*
      void bind(Object_Base*);

      bool operator== (const Object&) const;

      // User interface
      Object& operator+= (const Vector&); // translate
      Object& operator*= (const double);  // scale


      void move_to(const Point arg);      // set origin
      void scale (const double);

      void reflect(const Vector& axis);
      void rotate(const double angle, const Vector& axis);

    private:
      Object_Base* m_object;
      int* c_pt;

  }; // end of class Object

} /* end of namespace */

#endif /* EPIX2_OBJECT */
