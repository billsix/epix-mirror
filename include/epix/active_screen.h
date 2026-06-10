/*
 * active_screen.h -- ePiX screen_broker class
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

/*
 * A screen_broker is a simple stack of screen*, permitting client code
 * to activate a screen then restore the state later. No attempt is made
 * to handle concurrent threads, but the global "deactivate" function
 * in picture.h prints a warning if its argument is not active.
 */
#ifndef EPIX_ACTIVE_SCREEN
#define EPIX_ACTIVE_SCREEN

#include <list>

namespace ePiX {

class screen;

class screen_broker {
 public:
  screen_broker();

  // no need for destructor; we don't manage resources

  screen* active() const;
  void push(screen&);
  screen* pop();

 private:
  std::list<screen*> m_screens;

  // forbid copying
  screen_broker(const screen_broker&) = delete;
  screen_broker& operator=(const screen_broker&) = delete;
};

screen_broker& the_screen_stack();
screen* active_screen();
}  // namespace ePiX

#endif /* EPIX_ACTIVE_SCREEN */
