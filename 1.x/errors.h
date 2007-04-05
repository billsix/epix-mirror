/*
 * errors.h -- ePiX warning/error functions
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.23
 * Last Change: April 5, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
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

#ifndef EPIX_ERRORS
#define EPIX_ERRORS

#include <cstdlib>
#include <iostream>
#include <string>

namespace ePiX {

  inline void epix_warning(std::string msg)
  {
    std::cerr << "ePiX WARNING: " << msg << std::endl;
  } 

  inline void epix_error(std::string msg)
  {
    std::cerr << "ePiX ERROR: " << msg << std::endl;
    exit(1);
  } 

  // Shape error types
  enum constructor_error_type {MALFORMED, MULTIPLICITY, COLLINEAR_PTS};
  enum join_error_type {TANGENT, PARALLEL, COINCIDENT, NON_COPLANAR,
			SEPARATED, CONCENTRIC};

  // and handlers
  struct constructor_error {
    constructor_error_type type;
    constructor_error(constructor_error_type x)
    : type(x) { }
  };

  struct join_error {
    join_error_type type;
    join_error(join_error_type x) { type = x; }
  };
} // end of namespace

#endif /* EPIX_ERRORS */
