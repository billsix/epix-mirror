#include <epix.h>

int main()
{
  CartesianCoord cs(-2,4,-1,5);

  picture(P(12,12));
  unitlength("5mm");

  begin();

  cs.grid();
  cs.draw();

  end();

  return 0;
}

