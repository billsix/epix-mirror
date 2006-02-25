/*** 
 ***  World.h -- epix2::World class
 ***
 *** This file is part of ePiX, a program for creating high-quality 
 *** figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: February 12, 2006
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
 ***   Layer - Binary space partition tree node and algorithms
 ***
 ***  World class
 ***/

#ifndef EPIX2_WORLD
#define EPIX2_WORLD

#include <list>
#include <cmath>

#include "Length.h"
#include "Color.h"
#include "Styles.h"

#include "Camera.h"
#include "Object.h"
#include "Object_Rep.h"

#include "Screen.h"
#include "Picture.h"

namespace ePiX2 {

  class World {
  public:
    World(void) : m_fog(0), m_layers(0), m_updated(false) { }
    // World(void);
    // World(const World&); // copy shards
    //    ~World(void);

    // modify scenery
    World& operator<< (const Object&);
    World& operator<< (const World&);

    void clear(void);
    void clip(const Vector&);
    void slice(const Vector&);

    // apply to current Objects
    World& paint_line(const Color&, const Length&);
    World& paint_base(const Color&, const Length&);

    World& paint_fill(const Color&, const Color&,
		      const double shine=1, const double alpha=1);

    // Magic numbers
    World& paint_text(const Color&, const Color&, const Color&, 
		      const Length& pad=Length("3pt"),
		      const Length& bord=Length("1pt"));

    // set default attributes
    World& default_line(const Color&, const Length&);
    World& default_base(const Color&, const Length&);

    World& default_fill(const Color&, const Color&,
			const double shine=1, const double alpha=1);

    // Magic numbers
    World& default_text(const Color&, const Color&, const Color&, 
			const Length& pad=Length("3pt"),
			const Length& bord=Length("1pt")); 

    /*
    void line_color(const Color&);
    void line_width(const Length&);

    void base_color(const Color&);
    void base_width(const Length&);

    void fill_color(const Color&);
    void back_color(const Color&);

    void shine(const double);
    void alpha(const double);
    */

    void fog(const double k);

    // map scenery
    void photo(Screen&,  const Camera&); // face-normal shading
    void photo(Picture&, const Camera&);

    void flash(Screen&,  const Camera&); // flat shading
    void flash(Picture&, const Camera&);

    void trace(Screen&,  const Camera&); // edges only, hiding
    void trace(Picture&, const Camera&);

    void x_ray(Screen&,  const Camera&); // edges only, no hiding
    void x_ray(Picture&, const Camera&);

  private:
    Path_Style m_path_style;
    Surface_Style m_surface_style;
    Text_Style m_text_style;

    /*
    // Path and Surface
    Color  m_line_color;
    Length m_line_width;

    //Path
    Color  m_base_color;
    Length m_base_width;

    // Surface
    Color  m_fill_color;
    Color  m_back_color;

    //Path and Surface
    double m_shine;
    double m_alpha;

    //Label
    Color m_text_color;
    Color m_tfill_color;
    Color m_edge_color;

    Length m_text_pad;
    Length m_text_bord;

    epix2_align_type m_text_align;
    epix2_font_type m_text_font;
    double m_text_angle;
    */

    double m_fog;

    std::list<Object> m_scenery;

    Layer* m_layers;
    bool m_updated; // m_layers current?

    void build_tree(void);
    void shoot(Screen&, const Camera&, const Shard_Base::photo_type);

  }; // end of class World

} /* end of namespace */

#endif /* EPIX2_WORLD */
