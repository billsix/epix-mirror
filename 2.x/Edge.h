/***
 ***  Edge.h -- epix2::Edge and Screen_Edge classes
 ***
 *** This file is part of ePiX, a preprocessor for creating high-quality 
 *** line figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: July 29, 2005
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
 ***/


#ifndef EPIX2_EDGE
#define EPIX2_EDGE

#include "Point.h"
#include "Pair.h"
#include "Color.h"

namespace ePiX2 {

  class Edge {
  public:
    Point first, second;
    Color line_color;
    bool drawn;

    Edge(void) { }
    Edge(Point arg1, Point arg2, const Color& col, bool vis=true) 
      : first(arg1), second(arg2), line_color(col), drawn(vis) { }
  };

  class Screen_Edge {
  public:
    Pair first, second;
    Color line_color;
    bool drawn;

    Screen_Edge(void) { }
    Screen_Edge(Pair arg1, Pair arg2, const Color& col, bool vis=true) 
      : first(arg1), second(arg2), line_color(col), drawn(vis) { }

    void print(bool force=false) const;
  };

} /* end of namespace */

#endif /* EPIX2_EDGE */
