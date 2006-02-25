#include "epix2.h"

using namespace ePiX2;

Camera camera;

Point f1(double t)
{
  return Point(Sin(10*t), 3*Cos(10*t), Sin(20*t));
}

Point f2(double t)
{
  return Point(1+2*Sin(13*t), 1-3*Cos(7*t), 1-Sin(17*t));
}

Point f3(double t)
{
  return Point(2*Sin(12*t)-1, 3-2*Cos(3*t)-2, Sin(5*t)-1);
}

Point F(double x, double y)
{
  //  return Point(exp(x)*Sin(90*y), exp(x)*Cos(90*y), x);
  return Point(Cos(180*x)*Cos(90*y), Sin(180*x)*Cos(90*y), Sin(90*y));
}

/*
static void print(const Color& col)
{
  std::cerr << col.name() << std::endl;
}
*/
/*
static void print(std::string msg, const Point& p1)
{
  std::cerr << msg << "\n"
	    << "  (" << p1.x1() << "," << p1.x2() << "," << p1.x3() << ")\n";
}

static void print(const Point& p1, const Point& p2, const Point& p3)
{
  std::cerr << "Vertices:\n"
	    << "  (" << p1.x1() << "," << p1.x2() << "," << p1.x3() << ")\n"
	    << "  (" << p2.x1() << "," << p2.x2() << "," << p2.x3() << ")\n"
	    << "  (" << p3.x1() << "," << p3.x2() << "," << p3.x3() << ")\n";
}
*/

