/* 
 * Pair.cc -- Ordered pairs and operations for Screen coordinates
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 0.9.0rc1
 * Last Change: August 01, 2003
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

#include "Pair.h"

namespace ePiX3d {

  // Multiplication by i
  Pair J(Pair arg) { return Pair(0,1)*arg; }

  // dot product
  double operator |(Pair arg1, Pair arg2) { return real(arg1*conj(arg2)); }

  // componentwise product (a,b)&(x,y)=(ax,by)
  Pair operator &(Pair arg1, Pair arg2)
  {
    return Pair(real(arg1)*real(arg2), imag(arg1)*imag(arg2));
  }

} /* end of namespace */


