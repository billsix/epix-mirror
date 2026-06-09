/* 
 * path_style.cc -- ePiX's class for path styles.
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.18
 * Last Change: September 15, 2007
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
#include <cmath>
#include <string>
#include <vector>

#include "constants.h"

#include "functions.h" // for snip_to
#include "utils.h" // for get_chars
#include "path_style.h"

namespace ePiX {

  // true if arg is a string of ch
  static bool __epix_all_one_char(const std::string& arg, const char ch)
  {
    return arg.find_first_not_of(ch) == std::string::npos;
  }

  // convert string of dashes, spaces, and periods to a vector of breakpoints
  static std::vector<double> __epix_get_breakpts(const std::string& pattern)
  {
    std::vector<double> value;

    // Assume solid on anomalous input
    if (pattern.size() == 0
	|| __epix_all_one_char(pattern, '-')
	|| __epix_all_one_char(pattern, ' '))
      {
	value.push_back(0);
	value.push_back(1);

	return value;
      }

    // else
    const unsigned int sz(pattern.size()); // not zero
    const double dt(1.0/sz);

    for (unsigned int i=0; i<sz; ++i)
      {
	if (pattern.at(i) == '-')
	  {
	    // previous element was a dash; extend instead of adding breaks
	    if (0 < value.size()
		&& fabs(value.at(value.size()-1) - i*dt) < 0.25*dt)
	      value.at(value.size()-1) = (i+1)*dt;

	    else
	      {
		value.push_back(i*dt);
		value.push_back((i+1)*dt);
	      }
	  }

	else if (pattern.at(i) == '.')
	  {
	    value.push_back((i+0.5)*dt);
	    value.push_back((i+0.5)*dt); // same value twice
	  }
	// spaces have no breakpoints, just increment i
      }

    return value;
  } // end of __epix_get_breakpts


  path_state::path_state()
    : m_separation(AVG_SEP), m_natural_length(1), m_solid(true)
  {
    m_breakpoints.push_back(0);
    m_breakpoints.push_back(1);
  }

  path_state::path_state(std::string arg)
    : m_separation(AVG_SEP), m_solid(false)
  {
    // strip out everything but dash, space, period.
    const std::string pattern(get_chars(arg, "- ."));
    m_breakpoints = __epix_get_breakpts(pattern); // ok even if pattern=""

    m_natural_length = pattern.size();

    if (m_breakpoints.at(0) == 0 && m_breakpoints.at(1) == 1)
      {
	m_solid = true;
	m_natural_length = 1;
      }
  }

  path_state& path_state::separation(double len)
  {
    if (0 < len)
      m_separation = snip_to(len, MIN_SEP, MAX_SEP);

    else
      m_separation = AVG_SEP;

    return *this;
  }

  double path_state::separation() const
  {
    return m_separation;
  }

  unsigned int path_state::natural_length() const
  {
    return m_natural_length;
  }

  std::vector<double> path_state::breakpts() const
  {
    return m_breakpoints;
  }

  bool path_state::is_solid() const
  {
    return m_solid;
  }


  path_state& the_path_style()
  {
    static auto* the_path_state(new path_state());
    return *the_path_state;
  }
} // end of namespace
