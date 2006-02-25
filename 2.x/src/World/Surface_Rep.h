/*** 
 ***  Object_Rep.h -- epix2::Shard_Base and Layer classes
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX
 ***
 *** Version 2.0pre
 *** Last Change: February 4, 2006
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
 ***
 *** This file provides:
 ***
 ***   Shard_Base - A *convex* planar polygon or polyline and draw functions
 ***          .        
 ***    Front .  Back
 ***          .        
 ***   *------*- - - -o
 ***   |xxxxxx:       
 ***   |xxxxxx:       |
 ***   |xxxxxx:       
 ***   |xxxxxx:       |
 ***   |xxxxxx:       
 ***   *------*- - - -o
 ***          .
 ***          .        
 ***          .
 ***
 ***   . . . Cutting plane
 ***   ----- Visible edge
 ***   - - - Edge of back portion
 ***     :   Cut edge (undrawn)
 ***     *   Visible vertex
 ***     o   Vertex of "back" portion
 ***     x   Interior of front portion
 *** 
 ***   Layer, a binary space partition structure for sorting Shards
 ***   according to visibility. A Layer can be "photographed" in the
 ***   following styles:
 ***
 ***   - photo:  Interiors shaded according to face normal
 ***   - flash:  Interiors colored solid
 ***   - trace:  Edges only, with hiding
 ***   - x_ray:  Edges only, no hiding
 ***/

#ifndef EPIX2_OBJECT_REP
#define EPIX2_OBJECT_REP

#include <list>
#include <vector>

#include "Point.h"
#include "Vector.h"

#include "Handle.h"

#include "Length.h"
#include "Color.h"
#include "Camera.h"

#include "Screen.h"
#include "Picture.h"

#include "Tile.h"

namespace ePiX2 {
  class Layer;
  /*
  class Knife
    {
    public:
      Knife(const Vector& V, const Color& C) : m_blade(V), m_paint(C) { }

      Vector m_blade;
      Color m_paint;
    }; // end of class Knife
  */

  class Edge3 
    {
      friend class Wire;
      friend class Plate;
      friend void chop(const Point&, const Point&, std::list<Edge3>&, bool&);

    public:
      Edge3(const Point& fm=Point(), const Point& to=Point(), bool drawn=true)
	: m_from(fm), m_to(to), m_drawn(drawn) { }

    private:
      Point m_from;
      Point m_to;
      bool m_drawn;
    }; // end of class Edge3

  typedef std::list<Edge3>::const_iterator Ep3; // Edge3 pointer

  class Shard_Base;
  typedef Handle<Shard_Base> Shard;

  class Shard_Base {
    //    friend class World;
  public:
    virtual ~Shard_Base(void) { }

    virtual Shard_Base* clone(void) const = 0;

    enum photo_type { PHOTO, FLASH, X_RAY, TRACE };

    virtual void shoot(Screen& S, const Camera&, const double,
		       photo_type) const = 0;
    virtual void shoot(Picture& S, const Camera&, const double,
		       photo_type) const = 0;

    virtual Shard_Base* coord_rep(const Basis&, const double) const = 0;

    virtual void chop(const Point&, const Point&) = 0;
    virtual void chop(const Vector&) = 0;  // half-space cut
    virtual void slice(const Vector&) = 0; // quasi-planar cut

    virtual Point here(void) const = 0;
    virtual Vector perp(void) const = 0;
    virtual void add_to(Layer*) const = 0;

    virtual double area(void) const { return 0; } // true area
    // true if our area is *larger*; for tree sorting
    bool operator< (const Shard_Base&) const;

    virtual void line_color(const Color&) { }
    virtual void line_width(const Length&) { }

    virtual void base_color(const Color&) { }
    virtual void base_width(const Length&) { }

    virtual void fill_color(const Color&) { }
    virtual void back_color(const Color&) { }

    virtual void shine(const double) { }

    // Text_** interface
  }; // end of class Shard_Base

  typedef Handle<Shard_Base> Shard;
  inline bool operator< (const Shard S1, const Shard S2)
    {
      if (S1.operator->() == 0 || S1.operator->() == 0)
	{
	  std::cerr << "Bad Shard comparison\n";
	  return true;
	}
      return (S1->area() >= S2->area());
    }

  class Layer {
  public:
    Layer(void) : m_perp(Vector()), front(0), back(0) { }
    ~Layer(void);

