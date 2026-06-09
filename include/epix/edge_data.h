/* 
 * edge_data.h -- ePiX::edge implementation classes for polyline/polygon
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.2.5
 * Last Change: May 06, 2008
 *
 * 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
 * Department of Mathematics and Computer Science
 * College of the Holy Cross
 * Worcester, MA, 01610-2395, USA
 *
 *
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
#ifndef EPIX_EDGE_DATA
#define EPIX_EDGE_DATA
#include <algorithm>

#include "triples.h"
#include "pairs.h"

namespace ePiX {

  // basic edge data: ordered endpoints and visibility flag
  template<typename T> class edge_data {
  public:
    edge_data(const T& fm, const T& to, bool seen)
      : m_tail(fm), m_head(to), m_seen(seen) { }

    T tail()    const { return m_tail; }
    T head()    const { return m_head; }
    bool is_seen() const { return m_seen; }
    bool is_null() const { return m_tail == m_head; }

    edge_data& shift(const T& dX)
      {
	m_tail += dX;
	m_head += dX;
	return *this;
      }

    edge_data& reverse()
      {
	std::swap(m_tail, m_head);
	return *this;
      }

    double length() const
      {
	return norm(m_head - m_tail);
      }

    void seen(bool S) { m_seen=S; }

  private:
    T m_tail;
    T m_head;
    bool m_seen;
  };

  using edge3d = edge_data<P>;
  using edge2d = edge_data<pair>;
} // end of namespace

#endif /* EPIX_EDGE_DATA */
