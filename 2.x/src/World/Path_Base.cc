/* 
 *  Path_Base.cc -- Path-like Objects
 *
 * This file is part of ePiX, a program for creating high-quality 
 * figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: February 12, 2006
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
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

#include <list>
#include <vector>

#include "Functions.h"
#include "Point.h"
#include "Vector.h"
#include "Basis.h"
#include "Color.h"

#include "Chop.h"
#include "Path_Base.h"
#include "Path_Rep.h"

namespace ePiX2 {

  /* * * Path_Base functions * * */

  // TO DO: Make style function update Shards' attributes
  /*
  void Path_Base::line_color(const Color& col, bool override)
  {
    if (override || m_line_color.is_unset())
      m_line_color = col;
  }

  void Path_Base::line_width(const Length& len, bool override)
  {
    if (override || m_line_width.is_unset())
      m_line_width = len;
  }

  void Path_Base::base_color(const Color& col, bool override)
  {
    if (override || m_base_color.is_unset())
      m_base_color = col;
  }

  void Path_Base::base_width(const Length& len, bool override)
  {
    if (override || m_base_width.is_unset())
      m_base_width = len;
  }

  void Path_Base::shine(const double r, bool override)
  {
    if (override)
      m_shine=r;
  }
  */

  void Path_Base::add_shard(const Wire* S)
  {
    m_fragments.push_back(*S);
  }

  // virtuals from Object_Base
  void Path_Base::chop(const Vector& N)
  {
    std::list<Shard>::iterator p;
    for (p=m_fragments.begin(); p!=m_fragments.end(); ++p)
      (*p)->chop(N);
  }

  void Path_Base::slice(const Vector& N)
  {
    std::list<Shard>::iterator p;
    for (p=m_fragments.begin(); p!=m_fragments.end(); ++p)
      (*p)->slice(N);
  }

} // end of namespace

