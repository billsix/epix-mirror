/* 
 *  Octa.h -- epix2::Octahedron class
 *
 * This file is part of ePiX, a program for creating high-quality 
 * figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: August 07, 2005
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005
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
 *     - The Octahedron class (derived from Object) and operators
 */

#include <vector>

#include "Functions.h"
#include "Object.h"
#include "Triangle.h"
#include "Octa.h"

namespace ePiX2 {

  void Octahedron::shatter(void)
  {
    closed_oriented=true;

    // vertices
    Point v_1n(-side1,0,0), v_1p(side1,0,0);
    Point v_2n(0,-side2,0), v_2p(0,side2,0);
    Point v_3n(0,0,-side3), v_3p(0,0,side3);

    Triangle f_ppp(v_1p, v_2p, v_3p), f_ppn(v_2p, v_1p, v_3n);
    Triangle f_npp(v_2p, v_1n, v_3p), f_npn(v_1n, v_2p, v_3n);
    Triangle f_nnp(v_1n, v_2n, v_3p), f_nnn(v_2n, v_1n, v_3n);
    Triangle f_pnp(v_2n, v_1p, v_3p), f_pnn(v_1p, v_2n, v_3n);

    (*this) << f_ppp << f_ppn << f_npp << f_npn
	    << f_nnp << f_nnn << f_pnp << f_pnn;

    std::list<Object*>::iterator faces;

    for (faces=parts.begin(); faces!=parts.end(); ++faces)
      {
	(*faces)->skeleton(!solid);
	(*faces)->set_line_color(get_line_color());
	(*faces)->set_fill_color(get_fill_color());
      }

    this->Object::shatter();

  } // end of Octahedron::shatter

} /* end of namespace */
