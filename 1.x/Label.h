/* 
 * Label.h -- ePiX::Label class
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.11rc9
 * Last Change: June 23, 2004
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
 *     move_to(P) // change basepoint
 *     offset(P)  // change offset
 *     align(epix_label_posn) // change alignment
 */


#ifndef EPIX_LABEL
#define EPIX_LABEL

// #include<iostream>
#include<sstream>
#include "globals.h"
#include "triples.h"

namespace ePiX {

  class Label
    {
    private:
      P the_basepoint;
      std::string the_label_text;
      epix_mark_type the_label_type;
      epix_label_posn the_alignment;
      bool masked;
      P the_offset;

    public:
      Label(P basepoint, 
	    std::string label_text, 
	    epix_mark_type label_type=TEXT,
	    epix_label_posn alignment=none, 
	    bool mask=false,
	    P offset=P(0,0))
	{
	  the_basepoint = basepoint;
	  the_label_text = label_text;
	  the_label_type = label_type;
	  the_alignment = alignment;
	  masked = mask;
	  the_offset = offset;
	}

      Label(P basepoint, double f(P),
	    epix_label_posn alignment=none, 
	    bool mask=false,
	    P offset=P(0,0));

      Label move_to(P arg) { the_basepoint = arg; return *this; }
      Label offset(P arg) { the_offset = arg; return *this; }
      Label align(epix_label_posn arg) { the_alignment=arg; return *this; }

      void draw();

    }; /* end of class */

  inline void draw(Label arg) { arg.draw(); }
} /* end of namespace */

#endif /* EPIX_LABEL */
