/* 
 * Pair.cc -- Ordered pairs and operations for Screen coordinates
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: July 29, 2005
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

#include "Pair.h"

namespace ePiX2 {

  // complex arithmetic
  Pair J(const Pair arg) { return Pair(-arg.x2(), arg.x1()); }

  Pair operator *(const Pair arg1, const Pair arg2)
  {
    double a=arg1.x1(), b=arg1.x2();
    double c=arg2.x1(), d=arg2.x2();

    return Pair(a*c-b*d, a*d+b*c);
  }

  double operator |(const Pair arg1, const Pair arg2)
    {
      return arg1.x1()*arg2.x1() + arg1.x2()*arg2.x2();
    }

} /* end of namespace */
