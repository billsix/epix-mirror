/*
 * errors.h -- ePiX warning/error functions
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.2.3
 * Last Change: March 09, 2008
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008
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

#ifndef EPIX_ERRORS
#define EPIX_ERRORS

#include <iostream>
#include <cstdlib>
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
} // end of namespace

#endif /* EPIX_ERRORS */
