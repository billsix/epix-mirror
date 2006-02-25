/*
 * Output.cc -- ePiX2 Output functions
 *
 * This file is part of ePiX, a program for creating high-quality
 * figures in LaTeX
 *
 * Version 2.0pre
 *
 * Last Change: February 12, 2006
 */

/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Andrew D. Hwang <rot 13 nujnat at zngupf dot ubylpebff dot rqh>
 * Department of Mathematics and Computer Science
 * College of the Holy Cross
 * Worcester, MA, 01610-2395, USA
 *
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

#include <string>
#include <sstream>
#include <set>
#include <list>

#include "Constants.h"
#include "Functions.h"
#include "Styles.h"
#include "PSTricks.h"

namespace ePiX2 {

  PSTricks::PSTricks(const Length& lw, const Color& lc, const Color& fc)
  {
    m_line=lc;
    m_fill=fc;

    if (lw.magnitude() == 0)
      m_pen=Length(0.5); // Magic number: PLAIN_WIDTH
    else
      m_pen=lw;
  }


  // Mid-level functions
  std::string PSTricks::print_box(const Pair loc, std::string msg,
				  const Color& text, const Color& fill,
				  const Color& edge,
				  const Length& pad, const Length& bd,
				  epix2_align_type align,
				  epix2_font_type font, double angle) const
  {
    std::stringstream obuf;
    obuf << "\\rput";

    if (align != c)
      obuf << "[" << print(align) << "]";

    if (fabs(angle) > EPIX2_EPSILON)
      obuf << "{" << angle << "}";

    obuf << print(loc) << "{";

    if (fill != Color::Unset) // draw border if masking
      {
	obuf << "\\psframebox[framesep=" << print(pad);

	if (m_fill != fill)
	  obuf << ",fillcolor=" << print(fill);

	if (m_line != edge)
	  obuf << ",linecolor=" << print(edge);

	if (m_pen != bd)
	  obuf << ",linewidth=" << print(bd);

	obuf << "]";
      }

    else // not masking
      ;

    obuf << "{";

    // label text proper
    if (text != Black(1))
      obuf << "\\color{" << print(text) << "}";

    if (font != ROMAN)
      obuf << print(font) << "{";

    obuf << msg;

    if (font != ROMAN)
      obuf << "}";

    obuf << "}}" << std::endl;

    return obuf.str();
  } // end of PSTricks::print_box()


  // location; marker; text, fill, edge colors; padding, border width;
  std::string PSTricks::print_mark(const Pair loc, epix2_mark_type mark,
				   const Color& text, const Color& fill,
				   const Color& edge,
				   const Length& pad, const Length& bd) const
  {
    std::stringstream obuf;
    obuf << "\\rput" << print(loc) << "{";

    if (fill != Color::Unset) // draw border if masking
      {
	obuf << "\\psframebox*[framesep=" << print(pad);

	if (m_fill != fill)
	  obuf << ",fillcolor=" << print(fill);

	if (m_line != edge)
	  obuf << ",linecolor=" << print(edge);

	if (m_pen != bd)
	  obuf << ",linewidth=" << print(bd);

	obuf << "]";
      }

    else // not masking
      ;

    // set mark color
    if (text != Black(1))
      obuf << "\\begin{" << print(text) << "}";

    obuf << print(mark);

    if (text != Black(1))
      obuf << "\\end{" << print(text) << "}";

    obuf << "}" << std::endl;

    return obuf.str();
  } // end of PSTricks::print_mark()


