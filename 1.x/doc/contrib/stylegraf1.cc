#include "epix.h"
using namespace ePiX;
using namespace ePiX_contrib;

#include <math.h>

int main()
{
  HorizLogAxis axis;

  bounding_box(P(-2.2,0),P(3,1)); // [-2.2,3] x [0,1]
  picture(P(13,0.5));             // picture is 13 x 1 units
  unitlength("1cm");

  /* Start picture */
  begin();

  axis.stdstyle = 1;
  axis.draw(P(-1,0.35),4);
  label(P(-2.2,0.35),P(0,-3),"$stdstyle=1$:");

  /* End picture */
  end();

  return 0;
}
