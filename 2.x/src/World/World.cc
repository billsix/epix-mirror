/* 
 *  World.cc -- epix2::World class
 *
 * This file is part of ePiX, a program for creating high-quality 
 * figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: February 12, 2006
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005
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

#include "Constants.h"
#include "Functions.h"

#include "Camera.h"
#include "Screen.h"
#include "Object_Rep.h"
#include "Object.h"
#include "World.h"

namespace ePiX2 {

  /*
  World::World(void)
    : m_line_color(Black()), m_line_width(Length(ePiX2::PLAIN_WIDTH)),
      m_base_color(Black()), m_base_width(Length(ePiX2::PLAIN_WIDTH)),
      m_fill_color(White()), m_back_color(White()), m_shine(1), m_alpha(1),
      m_text_color(Black()), m_tfill_color(White()), m_edge_color(White()),
      m_text_pad(Length("3pt")), m_text_bord(Length("0pt")),
      m_text_align(c), m_text_font(ROMAN), m_text_angle(0),
      m_fog(0), m_layers(0), m_updated(false) { }
  */

  // modify scenery
  World& World::operator<< (const Object& obj)
  {
    Object_Base* copy = obj->clone();

    copy->update_by(m_path_style);
    copy->update_by(m_surface_style);
    copy->update_by(m_text_style);

    m_scenery.push_back(Object(copy));

    m_updated=false;

    return *this;
  }

  World& World::operator<<(const World& W)
  {
    std::list<Object>::const_iterator curr;
    for (curr=W.m_scenery.begin(); curr!=W.m_scenery.end(); ++curr)
      (*this) << (*curr);

    return *this;
  }


  void World::clear(void)
  {
    m_layers->clear();
    m_updated=false;
  }


  void World::clip(const Vector& N)
  {
    std::list<Object>::iterator curr;
    for (curr=m_scenery.begin(); curr!=m_scenery.end(); ++curr)
      (*curr)->chop(N);

    m_updated=false;
  }

  void World::slice(const Vector& N)
  {
    std::list<Object>::iterator curr;
    for (curr=m_scenery.begin(); curr!=m_scenery.end(); ++curr)
      (*curr)->slice(N);

    m_updated=false;
  }

  // private
  void World::build_tree(void)
  {
    if (m_updated)
      return;

    // collect shards
    std::vector<Shard> all_frags;
    std::list<Object>::const_iterator Op;

    for (Op=m_scenery.begin(); Op!=m_scenery.end(); ++Op)
      (*Op)->contribute(all_frags);

    // TO DO: Why is sort segfaulting...??
    // decreasing order of area
    //    sort(all_frags.begin(), all_frags.end());

    // initialize tree
    if (m_layers != 0)
      m_layers->clear();

    m_layers = new Layer;

    // and build
    m_layers->add(all_frags);
    m_updated=true;
  }


  // apply to current Objects
  World& World::paint_line(const Color& col, const Length& len)
  {
    std::list<Object>::iterator obj;
    for (obj = m_scenery.begin(); obj != m_scenery.end(); ++obj)
      (*obj)->set_line(col, len, false);

    return *this;
  }


  World& World::paint_base(const Color& col, const Length& len)
  {
    std::list<Object>::iterator obj;
    for (obj = m_scenery.begin(); obj != m_scenery.end(); ++obj)
      (*obj)->set_base(col, len, false);

    return *this;
  }


  World& World::paint_fill(const Color& fill, const Color& back,
			   const double shine, const double alpha)
  {
    std::list<Object>::iterator obj;
    for (obj = m_scenery.begin(); obj != m_scenery.end(); ++obj)
      (*obj)->set_fill(fill, back, false, shine, alpha);

    return *this;
  }


  // set default attributes
  World& World::default_line(const Color& col, const Length& len)
  {
    m_path_style.m_line_color = col;
    m_path_style.m_line_width = len;

    m_surface_style.m_line_color = col;
    m_surface_style.m_line_width = len;

    return *this;
  }

  World& World::default_base(const Color& col, const Length& len)
  {
    m_path_style.m_base_color = col;
    m_path_style.m_base_width = len;

    return *this;
  }

  World& World::default_fill(const Color& col, const Color& bcol,
			     const double shine, const double alpha)
  {
    m_surface_style.m_fill_color = col;
    m_surface_style.m_back_color = bcol;
    m_surface_style.m_shine = shine;
    m_surface_style.m_alpha = alpha;

    return *this;
  }

  World& World::default_text(const Color& text, const Color& fill,
			     const Color& edge, 
			     const Length& pad, const Length& bord)
  {
    m_text_style.m_text = text;
    m_text_style.m_fill = fill;
    m_text_style.m_edge = edge;

    m_text_style.m_pad  = pad;
    m_text_style.m_bord = bord;

    return *this;
  }

  /*
  void World::fill_color(const Color& col)
  {
    m_surface_style.m_fill_color = col;
  }

  void World::back_color(const Color& col)
  {
    m_surface_style.m_back_color = col;
  }

  void World::shine(const double r)
  {
    m_path_style.m_shine = r;
    m_surface_style.m_shine = r;
  }

  void World::alpha(const double r)
  {
    m_path_style.m_alpha = r;
    m_surface_style.m_alpha = r;
  }
  */

  void World::fog(const double k)
  {
    m_fog = k;
  }

  // map scenery (private)
  void World::shoot(Screen& S, const Camera& cam, 
		    const Shard_Base::photo_type T)
  {
    if (!m_updated)
      build_tree();

    m_layers->shoot(S, cam, m_fog, T);
  }

  // public wrappers for shoot()
  void World::photo(Screen& S, const Camera& cam)
  {
    shoot(S, cam, Shard_Base::PHOTO);
  }
  void World::flash(Screen& S, const Camera& cam)
  {
    shoot(S, cam, Shard_Base::FLASH);
  }
  void World::trace(Screen& S, const Camera& cam)
  {
    shoot(S, cam, Shard_Base::TRACE);
  }
  void World::x_ray(Screen& S, const Camera& cam)
  {
    shoot(S, cam, Shard_Base::X_RAY);
  }

  void World::photo(Picture& S, const Camera& cam)
  {
    shoot(S.screen(), cam, Shard_Base::PHOTO);
  }
  void World::flash(Picture& S, const Camera& cam)
  {
    shoot(S.screen(), cam, Shard_Base::FLASH);
  }
  void World::trace(Picture& S, const Camera& cam)
  {
    shoot(S.screen(), cam, Shard_Base::TRACE);
  }
  void World::x_ray(Picture& S, const Camera& cam)
  {
    shoot(S.screen(), cam, Shard_Base::X_RAY);
  }
} // end of namespace
