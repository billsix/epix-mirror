/* 
 * data_bins.cc -- ePiX::data_bins class for histogram data
 *
 * This file is part of ePiX, a C++ library for creating high-quality 
 * figures in LaTeX 
 *
 * Version 1.0.25
 * Last Change: May 20, 2007
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007
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
//#include <iostream>
//#include <math.h>
//#include <fstream>
#include <sstream>
#include <list>
#include <vector>
//#include <stdexcept>

//#include "debug.h"

//#include "globals.h"
#include "errors.h"
//
//#include "functions.h"
#include "triples.h"

#include "path.h"
#include "spline.h"

//#include "Label.h"
#include "curves.h"
//#include "output.h"

#include "data_bins.h"

namespace ePiX {

  typedef std::vector<double>::const_iterator VDCI;

  //// histogram data bin ////
  class d_bin {
  public:
    d_bin(const double low, const double high)
      : m_lo(low), m_hi(high), m_count(0) { }

    bool insert(const double x)
    {
      // TO DO: Standard behavior?
      if (m_lo <= x && x < m_hi)
	{
	  ++m_count;
	  return true;
	}
      else
	return false;
    }

    // get data
    double low()  const { return m_lo; }
    double high() const { return m_hi; }
    unsigned int count() const { return m_count; }

    void draw(const double scale) const
    {
      rect(P(m_lo,0), P(m_hi, scale*m_count));
    }

    P loc(const double scale) const
    {
      // std::cerr << m_lo << ", " << m_hi << "\n";
      return P(0.5*(m_lo + m_hi), scale*m_count);
    }

    d_bin* clone() const
    {
      return new d_bin(*this);
    }

  private:
    double m_lo, m_hi;    // end values
    unsigned int m_count; // number of elements
  }; // end of class d_bin


  //// data_bins functions ////
  data_bins::data_bins(const double lo, const double hi, unsigned int n)
    : m_lo_val(min(lo, hi)), m_hi_val(max(lo, hi)),
      m_lo_ct(0), m_hi_ct(0), m_pop(0), m_cuts_locked(false)
  {
    // divide [lo, hi] into n pieces (n>0), or into unit pieces (n=0)
    unsigned int ct(n>0 ? n : (unsigned int)(m_hi_val - m_lo_val));
    const double dx((m_hi_val - m_lo_val)/ct);

    for (unsigned int i=0; i<=ct; ++i)
      m_cuts.push_back(m_lo_val + i*dx);
  }

  data_bins::data_bins(const data_bins& db)
    : m_lo_val(db.m_lo_val), m_hi_val(db.m_hi_val),
      m_lo_ct(db.m_lo_ct), m_hi_ct(db.m_hi_ct), m_pop(db.m_pop),
      m_cuts_locked(db.m_cuts_locked), m_cuts(db.m_cuts)
  {
    for (std::vector<d_bin*>::const_iterator p=m_bins.begin();
	 p != m_bins.end(); ++p)
      m_bins.push_back((*p)->clone());
  }

  data_bins& data_bins::operator= (const data_bins& db)
  {
    if (this != &db)
      {
        std::vector<d_bin*> tmp;
        for (std::vector<d_bin*>::const_iterator p=db.m_bins.begin();
             p!=db.m_bins.end(); ++p)
          tmp.push_back((*p)->clone());

	// assign data
	m_lo_val = db.m_lo_val;
	m_hi_val = db.m_hi_val;

	m_lo_ct = db.m_lo_ct;
	m_hi_ct = db.m_hi_ct;
	m_pop = db.m_pop;
	m_cuts_locked = db.m_cuts_locked;

        // deallocate memory
        for (std::vector<d_bin*>::iterator p=m_bins.begin();
             p!=m_bins.end(); ++p)
          delete *p;

	m_cuts = db.m_cuts;
	swap(m_bins, tmp);
      }

    return *this;
  }

  data_bins::~data_bins()
  {
    for (std::vector<d_bin*>::iterator p=m_bins.begin(); p!=m_bins.end(); ++p)
      delete *p;
  }


  data_bins& data_bins::cut(const double x)
  {
    if (x < m_lo_val || m_hi_val < x)
      epix_warning("cut() out of range, no action");

    else if (m_cuts_locked)
      epix_warning("cut() called on locked data_bins, no action");

    else
      m_cuts.push_back(x);

    return *this;
  }

  data_bins& data_bins::read(const std::vector<double>& data)
  {
    if (!m_cuts_locked)
      initialize();

    for (VDCI p=data.begin(); p != data.end(); ++p)
      insert(*p);

    return *this;
  }

  // rectangles
  void data_bins::histogram(const double scale) const
  {
    if (0 < m_lo_ct + m_hi_ct)
      {
	std::stringstream obuf;
	obuf << "histogram has "
	     << m_lo_ct + m_hi_ct << " points out of range: "
	     << m_lo_ct << " below, "
	     << m_hi_ct << " above";

	epix_warning(obuf.str());
      }

    for (std::vector<d_bin*>::const_iterator p=m_bins.begin();
	 p != m_bins.end(); ++p)
      (*p)->draw(scale);
  }

  // smooth interpolation
  void data_bins::plot(const double scale) const
  {
    if (0 < m_lo_ct + m_hi_ct)
      {
	std::stringstream obuf;
	obuf << "data_bins.plot has "
	     << m_lo_ct + m_hi_ct << " points out of range: "
	     << m_lo_ct << " below, "
	     << m_hi_ct << " above";

	epix_warning(obuf.str());
      }

    std::vector<P> vertices;

    for (std::vector<d_bin*>::const_iterator p=m_bins.begin();
	 p != m_bins.end(); ++p)
      vertices.push_back((*p)->loc(scale));

    n_spline tmp(vertices);

    // TO DO: Avoid magic number
    tmp.data(20).draw();
  }


  // private bookkeeping functions
  void data_bins::initialize()
  {
    if (!m_cuts_locked) // defensive redundancy
      {
	// sort list of cuts, remove duplicates, and lock
	m_cuts.sort();
	m_cuts.unique();

	m_cuts_locked=true;

	// allocate d_bins
	for (std::list<double>::const_iterator curr = m_cuts.begin();
	     curr != m_cuts.end(); ++curr)
	  {
	    std::list<double>::const_iterator next(curr);
	    ++next;
	    if (next != m_cuts.end())
	      m_bins.push_back(new d_bin(*curr, *(next)));
	    else
	      m_bins.push_back(new d_bin(*curr, m_hi_val));
	  }
      }
  }

  void data_bins::insert(const double x)
  {
    ++m_pop;

    // TO DO: Standard behavior?
    if (x < m_lo_val)
      {
	++m_lo_ct;
	return;
      }

    // else
    bool success(false);
    std::vector<d_bin*>::iterator p=m_bins.begin();

    // try bins serially
    while (!success && p!=m_bins.end())
      success = (*p++)->insert(x);

    if (!success) // still not added
      {
	if (m_hi_val <= x) // sanity check
	  ++m_hi_ct;
	else
	  epix_warning("data_bin::insert() internal error");
      }
  }
} // end of namespace
