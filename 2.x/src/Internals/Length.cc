/***
 ***  Length.cc
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
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
#include <string>

#include "Errors.h"
#include "Constants.h"
#include "Length.h"

namespace ePiX2 {

  // Inviolable constants: number of pts per unit
  const double IN = 72.27;               // the gold standard
  const double CM = 28.452755905512;     // 2.54 cm = 72.27 pt
  const double MM = 2.8452755905512;     // 25.4 mm = 72.27 pt
  const double PC = 6.0225;              // 12 picas = 1 inch
  const double BP = 1.00375;             // 72 big pt = 72.27 pt
  const double SP = 0.0000152587890625;  // 65536 small pt = 1 pt

  Length Length::Unset = Length();

  // extract length from "double [spaces] length unit"
  Length::Length(std::string arg)
  {
    char* tmp_units;
    m_mag = strtod(arg.c_str(), &tmp_units);
    std::string putative_units(tmp_units);
      
    // skip spaces, get first two non-space characters
    std::string::size_type i=putative_units.find_first_not_of(" ");

    if (i == std::string::npos)
      {
	epix_warning("Missing units, using pt");
	m_units = "pt";
      }
    else
      m_units = putative_units.substr(i, 2);

    // See what we found; ugly, but not going to change...
    if (m_units == "pt")
      pts_per_unit = 1.0;

    else if (m_units == "in")
      pts_per_unit = IN;

    else if (m_units == "cm")
      pts_per_unit = CM;

    else if (m_units == "mm")
      pts_per_unit = MM;

    else if (m_units == "pc")
      pts_per_unit = PC;

    else if (m_units == "bp")
      pts_per_unit = BP;

    else if (m_units == "sp")
      pts_per_unit = SP;

    else
      {
	std::string warn="Unrecognized units \""+m_units+"\", assuming pt";
        epix_warning(warn);
	pts_per_unit = 1.0;
	m_units = "pt";
      }
  } // end of string constructor

  Length::Length(const double lgth)
    : m_mag(lgth), pts_per_unit(1.0), m_units("pt") { }

  Length& Length::operator+= (const Length& increment)
  {
    m_mag += increment.m_mag*increment.pts_per_unit/pts_per_unit;
    return *this;
  }

  Length& Length::operator*= (const double scale)
  {
    m_mag *= scale;
    return *this;
  }

  // convert to specified units
  void Length::to(std::string units)
  {
    units = units.substr(0,2);

    if (m_units == units)
      return; // null conversion

    if (units == "pt")
      {
	m_mag *= pts_per_unit;
	pts_per_unit = 1.0;
	m_units = units;
      }

    else if (units == "in")
      {
	m_mag *= pts_per_unit/IN;
	pts_per_unit = IN;
	m_units = units;
      }

    else if (units == "cm")
      {
	m_mag *= pts_per_unit/CM;
	pts_per_unit = CM;
	m_units = units;
      }

    else if (units == "mm")
      {
	m_mag *= pts_per_unit/MM;
	pts_per_unit = MM;
	m_units = units;
      }

    else if (units == "pc")
      {
	m_mag *= pts_per_unit/PC;
	pts_per_unit = PC;
	m_units = units;
      }

    else if (units == "bp")
      {
	m_mag *= pts_per_unit/BP;
	pts_per_unit = BP;
	m_units = units;
      }

    else if (units == "sp")
      {
	m_mag *= pts_per_unit/SP;
	pts_per_unit = SP;
	m_units = units;
      }

    else
      epix_warning("Ignoring conversion to unrecognized units \""+units+"\"");

  }

  bool Length::is_unset(void) const
  {
    return (this == &(Length::Unset));
  }

  bool Length::operator== (const Length& L) const
  {
    return (fabs(m_mag*pts_per_unit - L.m_mag*L.pts_per_unit) < EPIX2_EPSILON);
  }

  bool operator< (const Length& arg1, const Length& arg2)
  {
    return arg1.m_mag*arg1.pts_per_unit < arg2.m_mag*arg2.pts_per_unit;
  }

} /* end of namespace */
