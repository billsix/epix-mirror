/* 
 * data_file.cc -- ePiX::data_file class
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
#include <cmath>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <stdexcept>

#include "errors.h"

#include "functions.h"
#include "triples.h"
#include "path.h"
#include "spline.h"

#include "label_data.h"
#include "curves.h"

#include "interval.h"
#include "data_bins.h"
#include "data_mask.h"
#include "data_file.h"

namespace ePiX {

  const std::string default_delim("\t");
  const std::string default_commt("%");

  // magic number -- precision for data_file.write() 
  const unsigned int PRECISION(6);

  double truncate(double arg, const unsigned int n)
  {
    if (fabs(arg) < pow(0.1, n))
      return 0;

    return arg;
  }

  // warning message for data_file::tokenize
  void non_parsable(const std::string& msg)
  {
    std::stringstream obuf;
    obuf << "data_file::tokenise():" << std::endl
	 << "  Non-parsable input \"" << msg << "\", setting value to 0";
    epix_warning(obuf.str());
  }

  //// private data_file functions ////
  // returns # of columns, i.e. # of entries in first parsable row
  unsigned int data_file::entries(const char* filename)
  {
    std::ifstream input(filename);
    if (!input)
      {
	epix_warning("Cannot open file \""+std::string(filename)+"\"");
	return(0);
      }

    // else
    std::string linebuf;
    std::vector<double> tmp;

    while (getline(input, linebuf) && tmp.size() == 0)
      {
	// strip comments
	linebuf = linebuf.substr(0, linebuf.find_first_of(m_commt));
	if (linebuf.length() > 0)
	  tmp = tokenise(linebuf);
      }

    input.close();

    if (tmp.size() == 0)
      {
	std::stringstream obuf; // solely for code readability :)
	obuf << "File \"" << std::string(filename)
	     << "\" contains no parsable data";

	epix_warning(obuf.str());
      }

    return tmp.size();
  } // end of entries(const char*)


  // private function: 
  // Tokenise line using our delimiter, return a vector of doubles
  std::vector<double> data_file::tokenise(std::string line)
  {
    size_t pos(line.find(m_delim, 0)); // first delimiter
    std::string tmpStr;                // current chunk of input
    double tmpDbl;                     // current parsed chunk
    std::vector<double> tmpVec;        // output found so far

    // read current line into a vector of doubles
    while(pos != std::string::npos)
      {
	tmpStr = line.substr(0, pos);
	line.erase(0, pos+1);
	std::istringstream convStr(tmpStr);

	if (!(convStr >> tmpDbl))
	  {
	    tmpDbl = 0;
	    non_parsable(convStr.str());
	  }

	tmpVec.push_back(tmpDbl);
	pos = line.find(m_delim, 0);
      }

    if (line.size()) // There's input remaining
      {
	std::istringstream convStr(line);

	if (!(convStr >> tmpDbl))
	  {
	    tmpDbl = 0;
	    non_parsable(convStr.str());
	  }

	else
	  tmpVec.push_back(tmpDbl);
      }

    return tmpVec;
  }

  //// public data_file functions ////
  data_file::data_file(unsigned int n)
    : m_precision(PRECISION), m_data(n),
      m_delim(default_delim),
      m_commt(default_commt) { }

  // provide two version to avoid exposing default delim, commt
  data_file::data_file(const char* filename,
		       const std::string& delim, const std::string& commt )
    : m_precision(PRECISION), m_delim(delim), m_commt(commt)
  {
    // get number of columns by parsing first line of file
    const unsigned int N(entries(filename));
    m_data.resize(N);

    if (N > 0) // file contains data
      read(filename);

  } // end of data_file()

  // filename-only version
  data_file::data_file(const char* filename)
    : m_precision(PRECISION), m_delim(default_delim), m_commt(default_commt)
  {
    // get number of columns by parsing first line of file
    const unsigned int N(entries(filename));
    m_data.resize(N);

    if (N > 0) // file contains data
      read(filename);

  } // end of data_file(const char*)


  // file made from components
  data_file::data_file(double f(double),
		       double t_min, double t_max, unsigned int num_pts)
    : m_precision(PRECISION), m_data(1),
      m_delim(default_delim), m_commt(default_commt)
  {
    const double dt((t_max - t_min)/num_pts);
    for (unsigned int i=0; i<= num_pts; ++i)
      m_data.at(0).push_back(f(t_min+i*dt));
  }

  data_file::data_file(double f1(double), double f2(double), 
		       double t_min, double t_max, unsigned int num_pts)
    : m_precision(PRECISION), m_data(2),
      m_delim(default_delim), m_commt(default_commt)
  {
    const double dt((t_max - t_min)/num_pts);
    for (unsigned int i=0; i<= num_pts; ++i)
      {
	m_data.at(0).push_back(f1(t_min+i*dt));
	m_data.at(1).push_back(f2(t_min+i*dt));
      }
  }


  data_file::data_file(double f1(double), double f2(double), double f3(double),
		       double t_min, double t_max, unsigned int num_pts)
    : m_precision(PRECISION), m_data(3),
      m_delim(default_delim), m_commt(default_commt)
  {
    const double dt((t_max - t_min)/num_pts);
    for (unsigned int i=0; i<= num_pts; ++i)
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

    else if (0 < columns)
      {
	std::ifstream input(filename);
	std::string linebuf;
	std::vector<double> line;

	bool warned(false);

	while (getline(input, linebuf))
	  {
	    // strip comments
	    linebuf = linebuf.substr(0, linebuf.find_first_of(m_commt));
	    if (linebuf.length() > 0)
	      {
		line = tokenise(linebuf);

		if (line.size() > m_data.size())
		  {
		    if (!warned)
		      {
			epix_warning("File has more columns than allocated");
			warned = true;
		      }
		  }
		else if (line.size() < m_data.size())
		  {
		    if (!warned)
		      {
			epix_warning("File has fewer columns than allocated");
			warned = true;
		      }
		  }
		else
		  {
		    for (unsigned int i = 0; i < m_data.size(); i++)
		      m_data.at(i).push_back(line.at(i));
		  }
	      } // linebuf non-empty
	  } // end of file
	input.close();
      }
    return *this;
  } // end of data_file::read(const char*, const std::string&)


  // transform column(s)
  data_file& data_file::transform(double f(double), unsigned int col)
  {
    unsigned int rows(m_data.at(0).size());

    if (0 < col) // apply to selected column
      for (unsigned int i=0; i<rows; ++i)
	m_data.at(col-1).at(i) = f(m_data.at(col-1).at(i));

    else // apply to all columns, default
      {
	for (auto & j : m_data)
	  for (unsigned int i=0; i<rows; ++i)
	    j.at(i) = f(j.at(i));
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

  // Erase rows where the data is outside of the specified range
  data_file& data_file::prune(const data_mask& dm, const unsigned int col)
  {
    std::vector<std::vector<double>::iterator> iter(m_data.size());
    for (unsigned int i = 0; i < m_data.size(); i++)
      iter.at(i) = m_data.at(i).begin();

    while (iter.at(0) != m_data.at(0).end())
      {
	if ( dm.masks(*iter.at(col-1)) )
	  for (unsigned int j = 0; j < m_data.size(); j++)
	    m_data.at(j).erase(iter.at(j));

	else
	  for (unsigned int j = 0; j < m_data.size(); j++)
	    iter.at(j)++;
      }

    return *this;
  }

  data_file& data_file::prune(const interval& range, const unsigned int col)
  {
    data_mask dm(range);
    return prune(dm, col);
  }

  data_file& data_file::prune(double rmin, double rmax,
			      const unsigned int col)
  {
    data_mask dm(rmin, rmax);
    return prune(dm, col);
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
    if (1 <= n && n <= m_data.size())
      return m_data.at(n-1);
    else
      {
	if (0 < m_data.size())
	  {
	    epix_warning("Out of range argument to data_file::column()");
	    return m_data.at(0);
	  }
	else
	  {
	    epix_warning("data_file::column() requested for empty file");
	    std::vector<double> err_val(1, 0);
	    return err_val;
	  }
      }
  }

  // apply f to selected column
  std::vector<double> data_file::column(double f(double), unsigned int n) const
  {
    // must copy data
    std::vector<double> value(m_data.at(0).size());
    unsigned int col(n);

    if (col < 1 || m_data.size() < col)
      {
	if (0 < m_data.size())
	  {
	    epix_warning("Out of range argument to data_file::column()");
	    col=1;
	  }
	else
	  {
	    epix_warning("data_file::column() requested for empty file");
	    std::vector<double> err_val(1, 0);
	    return err_val;
	  }
      }

    for (unsigned int i=0; i<m_data.at(0).size(); ++i)
      value.at(i) = f(m_data.at(col-1).at(i));

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
	  output << m_delim << m_data.at(i).at(j);

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
	  output << pt_out(m_data.at(col1-1).at(i), m_data.at(col2-1).at(i));
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


  void data_file::plot(epix_mark_type TYPE,
		       unsigned int col1, unsigned int col2, unsigned int col3,
		       P f(double, double, double)) const
  {
    unsigned int num_entries(m_data.at(0).size());

    std::vector<P> data(num_entries);

    // create path
    for (unsigned int i=0; i < num_entries; ++i)
      try 
	{
	  if (col3 == 0)
	    data.at(i) = f(m_data.at(col1-1).at(i),
			   m_data.at(col2-1).at(i),
			   0);

	  else
	    data.at(i) = f(m_data.at(col1-1).at(i),
			   m_data.at(col2-1).at(i),
			   m_data.at(col3-1).at(i));
	}

      catch (std::out_of_range) 
	{
	  epix_warning("data_file::plot(): Invalid column index");
	  return;
	}

    // and write it
    if (PATH == TYPE) // N.B. Unaffected by "select"
      {
	path temp(data, false, false); // not closed or filled
	temp.draw();
      }

    else
      for (unsigned int i=0; i < num_entries; ++i)
	{
	  //	if (m_select(data.at(i)))
	  label_data temp(data.at(i), TYPE);
	  temp.draw();
	}
  }

  void data_file::plot(epix_mark_type TYPE, P f(double, double, double),
		       unsigned int col1, unsigned int col2,
		       unsigned int col3) const
  {
    plot(TYPE, col1, col2, col3, f);
  }

  //// global functions ////
  void plot(const char* filename, epix_mark_type TYPE,
	    unsigned int col1, unsigned int col2, unsigned int col3,
	    P f(double, double, double))
  {
    data_file temp(filename);
    temp.plot(TYPE, col1, col2, col3, f);
  }

  void plot(const char* filename, epix_mark_type TYPE,
	    P f(double, double, double),
	    unsigned int col1, unsigned int col2, unsigned int col3)
  {
    data_file temp(filename);
    temp.plot(TYPE, col1, col2, col3, f);
  }

  // pass 3rd arg by value
  void histogram(const char* filename, unsigned int col, data_bins db,
		 double scale)
  {
    data_file temp(filename);
    db.read(temp.column(col));
    db.histogram(scale);
  }

  void bar_chart(const char* filename, unsigned int col, data_bins db,
		 double scale)
  {
    data_file temp(filename);
    db.read(temp.column(col));
    db.bar_chart(scale);
  }
} // end of namespace
