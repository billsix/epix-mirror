/*
 * active_screen.cc -- ePiX screen_broker class
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.11
 * Last Change: August 15, 2007
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
#include "screen.h"
#include "picture_data.h"
#include "picture.h"
#include "active_screen.h"

namespace ePiX {

screen_broker::screen_broker() {
  m_screens.push_front(&the_picture().the_canvas);
}

screen* screen_broker::active() const { return *(m_screens.begin()); }

void screen_broker::push(screen& scr) { m_screens.push_front(&scr); }

screen* screen_broker::pop() {
  std::list<screen*>::const_iterator sp(m_screens.begin());

  screen* val(*sp);

  if (++sp != m_screens.end()) m_screens.pop_front();

  return val;
}

// global functions
screen_broker& the_screen_stack() {
  static auto* the_screen_broker(new screen_broker());
  return *the_screen_broker;
}

screen* active_screen() { return the_screen_stack().active(); }
}  // namespace ePiX
