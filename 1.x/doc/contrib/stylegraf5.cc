#include "epix.h"
using namespace ePiX;
using namespace ePiX_contrib;

#include <math.h>

int main()
{
  HorizLogAxis axis;

  bounding_box(P(-1.6,0),P(1,1)); // [-1.6,1] x [0,1]
  picture(P(13,0.5));             // picture is 13 x 1 units
  unitlength("1cm");

  /* Start picture */
  begin();

  axis.stdstyle = 5;
  axis.draw(P(-1,0.35),2);
  label(P(-1.6,0.35),P(0,-3),"$stdstyle=5$:");

  /* End picture */
  end();

  return 0;
}
