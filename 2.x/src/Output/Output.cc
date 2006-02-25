/*
 * Output.cc -- ePiX2 Output functions
 *
 * This file is part of ePiX, a program for creating high-quality
 * figures in LaTeX
 *
 * Version 2.0pre
 *
 * Last Change: January 08, 2006
 */

/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
 * Department of Mathematics and Computer Science
 * College of the Holy Cross
 * Worcester, MA, 01610-2395, USA
 *
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

#include <cstdlib>

#include "Output.h"
#include "Picture.h"

namespace ePiX2 {

  // global function the mediates writing a Picture to a Format
  void print(Picture& p, const char* filename, const Format& f)
  {
    p.print_to(filename, f);
  }

} /* end of namespace */
