/***
 ***  Output.h -- epix2::Output class
 ***
 *** This file is part of ePiX, a preprocessor for creating high-quality 
 *** line figures in LaTeX 
 ***
 *** Version 2.0pre
 *** Last Change: January 09, 2006
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
 ***/
/* This file provides:
 *
 *   The Format base class
 */

#ifndef EPIX2_OUTPUT
#define EPIX2_OUTPUT

#include <set>
#include <list>
#include <string>

#include "Pair.h"
#include "Length.h"
#include "Color.h"

#include "Styles.h"

namespace ePiX2 {

  class Picture;
  class Format {
  public:
    virtual ~Format(void) { }

    /* * * Functions that write Elements * * */
    // location; label string; text, fill, edge colors; padding, border width;
    // alignment: font: rotation angle
    virtual std::string print_box(const Pair, std::string,
				  const Color&, const Color&, const Color&,
				  const Length&, const Length&,
				  epix2_align_type, epix2_font_type, 
				  double) const = 0;

    // location; marker; text, fill, edge colors; padding, border width;
    virtual std::string print_mark(const Pair, epix2_mark_type,
				   const Color&, const Color&, const Color&,
				   const Length&, 
				   const Length&) const = 0;

    // default line color and width, fill color, filled/clipped flags, vertices
    virtual std::string print_sil(const Color&, const Length&, 
				  const Color&, bool, bool, 
				  const std::list<Pair>&) const = 0;

    // line/back color and width
    virtual std::string print_outline(const Color&, const Length&, 
				      const Color&, const Length&, 
				      const std::list<Pair>&) const = 0;

    // Corners, unitlength, pallet
    virtual std::string pic_header(const Pair sw, const Pair ne, const Length&,
				   const std::set<Color>&) const = 0;

    virtual std::string pic_footer(void) const = 0;

    // model, name, densities
    virtual std::string print_color(std::string, std::string, const double,
				    const double, const double) const = 0;

    virtual std::string print_color(std::string, std::string,
				    const double, const double,
				    const double, const double) const = 0;


    // e.g., local description
    virtual std::string pic_caption(std::string) const = 0;
    // e.g., cross-referencing tag, URL
    virtual std::string pic_xref(std::string) const = 0;
    // virtual void comment(std::string) = 0;

    virtual std::string print(const Pair&) const = 0;
    virtual std::string print(const Color&) const = 0;
    virtual std::string print(const Length&) const = 0;

  protected:
    /* * * Internal functions * * */
    // Low-level chunks
    virtual std::string print(const epix2_align_type arg) const = 0;
    virtual std::string print(const epix2_mark_type arg) const = 0;
    virtual std::string print(const epix2_font_type arg) const = 0;

  }; // end of class Format

  // Global function passing non-constant Picture ref
  void print(Picture&, const char*, const Format&);

} /* end of namespace */

#endif /* EPIX2_OUTPUT */
