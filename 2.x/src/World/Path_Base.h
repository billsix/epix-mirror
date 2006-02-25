/*** 
 ***  Path_Base.h -- epix2::Path-like Objects
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
 *** Path_Base:
 ***
 ***   In addition to Object_Base facilities, possesses a line color and
 ***   width, an under color and width, and a reflectivity factor.
 ***
 ***  [] {line,under}_color(Color)
 ***  [] {line,under}_width(Length)
 ***/

#ifndef EPIX2_PATH_BASE
#define EPIX2_PATH_BASE

#include <vector>

#include "Point.h"
#include "Vector.h"

#include "Color.h"
#include "Length.h"
#include "Styles.h"

#include "Object.h"
#include "Object_Rep.h"

namespace ePiX2 {

  class Path_Base : public Object_Base 
    {
    public:
      void line_color(const Color&, bool);
      void line_width(const Length&, bool);

      void base_color(const Color&, bool);
      void base_width(const Length&, bool);

      void shine(const double, bool);

      // paths hide nothing
      bool hides(const Point vpt, const Point X) const { return false; }

      void add_shard(const Wire*);

      void chop(const Vector&);  // half-space cut
      void slice(const Vector&); // planar cut
      void contribute(std::vector<Shard>) const;

      void update_by(const Path_Style& sty);

    private:
      Path_Style m_style;

      std::vector<Shard> m_fragments; // can be created w/o location

  }; // end of class Path_Base

} /* end of namespace */

#endif /* EPIX2_PATH_BASE */
