/*
 * Extensions to ePiX made by
 *
 *    version 3
 *
 *    Svend Daugaard Pedersen
 *    mail: rot 13 Firaq at QnhtnneqCrqrefra dot qx
 *
 * Implementation of CartesianCoord
 *
 * Last change February 7, 2004
 *
 */

#include <cstring>
#include <cfloat>

#include "epix_ext.h"


using namespace ePiX;

namespace ePiX_contrib
{
  CartesianCoord::CartesianCoord()
  {
    xAxis = &x;
    yAxis = &y;

    bounds(-1,10,-1,10);
  }

  CartesianCoord::CartesianCoord(double xmin,double xmax,double ymin,double ymax)
  {
    xAxis = &x;
    yAxis = &y;

    bounds(xmin,xmax,ymin,ymax);
  }

  void CartesianCoord::draw()
  {
  	 if( (ll.x1()==-DBL_MAX) || (ll.x2()==-DBL_MAX) )
  	   ll = P(x_min,y_min);
  	 if( (ur.x1()==DBL_MAX) || (ur.x2()==DBL_MAX) )
  	   ur = P(x_max,y_max);

    if( crossPoint.x1() == DBL_MAX )
      crossPoint = P(0,0);

    x.crossPoint = crossPoint.x1();
    y.crossPoint = crossPoint.x2();

    // Horizontal axis
    if( x.grid )
    {
      if( x.firstline == -DBL_MAX )
        x.firstline = ll.x1();
      if( x.lastline == DBL_MAX )
        x.lastline = ur.x1();
      if( x.linestart == -DBL_MAX )
        x.linestart = ll.x2();
      if( x.lineend == DBL_MAX )
        x.lineend = ur.x2();
      if( x.boldpos == -DBL_MAX )
        x.boldpos = crossPoint.x1();
    }

    // Vertical axis
    if( y.grid )
    {
      if( y.firstline == -DBL_MAX )
        y.firstline = ll.x2();
      if( y.lastline == DBL_MAX )
        y.lastline = ur.x2();
      if( y.linestart == -DBL_MAX )
        y.linestart = ll.x1();
      if( y.lineend == DBL_MAX )
        y.lineend = ur.x1();
      if( y.boldpos == -DBL_MAX )
        y.boldpos = crossPoint.x2();
    }

    Coord::draw();
  }

} // end of namespace
