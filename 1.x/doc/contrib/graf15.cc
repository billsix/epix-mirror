#include "epix.h"

int main()
{
  DoubleLogCoord cs(0.1,10,1,100);

  picture(P(12,12));
  unitlength("5mm");

  begin();

  cs.logStyle(4);
  cs.grid();
  cs.draw();

  end();

  return 0;
}

