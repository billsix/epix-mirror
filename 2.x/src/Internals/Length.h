/***
 ***  Length.h -- ePiX2::Length class
 ***
 *** This file is part of ePiX, a preprocessor for creating high-quality 
 *** line figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: January 14, 2006
 ***
 *** 
 *** Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 *** Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
 *** Department of Mathematics and Computer Science
 *** College of the Holy Cross
 *** Worcester, MA, 01610-2395, USA
 ***
 ***
 *** ePiX is free software; you can redistribute it and/or modify it
 *** under the terms of the GNU General Public License as published by
 *** the Free Software Foundation; either version 2 of the License, or
 *** (at your option) any later version.
 ***
 *** ePiX is distributed in the hope that it will be useful, but WITHOUT
 *** ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *** or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 *** License for more details.
 ***
 *** You should have received a copy of the GNU General Public License
 *** along with ePiX; if not, write to the Free Software Foundation, Inc.,
 *** 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 ***
 ***/
/*
 * This file provides the Length class
 *
 *   - Length(std::string) assumes argument is "double [space(s)] unit"
 *     Recognized units are pt (points), in, cm, mm, pc (picas = 1/12 in), 
 *     bp (big points = 1/72 in), sp (small points = 1/65536 in)
 *     
 *     Malformed strings are handled as follows:
 *     Missing doubles are assumed to be 0, units are assumed to be pt
 *
 *   - Length(double) sets length to specified number of pt
 *
 *   - operator += Increments a length, returning answer in LHS units, e.g.
 *     1cm+=2in is 6.08cm
 *     2in+=1cm is 2.3937in
 *
 *   - operator *= scales a Length by a double
 *   - operator< compares true lengths
 *   - to(string) converts a length to specified units
 *
 * A Length can be negative.
 */


#ifndef EPIX2_LENGTH
#define EPIX2_LENGTH

#include <string>

namespace ePiX2 {

  class Length {
  public:
    static Length Unset;

    explicit Length(std::string);
    explicit Length(const double = 0);

    Length& operator+= (const Length&); // increment by another length
    Length& operator*= (const double);  // scale
    void to(std::string); // convert to specified units

    bool is_unset(void) const;

    // compare, with units conversion
    bool operator== (const Length&) const;
    bool operator!= (const Length& L) const { return !((*this) == L); }
    friend bool operator< (const Length& arg1, const Length& arg2);

    // for output
    double magnitude(void) const { return m_mag; }
    std::string units(void) const { return m_units; }

  private:
    double m_mag;
    double pts_per_unit; // constants defined in Length.cc
    std::string m_units;
  }; // end of class Length

} /* end of namespace */

#endif /* EPIX2_LENGTH */
