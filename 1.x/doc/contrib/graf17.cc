#include "epix.h"

double f(double x)
{
  return 1.5*pow(1.2,x);
}

int main()
{
  CartesianCoord cs(0,10,0,10);

  picture(P(12,12));
  unitlength("5mm");

  begin();

  cs.grid();
  cs.draw();

  cs.plot((*f),0,10,50);
  cs.label(P(5,f(5)),P(0,0),"$1.5\\cdot1.2^x$",br);

  end();

  return 0;
}

