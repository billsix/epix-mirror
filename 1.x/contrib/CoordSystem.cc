/*
 * Extensions to ePiX made by
 *
 *    version 3
 *
 *    Svend Daugaard Pedersen
 *    mail: rot 13 Firaq at QnhtnneqCrqrefra dot qx
 *
 * Implementation of CoordSystem
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
  Coord::Coord()
  {
    ll = P(-DBL_MAX,-DBL_MAX);
    ur = P(DBL_MAX,DBL_MAX);
    crossPoint = P(DBL_MAX,DBL_MAX);
  }

  void Coord::draw()
  {
    printf("\n%%%% Coordinate system in box: [%g,%g] x [%g,%g]",
           ll.x1(), ur.x1(), ll.x2(), ur.x2());
    printf("\n%%%%");

    printf("\n%%%% horizontal axis:");
    xAxis->draw(P(ll.x1(),crossPoint.x2()),ur.x1()-ll.x1());
    printf("\n%%%% vertical axis:");
    yAxis->draw(P(crossPoint.x1(),ll.x2()),ur.x2()-ll.x2());
  }

} // end of namespace
