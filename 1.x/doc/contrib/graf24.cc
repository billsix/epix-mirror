#include "epix.h"

int main()
{
  DoubleLogCoord cs(1,10,1,10);

  picture(P(12,12));
  unitlength("5mm");

  begin();
  cs.draw();
  cs.marker(P(5,4),PLUS);
  cs.label(P(5,4),P(0,0),"ABCD",bl);
  end();

  return 0;
}
