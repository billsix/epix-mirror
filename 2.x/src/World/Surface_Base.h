/*** 
 ***  Surface_Base.h -- Interface for surface-like Objects
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
 *** This file provides:
 ***
 *** Surface_Base:
 ***
 ***   In addition to Object_Base facilities, possesses a line color and
 ***   width, a fill color, back color, and reflectivity.
 ***
 ***  [] line_{color,width}()
 ***  [] {fill,back}_color(Color)
 ***/

#ifndef EPIX2_SURFACE_BASE
#define EPIX2_SURFACE_BASE

#include <vector>

// #include "Point.h"
#include "Vector.h"

#include "Color.h"
#include "Length.h"
#include "Styles.h"

#include "Object.h"
#include "Surface_Rep.h"

namespace ePiX2 {

  class Surface_Base : public Object_Base
    {
    public:
      // we block vpt's view of X?
      // virtual bool hides(const Point vpt, const Point X) const = 0;

      void add_shard(const Plate*);

      void chop(const Vector&);  // half-space cut
      void slice(const Vector&); // planar cut
      void contribute(std::vector<Shard>&) const;

      /*
      void line_color(const Color&, bool);
      void line_width(const Length&, bool);

      void fill_color(const Color&, bool);
      void back_color(const Color&, bool);
      */

      void update_by(const Surface_Style& sty)
      {
	m_style.update_by(sty);
      }

      void set_line(const Color& col, const Length& len, bool bind)
	{
	  m_style.set_line(col, len, bind);
	}

      void set_fill(const Color& fill, const Color& back, bool bind,
		    const double shine, const double alpha)
	{
	  m_style.set_fill(fill, back, bind, shine, alpha);
	}

      void set_fill(const Color& col, bool bind,
		    const double shine, const double alpha)
	{
	  m_style.set_fill(col, col, bind, shine, alpha);
	}

      void set_shine(const double shine, bool bind)
	{
	  m_style.set_shine(shine, bind);
	}


    private:
      Surface_Style m_style;

      bool m_closed; // closed and oriented -> backface cullable

      std::vector<Plate> m_fragments;
    }; // end of class Surface_Base

} /* end of namespace */

#endif /* EPIX2_SURFACE_BASE */
