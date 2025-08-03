/*
 * utils.h -- global utility functions for ePiX
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.18
 * Last Change: September 15, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef EPIX_UTILS
#define EPIX_UTILS

#include <string>

namespace ePiX {

  const std::string epix_version();

  double truncate(double); // return 0 if fabs(arg) < epsilon

  // print GMT string for file header
  std::string date_and_time();

  // remove everything from arg except chars in srch
  std::string get_chars(std::string arg, const std::string& srch);

  // Use break to split arg into lines of at most FILE_WIDTH characters,
  // return mangled copy of arg
  std::string break_lines(std::string arg, const std::string& break_str);
}
#endif /* EPIX_UTILS */
