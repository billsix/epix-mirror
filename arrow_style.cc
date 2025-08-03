/* 
 * arrow_atyle.cc -- ePiX's arrowhead parameters
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.15
 * Last Change: September 08, 2007
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

/*
 * The class in this file implements backend for user-level global
 * functions defined in state.h. This header is not part of epix.h.
 */
#include <cmath>

#include "constants.h"
#include "functions.h" // for snip_to
#include "arrow_style.h"

namespace ePiX {

  arrowhead_state::arrowhead_state()
    : m_width(0.5*AVG_AWIDTH), m_ratio(AVG_ARATIO),
      m_inset(AVG_INSET), m_fill(false) { }

  // set
  arrowhead_state& arrowhead_state::width(double w)
  {
    m_width = fabs(0.5*snip_to(w, MIN_AWIDTH, MAX_AWIDTH));
    return *this;
  }

  arrowhead_state& arrowhead_state::ratio(double r)
  {
    m_ratio = fabs(snip_to(r, MIN_ARATIO, MAX_ARATIO));
    return *this;
  }

  arrowhead_state& arrowhead_state::inset(double arg)
  {
    m_inset = snip_to(arg, MIN_INSET, MAX_INSET);
    return *this;
  }

  arrowhead_state& arrowhead_state::fill(bool fill)
  {
    m_fill = fill;
    return *this;
  }


  // get
  double arrowhead_state::width() const
  {
    return m_width;
  }

  double arrowhead_state::ratio() const
  {
    return m_ratio;
  }

  double arrowhead_state::inset() const
  {
    return m_inset;
  }

  bool arrowhead_state::fill() const
  {
    return m_fill;
  }

  arrowhead_state& the_arrowhead_style()
  {
    static arrowhead_state* the_arrowhead_state(new arrowhead_state());
    return *the_arrowhead_state;
  }
} // end of namespace
