#ifndef __vecteurmg_h
#define __vecteurmg_h

#include  "array.hpp"
#include  "vector.hpp"

/*-------------------------------------------------*/
class VectorMG
{
private:
  Array<vector>  v_val;

public:

  void set_size(const VectorMG& u);

  Array<vector>&       val()       {return v_val;}
  const Array<vector>& val() const {return v_val;}
  vector&       operator()(int l)       {return v_val(l);}
  const vector& operator()(int l) const {return v_val(l);}
};

/*-------------------------------------------------*/
inline void VectorMG::set_size(const VectorMG& u)
{
  int lev = u.val().n();
  v_val.set_size(lev);
  for(int l=0;l<lev;l++)
    {
      (*this)(l).set_size(u(l).n());
    }
}

#endif