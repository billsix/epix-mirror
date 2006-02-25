/*** 
 *** Pair.h -- ePiX2::Pair_Map(_Base) classes
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: December 04, 2005
 ***
 *** 
 *** Copyright (C) 2001, 2002, 2003, 2004, 2005
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
 *** 
 *** This file provides the classes
 ***   * Pair_Map_Base
 ***     - Interface to affine scaling, rotation, reflection, etc., which
 ***       provides a Pair-valued evaluation operator
 ***
 ***   * Pair_Map_{Fcn, Affine, Rotate, Reflect}
 ***     - Child classes that implement function maps, affine scaling, 
 ***       rotation, and reflection
 ***
 ***   * Pair_Map
 ***     - A composable wrapper class for Pair_Map_Base, which provides
 ***       evaluation and composition ()-operators
 ***/

#ifndef EPIX2_PAIR_MAP
#define EPIX2_PAIR_MAP

#include <cmath>
#include "Pair.h"

namespace ePiX2 {

  // Pair mapping interface
  class Pair_Map_Base {
  public:
    virtual ~Pair_Map_Base(void) { }
    // evaluation
    virtual Pair operator() (const Pair) const = 0;
  }; // end of class Pair_Map_Base

  namespace ePiX2_Pair_Map_Base {

    class Pair_Map_Fcn : public Pair_Map_Base {
    public:
      static Pair_Map_Fcn Identity;
      Pair_Map_Fcn(Pair (*f)(Pair)) { mapping = f; }
      Pair_Map_Fcn(void); // Identity map

      Pair operator() (const Pair arg) const { return mapping(arg); }

    private:
      // the mapping we represent
      Pair (*mapping)(Pair);
    }; // end of class Pair_Map_Fcn

    class Pair_Map_Affine : public Pair_Map_Base {
    public:
      Pair_Map_Affine(void) { }
      Pair_Map_Affine(const Rect prnt, const Rect chld);
      Pair operator() (const Pair arg) const;

    private:
      Rect parent, child;
      Pair scale; // affine scale factor from child to parent
    }; // end of class Pair_Map_Affine


    class Pair_Map_Rotate : public Pair_Map_Base {
    public:
      Pair_Map_Rotate(const double th=0, const Pair ctr=Pair(0,0));
      Pair operator() (const Pair arg) const;

    private:
      double angle;
      Pair center;
    }; // end of class Pair_Map_Rotate

    class Pair_Map_Reflect : public Pair_Map_Base {
    public:
      // fixed points
      Pair_Map_Reflect(const Pair p1, const Pair p2) : fix1(p1), fix2(p2) { }
      // fixed point and angle of axis
      Pair_Map_Reflect(const double th=0, const Pair ctr=Pair(0,0));
      Pair operator() (const Pair arg) const;

    private:
      Pair fix1, fix2; // fixed points
    }; // end of class Pair_Map_Reflect
  } // end of ePiX2_Pair_Map_Base

  // composable wrapper class for Pair_Map_Base
  class Pair_Map : public Pair_Map_Base {
  public:
    static Pair_Map Identity;

    Pair_Map(Pair_Map_Base& = ePiX2_Pair_Map_Base::Pair_Map_Fcn::Identity);
    // evaluation
    Pair operator() (const Pair) const;
    // composition
    Pair_Map operator() (Pair_Map&);

  private:
    Pair_Map_Base* outer;  // the mapping we represent
    Pair_Map* inner;       // precompose with this
  }; // end of Pair_Map class

  // constructor wrappers
  Pair_Map Function(Pair f(Pair));
  Pair_Map Affine(const Rect prnt, const Rect chld);
  Pair_Map Rotate(const double th=0, const Pair ctr=Pair(0,0));
  // fixed points
  Pair_Map Reflect(const Pair p1, const Pair p2);
  // fixed point and angle of axis
  Pair_Map Reflect(const double th=0, const Pair ctr=Pair(0,0));

} /* end of namespace */

#endif /* EPIX2_PAIR_MAP */