  // default line color and width, fill color, closed/filled flags, vertices
  std::string PSTricks::print_sil(const Color& lc, const Length& lw,
				  const Color& fc,
				  bool filled, bool cropped,
				  const std::list<Pair>& vertices) const
  {
    std::stringstream obuf;
    obuf << "\\pspolygon";

    // command options
    std::stringstream cmd;
    bool empty=true;

    if (filled)
      if (m_fill != fc && fc != Color::Unset)
	{
	  // print_header sets fillstyle=solid
	  cmd << "fillcolor=" << print(fc);
	  empty=false;
	}

    if (m_line != lc && lc != Color::Unset)
      {
	if (!empty)
	  cmd << ",";
	if (!cropped)
	  cmd << "linecolor=" << print(lc);
	else // some edges "undrawn"; use fill color, then re-draw later
	  cmd << "linecolor=" << print(fc); 
	empty=false;
      }

    if (m_pen != lw)
      {
	if (!empty)
	  cmd << ",";
	cmd << "linewidth=" << print(lw);
	empty=false;
      }

    if (!empty) // got command options
      obuf << "[" << cmd.str() << "]";

    std::list<Pair>::const_iterator p;

    for (p=vertices.begin(); p!=vertices.end(); ++p)
      obuf << print(*p);

    obuf << std::endl;
    // finished drawing filled region

    // If cropped, caller re-draws its border

    return obuf.str();
  } // end of PSTricks::print_sil()


  // line/back color and width
  std::string PSTricks::print_outline(const Color& lc, const Length& lw, 
				      const Color& bc, const Length& bw, 
				      const std::list<Pair>& vertices) const
  {
    std::stringstream obuf;
    obuf << "\\psline[fillstyle=none";

    // TO DO: line style

    if (m_line != lc && lc != Color::Unset)
      obuf << ",linecolor=" << print(lc);

    if (m_pen != lw)
      obuf << ",linewidth=" << print(lw);

    // draw border
    if (bc != lc && lw < bw)
      {
	obuf << ",bordercolor=" << print(bc);

	Length tmp_b = bw;
	Length tmp_l = lw;
	tmp_b += (tmp_l *= -1); // subtract lw from bw
	tmp_b *= 0.5;

	obuf << ",border=" << print(tmp_b);
      }

    obuf << "]";
    std::list<Pair>::const_iterator p;

    for (p=vertices.begin(); p!=vertices.end(); ++p)
      obuf << print(*p);

    obuf << std::endl;

    return obuf.str();
  }


  std::string PSTricks::pic_header(const Pair sw, const Pair ne, 
				   const Length& len,
				   const std::set<Color>& pallet) const
  {
    std::stringstream obuf;
    // TO DO
    using std::endl;
    obuf << "%%  ePiX-2.0pre" << endl
	 << "\\psset{fillstyle=solid,unit=1" << len.units() << "}" << endl;

    std::set<Color>::const_iterator cp;
    for (cp=pallet.begin(); cp!=pallet.end(); ++cp)
      obuf << (*cp)->print_to(*this); 

    obuf << endl << "\\begin{pspicture}" << print(sw) << print(ne) << endl;

    return obuf.str();
  }

  std::string PSTricks::pic_footer(void) const
  {
    std::stringstream obuf;
    obuf << "\\end{pspicture}" << std::endl;
    return obuf.str();
  }

  std::string PSTricks::print_color(std::string model, std::string name,
				    const double d1, const double d2,
				    const double d3) const
  {
    std::stringstream obuf;
    obuf << "\\new" << model;
    if (model == "cmy")
      obuf << "k";

    obuf << "color{" << name << "}{"
	 << d1 << " " << d2 << " " << d3;

    if (model == "cmy")
      obuf << " " << min(min(d1,d2),d3);

    obuf << "}" << std::endl;

    return obuf.str();
  }

  std::string PSTricks::print_color(std::string model, std::string name,
				    const double d1, const double d2,
				    const double d3, const double d4) const
  {
    std::stringstream obuf;
    obuf << "\\new" << model << "color{" << name << "}{"
	 << d1 << " " << d2 << " " << d3 << " " << d4 << "}" << std::endl;
    return obuf.str();
  }


  std::string PSTricks::pic_caption(std::string caption) const
  {
    std::stringstream obuf;
    obuf << "\\caption{" << caption << "}" << std::endl;
    return obuf.str();
  }

  std::string PSTricks::pic_xref(std::string xref) const
  {
    std::stringstream obuf;
    obuf << "\\label{" << xref << "}" << std::endl;
    return obuf.str();
  }

