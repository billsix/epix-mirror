/*
 * Color_Base.h -- ePiX::Color interface
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.17
 * Last Change: September 13, 2007
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

#ifndef EPIX_COLOR_BASE
#define EPIX_COLOR_BASE

#include <string>
#include <vector>

namespace ePiX {

  class Color_Base {

  protected:
    struct RGB_Densities
    {
      // ensures densities are in [0,1]
      RGB_Densities (double r, double g, double b);

      double m_dens_red, m_dens_green, m_dens_blue;
    };

  public:
    virtual ~Color_Base() { }
    virtual Color_Base* clone() const = 0;

    // blending (all non-const)
    // apply us as filter
    virtual Color_Base& filter(const Color_Base&) = 0;

    // scale intensities
    virtual Color_Base& operator*= (double) = 0;

    // clip d to [0,1], then return (1-d)*this + d*col
    virtual Color_Base& blend(const Color_Base&, double) = 0;

    // add channels
    virtual Color_Base& superpose(const Color_Base&) = 0;

    // return negative color
    virtual Color_Base& invert() = 0;

    // internal colorname and densities
    virtual std::string model() const = 0;
    virtual std::string name() const = 0;
    virtual std::vector<double> densities() const = 0;

    bool operator== (const Color_Base& col) const;
    virtual RGB_Densities to_rgb() const = 0;

  }; // end of class Color_Base

} // end of namespace

#endif /* EPIX_COLOR_BASE */
