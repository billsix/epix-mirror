#include "epix.h"

int main()
{
  CartesianCoord cs(-3,5,-2,6);

  picture(P(12,12));
  unitlength("5mm");

  begin();

  cs.xName("t/s",POSITIVE);
  cs.yName("E/kJ",NEGATIVE);
  cs.draw();

  end();

  return 0;
}




