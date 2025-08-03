/*
 * Color_Sep.h -- ePiX CMYK Color separation classes
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.17
 * Last Change: September 13, 2007
 *
 * 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
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
#ifndef EPIX_COLOR_SEP
#define EPIX_COLOR_SEP
#include <string>
#include <vector>

#include "Color_Base.h"

namespace ePiX {

  class Cyan_Layer : public Color_Base {
  public:
    Cyan_Layer(double dens=1);
    // Return Cyan channel of input
    Cyan_Layer& filter(const Color_Base&);

    Cyan_Layer* clone() const;
    RGB_Densities to_rgb() const; // returns RGB(0,1,1)

    std::string model() const;
    std::string name() const;
    std::vector<double> densities() const;

  private:
    double m_dens;

    // null operations
    Cyan_Layer& operator*= (double);
    Cyan_Layer& blend(const Color_Base&, double);
    Cyan_Layer& superpose(const Color_Base&);
    Cyan_Layer& invert();
  };

  class Magenta_Layer : public Color_Base {
  public:
    Magenta_Layer(double dens=1);
    // Return Magenta channel of input
    Magenta_Layer& filter(const Color_Base&);

    Magenta_Layer* clone() const;
    RGB_Densities to_rgb() const; // returns RGB(0,1,1)

    std::string model() const;
    std::string name() const;
    std::vector<double> densities() const;

  private:
    double m_dens;

    // null operations
    Magenta_Layer& operator*= (double);
    Magenta_Layer& blend(const Color_Base&, double);
    Magenta_Layer& superpose(const Color_Base&);
    Magenta_Layer& invert();
  };

  class Yellow_Layer : public Color_Base {
  public:
    Yellow_Layer(double dens=1);
    // Return Yellow channel of input
    Yellow_Layer& filter(const Color_Base&);

    Yellow_Layer* clone() const;
    RGB_Densities to_rgb() const; // returns RGB(0,1,1)

    std::string model() const;
    std::string name() const;
    std::vector<double> densities() const;

  private:
    double m_dens;

    // null operations
    Yellow_Layer& operator*= (double);
    Yellow_Layer& blend(const Color_Base&, double);
    Yellow_Layer& superpose(const Color_Base&);
    Yellow_Layer& invert();
  };

  class Black_Layer : public Color_Base {
  public:
    Black_Layer(double dens=1);
    // Return Black channel of input
    Black_Layer& filter(const Color_Base&);

    Black_Layer* clone() const;
    RGB_Densities to_rgb() const; // returns RGB(0,1,1)

    std::string model() const;
    std::string name() const;
    std::vector<double> densities() const;

  private:
    double m_dens;

    // null operations
    Black_Layer& operator*= (double);
    Black_Layer& blend(const Color_Base&, double);
    Black_Layer& superpose(const Color_Base&);
    Black_Layer& invert();
  };
} // end of namespace
#endif /* EPIX_COLOR_SEP */
