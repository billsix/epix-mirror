/* 
 * dataplot.cc
 *
 * This file is part of ePiX, a preprocessor for creating high-quality 
 * line figures in LaTeX 
 *
 * Version 1.0.2
 * Last Change: December 31, 2004
 */

/* 
 * Copyright (C) 2001, 2002, 2003, 2004
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
#include <math.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

#include "globals.h"
#include "functions.h"
#include "triples.h"
#include "path.h"

#include "Label.h"
// #include "objects.h"
#include "curves.h"
#include "output.h"

#include "dataplot.h"

namespace ePiX {

  // Read file line-by-line into a stringstream, then extract doubles;
  void read(const char* filename, FILEDATA& data_columns)
  {
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
    unsigned int num_entries = data_columns.at(0).size();

    std::vector<vertex> data(num_entries);

    // create path
    for (unsigned int i=0; i < num_entries; ++i)
      try 
	{
	  if (0 == col3)
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

  // (col1|col2)
  static double dot_product(FILEDATA data,unsigned int col1, unsigned int col2)
  {
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
    double mean = avg(data, col1);
    int N = data.at(0).size();

    return dot_product(data, col1, col1) - mean*mean*N;
  }

  // covariance
  double covar(FILEDATA data, unsigned int col1, unsigned int col2)
  {
    int N = data.at(0).size();

    return dot_product(data, col1, col2) - N*avg(data, col1)*avg(data, col2);
  }

  void regression(FILEDATA data, unsigned int col1, unsigned int col2)
  {
    P mean = P(avg(data, col1), avg(data, col2));
    double m = covar(data, col1, col2)/var(data, col1);

    P dir = P(1, m);

    Line(mean, mean+dir);
  }


  // Histogram plot
  void histogram(const FILEDATA& data_columns, unsigned int column, 
		 double low, double high, int subdivs, double v_scale)
  {
    std::vector<unsigned int> data_count(subdivs+2);
    double step=(high-low)/subdivs; // length of subintervals

    unsigned int num_entries = data_columns.at(0).size();
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
		 double low, double high, int subdivs, double v_scale)
  {
    FILEDATA data_columns(columns);
    read(filename, data_columns);

    histogram(data_columns, column, low, high, subdivs, v_scale);
    end_stanza();
  }

} /* end of namespace */
