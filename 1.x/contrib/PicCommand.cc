/*
 * Extensions to ePiX made by
 *
 *    version 3
 *
 *    Svend Daugaard Pedersen
 *    mail: rot 13 Firaq at QnhtnneqCrqrefra dot qx
 *
 * General picture commands
 * (only documented in header file)
 *
 * Last change February 7, 2004
 *
 */


/*
*  General picture command
*/

#include <cstring>
#include <cfloat>

#include "epix_ext.h"


using namespace ePiX;

namespace ePiX_contrib
{
  void picCommand(char* cmd,int numPnts,triple& point, ...)
  {
    picCommand(cmd,numPnts,&point);
  }

  void picCommand(char* cmd,int numPnts,triple* points)
  {
    printf("\n%s",cmd);

    for( int i = 0 ; i < numPnts ; i++ )
    {
      printf("(%g,%g)",h_scale(points->x1()),v_scale(points->x2()));
      if( ((i+1) % 4) == 0 )
      {
        printf("%%\n       ");
      }
      points++;
    }
  }

} // end of namespace
