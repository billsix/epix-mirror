#include "epix.h"
using namespace ePiX;
using namespace ePiX_contrib;

#include <math.h>

int main()
{
  HorizLogAxis axis;

  bounding_box(P(-0.3,0),P(1,1)); // [-2.2,1] x [0,1]
  picture(P(13,0.5));             // picture is 13 x 1 units
  unitlength("1cm");

  /* Start picture */
  begin();

  axis.stdstyle = 6;
  axis.draw(P(0,0.35),1);
  label(P(-0.3,0.35),P(0,-3),"$stdstyle=6$:");

  /* End picture */
  end();

  return 0;
}
