double clip_0_1(double t)
{
  return (t<0) ? 0 : (t>1 ? 1 : t);
}

double trunc(double t)
{
  if (fabs(t) < 0.0001)
    t=0;

  return t;
}

namespace ePiX {

  class Spot {
  public:
    Spot(const Color& col, const P& loc) : m_tint(col), m_loc(loc) { }

    Color m_tint;
    P m_loc;
  };

  class Fog {
  public:
    Fog(const Color& c, const double k) : m_tint(c), m_dens(k) { }

    Color apply_to(const Color& col, const double dist) const
    {
      Color me(m_tint);
      return me.blend(col, exp(-dist*m_dens));
    }

  private:
    Color m_tint;
    double m_dens;
  };

  // cf. facet class in facet.h
  class Chip {
  public:
    Chip(const P& p1, const P& p2, const P& p3, const P& p4, const double s,
	 const Color& c)
      : pt1(p1), pt2(p2), pt3(p3), pt4(p4), m_shine(s), m_tint(c)
    {
      m_ctr = 0.25*(p1+p2+p3+p4);
      P n = (p2-p1)*(p4-p1);
      n *= recip(norm(n));
      m_perp=n;
    }

    double how_far() const { return norm(camera.viewpt()-m_ctr); }

    void draw(const ePiX::Fog& fog,
	      const ePiX::Spot& spot,
	      const ePiX::Color& ambient) const
    {
      fill(appearance(camera.viewpt(), fog, spot, ambient));
      ePiX::quad(pt1, pt2, pt3, pt4);
    }

  private:
    P pt1, pt2, pt3, pt4; // vertices
    double m_shine; // shininess
    Color m_tint; // out color

    P m_ctr; // our location
    P m_perp; // unit normal

    Color appearance(const P& viewer, const Fog& fog,
		     const Spot& spot, Color ambient) const
    {
      P spot_dir(spot.m_loc - m_ctr);
      double spot_dist(norm(spot_dir));
      spot_dir *= recip(spot_dist);

      P view_dir(viewer - m_ctr);
      double view_dist(norm(view_dir));
      view_dir *= recip(view_dist);

      // calculate relative intensities
      //double I_ambt = pow((m_perp|view_dir), 2); // cos^2 of normal angle
      double I_ambt(fabs(m_perp|view_dir)); // |cos| of normal angle

      // crude but fairly effective
      //double I_spot = 0.5*(1+((spot_dir*m_perp)|(m_perp*view_dir)));

      // reflect spot_dir across m_perp in the plane they span,
      // then take (cos(angle to viewer)^4)
      P refl_dir(-spot_dir + 2*(spot_dir|m_perp)*m_perp);
      double I_spot(pow((refl_dir|view_dir), 2));


      // light from spot that reaches our location and is reflected to viewer
      Color spot_refl(I_spot*(fog.apply_to(spot.m_tint, spot_dist)));

      // not shiny -> reflect mostly ambient
      Color refl(I_ambt*ambient.blend(spot_refl, m_shine));

      // Color we reflect toward viewer
      Color source(m_tint.filter(refl));
      return fog.apply_to(source, view_dist);
    }
  };

  class chip_distance {
  public:
    bool operator() (const Chip& arg1, const Chip& arg2)
      {
	return arg1.how_far() > arg2.how_far();
      }
  };
} // end of namespace

