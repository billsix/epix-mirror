#include "epix.h"
using namespace ePiX;
using namespace ePiX_contrib;

double f(double x)
{
  return 4.5-(x-3)*(x-3)/2;
}

int main()
{
  CartesianCoord cs(-1,7,-2,6);

  picture(P(12,12));
  unitlength("5mm");

  begin();

  cs.draw();
  cs.hatchArea(150,0.2,f,0,5.5,100);

  end();

  return 0;
}
