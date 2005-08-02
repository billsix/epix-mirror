/* 
 * Label.h -- ePiX3d::Label class
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.9.0rc1
 * Last Change: June 11, 2004
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004
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

/*
 *   This file provides:
 *
 *   The Label class
 *     - location
 *     - offset
 *     - alignment option
 *     - type
 *     - text field
 *
 *     Label(posn, f(posn)) // numerical label at specified location
 *     move_to(triple) // change basepoint
 *     offset(triple)  // change offset
 *     align(epix_label_posn) // change alignment
 */

#ifndef EPIX3d_LABEL
#define EPIX3d_LABEL

#include <string>
#include "Objects.h"
#include "Point.h"
#include "Pair.h"

namespace ePiX3d {

  class Label : public Object {

  protected:
    Point the_basepoint;
    std::string the_label_text;

  private:
    enum epix3d_label_posn {none, c, r, tr, t, tl, l, bl, b, br};

    epix3d_label_posn the_alignment;

    bool masked;
    Pair the_offset;

    public:
      Label(const Point location=Origin, std::string label_text = "", 
	    epix3d_label_posn alignment=none, 
	    bool mask=false, const Pair offset=Pair(0,0))
	{
	  the_basepoint = location;
	  the_label_text = label_text;

	  the_alignment = alignment;

	  masked = mask;
	  the_offset = offset;
	}

      Label(Point basepoint, double f(double, double, double),
	    epix3d_label_posn alignment=none, 
	    bool mask=false,
	    Pair offset=Pair(0,0));

      void move_to(Point arg) { the_basepoint = arg; }
      void offset(Pair arg) { the_offset = arg; }
      void align(epix3d_label_posn arg) { the_alignment=arg; }

      void draw();

  }; // end of class Label

  class Marker : protected Label {
    enum epix3d_mark_type {PATH, CIRC, SPOT, RING, DOT, DDOT, PLUS, OPLUS, 
			 TIMES, OTIMES, DIAMOND, UP, DOWN, BOX, BBOX, 
			 H_TICK, V_TICK, TEXT};
    
    epix3d_mark_type the_label_type;


  }; // end of class Marker

} /* end of namespace */

#endif /* EPIX3d_LABEL */
