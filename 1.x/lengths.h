/*
 * lengths.h -- ePiX true and LaTeX length manipulation
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.0
 * Last Change: September 04, 2004
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004
 * Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 * In ePiX, there are three systems of coordinates:              *
 *                                                               *
 *   Cartesian (C) -- All the user needs to know about           *
 *   Picture   (P) -- The language of the LaTeX front end        *
 *   True      (T) -- Physical location on page from lower left  *
 *                                                               *
 * The coordinate triumvirates' initials form a bad physics pun. *
 *                                                               *
 * Recognized LaTeX units are: bp, cm, in, mm, pc, pt, and sp    *
 *                                                               *
 * All objects and locations are specified in C by the user; the *
 * conversion to true coordinates is transparent. The rationale  *
 * for retaining LaTeX coordinates, rather than forcing all ePiX *
 * figures to be specified in true pt, is of course that figures *
 * are sized according to printing requirements, and often not   *
 * in terms of true pt. The reason for using true pt is that raw *
 * scaling of a LaTeX figure is not robust; label positions must *
 * be specified with true pt offsets if their locations are to   *
 * scale properly when a figure is resized.                      *
 *                                                               *
 * An ePiX figure is a rectangle, described as follows:          *
 *   Cartesian: [x_min, x_max] x [y_min, y_max]                  *
 *   Picture:   [0, h_size] x [0, v_size] + (h_offset, v_offset) *
 *   True:      Only (La)TeX knows/needs to know this            *
 *                                                               *
 * ePiX provides six scale-changing functions:                   *
 *   c2p, p2c -- Cartesian -> LaTeX and back for points          *
 *   c2s, s2c -- Cartesian -> LaTeX and back for vectors         *
 *   p2t, t2p -- LaTeX -> True and back                          *
 *                                                               *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef EPIX_LENGTHS
#define EPIX_LENGTHS

#include "pairs.h"

namespace ePiX {

  // If less than EPIX_EPSILON, set to zero 
  double truncate(double);

  pair truncate(pair);

  // "Cartesian" = Cartesian coordinates of crop_mask::Bounding_Box
  // "picture"   = Cartesian coordinates of crop_mask::Picture

  // Affine scaling
  pair c2p(pair);  // Cartesian to picture for positions
  pair p2c(pair);  // picture to Cartesian

  pair c2s(pair); // Cartesian to picture for displacements
  pair s2c(pair); // picture to Cartesian

  double p2t(double); // picture coords to true pt
  pair p2t(pair);

  double t2p(double); // true pt to picture
  pair t2p(pair);

  // functions of <pair> in picture coords
  double true_length(pair); // length in true pt
  pair true_unit(pair);     // parallel true pt unit vector

  double arc_scale(double r);
} /* end of namespace */

#endif /* EPIX_LENGTHS */
