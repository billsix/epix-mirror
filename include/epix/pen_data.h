/* 
 * pen_data.h -- Color and width
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.20
 * Last Change: September 19, 2007
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
 * This file defines ePiX's concept of a pen (Color and length)
 *
 * This header is not part of epix.h.
 */
#ifndef EPIX_PEN_DATA
#define EPIX_PEN_DATA

#include "Color.h"
#include "length.h"

namespace ePiX {

  class Camera;

  class pen_data {
  public:
    // plain black
    pen_data(const Color& col, const length& wid);
    pen_data(const Color& col=Black());

    // set
    void color(const Color& col);
    void width(const length& len);

    // get
    Color  color() const;
    length width() const;

    bool operator== (const pen_data& pen) const;
    bool operator!= (const pen_data& pen) const;

    pen_data seen_through(const Camera& cam) const;

    bool is_unset() const;

  private:
    Color  m_color;
    length m_width;  
  }; // end of class pen_data


  // undrawn
  const pen_data& Xfine();

} // end of namespace

#endif /* EPIX_PEN_DATA */
