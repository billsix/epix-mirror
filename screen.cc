/* 
 * screen.cc -- ePiX::screen_data class wrapper (for user interface)
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.18
 * Last Change: September 16, 2007
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
#include <string>
#include <sstream>

#include "triples.h"
#include "pairs.h"
#include "affine.h"

#include "Color.h"

#include "mask_ellipse.h"
#include "mask_rectangle.h"
#include "mask_diamond.h"

#include "screen_data.h"
#include "screen.h"

namespace ePiX {

  // N.B. Our interface handles only P, not pair.
  // Our implementation works only with pair, not P.

  P __epix_pair2P(const pair& arg)
  {
    return P(arg.x1(), arg.x2(), 0);
  }

  screen::screen() { }

  screen::screen(const P& arg1, const P& arg2)
    : m_screen(new screen_data(pair(arg1.x1(), arg1.x2()),
			       pair(arg2.x1(), arg2.x2()))) { }


  screen::screen(const screen& sc)
    : m_screen(sc.m_screen->clone()) { }

  screen& screen::operator= (const screen& sc)
  {
    if (this != &sc)
      {
	screen_data* tmp(sc.m_screen->clone());

	delete m_screen;
	m_screen = tmp;
      }

    return *this;
  }

  screen::~screen()
  {
    delete m_screen;
  }


  // Set mask; one named function for each type of screen_mask
  screen& screen::crop_mask_rectangle(const P& sw, const P& ne)
  {
    const pair BL(sw.x1(), sw.x2());
    const pair TR(ne.x1(), ne.x2());

    m_screen->set_mask(mask_rectangle(BL, TR));
    return *this;
  }

  screen& screen::crop_mask_ellipse(const P& sw, const P& ne)
  {
    const pair BL(sw.x1(), sw.x2());
    const pair TR(ne.x1(), ne.x2());

    m_screen->set_mask(mask_ellipse(BL, TR));
    return *this;
  }

  screen& screen::crop_mask_diamond(const P& sw, const P& ne)
  {
    const pair BL(sw.x1(), sw.x2());
    const pair TR(ne.x1(), ne.x2());

    m_screen->set_mask(mask_diamond(BL, TR));
    return *this;
  }

  // masks' cardinal points lie on circumrectangle, corners may not
  screen& screen::crop_mask_rectangle()
  {
    const pair BL((m_screen->l()).x1(), (m_screen->b()).x2());
    const pair TR((m_screen->r()).x1(), (m_screen->t()).x2());

    m_screen->set_mask(mask_rectangle(BL, TR));
    return *this;
  }

  screen& screen::crop_mask_ellipse()
  {
    const pair BL((m_screen->l()).x1(), (m_screen->b()).x2());
    const pair TR((m_screen->r()).x1(), (m_screen->t()).x2());

    m_screen->set_mask(mask_ellipse(BL, TR));
    return *this;
  }

  screen& screen::crop_mask_diamond()
  {
    const pair BL((m_screen->l()).x1(), (m_screen->b()).x2());
    const pair TR((m_screen->r()).x1(), (m_screen->t()).x2());

    m_screen->set_mask(mask_diamond(BL, TR));
    return *this;
  }


  screen screen::extract(const P& arg1, const P& arg2) const
  {
    screen value(arg1, arg2);

    value.m_screen
      = new screen_data(m_screen->extract(pair(arg1.x1(), arg1.x2()),
					  pair(arg2.x1(), arg2.x2())));
    return value;
  }

  screen screen::extract_diamond(const P& arg1, const P& arg2) const
  {
    screen value(arg1, arg2);

    value.m_screen
      = new screen_data(m_screen->extract_diamond(pair(arg1.x1(), arg1.x2()),
						  pair(arg2.x1(), arg2.x2())));
    return value;
  }

  screen screen::extract_ellipse(const P& arg1, const P& arg2) const
  {
    screen value(arg1, arg2);

    value.m_screen
      = new screen_data(m_screen->extract_ellipse(pair(arg1.x1(), arg1.x2()),
						  pair(arg2.x1(), arg2.x2())));
    return value;
  }


  screen& screen::set_crop(bool arg)
  {
    m_screen->set_crop(arg);
    return *this;
  }

  // crop contents to current mask
  screen& screen::crop()
  {
    m_screen->crop();
    return *this;
  }


  // inset and import
  screen& screen::paste(const screen& child, const P& sw, const P& ne)
  {
    m_screen->paste(*(child.m_screen), mask_rectangle(pair(sw.x1(), sw.x2()),
						      pair(ne.x1(), ne.x2())));
    return *this;
  }

  screen& screen::paste(const screen& child)
  {
    m_screen->paste(*(child.m_screen));
    return *this;
  }

  screen& screen::clear()
  {
    m_screen->clear();
    return *this;
  }

  screen& screen::border(const Color& col, double len)
  {
    std::stringstream buf;
    buf << len << "pt";
    m_screen->border(col, buf.str());
    return *this;
  }

  screen& screen::border(const Color& col, const std::string& len)
  {
    m_screen->border(col, len);
    return *this;
  }

  screen& screen::backing(const Color& col)
  {
    m_screen->backing(col);
    return *this;
  }


  screen& screen::border()
  {
    m_screen->border();
    return *this;
  }

  screen& screen::backing()
  {
    m_screen->backing();
    return *this;
  }

  // affine operations on contents (not border, backing)
  screen& screen::apply(const affine& af)
  {
    m_screen->apply(af);
    return *this;
  }

  screen& screen::shift(const P& arg)
  {
    m_screen->shift(pair(arg.x1(), arg.x2()));
    return *this;
  }

  // rotate by theta about ctr
  screen& screen::rotate(double theta, const P& ctr)
  {
    m_screen->rotate(theta, pair(ctr.x1(), ctr.x2()));
    return *this;
  }

  // reflect in angle-theta line through ctr
  screen& screen::reflect(double theta, const P& ctr)
  {
    m_screen->reflect(theta, pair(ctr.x1(), ctr.x2()));
    return *this;
  }

    // scale coord direction(s) fixing ctr
  screen& screen::h_scale(double sc, const P& ctr)
  {
    m_screen->h_scale(sc, pair(ctr.x1(), ctr.x2()));
    return *this;
  }

  screen& screen::v_scale(double sc, const P& ctr)
  {
    m_screen->v_scale(sc, pair(ctr.x1(), ctr.x2()));
    return *this;
  }

  screen& screen::scale(double sc, const P& ctr)
  {
    m_screen->scale(sc, pair(ctr.x1(), ctr.x2()));
    return *this;
  }

  // shear, fixing ctr, e.g. h_shear(sc) : (x,y) -> (x+sc*y, y)
  screen& screen::h_shear(double sc, const P& ctr)
  {
    m_screen->h_shear(sc, pair(ctr.x1(), ctr.x2()));
    return *this;
  }

  screen& screen::v_shear(double sc, const P& ctr)
  {
    m_screen->v_shear(sc, pair(ctr.x1(), ctr.x2()));
    return *this;
  }

  P screen::tr() const
  {
    return __epix_pair2P(m_screen->tr());
  }

  P screen::tl() const
  {
    return __epix_pair2P(m_screen->tl());
  }

  P screen::bl() const
  {
    return __epix_pair2P(m_screen->bl());
  }

  P screen::br() const
  {
    return __epix_pair2P(m_screen->br());
  }


  P  screen::t() const
  {
    return __epix_pair2P(m_screen->t());
  }

  P  screen::b() const
  {
    return __epix_pair2P(m_screen->b());
  }

  P  screen::l() const
  {
    return __epix_pair2P(m_screen->l());
  }

  P  screen::r() const
  {
    return __epix_pair2P(m_screen->r());
  }

  P  screen::c() const
  {
    return __epix_pair2P(m_screen->c());
  }


  double screen::h_min()  const
  {
    return m_screen->h_min();
  }

  double screen::h_max()  const
  {
    return m_screen->h_max();
  }

  double screen::h_size() const
  {
    return m_screen->h_size();
  }

  double screen::h_avg()  const
  {
    return m_screen->h_avg();
  }


  double screen::v_min()  const
  {
    return m_screen->v_min();
  }

  double screen::v_max()  const
  {
    return m_screen->v_max();
  }

  double screen::v_size() const
  {
    return m_screen->v_size();
  }

  double screen::v_avg()  const
  {
    return m_screen->v_avg();
  }
} // end of namespace
