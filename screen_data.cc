/* 
 * screen_data.cc -- ePiX::screen class implementation
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.2.1
 * Last Change: September 28, 2007
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
#include <list>
#include <algorithm>
#include <sstream>

#include "camera.h"

#include "pairs.h"

#include "functions.h"

#include "Color.h"

#include "utils.h"

#include "tile.h"

#include "pen_line.h"
#include "pen_fill.h"

#include "affine.h"
#include "mask_ellipse.h"
#include "mask_rectangle.h"
#include "mask_diamond.h"

#include "format.h"

// #include "screen_mask.h"
#include "screen_data.h"

namespace ePiX {

  typedef std::list<tile*>::const_iterator Tp;

  // constructor
  screen_data::screen_data(const pair& arg1, const pair& arg2)
    : m_bl(pair(min(arg1.x1(), arg2.x1()), min(arg1.x2(), arg2.x2()))),
      m_tr(pair(max(arg1.x1(), arg2.x1()), max(arg1.x2(), arg2.x2()))),
      m_mask(new mask_rectangle(m_bl, m_tr)),
      m_border(new pen_line()),
      m_backing(new pen_fill()),
      m_crop(false) { }

  // copy constructor
  screen_data::screen_data(const screen_data& scr)
    : m_bl(scr.m_bl),
      m_tr(scr.m_tr), 
      m_mask(scr.m_mask->clone()),
      m_border(scr.m_border->clone()),
      m_backing(scr.m_backing->clone()),
      m_crop(scr.m_crop)
  {
    for(Tp p=scr.m_tiles.begin(); p!= scr.m_tiles.end(); ++p)
      add_tile(*p);
  }

  // copy assignment
  screen_data& screen_data::operator= (const screen_data& scr)
  {
    if (this != &scr)
      {
	// copy scr's tiles before freeing ours
	std::list<tile*> tmp;
	for(Tp p=scr.m_tiles.begin(); p!= scr.m_tiles.end(); ++p)
	  add_tile(*p);

	screen_mask* tmp_mask(scr.m_mask->clone());

	pen_line* tmp_border(scr.m_border->clone());
	pen_fill* tmp_backing(scr.m_backing->clone());

	// okay, safe to muck with our data
	m_bl = scr.m_bl;
	m_tr = scr.m_tr;

	delete m_mask;
	m_mask = tmp_mask;

	delete m_border;
	m_border  = tmp_border;

	delete m_backing;
	m_backing = tmp_backing;

	for (Tp p=m_tiles.begin(); p!=m_tiles.end(); ++p)
	  delete *p;

	swap(m_tiles, tmp);

	m_crop = scr.m_crop;
      }
    return *this;
  } // end of copy assignment


  screen_data::~screen_data()
  {
    delete m_mask;
    delete m_border;
    delete m_backing;

    for(Tp p=m_tiles.begin(); p!= m_tiles.end(); ++p)
      delete *p;
  }

  screen_data* screen_data::clone() const
  {
    return new screen_data(*this);
  }


  // superimpose a layer by affine scaling
  // map child to inset (inset defaults to m_mask)
  screen_data& screen_data::paste(const screen_data& child,
				  const screen_mask& inset)
  {
    if (this != &child)
      {
	const double h_sc(inset.h_size()/child.h_size());
	const double v_sc(inset.v_size()/child.v_size());

	const pair tmp00(inset.bl() - (child.bl() & pair(h_sc, v_sc)));

	affine af(tmp00 + pair(h_sc, 0), tmp00 + pair(0, v_sc), tmp00);

	// backing
	if (!child.m_backing->is_empty())
	  {
	    // child is const, so must copy tile
	    pen_fill* temp(child.m_backing->clone());
	    add_tile(temp->map_by(af));
	  }

	// tiles
	for(Tp p=child.m_tiles.begin(); p!= child.m_tiles.end(); ++p)
	  {
	    tile* temp((*p)->clone());
	    add_tile(temp->map_by(af));
	  }

	// border
	if (!child.m_border->is_empty())
	  {
	    pen_line* temp(child.m_border->clone());
	    add_tile(temp->map_by(af));
	  }
      }

    return *this;
  } // end of paste(child, inset)


  screen_data& screen_data::paste(const screen_data& child)
  {
    if (this != &child)
      {
	const double h_sc(h_size()/child.h_size());
	const double v_sc(v_size()/child.v_size());

	const pair tmp00(bl() - (child.bl() & pair(h_sc, v_sc)));

	affine af(tmp00 + pair(h_sc, 0), tmp00 + pair(0, v_sc), tmp00);

	// backing
	if (!child.m_backing->is_empty())
	  {
	    // child is const, so must copy tile
	    pen_fill* temp(child.m_backing->clone());

	    add_tile(temp->map_by(af));
	  }

	// tiles
	for(Tp p=child.m_tiles.begin(); p!= child.m_tiles.end(); ++p)
	  {
	    tile* temp((*p)->clone());
	    add_tile(temp->map_by(af));
	  }

	// border
	if (!child.m_border->is_empty())
	  {
	    pen_line* temp(child.m_border->clone());
	    add_tile(temp->map_by(af));
	  }
      }

    return *this;
  }

  screen_data& screen_data::clear()
  {
    delete m_border;
    m_border = new pen_line();

    m_tiles.clear();

    delete m_backing;
    m_backing = new pen_fill();

    m_crop = false;
    return *this;
  }

  screen_data& screen_data::crop_to(const screen_mask& mask)
  {
    std::list<tile*> visibles;

    if (!m_border->is_empty())
      m_border->crop_to(mask);

    if (!m_backing->is_empty())
      m_backing->crop_to(mask);

    for (Tp curr=m_tiles.begin(); curr!=m_tiles.end(); ++curr)
      {
	(*curr)->crop_to(mask);

	if (!(*curr)->is_empty())
	  visibles.push_back(*curr);
      }

    swap(m_tiles, visibles);
    return *this;
  }

  screen_data& screen_data::crop()
  {
    return crop_to(*m_mask);
  }


  screen_data& screen_data::set_mask(const screen_mask& mask)
  {
    delete m_mask;
    m_mask = mask.clone();

    return *this;
  }

  screen_data& screen_data::set_crop(bool arg)
  {
    m_crop = arg;
    return *this;
  }

  screen_data screen_data::extract(const pair& arg1, const pair& arg2) const
  {
    screen_data value(arg1, arg2);
    value.set_crop(true);

    for (Tp curr=m_tiles.begin(); curr!=m_tiles.end(); ++curr)
      value.add_tile(*curr);

    return value;
  }

  screen_data screen_data::extract_diamond(const pair& arg1,
					   const pair& arg2) const
  {
    screen_data value(arg1, arg2);

    value.set_mask(mask_diamond(arg1, arg2)).set_crop(true);

    for (Tp curr=m_tiles.begin(); curr!=m_tiles.end(); ++curr)
      value.add_tile(*curr);

    return value;
  }

  screen_data screen_data::extract_ellipse(const pair& arg1,
					   const pair& arg2) const
  {
    screen_data value(arg1, arg2);

    value.set_mask(mask_ellipse(arg1, arg2)).set_crop(true);

    for (Tp curr=m_tiles.begin(); curr!=m_tiles.end(); ++curr)
      value.add_tile(*curr);

    return value;
  }


  screen_data& screen_data::border(const Color& col, const std::string& len)
  {
    delete m_border;
    m_border = new pen_line(m_mask->border(cam()(col), len));
    m_crop = true;

    return *this;
  }


  screen_data& screen_data::backing(const Color& col)
  {
    delete m_backing;
    m_backing = new pen_fill(m_mask->backing(cam()(col)));
    m_crop = true;

    return *this;
  }


  screen_data& screen_data::border()
  {
    delete m_border;
    m_border = new pen_line(m_mask->border());
    m_crop = true;

    return *this;
  }


  screen_data& screen_data::backing()
  {
    delete m_backing;
    m_backing = new pen_fill(m_mask->backing());
    m_crop = true;

    return *this;
  }

  // screen cropping done in these two functions
  void screen_data::add_tile(const tile& T)
  {
    tile* tmp(T.clone());
    if (m_crop)
      tmp->crop_to(*m_mask);

    if (!tmp->is_empty())
      m_tiles.push_back(tmp);
  }

  void screen_data::add_tile(tile* Tp)
  {
    tile* tmp(Tp->clone());
    if (m_crop)
      tmp->crop_to(*m_mask);

    if (!tmp->is_empty())
      m_tiles.push_back(tmp);
  }


  // affine operations on contents (not border, backing)
  void screen_data::apply(const affine& af)
  {
    for (Tp curr=m_tiles.begin(); curr!=m_tiles.end(); ++curr)
      (*curr)->map_by(af);
  }

  void screen_data::shift(const pair& arg)
  {
    affine af;
    af.shift(arg);
    apply(af);
  }

  // rotate by theta about ctr
  void screen_data::rotate(double theta, const pair& ctr)
  {
    affine af;
    af.rotate(theta, ctr);
    apply(af);
  }

  // reflect in angle-theta line through ctr
  void screen_data::reflect(double theta, const pair& ctr)
  {
    affine af;
    af.reflect(theta, ctr);
    apply(af);
  }

  // scale coord direction(s) fixing ctr
  void screen_data::h_scale(double sc, const pair& ctr)
  {
    affine af;
    af.h_scale(sc, ctr);
    apply(af);
  }

  void screen_data::v_scale(double sc, const pair& ctr)
  {
    affine af;
    af.v_scale(sc, ctr);
    apply(af);
  }

  void screen_data::scale(double sc, const pair& ctr)
  {
    affine af;
    af.scale(sc, ctr);
    apply(af);
  }

  // shear, fixing ctr, e.g. h_shear(sc) : (x,y) -> (x+sc*y, y)
  void screen_data::h_shear(double sc, const pair& ctr)
  {
    affine af;
    af.h_shear(sc, ctr);
    apply(af);
  }

  void screen_data::v_shear(double sc, const pair& ctr)
  {
    affine af;
    af.v_shear(sc, ctr);
    apply(af);
  }


  pair screen_data::tr() const
  {
    return m_tr;
  }

  pair screen_data::tl() const
  {
    return pair(m_bl.x1(), m_tr.x2());
  }

  pair screen_data::bl() const
  {
    return m_bl;
  }

  pair screen_data::br() const
  {
    return pair(m_tr.x1(), m_bl.x2());
  }


  pair screen_data::t() const
  {
    return pair(0.5*(m_bl.x1() + m_tr.x1()), m_tr.x2());
  }

  pair screen_data::b() const
  {
    return pair(0.5*(m_bl.x1() + m_tr.x1()), m_bl.x2());
  }

  pair screen_data::l() const
  {
    return pair(m_bl.x1(), 0.5*(m_bl.x2() + m_tr.x2()));
  }

  pair screen_data::r() const
  {
    return pair(m_tr.x1(), 0.5*(m_bl.x2() + m_tr.x2()));
  }

  pair screen_data::c() const
  {
    return pair(0.5*(m_bl.x1() + m_tr.x1()), 0.5*(m_bl.x2() + m_tr.x2()));
  }


  double screen_data::h_min()  const
  {
    return m_bl.x1();
  }

  double screen_data::h_max()  const
  {
    return m_tr.x1();
  }

  double screen_data::h_size() const
  {
    return m_tr.x1() - m_bl.x1();
  }

  double screen_data::h_avg()  const
  {
    return 0.5*(m_bl.x1() + m_tr.x1());
  }


  double screen_data::v_min()  const
  {
    return m_bl.x2();
  }

  double screen_data::v_max()  const
  {
    return m_tr.x2();
  }

  double screen_data::v_size() const
  {
    return m_tr.x2() - m_bl.x2();
  }

  double screen_data::v_avg()  const
  {
    return 0.5*(m_bl.x2() + m_tr.x2());
  }

  std::string screen_data::print_to(const format& fmt, const length& len,
				    bool wrap) const
  {
    std::ostringstream obuf;

    if (wrap)
      {
	obuf << fmt.start_picture(tr()-bl(), bl());
	fmt.reset_state(); // ensures drawing attribs set properly below
      }

    if (!m_backing->is_empty())
      {
	m_backing->add_to_palette();

	obuf << m_backing->print_to(fmt, len.units());
      }

    for (Tp curr = m_tiles.begin(); curr != m_tiles.end(); ++curr)
      {
	(*curr)->add_to_palette();

	obuf << (*curr)->print_to(fmt, len.units());
      }

    if (!m_border->is_empty())
      {
	m_border->add_to_palette();

	obuf << m_border->print_to(fmt, len.units());
      }

    if (wrap)
      obuf << fmt.end_picture();

    return obuf.str();
  } // end of print_to()

} // end of namespace
