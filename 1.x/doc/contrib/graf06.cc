#include "epix.h"

int main()
{
  CartesianCoord cs(-3,5,-2,6);

  picture(P(12,12));
  unitlength("5mm");

  begin();

  cs.axisBounds(P(4,5));
  cs.xName(CENTER);
  cs.yName(CENTER);
  cs.draw();

  end();

  return 0;
}





