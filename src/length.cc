/*
 * length.cc -- the ePiX::length class
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.2.0-2
 * Last Change: September 26, 2007
 *
 *
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
 * Department of Mathematics and Computer Science
 * College of the Holy Cross
 * Worcester, MA, 01610-2395, USA
 *
 *
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
#include <sstream>

#include "errors.h"
#include "constants.h"
#include "length.h"

namespace ePiX {

// Inviolable "private" constants: number of pts per unit
const double IN(72.27);            // the gold standard
const double CM(28.452755905512);  // 2.54 cm = 72.27 pt
const double MM(2.8452755905512);  // 25.4 mm = 72.27 pt
const double PC(6.0225);           // 12 picas = 1 inch
const double BP(1.00375);          // 72 big pt = 72.27 pt
// const double SP(0.0000152587890625);  // 65536 scaled pt = 1 pt

// extract length from "double [spaces] length unit"
length::length(std::string arg) {
  char* tmp_units;
  m_mag = strtod(arg.c_str(), &tmp_units);
  std::string putative_units(tmp_units);

  // skip spaces, get first two non-space characters
  std::string::size_type i = putative_units.find_first_not_of(" ");

  if (i == std::string::npos) {
    epix_warning("Missing units, using pt");
    m_units = "pt";
  } else
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
  /*
  else if (m_units == "sp")
    pts_per_unit = SP;
  */
  else {
    std::string warn = "Unrecognized units \"" + m_units + "\", assuming pt";
    epix_warning(warn);
    pts_per_unit = 1.0;
    m_units = "pt";
  }
}  // end of string constructor

length::length(double lgth) : m_mag(lgth), pts_per_unit(1.0), m_units("pt") {}

length& length::operator+=(const length& increment) {
  m_mag += increment.m_mag * increment.pts_per_unit / pts_per_unit;
  return *this;
}

length& length::operator*=(double scale) {
  m_mag *= scale;
  return *this;
}

// convert to specified units
length& length::to(std::string units) {
  units = units.substr(0, 2);

  if (m_units == units) return *this;  // null conversion

  if (units == "pt") {
    m_mag *= pts_per_unit;
    pts_per_unit = 1.0;
    m_units = units;
  }

  else if (units == "in") {
    m_mag *= pts_per_unit / IN;
    pts_per_unit = IN;
    m_units = units;
  }

  else if (units == "cm") {
    m_mag *= pts_per_unit / CM;
    pts_per_unit = CM;
    m_units = units;
  }

  else if (units == "mm") {
    m_mag *= pts_per_unit / MM;
    pts_per_unit = MM;
    m_units = units;
  }

  else if (units == "pc") {
    m_mag *= pts_per_unit / PC;
    pts_per_unit = PC;
    m_units = units;
  }

  else if (units == "bp") {
    m_mag *= pts_per_unit / BP;
    pts_per_unit = BP;
    m_units = units;
  }

  /*
  else if (units == "sp")
    {
      m_mag *= pts_per_unit/SP;
      pts_per_unit = SP;
      m_units = units;
    }
  */
  else
    epix_warning("Ignoring conversion to unrecognized units \"" + units + "\"");

  return *this;
}

bool length::operator==(const length& L) const {
  return (fabs(m_mag * pts_per_unit - L.m_mag * L.pts_per_unit) < EPIX_EPSILON);
}

double length::magnitude() const { return m_mag; }

std::string length::units() const { return m_units; }

std::string length::name() const {
  std::stringstream buf;
  buf << m_mag << m_units;
  return buf.str();
}

// value-returning operators
length operator+(length len1, const length& len2) { return len1 += len2; }

length operator*(double sc, length len) { return len *= sc; }

bool operator<(length arg1, length arg2) {
  return (arg1.to("pt").magnitude()) < (arg2.to("pt").magnitude());
}

// Parse a string into two lengths.
void get_lengths(std::string sz, length& length1, length& length2) {
  std::string units1;
  std::string units2;

  double len1(0), len2(0);

  // find first digit
  std::string::size_type i = sz.find_first_of("-0123456789.");

  if (i == std::string::npos)
    epix_warning("Invalid width in size string");

  else {
    sz.erase(0, i);  // remove initial garbage
    const char* tmp = sz.c_str();
    char* arg;
    len1 = strtod(tmp, &arg);
    sz = arg;
  }

  // expect either unitlength or multiplication character
  i = sz.find_first_not_of(" ");
  std::string::size_type j = sz.find_first_of("Xx*");
  if (i != j) units1 = sz.substr(i, 2);

  // next digit
  i = sz.find_first_of("-0123456789.");

  if (i == std::string::npos)
    epix_warning("Invalid height in size string");

  else {
    sz.erase(0, i);
    const char* tmp = sz.c_str();
    char* arg;
    len2 = strtod(tmp, &arg);
    sz = arg;
    i = sz.find_first_not_of(" ");
    units2 = sz.substr(i, 2);
  }

  if (units1 == "")  // e.g. sz = "4x6in"
    units1 = units2;

  // assign values
  std::stringstream buf1;
  buf1 << len1 << units1;
  length1 = length(buf1.str());

  std::stringstream buf2;
  buf2 << len2 << units2;
  length2 = length(buf2.str());
}
}  // namespace ePiX
