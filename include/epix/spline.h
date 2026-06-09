/* 
 * spline.h -- ePiX spline class
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.3
 * Last Change: March 04, 2007
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
#ifndef EPIX_SPLINE
#define EPIX_SPLINE

#include <vector>
#include "triples.h"

namespace ePiX {

  class path;

  class n_spline {
  public:

    n_spline(const std::vector<P>& data, bool loop=false);

    path data(const unsigned int) const;

  private:
    std::vector<P> m_vertices;

    // coefficient sequences
    std::vector<double> m_k0;
    std::vector<double> m_k1;
    std::vector<double> m_k2;
    std::vector<double> m_k3;
  }; // end of class n_spline

} // end of namespace

#endif /* EPIX_SPLINE */
