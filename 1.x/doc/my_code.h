/* my_code.h */
#include <cmath>   // standard library math header
#include "epix.h"  // ePiX header
using ePiX::P;

namespace Mine {   // to avoid name conflicts
  // functions for special relativity
  double lorentz_norm(P);
  bool   spacelike(P);
} // end of namespace
