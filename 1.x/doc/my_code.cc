/* my_code.cc */
#include "my_code.h"
using namespace ePiX;

namespace Mine {
  double lorentz_norm(P arg)
  {
    double x=arg.x1(), y=arg.x2(), z=arg.x3(); // extract coords
    return -x*x + y*y + z*z;
  }
  bool spacelike(P arg)
  {
    return (lorentz_norm(arg) > 0);
  }
} // end of namespace
