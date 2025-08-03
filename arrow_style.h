/* 
 * arrow_style.h -- ePiX's arrowhead parameters
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.15
 * Last Change: September 08, 2007
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

/*
 * The class in this file implements backend for user-level global
 * functions defined in state.h. This header is not part of epix.h.
 */
#ifndef EPIX_ARROW_STYLE
#define EPIX_ARROW_STYLE

namespace ePiX {

  // arrowhead parameters
  // Magic numbers: initialization constants
  class arrowhead_state {
  public:
    arrowhead_state();

    // set
    arrowhead_state& width(double w);
    arrowhead_state& ratio(double r);
    arrowhead_state& inset(double arg);

    arrowhead_state& fill(bool);

    // get
    double width() const;
    double ratio() const;
    double inset() const;

    bool   fill() const;

  private:
    double m_width; // Half-width of arrowheads in pt
    double m_ratio; // 2*Length/width ratio of arrowheads
    double m_inset; // Base indentation (frac of width*ratio)

    bool   m_fill;
  }; // end of class arrowhead_state

  arrowhead_state& the_arrowhead_style();

} // end of namespace

#endif /* EPIX_ARROW_STYLE */
