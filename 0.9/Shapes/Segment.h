/* 
 * segment.h -- ePiX::segment class and mathematical operators
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.10rc5
 * Last Change: April 13, 2003
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

#ifndef _EPIX_SEGMENT
#define _EPIX_SEGMENT

#include "../globals.h"
#include "../triples.h"

namespace ePiX {

  class segment
    {
    private:
      triple endpt1;
      triple endpt2;

    public:
      segment(triple p1, triple p2) { endpt1 = p1; endpt2 = p2; }

      // Note: Ends of segment are not "ordered" meaningfully
      triple end1() { return endpt1; }
      triple end2() { return endpt2; }

      segment slide(triple arg)
	{
	  return segment(arg+(*this).end1(), arg+(*this).end2());
	}

      triple midpoint()
	{ 
	  return 0.5*((*this).end1() + (*this).end2()); 
	}

    }; /* end of segment class */

  inline triple midpoint(triple arg1, triple arg2) 
    { 
      return 0.5*(arg1 + arg2); 
    }
  // alternative constructors
  inline segment join(triple p1, triple p2) { return segment(p1,p2); }
      
  triple operator * (segment& seg1, segment& seg2);

} /* end of namespace */

#endif /* _EPIX_SEGMENT */
