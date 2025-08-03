/* 
 * lens.h -- ePiX::Lens interface ans implementations
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.1.21
 * Last Change: September 22, 2007
 *
 * 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
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
#ifndef EPIX_LENS
#define EPIX_LENS

namespace ePiX {

  class frame;
  class P;
  class pair;

  class Lens {
  public:
    // lens map(arg, orient, viewpt, distance)
    virtual pair operator() (const P&,
			     const frame&, const P&, double) const = 0;
    virtual bool is_linear() const = 0; // lens preserves lines?
    virtual bool needs_clip() const = 0; // lens needs scene pre-clipping?
    virtual ~Lens();
    virtual Lens* clone() const = 0;
  }; // end of class Lens


  // actual lenses
  class Perspective : public Lens {
  public:
    pair operator()
      (const P&, const frame&, const P&, double) const;

    bool is_linear() const;
    bool needs_clip() const;
    Perspective* clone() const;
  }; // end of class Perspective


  class Orthog : public Lens {
  public:
    pair operator()
      (const P&, const frame&, const P&, double) const;

    bool is_linear() const;
    bool needs_clip() const;
    Orthog* clone() const;
  }; // end of class Orthog


  class Fisheye : public Lens {
  public:
    pair operator()
      (const P&, const frame&, const P&, double) const;

    bool is_linear() const;
    bool needs_clip() const;
    Fisheye* clone() const;
  }; // end of class Fisheye


  class Bubble : public Lens {
  public:
    pair operator()
      (const P&, const frame&, const P&, double) const;

    bool is_linear() const;
    bool needs_clip() const;
    Bubble* clone() const;
  }; // end of class Bubble

} // end of namespace

#endif /* EPIX_LENS */
