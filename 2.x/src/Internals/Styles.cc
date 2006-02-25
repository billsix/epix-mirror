/* 
 * Styles.cc -- ePiX2 style data
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

#include "Camera.h"
#include "Color.h"
#include "Styles.h"

namespace ePiX2 {

  void Path_Style::update_by(const Path_Style& arg)
  {
    if (unbd_lc)
      m_line_color = arg.m_line_color;

    if (unbd_bc)
      m_base_color = arg.m_base_color;


    if (unbd_lw)
      m_line_width = arg.m_line_width;

    if (unbd_bw)
      m_base_width = arg.m_base_width;
  }

  void Path_Style::filter_by(const Camera& cam)
  {
    m_line_color = cam(m_line_color);
    m_base_color = cam(m_base_color);
  }


  void Path_Style::set_line(const Color& col, const Length& len, bool bind)
  {
    m_line_color = col;
    m_line_width = len;

    if (bind)
      {
	unbd_lc=false;
	unbd_lw=false;
      }
  }
    
  void Path_Style::set_base(const Color& col, const Length& len, bool bind)
  {
    m_base_color = col;
    m_base_width = len;

    if (bind)
      {
	unbd_bc=false;
	unbd_bw=false;
      }
  }


  void Surface_Style::update_by(const Surface_Style& arg)
  {
    if (unbd_lc)
      m_line_color = arg.m_line_color;

    if (unbd_lw)
      m_line_width = arg.m_line_width;


    if (unbd_fc)
      m_fill_color = arg.m_fill_color;

    if (unbd_bc)
      m_back_color = arg.m_back_color;


    if (unbd_shine)
      m_shine = arg.m_shine;

    if (unbd_alpha)
      m_alpha = arg.m_alpha;
  }

  void Surface_Style::filter_by(const Camera& cam)
  {
    m_line_color = cam(m_line_color);
    m_fill_color = cam(m_fill_color);
    m_back_color = cam(m_back_color);
  }


  void Surface_Style::set_line(const Color& col, const Length& len, bool bind)
  {
    m_line_color = col;
    m_line_width = len;

    if (bind)
      {
	unbd_lc=false;
	unbd_lw=false;
      }
  }

  void Surface_Style::set_fill(const Color& fill, const Color& back, bool bind,
			       const double shine, const double alpha)
  {
    m_fill_color = fill;
    m_back_color = back;

    m_shine = shine;
    m_alpha = alpha;

    if (bind)
      {
	unbd_fc=false;
	unbd_bc=false;
	unbd_shine=false;
	unbd_alpha=false;
      }
  }

  void Surface_Style::set_shine(const double shine, bool bind)
  {
    m_shine = shine;
    if (bind)
      unbd_shine=false;
  }


  void Text_Style::update_by(const Text_Style& arg)
  {
    if (unbd_text)
      m_text = arg.m_text;

    if (unbd_fill)
      m_fill = arg.m_fill;

    if (unbd_edge)
      m_edge = arg.m_edge;


    if (unbd_pad)
      m_pad = arg.m_pad;

    if (unbd_bord)
      m_bord = arg.m_bord;


    if (unbd_align)
      m_align = arg.m_align;

    if (unbd_font)
      m_font = arg.m_font;

    if (unbd_angle)
      m_angle = arg.m_angle;
  }

  void Text_Style::filter_by(const Camera& cam)
  {
    m_text = cam(m_text);
    m_fill = cam(m_fill);
    m_edge = cam(m_edge);
  }

} /* end of namespace */
