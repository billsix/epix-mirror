/*** 
 ***  Chop.h -- ePiX2 clipping/snipping function
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: January 16, 2006
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
 ***/

/*
 * A chop-able class (Silhouette, Outline, Shard...) must contain members
 *
 *   std::list<Edge> m_border; // delineating edges
 *   bool m_cropped;           // true if some edge(s) undrawn
 *
 * The caller's Edge list is replaced by the Edge list of the chopped
 * object, and if chopping occurs, the bool is set to true.
 *
 * The "Pt" arguments are the tail and head of a Vector normal to the
 * cutting line/plane. The Vector points *toward* the chopped portion.
 *
 * "Pt" must provide affine operations (addition, scalar mult, subtraction,
 * and a diff of "Pt"s must have an inner product operator "|").
 *
 * An "Edge" must be constructible from two "Pt"s and a bool.
 */

#ifndef EPIX2_CHOP
#define EPIX2_CHOP

#include <list>

#include "Point.h"
#include "Pair.h"
#include "Object_Rep.h"

namespace ePiX2 {

  void chop(const Point& tail, const Point& head, 
	    std::list<Edge3>& border,
	    bool& is_chopped);

  void chop(const Pair& tail, const Pair& head, 
	    std::list<Edge2>& border,
	    bool& is_chopped);

} // end of namespace

#endif // EPIX2_CHOP
