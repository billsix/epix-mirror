#include "epix2.h"
using namespace ePiX2;

//#define TRIANGLES     // an octahedron with retracted faces
//#define TETRAHEDRA    // two complementary regular tetrahedra
//#define DISK_TEST     // stack of disks
//#define CIRC_TEST     // bouquet of circles
//#define POLYGON_TEST  // a cube and octahedron
//#define IMPLODE_FACES // use in conjunction with POLYGON_TEST
//#define POLYGON2_TEST // 3 golden rectangles
//#define SPHERE_TEST
//#define CUBE_TEST     // 3 intersecting cubes
#define BLOCK_TEST    // miscellaneous boxes

#define FACE_COLOR
#define EDGE_COLOR

#define SOLID // solid faces with face-normal shading
//#define FLAT  // solid faces with flat shading
//#define XRAY  // edges only, no hidden removal

//#define MULTI_IMAGE // uncomment to make a small stereo pair

// to be put into some initialization code eventually
namespace ePiX2{
  double epix_angle_units = M_PI/180;
}

int main() { 

  // declare a blank (3-D) "slate"
  Picture world; 

  // analogous to old "bounding_box"
  world.bounding_box(Pair(-2,-2), Pair(2,2));

  // same as old "picture"
  picture(Pair(6,6));

  // orthogonal, fisheye, bubble also available
  world.lens(perspective);

#ifndef MULTI_IMAGE
  world.view_from(20, 10, 6);
#endif

#ifdef CUBE_TEST

  Cube block0, block1, block2, block3;

#ifdef FACE_COLOR
  block1.red(0.8);
  block2.rgb(0.9, 0.7, 0.5);
  block3.rgb(0.5, 0.4, 0.3);
#endif

#ifdef EDGE_COLOR
  block0.green0();
  block1.red0(0.8);
  block2.rgb0(0.9, 0.7, 0.5);
  block3.rgb0(0.5, 0.4, 0.3);
#endif

  block0.skeleton();  
  block1.rotate(45, E_1());
  block2.rotate(45, E_2());
  block3.rotate(45, E_3());

  world << block1 << block2 << block3 << block0;

#endif // CUBE_TEST


#ifdef BLOCK_TEST

  Cube block0(Point(-0.5,-1,-0.1), Point(0.5, 1, 0.1));
  Cube block1(Point(-0.5,-1,-0.1), Point(0.5, 1, 0.1));
  Cube block2(Point(-0.5,-1,-0.1), Point(0.5, 1, 0.1));
  Cube block3(Point(-0.5,-1,-0.1), Point(0.5, 1, 0.1));

#ifdef FACE_COLOR
  block0.rgb(0.5, 0.4, 0.3);
  block1.red(0.8);
  block2.rgb(0.9, 0.7, 0.5);
  block3.rgb(0.7, 0.6, 0.5);
#endif

#ifdef EDGE_COLOR
  block0.red0(0.8);
  block1.red0(0.8);
  block2.yellow0();
  block3.yellow0();
#endif

  block0.rotate(90, E_3());
  block2.rotate(90, E_3());
  block0.rotate(30, Vector(Origin, 1, 1, 1));
  block1.rotate(60, Vector(Origin, -1, 1, 1));
  block2.rotate(60, Vector(Origin, 1, -1, 1));
  block3.rotate(60, Vector(Origin, 1, 1, -1));

  block2 += 0.5*E_1();

  world << block0 << block1 << block2 << block3;

#endif // BLOCK_TEST


#ifdef SPHERE_TEST

  Sphere S1(Origin, sqrt(3), 16, 8);
  Sphere S2(Point(1,1,1), 0.2, 6, 3);

  S1.rotate(30, Vector(Origin, 1, 1, 1));

  S1.red(0.6);
  S2.red();
  S1.yellow0();

  world << S1 << S2;

#endif // SPHERE_TEST


#ifdef CIRC_TEST
  const int N=4;

  /*
  world.pan(2);
  const double ht=1;
  Disk disk1(Point(0,0,-ht), sqrt((2+ht)*(2-ht)), E_3(Point(0,0,-ht)));
  Disk disk2(Point(0,0, ht), sqrt((2+ht)*(2-ht)), E_3(Point(0,0, ht)));
  Disk disk3(Origin, 2, E_3(Origin));

  disk1.red();
  disk2.red();
  disk3.red();

  world.add(&disk1);
  world.add(&disk2);
  world.add(&disk3);
  */

  std::vector<Disk> circs(N);

  for (int i=0; i<N; ++i)
    {
      circs.at(i)=Disk(Origin, 2, 
		       Vector(Origin, cos(i*M_PI/N), sin(i*M_PI/N), 0));
      circs.at(i).blue(0.6);
      circs.at(i).green0();
      world <<circs.at(i);
    }

#endif


#ifdef POLYGON_TEST

  double cb=1, sz = 2.0, disp, disp2;
#ifdef IMPLODE_FACES
  disp=-0.25, disp2=-0.5;
#else
  disp=disp2=0;
#endif

  Point v000(-cb,-cb,-cb), v100( cb,-cb,-cb);
  Point v010(-cb, cb,-cb), v110( cb, cb,-cb);
  Point v001(-cb,-cb, cb), v101( cb,-cb, cb);
  Point v011(-cb, cb, cb), v111( cb, cb, cb);

  // oriented by outward normals
  Quad f1a(v000, v001, v011, v010);
  Quad f1b(v111, v101, v100, v110);

  Quad f2a(v000, v100, v101, v001);
  Quad f2b(v111, v110, v010, v011);

  Quad f3a(v000, v010, v110, v100);
  Quad f3b(v111, v011, v001, v101);

  Point v1p(sz,0,0), v1n(-sz,0,0);
  Point v2p(0,sz,0), v2n(0,-sz,0);
  Point v3p(0,0,sz), v3n(0,0,-sz);

  Triangle face_ppp(v1p, v2p, v3p);
  Triangle face_pnp(v1p, v3p, v2n);
  Triangle face_nnp(v1n, v2n, v3p);
  Triangle face_npp(v1n, v3p, v2p);

  Triangle face_ppn(v1p, v3n, v2p);
  Triangle face_pnn(v1p, v2n, v3n);
  Triangle face_nnn(v1n, v3n, v2n);
  Triangle face_npn(v1n, v2p, v3n);

  face_ppp += Vector(Point( sz/3, sz/3, sz/3), disp, disp, disp);
  face_pnp += Vector(Point( sz/3,-sz/3, sz/3), disp,-disp, disp);
  face_nnp += Vector(Point(-sz/3,-sz/3, sz/3),-disp,-disp, disp);
  face_npp += Vector(Point(-sz/3, sz/3, sz/3),-disp, disp, disp);
  face_ppn += Vector(Point( sz/3, sz/3,-sz/3), disp, disp,-disp);
  face_pnn += Vector(Point( sz/3,-sz/3,-sz/3), disp,-disp,-disp);
  face_npn += Vector(Point(-sz/3, sz/3,-sz/3),-disp, disp,-disp);
  face_nnn += Vector(Point(-sz/3,-sz/3,-sz/3),-disp,-disp,-disp);

  f1a += -disp2*E_1(Point( cb,0,0));
  f1b +=  disp2*E_1(Point(-cb,0,0));

  f2a += -disp2*E_2(Point(0, cb,0));
  f2b +=  disp2*E_2(Point(0,-cb,0));

  f3a += -disp2*E_3(Point(0,0, cb));
  f3b +=  disp2*E_3(Point(0,0,-cb));

#ifdef FACE_COLOR
  face_ppp.red(0.8);
  face_pnp.red(0.8);
  face_npp.red(0.8);
  face_nnp.red(0.8);

  face_ppn.red(0.8);
  face_pnn.red(0.8);
  face_npn.red(0.8);
  face_nnn.red(0.8);

  f1a.rgb(0.5,0.7,0.6);
  f1b.rgb(0.5,0.7,0.6);

  f2a.rgb(0.5,0.7,0.7);
  f2b.rgb(0.5,0.7,0.7);

  f3a.rgb(0.5,0.7,0.8);
  f3b.rgb(0.5,0.7,0.8);
#endif // FACE_COLOR

#ifdef EDGE_COLOR
  f1a.cyan0();
  f1b.cyan0();

  f2a.cyan0();
  f2b.cyan0();

  f3a.cyan0();
  f3b.cyan0();

  face_ppp.yellow0();
  face_pnp.yellow0();
  face_npp.yellow0();
  face_nnp.yellow0();

  face_ppn.yellow0();
  face_pnn.yellow0();
  face_npn.yellow0();
  face_nnn.yellow0();
#endif // EDGE_COLOR

  world	<< f1a << f1b << f2a << f2b << f3a << f3b
	<< face_ppp << face_pnp	<< face_npp << face_nnp
	<< face_ppn << face_pnn	<< face_npn << face_nnn;

#endif // POLYGON_TEST


#ifdef POLYGON2_TEST

  double wd=1.0, ht=0.5*(1+sqrt(5));

  Point pt1a(  0, wd, ht), pt1b(  0,-wd, ht);
  Point pt1c(  0,-wd,-ht), pt1d(  0, wd,-ht);

  Point pt2a( ht,  0, wd), pt2b(-ht,  0, wd);
  Point pt2c(-ht,  0,-wd), pt2d( ht,  0,-wd);

  Point pt3a( wd, ht,  0), pt3b(-wd, ht,  0);
  Point pt3c(-wd,-ht,  0), pt3d( wd,-ht,  0);

  Quad face1(pt1a, pt1b, pt1c, pt1d);
  Quad face2(pt2a, pt2b, pt2c, pt2d);
  Quad face3(pt3a, pt3b, pt3c, pt3d);

#ifdef FACE_COLOR
  face1.rgb(0.6,0.7,0.8);
  face2.rgb(0.7,0.7,0.8);
  face3.rgb(0.7,0.6,0.8);
#endif

#ifdef EDGE_COLOR
  face1.green0(0.8);
  face2.green0(0.8);
  face3.green0(0.8);
#endif

  world << face1 << face2 << face3;

#endif // POLYGON2_TEST

#ifdef DISK_TEST

  Point pt1(0,0,0.5), pt2(0,0,1), pt3(0,0,1.5);

  Disk disk0(Origin, 2);
  Disk disk1(pt1, 1.75, Vector(pt1, 0,1,3));
  Disk disk2(pt2, 1.5, Vector(pt2, 0,1,2));
  Disk disk3(pt3, 1.25, Vector(pt3, 0,1,1));

#ifdef FACE_COLOR
  disk0.red(0.5);
  disk1.red(0.55);
  disk2.red(0.6);
  disk3.red(0.65);
#endif

  world << disk0 << disk1 << disk2 << disk3;

#ifndef MULTI_IMAGE
  Disk disk4(pt1, 1.75, E_1(pt1), false); // Circle
  Disk disk5(pt1, 1.5, E_1(pt1), false);
  Disk disk6(pt1, 1.25, E_1(pt1));
#ifdef FACE_COLOR
  disk4.red(0.5);
  disk4.blue0(0.9);

  disk5.red(0.5);
  disk5.blue0(0.9);

  disk6.blue(0.4);
  disk6.blue0(0.9);
#endif // FACE_COLOR
  world << disk4 << disk5 << disk6;
#endif

#endif // DISK_TEST

#ifdef TETRAHEDRA
  Point v000(-1,-1,-1), v100( 1,-1,-1), v010(-1, 1,-1), v110( 1, 1,-1);
  Point v001(-1,-1, 1), v101( 1,-1, 1), v011(-1, 1, 1), v111( 1, 1, 1);

  Triangle face1a(v000, v011, v101);
  Triangle face1b(v000, v101, v110);
  Triangle face1c(v000, v110, v011);
  Triangle face1d(v011, v101, v110);

  Triangle face2a(v111, v100, v010);
  Triangle face2b(v111, v010, v001);
  Triangle face2c(v111, v001, v100);
  Triangle face2d(v100, v010, v001);

#ifdef FACE_COLOR
  face1a.red(0.6);
  face1b.red(0.6);
  face1c.red(0.6);
  face1d.red(0.6);

  face2a.green(0.7);
  face2b.green(0.7);
  face2c.green(0.7);
  face2d.green(0.7);
#endif

  world << face1a << face1b << face1c << face1d 
	<< face2a << face2b << face2c << face2d;

#endif // TETRAHEDRA


#ifdef TRIANGLES
  double sz = 2.0;
  double disp=-0.2;

  Point v1p(sz,0,0), v1n(-sz,0,0);
  Point v2p(0,sz,0), v2n(0,-sz,0);
  Point v3p(0,0,sz), v3n(0,0,-sz);

  /*
  const int N=20;
  double sz2=1.25;
  std::vector<Segment> grid(2*N+2);
  for (int i=0; i<=N; ++i)
    {
      grid.at(i)=Segment(Point(sz2*(-1+i*2.0/N), -sz2, 0.25*sz2),
			 Point(sz2*(-1+i*2.0/N),  sz2, 0.25*sz2));

      grid.at(i+1+N)=Segment(Point(-sz2, sz2*(-1+i*2.0/N), 0.25*sz2),
			     Point( sz2, sz2*(-1+i*2.0/N), 0.25*sz2));
    }
  */


  Triangle face_ppp(v1p, v2p, v3p);
  Triangle face_pnp(v1p, v3p, v2n);
  Triangle face_nnp(v1n, v2n, v3p);
  Triangle face_npp(v1n, v3p, v2p);

  Triangle face_ppn(v1p, v3n, v2p);
  Triangle face_pnn(v1p, v2n, v3n);
  Triangle face_nnn(v1n, v3n, v2n);
  Triangle face_npn(v1n, v2p, v3n);

  face_ppp += Vector(Point( sz/3, sz/3, sz/3), disp, disp, disp);
  face_pnp += Vector(Point( sz/3,-sz/3, sz/3), disp,-disp, disp);
  face_nnp += Vector(Point(-sz/3,-sz/3, sz/3),-disp,-disp, disp);
  face_npp += Vector(Point(-sz/3, sz/3, sz/3),-disp, disp, disp);
  face_ppn += Vector(Point( sz/3, sz/3,-sz/3), disp, disp,-disp);
  face_pnn += Vector(Point( sz/3,-sz/3,-sz/3), disp,-disp,-disp);
  face_npn += Vector(Point(-sz/3, sz/3,-sz/3),-disp, disp,-disp);
  face_nnn += Vector(Point(-sz/3,-sz/3,-sz/3),-disp,-disp,-disp);

#ifdef FACE_COLOR
  double my_r=1, my_g=0.9, my_b=0.7;
  face_ppp.rgb(my_r, my_g, my_b);
  face_pnp.rgb(my_r, my_g, my_b);
  face_nnp.rgb(my_r, my_g, my_b);
  face_npp.rgb(my_r, my_g, my_b);

  face_ppn.rgb(my_r, my_g, my_b);
  face_pnn.rgb(my_r, my_g, my_b);
  face_nnn.rgb(my_r, my_g, my_b);
  face_npn.rgb(my_r, my_g, my_b);

  /*
  face_pnp.rgb(0.8,0.8,0.8);
  face_npn.cmyk(0.3, 0.2,0.2,0);
  */
#endif

#ifdef EDGE_COLOR
  /*
  for (int i=0; i <= N; ++i)
    {
      grid.at(i).blue0(0.7);
      grid.at(i+1+N).green0(0.7);
    }
  */

  face_ppp.red0();
  face_pnp.red0();
  face_nnp.red0();
  face_npp.red0();

  face_ppn.red0();
  face_pnn.red0();
  face_nnn.red0();
  face_npn.red0();
#endif

  world << face_ppp << face_pnp << face_npp << face_nnp 
	<< face_ppn << face_pnn << face_npn << face_nnn;

  /*
  for (int i=0; i <= 2*N+1; ++i)
    world << grid.at(i);
  */

#endif // end of TRIANGLES

  /******** End of scenery; process image **********/

#ifndef MULTI_IMAGE

  // "draw" the world in the screen
#ifdef SOLID
  world.expose();
#endif

#ifdef FLAT
  world.expose(SHADE_FLAT);
#endif

#ifdef XRAY
  world.x_ray();
#endif

  // In all cases, export world.screen to "the page" and print output file
  world.print();

#else

  world.view_from(20,10,6);
  world.range(10);
#ifdef SOLID
  world.expose();
#endif

#ifdef FLAT
  world.expose(SHADE_FLAT);
#endif

#ifdef XRAY
  world.x_ray();
#endif

  paste(world, Pair(0,0), Pair(2.5,2.5));
  world.clear();

  world.view_from(20,8,6);
  world.range(10);
#ifdef SOLID
  world.expose();
#endif

#ifdef FLAT
  world.expose(SHADE_FLAT);
#endif

#ifdef XRAY
  world.x_ray();
#endif

  paste(world, Pair(2.5, 0), Pair(5,2.5));
  world.clear();

  // uncommenting these overflows some PostScript stack(s) 
  // on my (ancient) test platform
  /*
  world.expose();
  paste(world, Pair(0, 0), Pair(2.75,2.75));
  world.clear();

  world.expose();
  paste(world, Pair(3.25, 0), Pair(6,2.75));
  world.clear();
  */

  /*
  world.expose();
  paste(world, Pair(0,3.5), Pair(1.5,5));
  world.clear();


  world.expose();
  paste(world, Pair(1.75,3.5), Pair(3.25,5));
  world.clear();

  world.expose();
  paste(world, Pair(3.5,3.5), Pair(5,5));
  world.clear();


  world.expose();
  paste(world, Pair(0,1.75), Pair(1.5,3.25));
  world.clear();

  world.expose();
  paste(world, Pair(1.75,1.75), Pair(3.25,3.25));
  world.clear();

  world.expose();
  paste(world, Pair(3.5,1.75), Pair(5,3.25));
  world.clear();


  world.expose();
  paste(world, Pair(0,0), Pair(1.5,1.5));
  world.clear();

  world.expose();
  paste(world, Pair(1.75,0), Pair(3.25,1.5));
  world.clear();

  world.expose();
  paste(world, Pair(3.5,0), Pair(5,1.5));
  world.clear();
  */

  // print "the page" to the output file
  page.print();

#endif // end of MULTI_IMAGE

  return 0;
}
