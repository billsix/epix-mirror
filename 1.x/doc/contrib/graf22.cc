#include "epix.h"
using namespace ePiX;
using namespace ePiX_contrib;

triple corners[] = {P(-2,-3),P(-3,1),P(1,0),P(2,3),P(4,-2),P(-1,-1)};

int main()
{
  bounding_box(P(-4,-3),P(4,2)); // [-4,4] x [-3,2]
  picture(P(12,7.5));            // picture is 12 x 7.5 units

  unitlength("5mm");

  begin();

  hatch_polygon(45,0.2,6,corners);

  end();

  return 0;
}
