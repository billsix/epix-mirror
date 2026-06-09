/* 
 * angle_units.cc -- Implementation of ePiX's angular units
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.22
 * Last Change: September 24, 2007
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

#include "constants.h"
#include "angle_units.h"

namespace ePiX {

  angle_state::angle_state()
    : m_rad_per_unit(1.0),
      m_unit_per_deg(PI_180),
      m_rev_per_unit(1.0/TWO_PI) { }

  void angle_state::set_radians_mode()
  {
    m_rad_per_unit = 1.0;
    m_unit_per_deg = PI_180;
    m_rev_per_unit = 1.0/TWO_PI;
  }

  void angle_state::set_degrees_mode()
  {
    m_rad_per_unit = PI_180;
    m_unit_per_deg = 1.0;
    m_rev_per_unit = 1.0/360;
  }

  void angle_state::set_revolutions_mode()
  {
    m_rad_per_unit = TWO_PI;
    m_unit_per_deg = 1.0/360;
    m_rev_per_unit = 1.0;
  }


  double angle_state::to_radians(double t)
  {
    return t *= m_rad_per_unit;
  }

  // result lies in (-180, 180]
  double angle_state::to_degrees(double t)
  {
    t *= m_rev_per_unit;

    t -= 0.5;     // shift half a turn
    t -= ceil(t); // map to (-1,0]
    t += 0.5;     // shift to (-0.5, 0.5]

    return t *= 360.0;
  }

  double angle_state::from_degrees(double t)
  {
    return t *= m_unit_per_deg;
  }

  angle_state& the_angle_style()
  {
    static angle_state* the_angle_state(new angle_state());
    return *the_angle_state;
  }
} // end of namespace
