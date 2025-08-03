/* 
 * state_data.cc -- Implementation of ePiX's current drawing state
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.4
 * Last Change: June 17, 2007
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
 * This file defines (wrapper functions for) static objects that hold
 * the current drawing state
 */

#include <list>

#include "constants.h"
#include "screen_mask.h"
#include "mask_ellipse.h"
#include "mask_rectangle.h"

#include "cropping.h"

namespace ePiX {

  crop_state::crop_state(const crop_state& cs)
    : crop_state_mask(cs.crop_state_mask->clone()) { }

  crop_state& crop_state::operator= (const crop_state& cs)
  {
    if (this != &cs)
      {
	screen_mask* tmp(cs.crop_state_mask->clone());

	delete crop_state_mask;
	crop_state_mask=tmp;
      }

    return *this;
  }

  crop_state::~crop_state()
  {
    delete crop_state_mask;
  }

  std::list<edge2d>& crop_state::crop_path(std::list<edge2d>& L) const
  {
    return crop_state_mask->crop_path(L);
  }

  std::list<edge2d>& crop_state::crop_loop(std::list<edge2d>& L) const
  {
    return crop_state_mask->crop_loop(L);
  }

  crop_state& the_crop_box()
  {
    const pair M(EPIX_INFTY, EPIX_INFTY);
    static crop_state* current_crop_box(new crop_state(mask_rectangle(-M,M)));
    return *current_crop_box;
  }
} // end of namespace
