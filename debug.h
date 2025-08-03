#ifndef EPIX_DEBUG
#define EPIX_DEBUG

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>

#include "pairs.h"
#include "triples.h"

#include "errors.h"

namespace ePiX
{
  inline void debug_msg(const std::string& msg)
    {
      std::cerr << msg << std::endl;
    }

  inline void debug_print(const pair& arg, const std::string& msg="")
    {
      std::cerr << msg << " (" 
		<< arg.x1() << ", " 
		<< arg.x2() << ")" << std::endl;
    }

  inline void debug_print(const P& arg, const std::string& msg="")
    {
      std::cerr << msg << " (" 
		<< arg.x1() << ", " 
		<< arg.x2() << ", " 
		<< arg.x3() << ")" << std::endl;
    }

}
#endif /* EPIX_DEBUG */
