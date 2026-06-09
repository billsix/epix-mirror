/* 
 * frame.h -- Orthonormal basis
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.15
 * Last Change: October 10, 2006
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*
 * In geometry, a "frame" is a right-handed orthonormal basis, that is,
 * an ordered set of three mututally perpendicular unit vectors, oriented
 * according to the right-hand rule. A frame has nothing to do with
 * picture frames.
 *
 *
 * This file provides:
 *   - the frame class, and routines for rotating a frame about the axes
 *     determined by its elements. (Note that there are no methods for
 *     rotating about fixed coordinate axes, and that rotations in R^3 do
 *     not generally commute.
 */

#ifndef EPIX_FRAME
#define EPIX_FRAME

#include "triples.h"

namespace ePiX {

  class frame {
  public:
    // standard basis
    frame();

    // Gram-Schmidt
    frame(P arg1, P arg2, P arg3); // need args by value

    // frame elements
    P sea() const;
    P sky() const;
    P eye() const;

    // rotations about frame elements
    frame& rot1(double angle);
    frame& rot2(double angle);
    frame& rot3(double angle);

  private:
    P m_e1; // sea
    P m_e2; // sky
    P m_e3; // eye
  }; // end of class frame

} // end of namespace

#endif /* EPIX_FRAME */
