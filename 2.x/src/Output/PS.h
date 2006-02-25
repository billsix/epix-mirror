/***
 ***  PS.h -- epix2 PostScript output format
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

#ifndef EPIX2_PS
#define EPIX2_PS

#include "Output.h"

namespace ePiX2 {

  class PS : public Format {
  public:
    PS(void) { }

    // location; label string; text, fill, edge colors; padding, border width;
    // alignment: font: rotation angle
    std::string print_box(const Pair, std::string,
			  const Color&, const Color&, const Color&,
			  const Length&, const Length&,
			  epix2_align_type, epix2_font_type, double) const;

    // location; marker; text, fill, edge colors; padding, border width;
    std::string print_mark(const Pair, epix2_mark_type,
			   const Color&, const Color&, const Color&,
			   const Length&, const Length&) const;

    // default line color and width, fill color, filled/cropped flags, vertices
    std::string print_sil(const Color&, const Length&, const Color&,
			  bool, bool, const std::list<Pair>&) const;

    // line/back color and width
    std::string print_outline(const Color& lc, const Length& lw,
                              const Color& bc, const Length& bw,
                              const std::list<Pair>& vertices) const;

    std::string pic_header(const Pair sw, const Pair ne, const Length&,
			   const std::set<Color>&) const;

    std::string pic_footer(void) const;

    std::string print_color(std::string, std::string, const double,
			    const double, const double) const;

    std::string print_color(std::string,  std::string,
			    const double, const double,
			    const double, const double) const;

    std::string pic_caption(std::string) const;
    std::string pic_xref(std::string) const;

    std::string print(const Pair&) const;
    std::string print(const Color&) const;
    std::string print(const Length&) const;

  private:
    Length m_pen;
    Color m_line;
    Color m_fill;

    std::string print(const epix2_align_type arg) const;
    std::string print(const epix2_mark_type arg) const;
    std::string print(const epix2_font_type arg) const;

  }; // end of class PS
} /* end of namespace */

#endif /* EPIX2_PS */
