#include "epix.h"

#include <math.h>

int main()
{
  HorizLogAxis axis;

  bounding_box(P(-1.9,0),P(2,1)); // [-1.9,2] x [0,1]
  picture(P(13,0.5));             // picture is 13 x 1 units
  unitlength("1cm");

  /* Start picture */
  begin();

  axis.stdstyle = 4;
  axis.draw(P(-1,0.35),3);
  label(P(-1.9,0.35),P(0,-3),"$stdstyle=4$:");

  /* End picture */
  end();

  return 0;
}
