/* 
 * lengths.cc: Scaling and length conversion
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc8
 * Last Change: June 21, 2004
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

/*
 *
 * There are three types of coordinates/length in ePiX:
 *   (1) Cartesian (the user interface)
 *   (2) LaTeX picture coordinates (unitlength, the output)
 *   (3) File-independent true unit (pt, used for visual formatting)
 *
 * Cartesian coordinates live in [x_min, x_max] x [y_min, y_max], the 
 * "bounding box". Latex picture coordinates live in the "picture box"
 * [0, h_size] x [0, v_size]. The functions hscale and vscale handle
 * Cartesian to picture coordinate conversion; h_unscale and v_unscale
 * are the inverse functions.
 * 
 * The user specifies objects' locations in Cartesian coordinates, which
 * facilitates the design of portable, mathematically accurate figures. 
 * There is also a facility for offsetting labels by amounts in true pt;
 * This eases the visual placement of labels, and helps to make the figure 
 * portable: A change of bounding box re-positions the label's basepoint
 * globally, while the offset (which is scale-independent) adjusts the
 * label from its new basepoint.
 *
 * True-coordinate changes are handled by scale (which converts true pt
 * to (user-specified) LaTeX picture units and its inverse, unscale. In
 * summary, "scale" functions convert *to* picture coordinates, "unscale"
 * functions convert *from* picture coordinates. These functions know about 
 * the following LaTeX dimensions: "cm", "in", "mm", "pc", and "pt"
 */

#include <iostream>

#include "globals.h"
#include "pairs.h"
#include "cropping.h"
#include "lengths.h"
#include "output.h"

namespace ePiX {

  extern double pic_size;
  extern char *pic_unit;

  bool scale_warning=false;

  double truncate(double t)
  {
    return (fabs(t) < EPIX_EPSILON) ? 0 : t;
  }

  pair truncate (pair p)
  {
    double temp1 = truncate(p.x1());
    double temp2 = truncate(p.x2());

    return pair(temp1, temp2);
  }

  // affine scalings for positions
  // Cartesian to picture
  pair c2p(pair X)
  {
    pair scale = pair(h_size()/x_size, v_size()/y_size);
    pair origin = crop_mask::Bounding_Box.bl();

    return scale & (X - origin);
  }
  // picture to Cartesian
  pair p2c(pair H)
  {
    pair unscale = pair(x_size/h_size(), y_size/v_size());
    pair origin = crop_mask::Bounding_Box.bl();

    return origin + (unscale & H);
  }

  // affine scalings for displacements
  // Cartesian to picture
  pair c2s(pair X)
  {
    pair scale = pair(h_size()/x_size, v_size()/y_size);
    return scale & X;
  }

  pair s2c(pair H)
  {
    pair unscale = pair(x_size/h_size(), y_size/v_size());
    return unscale & H;
  }

  // picture units to true pt
  double p2t(double dimen)
  {
    if (! strcmp((char *)pic_unit,"cm"))
      return (pic_size)*(72.27/2.54)*dimen;

    else if (! strcmp((char *)pic_unit,"in"))
      return (pic_size)*(72.27)*dimen;

    else if (! strcmp((char *)pic_unit,"mm"))
      return (pic_size)*(72.27/25.4)*dimen;

    else if (! strcmp((char *)pic_unit,"pc"))
      return (pic_size)*(12.0)*dimen;

    else if (! strcmp((char *)pic_unit,"pt"))
      return (pic_size)*dimen;

    // Invalid unit length and no warning printed yet
    else if (!scale_warning) 
      {
	scale_warning = true;
      
	epix_warning("Unrecognized length units, assuming pt");
	epix_newline();

	return (pic_size)*dimen;
      }

    // Warning issued already; try our best to comply...
    else
      return (pic_size)*dimen;
  }

  pair p2t(pair H)
  {
    return p2t(1.0)*H;
  }

  // true pt to picture units
  double t2p(double t)
  {
    return t/p2t(1.0);
  }
  pair t2p(pair T)
  {
    return (1.0/p2t(1.0))*T;
  }

  // pair given in picture coordinates -> length in true pt
  double true_length(pair p)
  {
    return norm(p2t(p));
  }

  // pair given in picture units -> scale to 1 true pt length
  pair true_unit(pair p)
  {
    if ( true_length(p) > EPIX_EPSILON) // return (0,0) if p = (0,0)
      p *= 1.0/true_length(p);

    return p;
  }

  // Approx. number of true pts per angular unit at distance r
  double arc_scale(double r)
  {
    pair temp = c2s(pair(r,r));
    return angle(0.5)*p2t(temp.x1() + temp.x2());
  }

} /* end of namespace */
