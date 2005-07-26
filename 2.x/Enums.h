/***
 *** Enums.h -- ePiX2 enumerated types
 ***
 *** This file is part of ePiX, a preprocessor for creating high-quality
 *** line figures in LaTeX
 ***
 *** Version 2.0pre
 *** Last Change: July 24, 2005
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
 ***/

#ifndef EPIX2_ENUMS
#define EPIX2_ENUMS

namespace ePiX2 {

  enum epix2_screen_shape { MASK_RECT, MASK_ELLIPSE, MASK_DIAMOND };

  // subset of those provided by xcolor
  enum epix2_color_model {MONO, rgb, cmy, cmyk, gray};

  enum epix2_shade_type {SHADE_NONE, SHADE_SOLID, SHADE_FLAT};

  enum epix2_align_type {none, c, r, tr, rt, t, tl, lt, l, bl, lb, b, br, rb};

  enum epix2_mark_type {MK_PATH, MK_CIRC, MK_SPOT, MK_RING, 
			MK_DOT, MK_DDOT, MK_PLUS, MK_OPLUS,
			MK_TIMES, MK_OTIMES, MK_DIAMOND, 
			MK_UP, MK_DOWN, MK_BOX, MK_BBOX,
			MK_HTICK, MK_VTICK, MK_TEXT};

  // type field for Pair Camera::operator(Point)
  enum epix2_lens_type { orthogonal, perspective, fisheye, bubble };

} /* end of namespace */

#endif /* EPIX2_ENUMS */
