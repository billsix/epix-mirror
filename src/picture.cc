/* 
 * picture.cc -- ePiX::epix_picture class, and global functions
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.2.0-2
 * Last Change: September 26, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Andrew D. Hwang <ahwang -at- holycross -dot- edu>
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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <sstream>

#include "triples.h"
#include "errors.h"

#include "functions.h"
#include "length.h"

#include "verbatim.h"
#include "screen_data.h"

#include "screen_mask.h"

#include "screen.h"
#include "active_screen.h"

#include "eepic.h"
#include "pst.h"
#include "tikz.h"

#include "picture_data.h"
#include "picture.h"

namespace ePiX {

  // static objects
  picture_data& the_picture()
  {
    static auto* the_root_picture(new picture_data());
    return *the_root_picture;
  }

  double& xmin()
  {
    static auto* the_x_min(new double);
    return *the_x_min;
  }

  double& xmax()
  {
    static auto* the_x_max(new double);
    return *the_x_max;
  }

  double& xsize()
  {
    static auto* the_x_size(new double);
    return *the_x_size;
  }

  double& ymin()
  {
    static auto* the_y_min(new double);
    return *the_y_min;
  }

  double& ymax()
  {
    static auto* the_y_max(new double);
    return *the_y_max;
  }

  double& ysize()
  {
    static auto* the_y_size(new double);
    return *the_y_size;
  }


  double & tix()
  {
    static auto* the_tix(new double);
    return *the_tix;
  }


  void bounding_box(const P& arg1, const P& arg2)
  {
    xmin()=min(arg1.x1(), arg2.x1());
    xmax()=max(arg1.x1(), arg2.x1());

    ymin()=min(arg1.x2(), arg2.x2());
    ymax()=max(arg1.x2(), arg2.x2());

    xsize() = xmax() - xmin();
    if (xsize() == 0)
      epix_error("Bounding box has width zero!");

    ysize() = ymax() - ymin();
    if (ysize() == 0)
      epix_error("Bounding box has height zero!");

    // else
    the_picture().the_canvas
      = screen(P(xmin(), ymin()), P(xmax(), ymax()));
  }


  // one-line initializer
  void picture(const P& arg1, const P& arg2, std::string sz)
  {
    // bounding_box()
    the_picture().the_canvas
      = screen(P(arg1.x1(), arg1.x2()), P(arg2.x1(), arg2.x2()));

    xmin()  = the_picture().the_canvas.h_min();
    xmax()  = the_picture().the_canvas.h_max();
    xsize() = the_picture().the_canvas.h_size();

    ymin()  = the_picture().the_canvas.v_min();
    ymax()  = the_picture().the_canvas.v_max();
    ysize() = the_picture().the_canvas.v_size();


    // parse size string
    length pic_hsize, pic_vsize;
    get_lengths(sz, pic_hsize, pic_vsize);

    // width units take precedence
    std::string u(pic_hsize.units());
    pic_vsize.to(u);
    the_picture().the_hoffset.to(u);
    the_picture().the_voffset.to(u);

    // unitlength()
    the_picture().the_unitlength = length("1"+u);

    // offset()
    the_picture().the_hoffset = length("0"+u);
    the_picture().the_voffset = length("0"+u);

    // picture()
    the_picture().the_page
      = screen(P(0,0), P(pic_hsize.magnitude(), pic_vsize.magnitude()));

    if (xsize() == 0 || pic_hsize.magnitude() == 0)
      epix_error("Picture has width zero!");

    if (ysize() == 0 || pic_vsize.magnitude() == 0)
      epix_error("Picture has height zero!");
  }

  // old initializers
  void picture(double horiz, double vert)
  {
    if (horiz == 0)
      epix_error("Picture has width zero!");

    if (vert == 0)
      epix_error("Picture has height zero!");

    the_picture().the_page = screen(P(0,0), P(horiz, vert));
  }

  void picture(const P& arg)
  {
    picture(arg.x1(), arg.x2());
  }

  // same syntax as for picture dimensions
  void offset(std::string sz)
  {
    length hoff, voff;
    std::string units(the_picture().the_unitlength.units());

    get_lengths(sz, hoff, voff);

    the_picture().the_hoffset=hoff.to(units);
    the_picture().the_voffset=voff.to(units);
  }

  void offset(double hoff, double voff)
  {
    std::string u(the_picture().the_unitlength.units());

    std::stringstream hbuf, vbuf;
    hbuf << hoff << u;
    vbuf << voff << u;

    the_picture().the_hoffset = length(hbuf.str());
    the_picture().the_voffset = length(vbuf.str());
  }

  void offset(const P& arg)
  {
    offset(arg.x1(), arg.x2());
  }

  void unitlength(const std::string& units)
  {
    the_picture().the_unitlength = length(units);
  }


  void begin()
  {
    const double mult(the_picture().the_unitlength.magnitude());
    if (mult != 1) // someone called unitlength(); must adjust size, offsets
      {
	const P TR(the_picture().the_page.tr());
	the_picture().the_page = screen(P(0,0), mult*TR);

	the_picture().the_hoffset *= mult;
	the_picture().the_voffset *= mult;
      }

    // If we've seen bounding_box or equivalent, this properly initializes.
    // Otherwise we'll segfault momentarily... :(
    active_screen();
  }

  void end_picture()
  {
    the_picture().print();
  }

  screen& canvas()
  {
    return the_picture().the_canvas;
  }

  void activate(screen& scr)
  {
    the_screen_stack().push(scr);
  }

  void deactivate(screen& scr)
  {
    const screen* active(the_screen_stack().pop());

    if (active != &scr)
      epix_warning("Deactivation requested for non-active screen");
  }

  // page layout
  void import()
  {
    if (&(the_picture().the_canvas) != active_screen())
      the_picture().the_canvas.paste(*active_screen());
  }

  void inset(const P& sw, const P& ne)
  {
    if (&(the_picture().the_canvas) != active_screen())
      the_picture().the_canvas.paste(*active_screen(), sw, ne);
  }


  void import(const screen& child)
  {
    the_picture().the_canvas.paste(child);
  }

  void inset(const screen& child, const P& sw, const P& ne)
  {
    the_picture().the_canvas.paste(child, sw, ne);
  }

  void inset(const screen& child)
  {
    the_picture().the_canvas.paste(child, child.bl(), child.tr());
  }

  void inlay(const screen& child, const P& loc)
  {
    const std::string units(the_picture().the_unitlength.units());
    const double scale(length("1pt").to(units).magnitude());
    double H(active_screen()->h_size()/the_picture().the_page.h_size());
    double V(active_screen()->v_size()/the_picture().the_page.v_size());

    active_screen()->paste(child,
			   loc + H*scale*child.bl(),
			   loc + V*scale*child.tr());
  }

  // interpret stated coord as true pt, the other as Cartesian
  void inlay_vertical(const screen& child, const P& loc)
  {
    const std::string units(the_picture().the_unitlength.units());
    const double scale(length("1pt").to(units).magnitude());
    double V(active_screen()->v_size()/the_picture().the_page.v_size());

    active_screen()->paste(child,
			   loc + child.l() + V*scale*child.b(),
			   loc + child.r() + V*scale*child.t());
  }

  void inlay_horizontal(const screen& child, const P& loc)
  {
    const std::string units(the_picture().the_unitlength.units());
    const double scale(length("1pt").to(units).magnitude());
    double H(active_screen()->h_size()/the_picture().the_page.h_size());

    active_screen()->paste(child,
			   loc + child.b() + H*scale*child.l(),
			   loc + child.t() + H*scale*child.r());
  }

  void backing(const Color& col)
  {
    active_screen()->backing(col);
  }

  void border(const Color& col, double len)
  {
    active_screen()->border(col, len);
  }

  void border(const Color& col, const std::string& len)
  {
    active_screen()->border(col, len);
  }

  void backing()
  {
    active_screen()->backing();
  }

  void border()
  {
    active_screen()->border();
  }

  void write(const std::string& text)
  {
    active_screen()->m_screen->add_tile(verbatim(text));
  }

  void pre_write(const std::string& text)
  {
    the_picture().pre_write(text);
  }

  void post_write(const std::string& text)
  {
    the_picture().post_write(text);
  }

  void eepic_format()
  {
    the_picture().set_format(eepic());
  }

  void tikz_format()
  {
    the_picture().set_format(tikz());
  }

  void pst_format()
  {
    the_picture().set_format(pst());
  }


  // write in specified format to named output file
  void print_eepic(const std::string& filename)
  {
    the_picture().print_to(filename, eepic());
  }

  void print_tikz(const std::string& filename)
  {
    the_picture().print_to(filename, tikz());
  }

  void print_pst(const std::string& filename)
  {
    the_picture().print_to(filename, pst());
  }


  double pic_units_per_pt()
  {
    return length("1pt").to(the_picture().the_unitlength.units()).magnitude();
  }

  // approximate, but correct at true aspect ratio
  double units_per_pic_unit()
  {
    const double H(active_screen()->h_size()/the_picture().the_page.h_size());
    const double V(active_screen()->v_size()/the_picture().the_page.v_size());

    return 0.5*(H+V);
  }

  double pt_to_screen(double arg)
  {
    return arg*units_per_pic_unit()*pic_units_per_pt();
  }

  // initialize deprecated globals
  double x_min(xmin()), x_max(xmax()), x_size(xsize());
  double y_min(ymin()), y_max(ymax()), y_size(ysize());

} // end of namespace
