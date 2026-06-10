/*
 * Color_Utils.h -- Functions for manipulating color densities
 *
 * This file is part of ePiX, a C++ library for creating high-quality
 * figures in LaTeX
 *
 * Version 1.1.17
 *
 * Last Change: September 14, 2007
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

#ifndef EPIX_COLOR_UTILS
#define EPIX_COLOR_UTILS

#include <string>

namespace ePiX {

double clip_to_unit(double t);  // 0 if t<0, 1 if t>1, t otherwise

double rd(double);  // assumes arg is in [0,1]

// represent a double in [0,1] by an 8-bit hex string
std::string dtohex(double arg);  // assumes arg>0

// saw2th:R -> [-2,2] is (1) a sawtooth function; (2) the identity on [-2,2]
double saw2th(double x);

/*
 *  clippy(x):
 *  1....          ____                   if (x >= 0)
 *      .\        /.  .                     return x <=  1 ? x :  1;
 *      . \      / .  .
 *  0......\____/  .  .                   else
 *      -2 -1   0  1  2 --> x               return x >= -1 ? 0 : -1-x;
 */
double clippy(double x);

double make_r(double r, double g, double b);
double make_g(double r, double g, double b);
double make_b(double r, double g, double b);
}  // namespace ePiX

#endif /* EPIX_COLOR_UTILS */
