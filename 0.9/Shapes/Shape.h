/* 
 * Shapes.h -- ePiX::Shape base class and operators
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.8.10rc5
 * Last Change: April 14, 2003
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

#include <iostream>

#include "../triples.h"

namespace ePiX_Shapes {

  enum constructor_error_type {MULTIPLICITY, COLLINEAR_PTS};
  enum join_error_type {TANGENT, PARALLEL, COINCIDENT, NON_COPLANAR,
			SEPARATED, CONCENTRIC};

  // Error handlers
  struct constructor_error {
    constructor_error_type type;
    constructor_error(constructor_error_type x) { type = x; }
  };

  struct join_error {
    join_error_type type;
    join_error(join_error_type x) { type = x; }
  };

} /* end of namespace ePiX_Shapes */

