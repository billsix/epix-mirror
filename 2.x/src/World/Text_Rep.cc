/* 
 *  Object.cc -- epix2::Object_Base and Object classes
 *
 * This file is part of ePiX, a program for creating high-quality 
 * figures in LaTeX 
 *
 * Version 2.0pre
 * Last Change: February 4, 2006
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

#include "Functions.h"
#include "Pair.h"
#include "Point.h"
#include "Vector.h"

#include "Color.h"
#include "Length.h"

#include "Camera.h"
#include "Screen.h"
#include "Picture.h"

#include "Tile.h"
#include "Chop.h"
#include "Silhouette.h"
#include "Outline.h"

#include "Object_Rep.h"

namespace ePiX2 {

  /*
  Point Edge::is_cut_by(const Vector& N) const // replaces perp_hits
  {
    Point O(N.tail());
    Vector dir = m_to - m_from;
    dir.move_to(O);
    double t = -((m_from-O)|N)/(dir|N);

    return m_from + t*dir;
  }
  // end of Edge functions
  */


  /* * * Shard_Base function * * */
  bool Shard_Base::operator< (const Shard_Base& arg) const
  {
    return (area() >= arg.area());
  }


  /* * * Layer functions * * */
  Layer::~Layer(void) { }
  /*
  {
    delete front;
    delete back;

    std::list<Shard>::iterator p;
    for (p=m_fragments.begin(); p!=m_fragments.end(); ++p)
      delete (*p);
  }
  */

  void Layer::clear(void)
  {
    if (back != 0)
      {
	back->clear();
	delete back;
      }

    m_fragments.clear();

    if (front != 0)
      {
	front->clear();
	delete front;
      }
  }

  // indirection necessary b/c insertion dep. on Shard_Base type
  void Layer::add(Shard S)
  {
    m_fragments.push_back(S);
  }

  void Layer::add_f(Shard S)
  {
    if (front == 0)
      {
	front = new Layer(S->perp());
	front->add(S);
	return;
      }

    S->add_to(this->front);
  }

  void Layer::add_b(Shard S)
  {
    if (back == 0)
      {
	back = new Layer(S->perp());
	back->add(S);
	return;
      }

    S->add_to(this->back);
  }

  void Layer::add(const std::list<Shard> LS)
  {
    std::list<Shard>::const_iterator p;
    for (p=LS.begin(); p!=LS.end(); ++p)
      (*p)->add_to(this);
  }

  void Layer::add(const std::vector<Shard> LS)
  {
    std::vector<Shard>::const_iterator p;
    for (p=LS.begin(); p!=LS.end(); ++p)
      (*p)->add_to(this);
  }

  // shoot back to front
  void Layer::shoot(Screen& S, const Camera& cam, const double d,
		    Shard_Base::photo_type T) const
  {
    if (this->m_fragments.size() == 0)
      return;

    Vector dir = cam.viewpt() - m_perp.tail();

    if ((dir|m_perp) >= 0)
      {
	if (back != 0)
	  back->shoot(S, cam, d, T);

	std::list<Shard>::const_iterator p;

	for (p=m_fragments.begin(); p!=m_fragments.end(); ++p)
	  (*p)->shoot(S, cam, d, T);

	if (front != 0)
	  front->shoot(S, cam, d, T);
      }
    else
      {
	if (front != 0)
	  front->shoot(S, cam, d, T);

	std::list<Shard>::const_iterator p;
	for (p=m_fragments.begin(); p!=m_fragments.end(); ++p)
	  (*p)->shoot(S, cam, d, T);

	if (back != 0)
	  back->shoot(S, cam, d, T);
      }
  } // end of Layer::shoot()


  /* * * Wire function * * */
  /*
  void Wire::shoot(Screen& S, const Camera& cam, const double d,
		   Shard_Base::photo_type SHADE) const
  {
    Point tmp_fm, tmp_to;
    double shine=3*m_shine; // quasi-Magic number

    for (Ep3 p=m_border.begin(); p!=m_border.end(); ++p)
      {
	if ((*p).m_drawn)
	  {
	    tmp_fm=(*p).m_from;
	    tmp_to=(*p).m_to;

	    // calculate ambient reflectance factor; N.B. dot product fiddling
	    double dens=1; // FLASH or X_RAY
	    if (SHADE == PHOTO || SHADE == TRACE)
	      {
		Vector dir = (cam.viewpt() - midpoint(tmp_to, tmp_fm));
		Vector E(tmp_to - tmp_fm);
		double c=(E|dir)/(norm(E)*norm(dir)); // sin(theta)!

		dens = (shine+1-c*c)/(shine+1); // E is *parallel* to edge
	      }

	    Outline* value 
	      = new Outline(cam(dens*(m_line_color)), m_line_width,
			    cam(dens*(m_base_color)), m_base_width, false);

	    value->add_edge(cam((*p).m_from), cam((*p).m_to), true); // drawn

	    S << Tile(value);
	    delete value;
	  }
      }
  } // end of Wire::shoot()


  void Wire::shoot(Picture& S, const Camera& cam, const double d,
		   Shard_Base::photo_type SHADE) const
  {
    shoot(S.screen(), cam, d, SHADE);
  }


  Wire* Wire::coord_rep(const Basis& B, const double c) const
  {
    // Allocation
    Wire* value = new Wire;

    // copy attributes
    value->line_color(m_line_color);
    value->line_width(m_line_width);
    value->shine(m_shine);

    value->base_color(m_base_color);
    value->base_width(m_base_width);

    for (Ep3 p=m_border.begin(); p!=m_border.end(); ++p)
      add_edge(B.coords((*p).m_from, c), B.coords((*p).m_to, c), (*p).m_drawn);

    return value;
  }

  void Wire::chop(const Point& N_tail, const Point& N_head)
  {
    if (is_seen())
      ePiX2::chop(N_tail, N_head, m_border, m_clipped); // Internals/Chop.h

    if (m_border.size() == 0)
      hide();
  }

  void Wire::chop(const Vector& knife)
  {
    if (is_seen())
      ePiX2::chop(knife.tail(), knife.head(), m_border, m_clipped);

    if (m_border.size() == 0)
      hide();
  }

  void Wire::slice(const Vector& knife)
  {
    if (!is_seen())
      return;

    Vector dX = knife;
    dX *= EPIX2_EPSILON/norm(dX);

    Point O_ref = knife.tail();
    ePiX2::chop(O_ref, O_ref+dX, m_border, m_clipped);

    if (m_border.size() > 0)
      ePiX2::chop(O_ref, O_ref-dX, m_border, m_clipped);

    if (m_border.size() == 0)
      hide();
  }


  void Wire::add_to(const Point& vpt, Layer* T)
  {
    if (this->m_border.size() == 0)
      return;

    Edge_Viewer viewer(vpt);

    sort(m_border.begin(), m_border.end(), viewer());

    double ht1, ht2;

    for (Ep3 p=m_border.begin(); p!=m_border.end(); ++p)
      {
	// add segment to shard tree
	ht1 = (((*p).m_from - T->m_perp.tail())|m_perp);
	ht2 = (((*p).m_to - T->m_perp.tail())|m_perp);
	if (ht1 < 0 && ht2 < 0)
	  (*p).add_to(vpt
      }

    Point O_ref = m_datum->normal.tail();
    Vector dir = vpt - O_ref;

    Ep3 p = S->m_border.begin();
    Point curr = (*p).m_from;

    double tmp_ht = (dir|(curr - O_ref));
    double max_ht = tmp_ht;
    double min_ht = tmp_ht;

    // calculate heights of vertices wrt this
    for(p=S->m_border.begin(); p!=S->m_border.end(); ++p)
      {
	curr = (*p).m_to;
	tmp_ht = (m_datum->normal|(curr - O_ref));

	min_ht = min(min_ht, tmp_ht);
	max_ht = max(max_ht, tmp_ht);
      }

    if (max_ht <= -EPIX2_EPSILON)
      {
        if (back == 0)
          back = new Layer(vpt, S);
        else
          back->add_to(vpt, S);
      }

    else if (min_ht >= EPIX2_EPSILON)
      {
        if (front == 0)
          front = new Layer(vpt, S);

        else
          front->add_to(vpt, S);
      }

    else
      {
	// TO DO: Check more carefully for ordering using vpt
        Shard_Base S_top=S;
        S_top->chop(m_datum->normal);

        if (front == 0)
          front = new Layer(vpt, S_top);

        else
          front->add_to(vpt, S_top);

        Shard_Base S_bot=S;
        S_bot->chop(-m_datum->normal);

        if (back == 0)
          back = new Layer(vpt, S_bot);

        else
          back->add_to(vpt, S_bot);
      }
  } // end of Wire::add_to
  */

  /* * * Plate functions * * */
  Plate::Plate(const Vector& N, bool is_filled, bool is_clipped)
    : normal(N), m_filled(is_filled), m_clipped(is_clipped),
      m_area(0), m_updated(false) { }

  Plate* Plate::clone(void) const
  {
    return new Plate(*this);
  }

  // d = fog density, from world
  void Plate::shoot(Screen& S, const Camera& cam, const double d,
		    Shard_Base::photo_type SHADE) const
  {
    double shine=3*m_shine; // quasi-Magic number
    double dist=norm(here()-cam.viewpt());

    double dens = 1; // FLASH or X_RAY
    if (SHADE == PHOTO || SHADE == TRACE)
      {
	Vector dir = (cam.viewpt() - normal.tail());
	double c=(normal|dir)/(norm(normal)*norm(dir)); // cos(theta)

	dens = (shine+c*c)/(shine+1);
      }

    Silhouette* value;

    // initialize according to photograph type
    if (SHADE == TRACE)
      value = new Silhouette(cam(dens*(m_line_color.fog(d*dist))),
			     m_line_width, cam(White()), m_filled, m_clipped);

    else if (SHADE == X_RAY)
      value = new Silhouette(cam(m_line_color.fog(d*dist)), m_line_width,
			     cam(White()), false, m_clipped);

    else // PHOTO or FLASH
      {
	if ((normal|cam.eye().move_to(normal.tail())) > 0)
	  value = new Silhouette(cam(dens*(m_line_color.fog(d*dist))),
				 m_line_width,
				 cam(dens*(m_fill_color.fog(d*dist))),
				 m_filled, m_clipped);

	else 
	  value = new Silhouette(cam(dens*(m_line_color.fog(d*dist))),
				 m_line_width,
				 cam(dens*(m_back_color.fog(d*dist))),
				 m_filled, m_clipped);
      }

    for (Ep3 p=m_border.begin(); p!=m_border.end(); ++p)
      value->add_edge(cam((*p).m_from), cam((*p).m_to), (*p).m_drawn);

    S << Tile(value);
  } // end of Plate::shoot()

  void Plate::shoot(Picture& S, const Camera& cam, const double d,
		    Shard_Base::photo_type SHADE) const
  {
    shoot(S.screen(), cam, d, SHADE);
  }


  Plate* Plate::coord_rep(const Basis& B, const double c) const
  {
    Vector new_N =  B.coords(normal.head(), c) - B.coords(normal.tail(), c);

    // Allocation
    Plate* value = new Plate(new_N, m_filled, m_clipped);

    // copy attributes
    value->line_color(m_line_color);
    std::cerr << "  line_color set to " << value->m_line_color.name() << "\n";

    value->line_width(m_line_width);

    value->shine(m_shine);

    value->fill_color(m_fill_color);
    value->back_color(m_back_color);

    value->m_area = c*c*m_area;
    value->m_updated=true;

    for (Ep3 p=m_border.begin(); p!=m_border.end(); ++p)
      value->add_edge(B.coords((*p).m_from, c), 
		      B.coords((*p).m_to, c), (*p).m_drawn);

    return value;
  } // end of Plate::coord_rep


  void Plate::chop(const Point& N_tail, const Point& N_head)
  {
    if (is_seen())
      {
	ePiX2::chop(N_tail, N_head, m_border, m_clipped); // Internals/Chop.h
	m_updated=false;
      }

    if (m_border.size() == 0)
      hide();
  }

  void Plate::chop(const Vector& knife)
  {
    if (is_seen())
      {
	ePiX2::chop(knife.tail(), knife.head(), m_border, m_clipped);
	m_updated=false;
      }

    if (m_border.size() == 0)
      hide();
  }

  void Plate::slice(const Vector& knife)
  {
    //    if (!is_seen())
    //      return;

    Vector dX = knife;
    dX *= EPIX2_EPSILON/norm(dX);

    Point O_ref = knife.tail();
    ePiX2::chop(O_ref, O_ref+dX, m_border, m_clipped);

    if (m_border.size() > 0)
      ePiX2::chop(O_ref, O_ref-dX, m_border, m_clipped);

    if (m_border.size() == 0)
      hide();
  }

  void Plate::add_to(Layer* T) const
  {
    if (m_border.size() < 3)
      return;

    Vector N = T->perp();
    if (norm(N) < EPIX2_EPSILON) // == Vector()) // zero vector
      {
	// Allocate
	Plate* tmp = clone();

	T->perp(tmp->perp());
	T->add(tmp);
	return;
      }

    const Point O_ref = N.tail();

    Ep3 p = this->m_border.begin();

    double tmp_ht = (N|((*p).m_from - O_ref));
    double max_ht = tmp_ht;
    double min_ht = tmp_ht;

    // calculate heights of vertices wrt this
    for(p=this->m_border.begin(); p!=this->m_border.end(); ++p)
      {
	tmp_ht = (N|((*p).m_to - O_ref));

	min_ht = min(min_ht, tmp_ht);
	max_ht = max(max_ht, tmp_ht);
      }

    Plate* S = clone();

    if (max_ht <= 0)
      T->add_b(S); // add to back

    else if (min_ht >= 0)
      T->add_f(S);

    else if (-EPIX2_EPSILON <= min_ht && max_ht <= EPIX2_EPSILON)
      T->add(S);

    else // split
      {
        Plate* S_top = S->clone();
        S_top->chop(N);
	T->add_f(S_top);

        Plate* S_bot = S->clone();
        S_bot->chop(-N);
	T->add_b(S_top);
      }
  } // end of Plate::add_to(Layer*)


  inline double area(const Point& x0, const Point& x1, const Point& x2)
  {
    Vector V1(x1-x0), V2(x2-x0);
    double IP=(V1|V2);

    return 0.5*sqrt((V1|V1)*(V2|V2) - IP*IP); // half norm of cross product
  }

  double Plate::area(void) const
  {
    if (!m_updated)
      {
	double val=0;

	Ep3 p = m_border.begin();
	Point O_ref = (*p).m_from;

	// Shard_Bases are convex, so needn't worry about local orientation
	for (p=m_border.begin(); p!=m_border.end(); ++p)
	  val += ePiX2::area(O_ref, (*p).m_from, (*p).m_to);

	m_area = val;
	m_updated=true;
      }

    return m_area;
  }

} // end of namespace

