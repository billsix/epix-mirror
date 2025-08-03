/* 
 * screen_crop.h -- ePiX functions for cropping 2D paths and loops
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.8
 * Last Change: July 17, 2007
 *
 * 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
 * Department of Mathematics and Computer Science
 * College of the Holy Cross
 * Worcester, MA, 01610-2395, USA
 *
 *
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
#ifndef EPIX_SCREEN_CROP
#define EPIX_SCREEN_CROP

#include <list>

#include "edge_data.h"

namespace ePiX {

  // return reference to second argument
  std::list<edge2d>& crop_path_2nd(const std::list<edge2d>&,
				   std::list<edge2d>&);

  std::list<edge2d>& crop_loop_2nd(const std::list<edge2d>&,
				   std::list<edge2d>&);
} // end of namespace

#endif /* EPIX_SCREEN_CROP */
