/*
 * Extensions to ePiX made by
 *
 *    version 3
 *
 *    Svend Daugaard Pedersen
 *    mail: Svend@DaugaardPedersen.dk
 *
 * Implementation of CoordAxis
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
  // Default values define
  #define DEF_NAMEPOS NEGATIVE
  #define DEF_GRIDVAL false

  // Line thickness command strings
  char* normalThickness     = "\\thinlines";
  char* normalAxisThickness = "\\thinlines";
  char* boldAxisThickness   = "\\thicklines";
  char* boldGridThickness   = "\\thinlines";
  char* mediumGridThickness = "\\allinethickness{0.075mm}";
  char* fineGridThickness   = "\\allinethickness{0.03mm}";
  char* crossHatchThickness = "\\allinethickness{0.04mm}";

  Axis::Axis()
  {
    drawaxis = true;
    boldaxis = false;
    orientation = HORIZONTAL;
    grid = DEF_GRIDVAL;
    linestart = -DBL_MAX;
    lineend = DBL_MAX;

    name =  NULL;
    nampos = DEF_NAMEPOS;

    currentThickness = "";
  }

  void Axis::lineThickness(char* thicknessStr)
  {
    if( strcmp(thicknessStr,currentThickness) )
    {
      printf("\n%s%%",thicknessStr);
      currentThickness = thicknessStr;
    }
  }

} // end of namespace
