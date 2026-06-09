/* 
 * marker_style.cc -- ePiX's dot size and tick length
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.9
 * Last Change: July 29, 2007
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

#include "functions.h"
#include "marker_style.h"

namespace ePiX {

  marker_sizes::marker_sizes()
    : the_dotsize(AVG_DOT), the_ticksize(AVG_TICK) { }

  marker_sizes& marker_sizes::dot_size(double diam)
  {
    if (0 < diam)
      the_dotsize = snip_to(diam, MIN_DOT, MAX_DOT);
    else
      the_dotsize = AVG_DOT;

    return *this;
  }

  marker_sizes& marker_sizes::tick_size(double len)
  {
    if (0 < len)
      the_ticksize = snip_to(len, MIN_TICK, MAX_TICK);
    else
      the_ticksize = AVG_TICK;

    return *this;
  }

  double marker_sizes::dot_size()  const
  {
    return the_dotsize;
  }

  double marker_sizes::tick_size()  const
  {
    return the_ticksize;
  }

  marker_sizes& the_mark_size()
  {
    static auto* the_mark_size(new marker_sizes());
    return *the_mark_size;
  }
} // end of namespace
