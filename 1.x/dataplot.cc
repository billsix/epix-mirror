/* 
 * dataplot.cc
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.23
 * Last Change: January 29, 2007
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
#include <iostream>
// #include <math.h>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <stdexcept>

#include "globals.h"
#include "errors.h"

#include "functions.h"
#include "triples.h"
#include "path.h"

#include "Label.h"
// #include "objects.h"
#include "curves.h"
#include "output.h"

#include "dataplot.h"

namespace ePiX {

  typedef std::vector<double>::const_iterator VDCI;

  // magic number -- precision for data_file.write() 
  const unsigned int PRECISION(6);

  double truncate(double arg, const unsigned int n)
  {
    if (fabs(arg) < pow(0.1, n))
      return 0;

    return arg;
  }

  unsigned int entries(const char* filename)
  {
    std::ifstream input(filename);
    if (!input)
      {
	epix_warning("Cannot open file");
	return(0);
      }

    // else
    std::string linebuf;
    double curr_entry;
    unsigned int entry_count(0);

    while (getline(input, linebuf) && entry_count == 0)
      {
	// strip comments
	linebuf = linebuf.substr(0, linebuf.find_first_of("%"));
	if (linebuf.length() > 0)
	  {
	    std::istringstream linestream(linebuf, std::ios_base::in);

	    while (linestream >> curr_entry)
	      ++entry_count;
	  }
      }

    input.close();

    return entry_count; // "0" signals error
  } // end of entries(const char*)


  /*
  // selection functions
  bool null_selection(const P& arg)
  {
    return true;
  }

  selection& selection::all()
  {
    static selection* all_selection(new selection(null_selection));
    return *all_selection;
  }

  bool selection::operator() (const P& arg) const
  {
    return m_selector(arg);
  }
  */


  data_file::data_file(unsigned int n)
    : m_precision(PRECISION), m_data(n) { }

  data_file::data_file(const char* filename)
    : m_precision(PRECISION), m_data(entries(filename))
  {
    if (0 < m_data.size()) // file contains data
      {
	std::ifstream input(filename); // open file
	std::string linebuf;
	double curr_entry;
	unsigned int entry_count;

	bool warned(false);

	while (getline(input, linebuf))
	  {
	    // strip comments
	    linebuf = linebuf.substr(0, linebuf.find_first_of("%"));
	    if (linebuf.length() > 0)
	      {
		std::istringstream linestream(linebuf, std::ios_base::in);

		entry_count=0;
		while (linestream >> curr_entry)
		  {
		    try 
		      {
			m_data.at(entry_count++).push_back(curr_entry);
		      }

		    catch (std::out_of_range)
		      {
			if (!warned)
			  epix_warning("File has more columns than allocated");

			warned = true;
		      }
		  }

		// found enough entries?
		if (entry_count < m_data.size())
		  {
		    if (!warned)
		      epix_warning("File has fewer columns than expected");

		    warned = true;
		  }

	      } // linebuf non-empty

	  } // end of file
	input.close();
      }
  } // end of data_file(const char*)


  // file made from components
  data_file::data_file(double f(double),
		       double t_min, double t_max, unsigned int num_pts)
    : m_precision(PRECISION), m_data(1)
  {
    const double dt((t_max - t_min)/num_pts);
    for (int i=0; i<= num_pts; ++i)
      m_data.at(0).push_back(f(t_min+i*dt));
  }

  data_file::data_file(double f1(double), double f2(double), 
		       double t_min, double t_max, unsigned int num_pts)
    : m_precision(PRECISION), m_data(2)
  {
    const double dt((t_max - t_min)/num_pts);
    for (int i=0; i<= num_pts; ++i)
      {
	m_data.at(0).push_back(f1(t_min+i*dt));
	m_data.at(1).push_back(f2(t_min+i*dt));
      }
  }


  data_file::data_file(double f1(double), double f2(double), double f3(double),
		       double t_min, double t_max, unsigned int num_pts)
    : m_precision(PRECISION), m_data(3)
  {
    const double dt((t_max - t_min)/num_pts);
    for (int i=0; i<= num_pts; ++i)
      {
	m_data.at(0).push_back(f1(t_min+i*dt));
	m_data.at(1).push_back(f2(t_min+i*dt));
	m_data.at(2).push_back(f3(t_min+i*dt));
      }
  }


  data_file& data_file::read(const char* filename)
  {
    unsigned int columns(entries(filename));
    if (columns != m_data.size())
      {
	std::stringstream msg;
	msg << "Column count mismatch in file " << filename;
	epix_warning(msg.str());
      }

    else
      {
	std::ifstream input(filename);
	std::string linebuf;
	double curr_entry;
	unsigned int entry_count;

	bool warned(false);

	while (getline(input, linebuf))
	  {
	    // strip comments
	    linebuf = linebuf.substr(0, linebuf.find_first_of("%"));
	    if (linebuf.length() > 0)
	      {
		std::istringstream linestream(linebuf, std::ios_base::in);

		entry_count=0;
		while (linestream >> curr_entry)
		  {
		    try 
		      {
			m_data.at(entry_count++).push_back(curr_entry);
		      }

		    catch (std::out_of_range)
		      {
			if (!warned)
			  epix_warning("File has more columns than allocated");

			warned = true;
		      }
		  }

		// found enough entries?
		if (entry_count < m_data.size())
		  {
		    if (!warned)
		      epix_warning("File has fewer columns than expected");

		    warned = true;
		  }

	      } // linebuf non-empty

	  } // end of file
	input.close();
      }
  } // end of data_file::read(const char*)


    // transform column(s)
  data_file& data_file::transform(double f(double), unsigned int col)
  {
    unsigned int rows(m_data.at(0).size());

    if (0 < col) // apply to selected column
      for (unsigned int i=0; i<rows; ++i)
	m_data.at(col-1).at(i) = f(m_data.at(col-1).at(i));

    else // apply to all columns, default
      {
	for (unsigned int j=0; j<m_data.size(); ++j)
	  for (unsigned int i=0; i<rows; ++i)
	    m_data.at(j).at(i) = f(m_data.at(j).at(i));
      }

    return *this;
  }

    // apply f to selected columns, components of image go back to columns
  data_file& data_file::transform(P f(double, double),
				  unsigned int col1, unsigned int col2)
  {
    unsigned int rows(m_data.at(0).size());

    for (unsigned int i=0; i<rows; ++i)
      {
	P tmp(f(m_data.at(col1 - 1).at(i), m_data.at(col2 - 1).at(i)));

	m_data.at(col1 - 1).at(i) = tmp.x1();
	m_data.at(col2 - 1).at(i) = tmp.x2();
      }

    return *this;
  }

  data_file& data_file::transform(P f(double, double, double),
				  unsigned int col1, unsigned int col2)
  {
    unsigned int rows(m_data.at(0).size());

    for (unsigned int i=0; i<rows; ++i)
      {
	P tmp(f(m_data.at(col1 - 1).at(i), m_data.at(col2 - 1).at(i), 0));

	m_data.at(col1 - 1).at(i) = tmp.x1();
	m_data.at(col2 - 1).at(i) = tmp.x2();
      }

    return *this;
  }

  data_file& data_file::transform(P f(double, double, double),
				  unsigned int col1,
				  unsigned int col2,
				  unsigned int col3)
  {
    unsigned int rows(m_data.at(0).size());

    for (unsigned int i=0; i<rows; ++i)
      {
	P tmp(f(m_data.at(col1 - 1).at(i),
		m_data.at(col2 - 1).at(i),
		m_data.at(col3 - 1).at(i)));

	m_data.at(col1 - 1).at(i) = tmp.x1();
	m_data.at(col2 - 1).at(i) = tmp.x2();
	m_data.at(col3 - 1).at(i) = tmp.x3();
      }

    return *this;
  }


  // (col1|col2)
  double data_file::dot(unsigned int col1, unsigned int col2) const
  {
    double sum(0);

    if ((col1 > m_data.size()) || (col2 > m_data.size()) )
      epix_warning("Invalid column index in dot product");
	
    else
      for (unsigned int i=0; i < m_data.at(0).size(); ++i)
	sum += m_data.at(col1-1).at(i)*m_data.at(col2-1).at(i);

    return sum;
  } // end of dot

  // avg (mean) of col1
  double data_file::avg(unsigned int col1) const
  {
    double sum(0);

    if (col1 > m_data.size())
      epix_warning("Invalid column index in mean");
	
    else
      for (unsigned int i=0; i < m_data.at(0).size(); ++i)
	sum += m_data.at(col1-1).at(i);

    return sum/m_data.at(0).size();
  } // end of avg

  // variance (x|x) - n*\bar{x}^2
  double data_file::var(unsigned int col1) const
  {
    double mean(avg(col1));

    return dot(col1, col1) - mean*mean*m_data.at(0).size();
  } // end of var

  // covariance (x|y) - n*\bar{x}*\bar{y}
  double data_file::covar(unsigned int col1, unsigned int col2) const
  {
    return dot(col1, col2) - (m_data.at(0).size())*avg(col1)*avg(col2);
  } // end of covar

  void data_file::regression(unsigned int col1, unsigned int col2) const
  {
    Line(P(avg(col1), avg(col2)), covar(col1, col2)/var(col1));
  }


  //// Output functions ////
  // return selected column
  std::vector<double> data_file::column(unsigned int n) const
  {
    if (n < m_data.size())
      return m_data.at(n);
    else
      {
	epix_warning("Out of range argument to data_file::column()");
	return m_data.at(0);
      }
  }

  // apply f to selected column
  std::vector<double> data_file::column(double f(double), unsigned int n) const
  {
    // must copy data
    std::vector<double> value(m_data.at(0).size());
    unsigned int col(n);

    if (m_data.size() <= n)
      {
	epix_warning("Out of range argument to data_file::column()");
	col=0;
      }

    for (unsigned int i=0; i<m_data.at(0).size(); ++i)
      value.at(i) = f(m_data.at(col).at(i));

    return value;
  }


  // set precision for write()
  void data_file::precision(const unsigned int n) const
  {
    m_precision=n;
  }

  // write file or selected columns
  void data_file::write(const char* filename) const
  {
    std::ofstream output(filename);
    if (!output)
      {
	epix_warning("data_file::write() cannot open file");
	return;
      }

    // else
    output.precision(m_precision);
    output.setf(std::ios_base::showpoint); // pad with 0s
    output.setf(std::ios_base::right);     // right-justify

    unsigned int cols(m_data.size());

    for (unsigned int j=0; j < m_data.at(0).size(); ++j)
      {
	output << m_data.at(0).at(j);
	for (unsigned int i=1; i < cols; ++i)
	  output << "\t" << m_data.at(i).at(j);
      
	output << std::endl;
      }
    output.close();
  }


  void data_file::write(const char* fname, std::string pt_out(double, double),
			unsigned int col1, unsigned int col2) const
  {
    std::ofstream output(fname);
    if (!output)
      {
	epix_warning("data_file::write() cannot open file");
	return;
      }

    // else
    output.precision(m_precision);

    for (unsigned int i=0; i < m_data.at(0).size(); ++i)
      try 
	{
	  output << pt_out(m_data.at(i).at(col1-1), m_data.at(i).at(col2-1));
	}

      catch (std::out_of_range) 
	{
	  epix_warning("data_file::write(): Invalid column index");
	  return;
	}

    output.close();
  } // end of data_file::write(const char*, string function)

  // experimental: LaTeX tabular environment
  void data_file::tabular(const char* filename,
			  const std::string& alignment,
			  const std::string& legend) const
  {
    std::ofstream output(filename);
    if (!output)
      {
	epix_warning("data_file::table() cannot open file");
	return;
      }

    // else
    using std::endl;

    output.precision(m_precision);
    unsigned int cols(m_data.size());

    output << "\\begin{tabular}{" << alignment << "}" << endl
	   << "\\hline" << endl;
    if (legend != "")
      output << legend << endl << "\\hline" << endl;

    for (unsigned int j=0; j < m_data.at(0).size(); ++j)
      {
	output << "$" << truncate(m_data.at(0).at(j), m_precision) << "$";
	for (unsigned int i=1; i < cols; ++i)
	  output << " & $" << truncate(m_data.at(i).at(j), m_precision) << "$";
      
	output << " \\\\" << endl;
      }

    output << "\\hline" << endl
	   << "\\end{tabular}" << endl;

    output.close();
  } // end of data_file::tabular()


  /*
  // (un)set cropping criterion
  void data_file::select(bool Fsel(P))
  {
    m_select=selection(Fsel);
  }

  void data_file::select(const selection& sel)
  {
    m_select=sel;
  }
  */

  void data_file::plot(epix_mark_type TYPE,
		       unsigned int col1, unsigned int col2, unsigned int col3,
		       P f(double, double, double)) const
  {
    unsigned int num_entries(m_data.at(0).size());

    std::vector<vertex> data(num_entries);

    // get crop state
    bool temp_crop(epix::cropping);
    crop();

    // create path
    for (unsigned int i=0; i < num_entries; ++i)
      try 
	{
	  if (col3 == 0)
	    data.at(i) = vertex(f(m_data.at(col1-1).at(i),
				  m_data.at(col2-1).at(i), 0));

	  else
	    data.at(i) = vertex(f(m_data.at(col1-1).at(i),
				  m_data.at(col2-1).at(i),
				  m_data.at(col3-1).at(i)));
	}

      catch (std::out_of_range) 
	{
	  epix_warning("Invalid column index");
	  return;
	}

    // and write it
    if (PATH == TYPE) // N.B. Unaffected by "select"
      {
	path temp(data, false); // not closed
	temp.draw();
      }

    else
      for (unsigned int i=0; i < num_entries; ++i)
	//	if (m_select(data.at(i).here()))
	Label(data.at(i).here(), "", TYPE, c).draw();

    // restore state
    crop(temp_crop);
  }

  void data_file::plot(epix_mark_type TYPE, P f(double, double, double),
		       unsigned int col1, unsigned int col2,
		       unsigned int col3) const
  {
    plot(TYPE, col1, col2, col3, f);
  }



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

    d_bin* clone() const
    {
      return new d_bin(*this);
    }

  private:
    double m_lo, m_hi;    // end values
    unsigned int m_count; // number of elements
  }; // end of class d_bin


  //// data_bins functions ////
  data_bins::data_bins(const double x_min, const double x_max, unsigned int n)
    : m_lo_val(min(x_min,x_max)), m_hi_val(max(x_min,x_max)),
      m_pop(0), m_lo_ct(0), m_hi_ct(0), m_cuts_locked(false)
  {
    // divide [x_min, x_max] into n pieces (n>0), or into unit pieces (n=0)
    const double dx(n>0 ? (m_hi_val - m_lo_val)/n : 1);
    unsigned int ct(n>0 ? n : (unsigned int)(m_hi_val - m_lo_val));

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

	epix_warning("obuf.str()");
      }

    // TO DO: Interpolate data points
    /*
    for (std::vector<d_bin*>::const_iterator p=m_bins.begin();
	 p != m_bins.end(); ++p)
      (*p)->draw(scale);
    */
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
	    m_bins.push_back(new d_bin(*curr, *(++curr)));
	    --curr;
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


  ////////////////////////////////////////////////////////////////
  //// Obsolete functions retained for backward compatibility ////
  ////////////////////////////////////////////////////////////////

  // Read file line-by-line into a stringstream, then extract doubles;
  void read(const char* filename, FILEDATA& data_columns)
  {
    epix_warning("read(const char*, FILEDATA) is obsolete; use data_file");

    std::ifstream input(filename);
    if (!input)
      epix_warning("Cannot open file");

    else
      {
	std::string linebuf;
	double curr_entry;
	unsigned int entry_count;

	bool warned=false;

	while (getline(input, linebuf))
	  {
	    // strip comments
	    linebuf = linebuf.substr(0, linebuf.find_first_of("%"));
	    if (linebuf.length() > 0)
	      {
		std::istringstream linestream(linebuf, std::ios_base::in);

		entry_count=0;
		while (linestream >> curr_entry)
		  {
		    try 
		      {
			data_columns.at(entry_count++).push_back(curr_entry);
		      }

		    catch (std::out_of_range)
		      {
			if (!warned)
			  epix_warning("File has more columns than allocated");

			warned = true;
		      }
		  }

		// found enough entries?
		if (entry_count < data_columns.size())
		  {
		    if (!warned)
		      epix_warning("File has fewer columns than expected");

		    warned = true;
		  }

	      } // linebuf non-empty

	  } // end of file
	input.close();
      }
  } // end of read


  void plot(const FILEDATA& data_columns, epix_mark_type TYPE,
	    unsigned int col1, unsigned int col2, unsigned int col3, 
	    P F(double, double, double))
  {
    epix_warning("plot(FILEDATA...) is obsolete; use data_file");

    // get crop state
    bool temp_crop(epix::cropping);
    crop();

    unsigned int num_entries(data_columns.at(0).size());

    std::vector<vertex> data(num_entries);

    // create path
    for (unsigned int i=0; i < num_entries; ++i)
      try 
	{
	  if (col3 == 0)
	    data.at(i) = vertex(F(data_columns.at(col1-1).at(i),
				  data_columns.at(col2-1).at(i),
				  0));

	  else
	    data.at(i) = vertex(F(data_columns.at(col1-1).at(i),
				  data_columns.at(col2-1).at(i),
				  data_columns.at(col3-1).at(i)));
	}

      catch (std::out_of_range) 
	{
	  epix_warning("Invalid column index");
	  return;
	}

    // and write it
    if (PATH == TYPE)
      {
	path temp(data, false); // not closed
	temp.draw();
      }
    else
      for (unsigned int i=0; i < num_entries; ++i)
	Label(data.at(i).here(), "", TYPE, c).draw();

    crop(temp_crop);
  }


  void plot(const char* filename, epix_mark_type TYPE, int columns, 
	    unsigned int col1, unsigned int col2, unsigned int col3, 
	    P F(double, double, double))
  {
    FILEDATA data_columns(columns);
    read(filename, data_columns);

    plot(data_columns, TYPE, col1, col2, col3, F);
    end_stanza();
  }


  // new call order facilitates (e.g.) plot(data, BOX, log_lin), 1/2/7
  void plot(const char* filename, epix_mark_type TYPE, int columns, 
	    P f(double, double, double),
	    unsigned int col1, unsigned int col2, unsigned int col3)
  {
    FILEDATA data_columns(columns);
    read(filename, data_columns);

    plot(data_columns, TYPE, col1, col2, col3, f);
    end_stanza();
  }



  void data_plot(const char* filename, epix_mark_type TYPE)
  {
    epix_warning("data_plot deprecated; please use plot");
    plot(filename, TYPE, 2, 1, 2, 0);
  }

  // (col1|col2) -- local function
  double dot_product(FILEDATA data,unsigned int col1, unsigned int col2)
  {
    epix_warning("dot_product(FILEDATA...) is obsolete; use data_file");

    double sum=0;

    if ((col1 > data.size()) || (col2 > data.size()) )
      epix_warning("Invalid column index in dot product");
	
    else
      for (unsigned int i=0; i < data.at(0).size(); ++i)
	sum += data.at(col1-1).at(i)*data.at(col2-1).at(i);

    return sum;
  } // end of dot_product

  // avg (mean) of col1
  double avg(FILEDATA data, unsigned int col1)
  {
    epix_warning("avg(FILEDATA...) is obsolete; use data_file");

    double sum=0;

    if (col1 > data.size())
      epix_warning("Invalid column index in mean");
	
    else
      for (unsigned int i=0; i < data.at(0).size(); ++i)
	sum += data.at(col1-1).at(i);

    return sum/data.at(0).size();
  } // end of avg

  // variance
  double var(FILEDATA data, unsigned int col1)
  {
    epix_warning("var(FILEDATA...) is obsolete; use data_file");

    double mean(avg(data, col1));
    return dot_product(data, col1, col1) - mean*mean*data.at(0).size();
  }

  // covariance
  double covar(FILEDATA data, unsigned int col1, unsigned int col2)
  {
    epix_warning("covar(FILEDATA...) is obsolete; use data_file");

    return dot_product(data, col1, col2) - 
      (data.at(0).size())*avg(data, col1)*avg(data, col2);
  }

  void regression(FILEDATA data, unsigned int col1, unsigned int col2)
  {
    epix_warning("regression(FILEDATA...) is obsolete; use data_file");

    Line(P(avg(data, col1), avg(data, col2)),
	 covar(data, col1, col2)/var(data, col1));
  }


  // Histogram plot
  void histogram(const FILEDATA& data_columns, unsigned int column, 
		 int subdivs, double low, double high, double v_scale)
  {
    epix_warning("histogram(FILEDATA...) is obsolete; use data_file");

    std::vector<unsigned int> data_count(subdivs+2);
    double step((high-low)/subdivs); // length of subintervals

    unsigned int num_entries(data_columns.at(0).size());
    double curr, temp;

    std::vector<vertex> data(num_entries);

    // count data points in each bin
    for (unsigned int i=0; i < num_entries; ++i)
      try
	{
	  curr = data_columns.at(column-1).at(i);
	  if (curr < low)
	    ++data_count.at(0);

	  else
	    {
	      temp=low;

	      unsigned int j=0;
	      while ( (temp < curr) && (temp < high) )
		{
		  temp += step;
		  ++j;
		}
	      ++data_count.at(j);
	    }
	}

      catch (std::out_of_range) 
	{
	  epix_warning("Invalid column index");
	  return;
	}

    // draw rectangles
    for (unsigned int j=0; j < (unsigned int) subdivs+1; ++j)
      if (data_count.at(j) > 0)
	rect(P(low+(j-0.5)*step,0), 
	     P(low+(j+0.5)*step, 
	       data_count.at(j)*v_scale/(step*num_entries)));

  } // end of histogram()

  void histogram(const char* filename, int columns, unsigned int column, 
		 int subdivs, double low, double high, double v_scale)
  {
    epix_warning("histogram(filename...) is obsolete; please see the manual");

    FILEDATA data_columns(columns);
    read(filename, data_columns);

    histogram(data_columns, column, subdivs, low, high, v_scale);
    end_stanza();
  }
} // end of namespace
