/*
 * Output.cc -- ePiX2 Output functions
 *
 * This file is part of ePiX, a preprocessor for creating high-quality
 * line figures in LaTeX
 *
 * Version 2.0pre
 *
 * Last Change: July 31, 2005
 */

/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005
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

#include <string>

#include "Color.h"
#include "Output.h"

namespace ePiX2 {

  Output_State::Output_State(void)
  {
    linecolor="rgb_999_999_999"; // initially white
    fillcolor="rgb_0_0_0";       // initially black
    fillstyle="none";
  }

  Output_State Output_State::output_state=Output_State();

  // return value true if we've updated
  bool linecolor_isnt(const Color& col)
  {
    bool value = (Output_State::output_state.linecolor != col.name());
    if (value)
      Output_State::output_state.linecolor = col.name();

    return value;
  }

  bool fillcolor_isnt(const Color& col)
  {
    bool value = (Output_State::output_state.fillcolor != col.name());
    if (value)
      Output_State::output_state.fillcolor = col.name();

    return value;
  }

  bool fillstyle_isnt(const std::string& style)
  {
    bool value = (Output_State::output_state.fillstyle != style);
    if (value)
      Output_State::output_state.fillstyle = style;

    return value;
  }

} /* end of namespace */
