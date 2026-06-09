/* 
 * enums.h -- ePiX's user-visible enumeration types
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.0.23
 * Last Change: January 10, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
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

#ifndef EPIX_ENUMS
#define EPIX_ENUMS

namespace ePiX {

  enum epix_mark_type {PATH, CIRC, SPOT, RING, DOT, DDOT, PLUS, OPLUS, 
		       TIMES, OTIMES, DIAMOND, UP, DOWN, BOX, BBOX, 
		       HTICK, VTICK, TEXT};

  enum epix_label_posn {none, c, r, tr, rt, t, tl, lt, l, bl, lb, b, br, rb};

  enum epix_integral_type {LEFT, RIGHT, UPPER, LOWER, TRAP, MIDPT};

  enum epix_field_type {SLOPE, DART, VECTOR};

} // end of namespace

#endif /* EPIX_ENUMS */
