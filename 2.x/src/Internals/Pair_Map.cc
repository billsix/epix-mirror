/*** 
 *** Pair_Map.cc -- ePiX2::Pair_Map(_Base) class functions
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
 ***/

#include "Constants.h"
#include "Functions.h"
#include "Pair.h"
#include "Pair_Map.h"

namespace ePiX2 {

  /*** Pair_Map_Base functions ***/
  // Fcn
  static Pair Ident(Pair arg) { return arg; }

  namespace ePiX2_Pair_Map_Base {
    Pair_Map_Fcn Pair_Map_Fcn::Identity = Pair_Map_Fcn(Ident);
    Pair_Map_Fcn::Pair_Map_Fcn(void) { mapping = Ident; }

    // Affine
    Pair_Map_Affine::Pair_Map_Affine(const Rect prnt, const Rect chld)
      : parent(prnt), child(chld)
    {
      scale=Pair(prnt.h_size()/chld.h_size(), prnt.v_size()/chld.v_size());
    }

    Pair Pair_Map_Affine::operator() (const Pair arg) const
    {
      Pair dir = arg - child.bl();
      return parent.bl() + (scale&dir);
    }

    // Rotate
    Pair_Map_Rotate::Pair_Map_Rotate(const double th, const Pair ctr)
      : angle(th), center(ctr) { }

    Pair Pair_Map_Rotate::operator() (const Pair arg) const
    {
      Pair dir=arg-center;
      double x=dir.x1(), y=dir.x2();
      return center + Pair(x*Cos(angle)-y*Sin(angle), 
			   y*Cos(angle)+x*Sin(angle));
    }

    // Reflect
    Pair_Map_Reflect::Pair_Map_Reflect(const double th, const Pair ctr)
      : fix1(ctr)
    {
      fix2 = ctr + Pair(Cos(th),Sin(th));
    }

    Pair Pair_Map_Reflect::operator() (const Pair arg) const
    {
      if (norm(fix2-fix1) > EPIX2_EPSILON)
	{
	  Pair dir=J(fix2-fix1);
	  dir *= recip(norm(dir));
	  return arg - (2*(dir|(arg-fix1)))*dir;
	}
      else // "reflect" about fix1=fix2
	return 2*fix1 - arg;
    }
  } // end of namespace ePiX2_Pair_Map_Base

  /* * * Pair_Map operators * * */
  Pair_Map Pair_Map::Identity 
  = Pair_Map(ePiX2_Pair_Map_Base::Pair_Map_Fcn::Identity);

  Pair_Map::Pair_Map(Pair_Map_Base& map)
  {
    outer = &map;
    inner = &Pair_Map::Identity;
  }

  Pair Pair_Map::operator() (const Pair arg) const
  {
    Pair value = arg;

    if (this != &Pair_Map::Identity)
      value = (*inner)(arg);

    return (*outer)(value);
  }

  Pair_Map Pair_Map::operator() (Pair_Map& precomp)
  {
    if (this != &precomp) // not iterating ourselves
      {
	Pair_Map composition;
	composition.outer = this;
	composition.inner  = &precomp;
	return composition;
      }

    else
      {
	Pair_Map* composition = new Pair_Map;
	composition->outer = this;
	composition->inner = this;
	return *composition;
      }
  }


  // wrapper constructors
  Pair_Map Function(Pair f(Pair))
  {
    ePiX2_Pair_Map_Base::Pair_Map_Fcn* Map 
      = new ePiX2_Pair_Map_Base::Pair_Map_Fcn;

    *Map = ePiX2_Pair_Map_Base::Pair_Map_Fcn(f);
    return Pair_Map(*Map);
  }

  Pair_Map Affine(const Rect prnt, const Rect chld)
  {
    ePiX2_Pair_Map_Base::Pair_Map_Affine* Map 
      = new ePiX2_Pair_Map_Base::Pair_Map_Affine;

    *Map = ePiX2_Pair_Map_Base::Pair_Map_Affine(prnt, chld);
    return Pair_Map(*Map);
  }

  Pair_Map Rotate(const double th, const Pair ctr)
  {
    ePiX2_Pair_Map_Base::Pair_Map_Rotate* Map 
      = new ePiX2_Pair_Map_Base::Pair_Map_Rotate;

    *Map = ePiX2_Pair_Map_Base::Pair_Map_Rotate(th, ctr);
    return Pair_Map(*Map);
  }

  // fixed points
  Pair_Map Reflect(const Pair p1, const Pair p2)
  {
    ePiX2_Pair_Map_Base::Pair_Map_Reflect* Map 
      = new ePiX2_Pair_Map_Base::Pair_Map_Reflect;

    *Map = ePiX2_Pair_Map_Base::Pair_Map_Reflect(p1, p2);
    return Pair_Map(*Map);
  }

  // fixed point and angle of axis
  Pair_Map Reflect(const double th, const Pair ctr)
  {
    ePiX2_Pair_Map_Base::Pair_Map_Reflect* Map 
      = new ePiX2_Pair_Map_Base::Pair_Map_Reflect;

    *Map = ePiX2_Pair_Map_Base::Pair_Map_Reflect(th, ctr);
    return Pair_Map(*Map);
  }

} /* end of namespace */
