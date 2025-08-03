/* 
 * angle_units.h -- Implementation of ePiX's angular units
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.22
 * Last Change: September 24, 2007
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

/*
 * This file contains:
 *
 *  [] angle_state
 *     Angular mode (radians, degrees, revolutions)
 *
 * The class in this file implements backend for user-level global
 * functions defined in state.h. This header is not part of epix.h.
 */
#ifndef EPIX_ANGLE_UNITS
#define EPIX_ANGLE_UNITS

#include <cmath>

namespace ePiX {

  class angle_state {
  public:
    angle_state();

    void set_radians_mode();
    void set_degrees_mode();
    void set_revolutions_mode();

    // convert to radians, for angle-sensitive trig functions
    double to_radians(double t);

    // current angle units to/from degrees, for label_style
    double to_degrees(double t);
    double from_degrees(double t);

  private:
    // store state redundantly; small overhead, saves conversion
    double m_rad_per_unit;
    double m_unit_per_deg;
    double m_rev_per_unit;
  }; // end of class angle_state

  angle_state& the_angle_style();

} // end of namespace

#endif /* EPIX_ANGLE_UNITS */
