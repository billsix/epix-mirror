/* 
 *  Surface_Base.cc -- Surface-like Objects
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

#include <vector>

#include "Functions.h"
#include "Vector.h"

#include "Styles.h"

#include "Chop.h"

#include "Surface_Base.h"
#include "Surface_Rep.h"
#include "Object.h"

namespace ePiX2 {

  /* * * Surface_Base Functions * * */
  void Surface_Base::add_shard(const Plate* S)
  {
    m_fragments.push_back(*S);
  }

  // virtuals from Object_Base
  void Surface_Base::chop(const Vector& N)
  {
    Vector loc_N=in_coords(N);

    std::vector<Plate>::iterator p;
    for (p=m_fragments.begin(); p!=m_fragments.end(); ++p)
      (*p).chop(loc_N);
  }

  void Surface_Base::slice(const Vector& N)
  {
    Vector loc_N=in_coords(N);

    std::vector<Plate>::iterator p;
    for (p=m_fragments.begin(); p!=m_fragments.end(); ++p)
      (*p).slice(loc_N);
  }

  void Surface_Base::contribute(std::vector<Shard>& S) const
  {
    std::vector<Plate>::const_iterator p;
    for (p=m_fragments.begin(); p!=m_fragments.end(); ++p)
      {
	//	Shard copy((*p)->clone());
	Plate* copy = (*p).coord_rep(m_orient, m_scale);

	/* TO DO: Copy our style to Plate
	copy->line_color(m_line_color);
	copy->line_width(m_line_width);
	copy->fill_color(m_fill_color);
	copy->back_color(m_back_color);
	copy->shine(m_shine);
	*/
	//	S.push_back(copy->coord_rep(m_orient, m_scale));
	S.push_back(Shard(copy));
      }
  }


  /*
  void Surface_Base::line_color(const Color& col, bool override)
  {
    if (override || m_line_color.is_unset())
      m_line_color = col;
  }

  void Surface_Base::line_width(const Length& len, bool override)
  {
    if (override || m_line_width.is_unset())
      m_line_width = len;
  }

  void Surface_Base::fill_color(const Color& col, bool override)
  {
    if (override || m_fill_color.is_unset())
      m_fill_color = col;
  }

  void Surface_Base::back_color(const Color& col, bool override)
  {
    if (override || m_back_color.is_unset())
      m_back_color = col;
  }

  void Surface_Base::shine(const double r, bool override)
  {
    if (override)
      m_shine = r;
  }

  void Surface_Base::set_defaults(const Color& line_c, const Length& line_w,
				  const Color& fill_c, const Color& back_c,
				  const double shine, const double alpha,
				  bool override)
  {
    if (override || m_line_color.is_unset())
      m_line_color = line_c;

    if (override || m_line_width.is_unset())
      m_line_width = line_w;

    if (override || m_fill_color.is_unset())
      m_fill_color = fill_c;

    if (override || m_back_color.is_unset())
      m_back_color = back_c;

    if (override)
      {
	m_shine = shine;
	m_alpha = alpha;
      }
  }

  */

} // end of namespace