    Layer(const Vector& N) : m_perp(N), front(0), back(0) { }

    void perp(const Vector& N) { m_perp = N; }
    Vector perp(void) const { return m_perp; }

    void clear(void);

    void add(Shard);   // push onto list
    void add_f(Shard); // front
    void add_b(Shard); // back

    void add(std::list<Shard>);   // check each for need to split
    void add(std::vector<Shard>);

    void shoot(Screen&, const Camera&,
	       const double, Shard_Base::photo_type) const;

  private:
    Vector m_perp;
    std::list<Shard> m_fragments;

    Layer* front;
    Layer* back;

  }; // end of Layer


  /*
  class Wire : public Shard_Base
    {
      friend class Path_Base;
    public:
      Wire(void) { }

      Wire* clone(void) const;

      void shoot(Screen& S, const Camera&, const double, photo_type) const;
      void shoot(Picture& S, const Camera&, const double, photo_type) const;

      Wire* coord_rep(const Basis&, const double) const;

      void chop(const Point&, const Point&);
      void chop(const Vector&);  // half-space cut
      void slice(const Vector&); // quasi-planar cut

      Point here(void) const;
      Vector perp(void) const;
      void add_to(Layer*) const;

      void add_edge(const Point& from, const Point& to, bool drawn)
	{
	  m_border.push_back(Edge3(from, to, drawn));
	}

      void add_edge(const Edge3& E) { m_border.push_back(E); }

      bool is_clipped(void) const { return m_clipped; }

      void line_color(const Color& col)	{ m_line_color = col; }
      void line_width(const Length& len) { m_line_width = len; }
      void shine(const double r) { m_shine = r; }

      void base_color(const Color& col) { m_base_color = col; }
      void base_width(const Length& len) { m_base_width = len; }

      void hide(void) const { m_visible = false; }
      bool is_seen(void) const { return m_visible; }

    private:
      std::vector<Edge3> m_border; // needs to be sortable

      // Acquire from Path_Base
      Color m_line_color;
      Length m_line_width;

      Color m_base_color;
      Length m_base_width;

      double m_shine;

      bool m_clipped; // some edges undrawn
      mutable bool m_visible;
    }; // end of class Wire
  */

  class Plate : public Shard_Base
    {
      //      friend class Surface_Base;
    public:
      Plate(const Vector& N=Vector(), bool filled=true, bool clipped=false);

      Plate* clone(void) const;
      void perp(const Vector& N) { normal = N; }

      void shoot(Screen& S, const Camera&, const double, photo_type) const;
      void shoot(Picture& S, const Camera&, const double, photo_type) const;

      Plate* coord_rep(const Basis&, const double) const;
      double area(void) const;
      // void shadow_area(const Point&) const;

      void chop(const Point&, const Point&);
      void chop(const Vector&);  // half-space cut
      void slice(const Vector&); // quasi-planar cut

      Point here(void) const { return normal.tail(); }
      Vector perp(void) const { return normal; }
      void add_to(Layer*) const;

      void add_edge(const Point& from, const Point& to, bool drawn=true)
	{
	  m_border.push_back(Edge3(from, to, drawn));
	  m_updated = false;
	}

      void add_edge(const Edge3& E)
	{
	  m_border.push_back(E);
	  m_updated = false;
	}

      bool is_clipped(void) const { return m_clipped; }

      void line_color(const Color& col)	{ m_line_color = col; }
      void line_width(const Length& len) { m_line_width = len; }
      void shine(const double r) { m_shine = r; }

      void fill_color(const Color& col)	{ m_fill_color = col; }
      void back_color(const Color& col)	{ m_back_color = col; }

      void hide(void) const { m_visible = false; }
      bool is_seen(void) const { return m_visible; }

    private:
      Vector normal;

      std::list<Edge3> m_border;

      // Acquire from Surface_Base
      Color m_line_color;
      Length m_line_width;

      Color m_fill_color;
      Color m_back_color;

      double m_shine;

      bool m_filled;  // solid face?
      bool m_clipped; // some edge(s) undrawn

      mutable bool m_visible;

      mutable double m_area;  // true area
      mutable bool m_updated; // area current?
      // mutable double m_shadow; // area under orthogonal projection

    }; // end of class Plate

} /* end of namespace */

#endif /* EPIX2_OBJECT_REP */
