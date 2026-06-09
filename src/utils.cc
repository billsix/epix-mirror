/*
 * utils.cc -- global utility functions for ePiX
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2.5
 * Last Change: June 18, 2008
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008
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

#include <string>
#include <cmath>
#include <ctime>

#include "constants.h"
#include "utils.h"

namespace ePiX {

  const std::string epix_version()
  {
    return "1.2.22";
  }

  double truncate(double arg)
  {
    return fabs(arg) < 0.0001 ? 0 : arg; // 10^-4, avoid exponential notation
  }

  std::string date_and_time()
    {
      ////// N.B.: Revise this if time format string changes!
      const int max_len = 31;
      char buf[max_len];
      time_t t=time(nullptr);

      // strftime(buf, max_len, "%c", localtime(&t));

      // Sat Jul  1 23:25:39 2006 (GMT)
      strftime(buf, max_len, "%a %b %d %X %Y (%Z)", gmtime(&t));
      return std::string(buf);
    }


  // remove everything from arg except chars in srch
  std::string get_chars(std::string arg, const std::string& srch)
  {
    std::string::size_type curr(0);
    std::string::size_type next(arg.find_first_of(srch));

    while (next != std::string::npos)
      {
	arg.erase(curr, next-curr);

	curr = arg.find_first_not_of(srch);
	next = arg.find_first_of(srch, curr);
      }

    if (next != curr)
      arg.erase(curr, next);

    return arg;
  }

  // add linebreaks before opening parentheses to make lines at most
  // FILE_WIDTH characters long; return reference to arg
  std::string break_lines(std::string arg, const std::string& path_break)
  {
    std::string value; // append here; swap with arg at end

    // initialize markers prev, curr, next
    std::string::size_type prev(0); // position of last line break
    std::string::size_type next(arg.find_first_of("("));
    std::string::size_type curr(next);

    unsigned int pbk_sz(path_break.size());

    while (next != std::string::npos)
      {
	// seek next break points
	while (next-prev <= EPIX_FILE_WIDTH - pbk_sz)
	  {
	    curr = next;
	    next = arg.find_first_of("(", ++next);
	  }

	value.append(arg, prev, curr-prev); // append current line
	// Magic number 20 (greater than width of a printed pair)
	if (next - prev <= 20+EPIX_FILE_WIDTH) // not at end of data
	  value += path_break;              // and formatting string
	prev = curr;                        // update front of line
      }

    // get remaining characters
    value.append(arg, prev, next-prev);

    swap(arg, value);
    return arg;
  }
} // end of namespace
