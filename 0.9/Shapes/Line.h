/* 
 * Shapes/Line.h -- ePiX::Line class
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.10rc7
 * Last Change: April 18, 2003
 */

/* 
 * Copyright (C) 2001, 2002, 2003
 * Andrew D. Hwang <ahwang@mathcs.holycross.edu>
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

#ifndef _EPIX_LINE
#define _EPIX_LINE

#include "exceptions.h"
#include "../globals.h"
#include "../triples.h"
#include "../lengths.h"

namespace ePiX {

  class Line
    {
    private:
      triple X0;
      triple direction;

    public:
      // Line through two points
      Line(triple arg1, triple arg2=P(0,0,0)) 
	{ 
	  X0 = arg1;
	  if (norm(arg2 - arg1) < EPIX_EPSILON)
	    throw constructor_error(NULL_LINE);
	  else
	    direction = arg2 - arg1; 
	}

      // Line through point in given direction
      Line Ray(triple arg1, triple arg2) 
	{ 
	  X0 = arg1;
	  if (norm(arg2) < EPIX_EPSILON)
	    throw constructor_error(NULL_LINE);
	  else
	    direction = arg2; 
	}

      // point-slope constructor
      Line(triple tail, double slope)
	{
	  Ray(tail, tail+P(1, slope, 0));
	}    

      // intersection
      friend triple operator * (Line& L1, Line& L2);

      // parallel
      friend bool operator || (const Line& L1, const Line& L2);
      // (in)equality
      friend bool operator == (const Line& L1, const Line& L2);
      friend bool operator != (const Line& L1, const Line& L2);
      //      friend bool contains (const triple& arg);

      // "Visible" portion of *this
      void draw(int n=1);

    }; /* end of Line class */

} /* end of namespace */

#endif /* _EPIX_LINE */
