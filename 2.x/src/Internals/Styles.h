/*** 
 ***  Styles.h -- ePiX2 style data structures
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
 *** This file provides:
 ***
 *** Path_Style, Surface_Style, and Label_Style, to facilitate sharing of 
 *** style data among path-like, surface-like, and textual elements.
 ***
 *** Path_Style comprises two colors (line and base), two widths (line
 *** and base), and two doubles (shine and alpha).
 ***
 *** Surface_Style comprises three colors (line, fill, and back), line
 ***  width, and two doubles (shine and alpha).
 ***
 *** Label_Style comprises three colors (text, background, and border),
 *** two lengths (padding, border width), an alignment flag, a font flag,
 *** and a rotation angle in degrees.
 *** 
 ***/
#ifndef EPIX2_STYLES
#define EPIX2_STYLES

#include "Constants.h"
#include "Length.h"
#include "Color.h"
#include "Camera.h"

namespace ePiX2 {

  enum epix2_align_type 
    {B, Br, Bl, c, r, tr, rt, t, tl, lt, l, bl, lb, b, br, rb};

  enum epix2_mark_type {MK_NONE, MK_CIRC, MK_SPOT, MK_RING,
			MK_DOT, MK_DDOT, MK_PLUS, MK_OPLUS,
			MK_TIMES, MK_OTIMES, MK_DIAMOND,
			MK_UP, MK_DOWN, MK_BOX, MK_BBOX,
			MK_HTICK, MK_VTICK};

  enum epix2_font_type { ROMAN, ITALIC, BFACE, TTYPE };

  class Path_Style
    {
      friend class Path_Base;
      friend class World;

    public:
      Path_Style(void)
	: m_line_color(Black(1)), m_base_color(White(1)), 
	m_line_width(Length(ePiX2::PLAIN_WIDTH)),
	m_base_width(Length(ePiX2::PLAIN_WIDTH)),
	unbd_lc(true), unbd_lw(true), unbd_bc(true), unbd_bw(true) { }

      void update_by(const Path_Style&);
      void filter_by(const Camera& cam);

      void set_line(const Color&, const Length&, bool bind);
      void set_base(const Color&, const Length&, bool bind);

    private:
      Color m_line_color;
      Color m_base_color;

      Length m_line_width;
      Length m_base_width;

      // bound attribute flags
      bool unbd_lc;
      bool unbd_lw;

      bool unbd_bc;
      bool unbd_bw;

    }; // end of Path_Style


  class Surface_Style
    {
      friend class Surface_Base;
      friend class World;

    public:
      Surface_Style(void)
	: m_line_color(Black(1)), m_line_width(Length(ePiX2::PLAIN_WIDTH)),
	m_fill_color(White(1)), m_back_color(White(1)), 
	m_shine(1), m_alpha(1),
	unbd_lc(true), unbd_lw(true), unbd_fc(true), unbd_bc(true),
	unbd_shine(true), unbd_alpha(true) { }

      void update_by(const Surface_Style&);
      void filter_by(const Camera& cam);

      void set_line(const Color&, const Length&, bool bind);
      void set_fill(const Color&, const Color&, bool bind,
		    const double shine=1, const double alpha=1);

      void set_shine(const double, bool);

    private:
      Color  m_line_color;
      Length m_line_width;

      Color m_fill_color;
      Color m_back_color;

      double m_shine;
      double m_alpha;

      // bound attribute flags
      bool unbd_lc;
      bool unbd_lw;

      bool unbd_fc;
      bool unbd_bc;

      bool unbd_shine;
      bool unbd_alpha;
    }; // end of Surface_Style


  // Catch-all style data class for textual Elements
  class Text_Style
    {
      friend class Text_Mark3;
      friend class Text_Mark2;
      friend class Text_Box3;
      friend class Text_Box2;
      friend class World;

    public:
      // Magic number "3pt", PSTricks default
      Text_Style(void)
	: m_text(Black(1)), m_fill(White(1)), m_edge(White(1)),
	m_pad(Length("3pt")), m_bord(Length(0)), m_align(B), m_font(ROMAN),
	m_angle(0),
	unbd_text(true), unbd_fill(true), unbd_edge(true),
	unbd_pad(true), unbd_bord(true),
	unbd_align(true), unbd_font(true), unbd_angle(true) { }

      void update_by(const Text_Style&);
      void filter_by(const Camera& cam);

    private:
      Color m_text;
      Color m_fill;
      Color m_edge;

      Length m_pad;  // framebox padding
      Length m_bord; // border width

      epix2_align_type m_align;
      epix2_font_type m_font;

      double m_angle; // stored in degrees

      // bound attribute flags
      bool unbd_text;
      bool unbd_fill;
      bool unbd_edge;

      bool unbd_pad;
      bool unbd_bord;

      bool unbd_align;
      bool unbd_font;
      bool unbd_angle;
    }; // end of Text_Style

} /* end of namespace */

#endif /* EPIX2_STYLES */
