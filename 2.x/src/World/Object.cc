/* 
 *  Object.cc -- epix2::Object_Base and Object classes
 *
 * This file is part of ePiX, a program for creating high-quality 
 * figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: February 12, 2006
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
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
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <list>
#include <vector>

#include "Functions.h"
#include "Point.h"
#include "Vector.h"
#include "Basis.h"
#include "Color.h"

#include "Chop.h"
#include "Object_Rep.h"
#include "Object.h"

namespace ePiX2 {

  /* * * Object_Base functions * * */
  Object_Base& Object_Base::operator+= (const Vector& dX)
  {   
    m_orient.move_to(m_orient.here()-dX); // N.B. apply *inverse* to basis
    return *this;
  }

  void Object_Base::move_to(const Point arg)
  {
    m_orient.move_to(arg);
  }

  Object_Base& Object_Base::operator*= (const double c)
  {
    m_scale *= c;
    return *this;
  }

  void Object_Base::scale (const double c)
  {
    m_scale *= c;
  }

  void Object_Base::reflect(const Vector& axis)
  {
    m_orient.reflect(axis); // reflection same as inverse
  }

  void Object_Base::rotate(const double angle, const Vector& axis)
  {
    m_orient.rotate(-angle, axis); // N.B. inverse
  }


  Point Object_Base::in_coords(const Point& arg) const
  {
    return m_orient.coords(arg, m_scale);
  }

  Vector Object_Base::in_coords(const Vector& N) const
  {
    /*
    Vector tmp_N=N;
    tmp_N.move_to(m_orient.here());
    tmp_N *= recip(m_scale);

    return (tmp_N.dx1()*m_orient.sea()) + (tmp_N.dx2()*m_orient.sky()) +
      (tmp_N.dx3()*m_orient.eye());
    */
    return Vector(in_coords(N.head()) - in_coords(N.tail()));
  }

  /*
  Point Object_Base::here(void) const
  {
    return m_orient.here();
  }
  */

  /* * * Object functions * * */
  Object::Object(void) 
    : m_object(0), c_pt(new int(1)) { }

  Object::Object(Object_Base* obj)
    : m_object(obj), c_pt(new int(1)) { }

  Object::Object(const Object& r)
    : m_object(r.m_object), c_pt(r.c_pt) 
  { 
    (*c_pt)++; 
  }

  Object& Object::operator= (const Object& r)
  {
    if (m_object == r.m_object)
      return *this;

    if (--(*c_pt) == 0)
      {
	delete m_object;
	delete c_pt;
      }

    m_object = r.m_object;
    c_pt = r.c_pt;
    (*c_pt)++;
    return *this;
  }

  Object::~Object(void)
  {
    if (--(*c_pt) == 0)
      {
	delete m_object;
	delete c_pt;
      }
  }

  Object_Base* Object::operator->() const
  {
    return m_object;
  }

  void Object::bind(Object_Base* obj)
  {
    if (obj != m_object)
      {
	if (--*c_pt == 0)
	  {
	    delete m_object;
	    *c_pt = 1;
	  }

	else
	  c_pt = new int(1);

	m_object = obj;
      }
  }


  bool Object::operator== (const Object& obj) const
  {
    return m_object == obj.m_object;
  }


  Object& Object::operator+= (const Vector& V)
  {
    (*m_object) += V;
    return *this;
  }

  Object& Object::operator*= (const double c)
  {
    (*m_object) *= c;
    return *this;
  }


  void Object::move_to(const Point arg)
  {
    m_object->move_to(arg);
  }

  void Object::scale (const double c)
  {
    m_object->scale(c);
  }

  void Object::reflect(const Vector& axis)
  {
    m_object->reflect(axis);
  }

  void Object::rotate(const double angle, const Vector& axis)
  {
    m_object->rotate(angle, axis);
  }

} // end of namespace

