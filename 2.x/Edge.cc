/* 
 * Edge.cc -- Ordered edges and operations for Screen coordinates
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

#include <iostream>

#include "Output.h"
#include "Edge.h"

namespace ePiX2 {

  void Screen_Edge::print(bool force) const
    {
      if (drawn || force) // we're visible or forced
	{
	  if (linecolor_isnt(line_color))
	    std::cout << std::endl << "\\psset{linecolor=" 
		      << line_color.name() << "}";

	  if (fillstyle_isnt("none"))
	    std::cout << std::endl << "\\psset{fillstyle=none}";

	  std::cout << std::endl << "\\psline";
	  raw_print(first);
	  raw_print(second);
	}
    }

} /* end of namespace */
