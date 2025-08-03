/*
 * std_F.h -- A class to draw the action of an affine transformation.
 *
 * This file is a C++ class tutorial (illustrating basic features), as
 * well as sample code for a small library, and style guide to writing
 * external modules for ePiX.
 *
 * Last Change: September 23, 2007
 */

/*
 * This file provides the "std_F" class, which draws the image of an "F"
 * (the first roman letter having no symmetries) under an affine map.
 *
 *   std_F();    // An F in the unit square [0,1] x [0,1].
 *   map_by(af); // Apply the affine map af
 *
 *   // Set the background, foreground, border style
 *   backing(color);
 *   fill(folor);
 *   border(color, width);
 *
 *   draw();     // Draw in the active screen
 *
 * The file affine.xp illustrates use.
 */
// "MINIMAL" compilation works, and entails fewer dependencies,
// but places a larger burdern on the programer
#ifdef MINIMAL
#include "epix/pairs.h"
#include "epix/Color.h"
#else

#include "epix.h"
#endif

namespace ePiX {

  // forward declarations if we don't include epix.h
#ifdef MINIMAL
  class affine;
  class P;
#endif

  // declare a new class "std_F"
  class std_F {
    // The "public" part of a class declaration specifies the interface:
  public:
    // A "constructor";  creates the standard F in the unit square
    std_F();

    // apply the affine map af; return a reference to this object so
    // calls can be daisy-chained
    std_F& map_by(const affine& af);

    // set the background and foreground colors
    std_F& backing(const Color&);
    std_F& fill(const Color&);

    // set outline pen
    std_F& border(const Color&, double);

    void draw() const; // draw in the active screen

    // The private section contains implementation data, etc.  The
    // shape of the F is not stored in the class data, but generated
    // at runtime by draw().
  private:
    pair m_loc; // lower left corner
    pair m_e1;  // lower right corner
    pair m_e2;  // upper left corner

    Color m_fore;
    Color m_back;
    Color m_edge;

    double m_edge_width;

    // for internal use; convert (x,y) in [0,1] x [0,1] to a location
    P pr(double x, double y) const;
  }; // end of std_F; class declarations must end with a semicolon

} // end of namespace
