/* 
 * screen_mask.cc -- ePiX::screen_mask interface
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.4
 * Last Change: June 17, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
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

#include "constants.h"

#include "pairs.h"

#include "screen_mask.h"

namespace ePiX {

  //// screen_mask::alignment points ////
  pair screen_mask::tr() const { return pair(h_max(), v_max()); }
  pair screen_mask::tl() const { return pair(h_min(), v_max()); }
  pair screen_mask::bl() const { return pair(h_min(), v_min()); }
  pair screen_mask::br() const { return pair(h_max(), v_min()); }

  pair screen_mask::t()  const { return pair(h_avg(), v_max()); }
  pair screen_mask::b()  const { return pair(h_avg(), v_min()); }
  pair screen_mask::l()  const { return pair(h_min(), v_avg()); }
  pair screen_mask::r()  const { return pair(h_max(), v_avg()); }
  pair screen_mask::c()  const { return pair(h_avg(), v_avg()); }

} // end of ePiX namespace