int main()
{
  //#define LEN_TEST
  //#define SIL_TEST
  //#define CAM_TEST // src-test
  //#define SHARD_TEST
#define SHARD_TEST2
  //#define COLOR_TEST
  //#define LABEL_TEST
  //#define OBJ_TEST
  //#define COLOR_SEP_TEST
  //#define WORLD_TEST
  //#define TRI_TEST
  //#define TRI_TEST2

#ifdef TRI_TEST2

  camera.at(Point(6,-2,-3));
  //  camera.filter(White());

  World world;

  Screen screen(Pair(-2,-2), Pair(2,2));
  Picture picture(Pair(-2,-2), Pair(2,2), "5 x 5in");

  world.line_color(Green(0.6)); //RGB(0.7,0,0.9));
  world.line_width(Length("0.4pt"));
  world.fill_color(Blue(1.5));
  world.back_color(Green(0.6));
  //  world.fog(0.5);


  Triangle* T = new Triangle(Point(1,0,0), Point(-1,0,0), Point(0,0,sqrt(3)));

  Object tri(T);

  //  tri.line_color(Black());
  //  tri.fill_color(Red());

  world << tri;
  /*
  Vector axis(Origin, 0, 1, 1);

  for (int i=0; i<24; ++i)
    {
      tri.rotate(15, axis);
      world << tri;
    }
  */
  world.photo(screen, camera);
  picture.paste(screen, Rect(Pair(-2,-2), Pair(2,2)));
  //  picture.crop();
  print(picture, "outtest.eepic",  PSTricks());

#endif // TRI_TEST2


#ifdef TRI_TEST
  camera.at(Point(6,-2,3));
  camera.filter(White());

  World world;

  Screen screen(Pair(-2,-2), Pair(2,2));
  Picture picture(Pair(-2,-2), Pair(2,2), "5 x 5in");

  world.line_color(Blue(1.5)); //RGB(0.7,0,0.9));
  world.line_width(Length("0.4pt"));
  world.fill_color(Blue(1.5));
  world.back_color(Blue(-1.5));

  int Nx=64, Ny=32;
  double u, v, du=2.0/Nx, dv=2.0/Ny;

  for (int i=0; i<Nx; ++i)
    for (int j=0; j<Ny; ++j)
      {
	u = -1 + i*du;
	v = -1 + j*dv;

	world << Object(new Triangle(F(u,v), F(u+du,v), F(u+du,v+dv)));
	world << Object(new Triangle(F(u,v), F(u+du,v+dv), F(u,v+dv)));
      }

  world.photo(screen, camera);
  picture.paste(screen, Rect(Pair(-2,-2), Pair(2,2)));
  //  picture.crop();
  print(picture, "outtest.ps",  PS());

#endif // TRI_TEST

#ifdef WORLD_TEST

  camera.at(Point(6,4,3));
  camera.filter(White());

  World world;

  Screen screen(Pair(-2,-2), Pair(2,2));
  Picture picture(Pair(-2,-2), Pair(2,2), "5 x 5in");

  //  world.line_color(Red(1.5)); //RGB(0.7,0,0.9));
  world.line_width(Length("1pt"));
  world.fill_color(Blue(1.5));
  world.back_color(Blue(-1.5));

  double side1 = 2, side2 = 2, side3 = 2;

  Point v_1n(-side1,0,0), v_1p(side1,0,0);
  Point v_2n(0,-side2,0), v_2p(0,side2,0);
  Point v_3n(0,0,-side3), v_3p(0,0,side3);

#define OCTA1
#define OCTA2
#define OCTA3
#define OCTA4

  Object T(new Triangle(Point(1,-2,-1), Point(1,2,-1), Point(1,0,2)));
  world << T;

#ifdef OCTA1
  Triangle* f_ppp = new Triangle(v_1p, v_2p, v_3p);
  Triangle* f_ppn = new Triangle(v_2p, v_1p, v_3n);
  Object T_ppp(f_ppp);
  Object T_ppn(f_ppn);
  world << T_ppp << T_ppn;
#endif

#ifdef OCTA2
  Triangle* f_pnp = new Triangle(v_2n, v_1p, v_3p);
  Triangle* f_pnn = new Triangle(v_1p, v_2n, v_3n);
  Object T_pnp(f_pnp);
  Object T_pnn(f_pnn);
  world << T_pnp << T_pnn;
#endif

#ifdef OCTA3
  Triangle* f_nnp = new Triangle(v_1n, v_2n, v_3p);
  Triangle* f_nnn = new Triangle(v_2n, v_1n, v_3n);
  Object T_nnp(f_nnp);
  Object T_nnn(f_nnn);
  world  << T_nnp << T_nnn;
#endif

#ifdef OCTA4
  Triangle* f_npp = new Triangle(v_2p, v_1n, v_3p);
  Triangle* f_npn = new Triangle(v_1n, v_2p, v_3n);
  Object T_npp(f_npp);
  Object T_npn(f_npn);
  world << T_npp << T_npn;
#endif

  world.photo(screen, camera);
  picture.paste(screen, Rect(Pair(-2,-2), Pair(2,2)));
  picture.crop();
  print(picture, "outtest.ps",  PS());
  //  print(picture, "outtest.eepic",  PSTricks());

#endif // WORLD_TEST


#ifdef OBJ_TEST

  camera.at(Point(20,-10,40));
  camera.filter(White());

  double side1 = 2, side2 = 2, side3 = 2;

  Point v_1n(-side1,0,0), v_1p(side1,0,0);
  Point v_2n(0,-side2,0), v_2p(0,side2,0);
  Point v_3n(0,0,-side3), v_3p(0,0,side3);

  Triangle* f_ppp = new Triangle(v_1p, v_2p, v_3p);
  Triangle* f_ppn = new Triangle(v_2p, v_1p, v_3n);

  Triangle* f_npp = new Triangle(v_2p, v_1n, v_3p);
  Triangle* f_npn = new Triangle(v_1n, v_2p, v_3n);

  Triangle* f_nnp = new Triangle(v_1n, v_2n, v_3p);
  Triangle* f_nnn = new Triangle(v_2n, v_1n, v_3n);

  Triangle* f_pnp = new Triangle(v_2n, v_1p, v_3p);
  Triangle* f_pnn = new Triangle(v_1p, v_2n, v_3n);

  Object T_ppp(f_ppp);
  Object T_ppn(f_ppn);

  Object T_pnp(f_pnp);
  Object T_pnn(f_pnn);

  Object T_npp(f_npp);
  Object T_npn(f_npn);

  Object T_nnp(f_nnp);
  Object T_nnn(f_nnn);

  World world;

  Screen screen(Pair(-2,-2), Pair(2,2));
  Picture picture(Pair(-2,-2), Pair(2,2), "5 x 5in");

  world.line_color(RGB(0.7,0,0.9));
  world.line_width(Length("0.5pt"));
  world.fill_color(Blue(-1.5));
  world.back_color(Blue(1.5));
  //  T.edge_width(Length("1pt"));

  world << T_ppp << T_ppn  << T_pnp << T_pnn 
	<< T_npp << T_npn  << T_nnp << T_nnn;

  world.photo(screen, camera);

  picture.paste(screen);

  picture.crop();
  print(picture, "outtest.eepic",  PSTricks());

#endif // OBJ_TEST


#ifdef COLOR_SEP_TEST
  Text_Box3 flag(Point(0,0,0), "Hello, World!");

  Text_Box3 flag_c(Point(2,3.5,0), "Cyan");
  Text_Box3 flag_m(Point(2,2.5,0), "Magenta");
  Text_Box3 flag_y(Point(2,1.5,0), "Yellow");
  Text_Box3 flag_n(Point(2,0.5,0), "White");

  flag.text(Cyan());
  flag.edge(Magenta());
  flag.fill(Yellow());

  flag.font(BFACE);
  flag.pad("3pt");
  flag.edge("1pt");

  Picture picture(Pair(0,0), Pair(1,4), "1 x 4in");

  camera.filter(Cyan());
  Tile T_c(flag.shoot(camera));

  camera.filter(Magenta());
  Tile T_m(flag.shoot(camera));

  camera.filter(Yellow());
  Tile T_y(flag.shoot(camera));

  camera.filter(CMY_White(0));
  Tile T_n(flag.shoot(camera));

  picture << Tile(flag_c.shoot(camera))
	  << Tile(flag_m.shoot(camera))
	  << Tile(flag_y.shoot(camera))
	  << Tile(flag_n.shoot(camera));

  Rect bord(Pair(-2,-2), Pair(2,2));
  Screen canvas_c(bord);
  canvas_c << T_c;

  Screen canvas_m(bord);
  canvas_m << T_m;

  Screen canvas_y(bord);
  canvas_y << T_y;

  Screen canvas_n(bord);
  canvas_n << T_n;

  picture.paste(canvas_c, Rect(Pair(0,3), Pair(1,4)));
  picture.paste(canvas_m, Rect(Pair(0,2), Pair(1,3)));
  picture.paste(canvas_y, Rect(Pair(0,1), Pair(1,2)));
  picture.paste(canvas_n, Rect(Pair(0,0), Pair(1,1)));

  print(picture, "outtest.eepic",  PSTricks());

#endif // COLOR_SEP_TEST


#ifdef OBJ_TEST_OLD

  Point p1(-1,0,0), p2(1,0,0), p3(0,1,2);

  Shard_Base T((p2-p1)*(p3-p1));
  T.add_edge(p1,p2);
  T.add_edge(p2,p3);
  T.add_edge(p3,p1);

  Tile_Base* sil = new Silhouette;

  for (int Ct=0; Ct < 72; ++Ct)
    {
      Point CAM=cylindrical(10, 5*Ct, 2);

      Picture picture(Pair(-2,-2), Pair(2,2), "5 x 5in");

      Silhouette* outline
	= new Silhouette(Green(0.6), Length(1), Blue(1.9), true);

      outline->add_edge(Pair(-2,-2), Pair( 2,-2));
      outline->add_edge(Pair( 2,-2), Pair( 2, 2));
      outline->add_edge(Pair( 2, 2), Pair(-2, 2));
      outline->add_edge(Pair(-2, 2), Pair(-2,-2));

      Tile bord(outline);

      picture << bord;

      camera.at(CAM);

      sil = T.photo(camera, Red(), Length("1pt"), Green(0.6), Blue(1.2),
		    true, false);
      Tile tri(sil);

      picture << tri;

      std::ostringstream ofile;
      ofile << "frame";
      if (Ct < 10)
	ofile << "0";

      ofile << Ct << ".eepic";
      std::string temp=ofile.str();
      const char* o=temp.c_str();

      picture.crop();
      print(picture, o,  PSTricks());
    }

#endif // OBJ_TEST_OLD

#ifdef LABEL_TEST
  Point CAM(0.72356,0,10);

#define BOX3

#ifdef BOX3
  Text_Box3 flag(Point(0,0,0), "Hello, World!");
  flag.rotate(30);
  flag.text(Magenta());
  flag.edge("1pt");
  flag.edge(Red());
  flag.fill(Yellow());
  flag.align(tl);
#endif
  Color tint=Red(1);
  Length plain(0.6);

  camera.filter(Yellow());

  camera.at(CAM);

  Picture picture(Pair(-2,-2), Pair(2,2), "5 x 5in");

  Outline* outline
    = new Outline(Green(0.6), Length(0.6));

  outline->add_edge(Pair(1,-2), Pair(2,-2));
  outline->add_edge(Pair(2,-2), Pair(2,-1));
  outline->add_edge(Pair(2,-1), Pair(1,-1));
  outline->add_edge(Pair(1,-1), Pair(1,-2));

  Silhouette* sil
    = new Silhouette(Green(1.5), Length(1), Blue(1.9), true);

  Silhouette* sil1 = new Silhouette(Red(), Length(1.5), Red(0.6), true);

  Silhouette* sil2 
    = new Silhouette(tint, Length(0.6), RGB(0.8, 0.7, 0.5), true);

  Pair p1(-1,-1), p2(1,1), p3(0,2), p4(0,-1);
  Pair cor1(-2,-2), cor2(2,-2), cor3(2,2), cor4(-2,2);

  sil->add_edge(cor1, cor2, true);
  sil->add_edge(cor2, cor3, true);
  sil->add_edge(cor3, cor4, true);
  sil->add_edge(cor4, cor1, true);

  sil1->add_edge(p1, p2, true);
  sil1->add_edge(p2, p3, true);
  sil1->add_edge(p3, p1, true);

  sil2->add_edge(p1, p2, true);
  sil2->add_edge(p2, p4, true);
  sil2->add_edge(p4, p1, true);

  Tile T(sil);
  Tile T1(sil1);
  Tile T2(sil2);
  //  Tile T4(outline);

#ifdef BOX3
  Tile T3(flag.shoot(camera));
#endif

  Rect bord(Pair(-2,-2), Pair(2,1.9));
  Screen canvas(bord);
  canvas << T1;// << T2;

#ifdef BOX3
  canvas << T3;
#endif

  Rect extract1(Pair(0,0), Pair(1,1));
  Rect extract2(Pair(0,0), Pair(2,2));
  Rect extract3(Pair(0,0), Pair(1,1.5));

  picture << T; // << T4;

  picture.paste(canvas);
  /*
  picture.paste(canvas, extract1, Rect(Pair(1,1), Pair(2,2)));
  picture.paste(canvas, extract2, Rect(Pair(-0.5,-1), Pair(1,0.5)));
  picture.paste(canvas, extract3, Rect(Pair(-2.5,-1), Pair(-1,1)));
  */
  picture.crop();
  print(picture, "outtest.eepic",  PSTricks());

#endif // LABEL_TEST

#ifdef COLOR_TEST
  Color tint=Red().fog(1);
  Color tint2=RGB(1, 0.3,0.4);
  Color filter=CMYK(1, 0.75, 0.5, 0.25);

  print(tint*=1.25);

  tint2=RGB(1, 0.3,0.4);
  print(tint);
  print(tint*=0.75);
  print(tint*=0.6666);

  tint2=RGB(1, 0.3,0.4);
  tint *= -1;
  print(tint);

  tint *= 0.75;
  print(tint);

  tint *= 0.6666;
  print(tint);

  print(filter);

  print(filter(tint));
  print(filter(tint2));

#endif // COLOR_TEST


#ifdef SHARD_TEST
  Plate plate(Vector(Origin, 1, 1, 1));
  Plate plate2(Vector(Origin, 0, 0, 1));

  Point CAM(0,0,10);

  Point vtx1(1,0,0), vtx2(0,1,0), vtx3(0,0,1);
  Point vtx4(-2,0,0), vtx5(2,1,0), vtx6(1,0,0);

  plate.add_edge(vtx1, vtx2);
  plate.add_edge(vtx2, vtx3);
  plate.add_edge(vtx3, vtx1);

  plate2.add_edge(vtx1, vtx4);
  plate2.add_edge(vtx4, vtx5);
  plate2.add_edge(vtx5, vtx6);
  plate2.add_edge(vtx6, vtx1);

  Rect mask(Pair(-2,-2), Pair(2,2));
  Rect mask2(Pair(0,0), Pair(1,1));

  Picture canvas2(Pair(-2,-2), Pair(2,2), "5in x 5in");

  camera.at(CAM);

  plate.line_color(Red());
  plate.line_width(Length("1pt"));
  plate.fill_color(Red(0.6).fog(0.5));

  plate2.line_color(Blue());
  plate2.line_width(Length("1pt"));
  plate2.fill_color(Blue().fog(3));

  plate.shoot(canvas2, camera, 0, Shard_Base::PHOTO);
  plate2.shoot(canvas2, camera, 0, Shard_Base::PHOTO);

  canvas2.crop();
  print(canvas2, "outtest.eepic", PSTricks());

#endif // SHARD_TEST

#ifdef SHARD_TEST2
  Point CAM(0,0,1);

  Picture canvas(Pair(-1,-1), Pair(1,1), "5in x 5in");

  camera.at(CAM);
  camera.filter(Red(1.9));
  int N=10;

  for (int i=0; i<N; ++i)
    {
      Point vtx1(-1,-1+i*2.0/N,0), vtx2(-1,-1+(i+1)*2.0/N,0);
      Point vtx4( 1,-1+i*2.0/N,0), vtx3( 1,-1+(i+1)*2.0/N,0);

      Plate plate(Vector(Origin, 0, 0, 1));

      Color tint=RGB(0.6,0.1,0.8).fog(i*2.0/N);
      plate.line_color(tint);
      plate.fill_color(tint);

      plate.add_edge(vtx1, vtx2);
      plate.add_edge(vtx2, vtx3);
      plate.add_edge(vtx3, vtx4);
      plate.add_edge(vtx4, vtx1);

      plate.shoot(canvas, camera, 0, Shard_Base::PHOTO);
    }

  print(canvas, "outtest.eepic", PSTricks());

#endif // SHARD_TEST2

#ifdef LEN_TEST
  Length len1a("12in");
  Length len1b("1cm");

  print(len1a);
  print(len1b);

  len1a+=len1b;
  print(len1a);
#endif // LEN_TEST

#ifdef SIL_TEST
  Silhouette* sil = new Silhouette;
  Silhouette* sil2 = new Silhouette;

  Color tint(Red(1));
  Length plain(0.6);

  Pair p1(-1,-1), p2(1,1), p3(0,2), p4(0,-1);

  sil->add_edge(Screen_Edge(p1,p2,tint, plain));
  (*sil2) = (*sil);

  sil->add_edge(Screen_Edge(p2,p3,tint, plain));
  sil->add_edge(Screen_Edge(p3,p1,tint, plain));

  sil2->add_edge(Screen_Edge(p2,p4,tint, plain));
  sil2->add_edge(Screen_Edge(p4,p1,tint, plain));

  Tile T1(sil);

  Rect mask(Pair(-2,-2), Pair(2,2));
  Rect mask2(Pair(0,0), Pair(2,2));

  //  sil->draw();

  Screen canvas(mask);
  Screen canvas2(mask2);
  sil2->sil();

  Tile T2(sil2);

  canvas << T1 << T2;

  Rect inset(Pair(0,0), Pair(2,2));

  canvas2.paste(canvas, Rect(Pair(0,0), Pair(1,1)), inset);

  /*
  Pair_Map aff=Affine(mask2, mask);
  sil.map_by(aff)->draw();
  */
  canvas.draw();
  canvas2.draw();
  /*
  Silhouette* crop1 = new Silhouette;
  Silhouette* crop2 = new Silhouette;

  crop1 = sil.crop_to(mask);
  crop2 = crop1->crop_to(mask2);

  crop1->draw();
  crop2->draw();
  */
#endif // SIL_TEST


#ifdef CAM_TEST
  //  Pair pt(2,0), vec(1,1), scale(2,1);
  //  Rect Box(Pair(-2,-1), Pair(1.5,1.25));
  //  Rect Box2(Pair(-1,-2), Pair(1,1.5));

  Vector axis(Origin, 1, 1, 1);
  Vector vec0(Origin, 2, 2, 2);
  Vector vec1(Origin, -2, 2, 2);
  Vector vec2(Origin, 2, -2, 2);
  Vector vec3(Origin, -2, -2, 2);

  epix_header();

  //  camera.orthog();
  //  camera.at(Point(4,3,2));
  //  camera.look_at(Point(0,2,0));
  camera.at(Point(4,1,2));
  camera.range(10);

  //  camera.fisheye();
  camera.roll(-5);
  /*
  camera.tilt(5);
  camera.roll(60);
  camera.pan(5);
  camera.tilt(-5);
  */

  grid(8,8);

  vector_arrow(axis);
  vector_arrow(axis^E_1());
  vector_arrow(axis^E_2());
  vector_arrow(axis^E_3());

  /* Color testing
  camera.filter(Color(0.3,0.6,0.9));

  Color tint=camera(Red(-1));
  */

  /*
  vector_arrow((0.5*vec0)%E_1());
  vector_arrow((0.5*vec0)%E_2());
  vector_arrow((0.5*vec0)%E_3());

  vector_arrow((0.5*vec0)&E_1());
  vector_arrow((0.5*vec0)&E_2());
  vector_arrow((0.5*vec0)&E_3());

  vector_arrow((0.5*vec0)^E_1());
  vector_arrow((0.5*vec0)^E_2());
  vector_arrow((0.5*vec0)^E_3());
  */

  /*
  vector_arrow(0.5*vec1);
  vector_arrow(0.5*vec2);
  vector_arrow(0.5*vec3);
  */

  //  print(Box);

  /*
  Pair tmp(0.8,0.605), val(0.625,0);
  for (int i=0; i<100; ++i)
    if (!Box.hides((val*=tmp)&scale))
      {
	pair_arrow(val&scale);
	//	pair_arrow((val)^Pair(1,0));
      }
  */

  /*
  pair_arrow(pt);
  pair_arrow(pt*vec);
  pair_arrow(vec*pt);

  pair_arrow(pt%vec);
  pair_arrow(pt^vec);

  pair_arrow(vec%pt);
  pair_arrow(vec^pt);
  */

  epix_footer();
#endif // CAM_TEST
}