  /*
  void PSTricks::comment(std::string msg)
  {
    std::stringstream obuf;
    obuf << "%%  " << msg << std::endl;
  }
  */

  // Utility functions
  /*
  std::string epix2_rule(const Length& x, const Length& y,
			 const Length& dx=Length(0), 
			 const Length& dy=Length(0))
  {
    std::stringstream buf;
    buf << "\\kern " << dx << " \\rule[" << dy << "]"
	<< "{" << x << "}{" << y << "}";

    return buf.str();
  }
  */

  static std::string epix2_rule(const double x, const double y)
  {
    std::stringstream buf;
    buf << "\\rule{" << x << "pt}{" << y << "pt}";

    return buf.str();
  }

  std::string PSTricks::print(const epix2_align_type arg) const
  {
    std::string val;
    switch(arg) 
      {
      case B:
	val="B";
	break;
      case Br:
	val="Br";
	break;
      case Bl:
	val="Bl";
	break;
      case c:
	val="c";
	break;
      case r:
	val="r";
	break;
      case tr:
      case rt:
	val="tr";
	break;
      case t:
	val="t";
	break;
      case tl:
      case lt:
	val="tl";
	break;
      case l:
	val="l";
	break;
      case bl:
      case lb:
	val="bl";
	break;
      case b:
	val="b";
	break;
      case br:
      case rb:
	val="br";
	break;
      }

    return val;
  } // end of PSTricks::print(epix2_align_type)

  std::string PSTricks::print(epix2_mark_type mark) const
  {
    std::string val="\\$";

    switch(mark)
      {
      case MK_NONE:
	val = ""; // N.B. reassign, not append
	break;
      case MK_CIRC:
	val += "\\circle";
	break;
      case MK_SPOT:
	val += "\\circle*";
	break;
      case MK_RING:
	val += "\\circle";
	break;
      case MK_DOT:
	val += "\\circle*";
	break;
      case MK_DDOT:
	val += "\\circle*";
	break;
      case MK_PLUS:
	val += "+";
	break;
      case MK_OPLUS:
	val += "\\oplus";
	break;
      case MK_TIMES:
	val += "\\times";
	break;
      case MK_OTIMES:
	val += "\\otimes";
	break;
      case MK_DIAMOND:
	val += "\\diamond";
	break;
      case MK_UP:
	val += "\\bigtriangleup";
	break;
      case MK_DOWN:
	val += "\\bigtriangledown";
	break;
      case MK_BOX:
	val += epix2_rule(2, 2); // Magic numbers
	break;
      case MK_BBOX:
	val += epix2_rule(1.5, 1.5);
	break;
      case MK_HTICK:
	val += epix2_rule(4, 0.5);
	break;
      case MK_VTICK:
	val += epix2_rule(0.5, 4);
	break;
      }

    if (mark != MK_NONE)
      val +="$";
    return val;
  } // end of PSTricks::print(epix2_mark_type)


  std::string PSTricks::print(epix2_font_type f) const
  {
    std::string val = "\\";

    switch(f)
      {
      case ROMAN:
	val = ""; // N.B. reassign
	break;
      case ITALIC:
	val += "textit";
	break;
      case BFACE:
	val += "textbf";
	break;
      case TTYPE:
	val += "texttt";
	break;
      }

    return val;
  } // end of PSTricks::print(epix2_font_type)

  static double trunc(double t)
  {
    double tmp=t;
    if (fabs(tmp) < EPIX2_EPSILON)
      tmp=0;

    return tmp;
  } // end of trunc

  std::string PSTricks::print(const Pair& arg) const
  {
    std::stringstream o;
    o << "(" << trunc(arg.x1()) << "," << trunc(arg.x2()) << ")";
    return o.str();
  }

  std::string PSTricks::print(const Color& col) const
  {
    return col.name();
  }

  std::string PSTricks::print(const Length& len) const
  {
    std::stringstream o;
    o << len.magnitude() << len.units();
    return o.str();
  }

} /* end of namespace */
