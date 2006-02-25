/***
 ***  Handle.h -- General-purpose Handle class
 ***
 ***  Adapted from Stroustrup, "The C++ Programming Language", pp. 782ff
 ***/
#ifndef EPIX2_HANDLE
#define EPIX2_HANDLE

namespace ePiX2 {
  template <class T> class Handle {
    T* rep;
    int* pcount;

  public:
    Handle(T* pp) : rep(pp), pcount(new int(1)) { }
    Handle(const Handle& r) : rep(r.rep), pcount(r.pcount) { (*pcount)++; }
    Handle& operator= (const Handle& r)
      {
	if (rep == r.rep)
	  return *this;

	if (--(*pcount) == 0)
	  {
	    delete rep;
	    delete pcount;
	  }

	rep = r.rep;
	pcount = r.pcount;
	(*pcount)++;
	return *this;
      }

    ~Handle(void)
      {
	if (--(*pcount) == 0)
	  {
	    delete rep;
	    delete pcount;
	  }
      }

    T* operator->() const { return rep; }

    void bind(T* pp)
      {
	if (pp != rep)
	  {
	    if (--*pcount == 0)
	      {
		delete rep;
		*pcount = 1;
	      }

	    else
	      pcount = new int(1);

	    rep = pp;
	  }
      }

    bool operator== (const T& r) const
      {
	return (*rep) == *(r.rep);
      }
    bool operator!= (const T& r) const
      {
	return (*rep) != *(r.rep);
      }
  }; // end of class Handle

} /* end of namespace */

#endif /* EPIX2_HANDLE */
