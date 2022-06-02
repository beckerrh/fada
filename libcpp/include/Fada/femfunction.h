#ifndef __Fada_FemFunction_h
#define __Fada_FemFunction_h

#include  "Alat/node.h"
#include  "Alat/doublevector.h"
#include  "Alat/vector.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{                              
  class AssembleVector;
  class FemInterface;  
  
  class FemFunction
  {
  private:
    Alat::DoubleVector _u;
    Alat::Vector<Alat::Node> _du;

  public:
    ~FemFunction();
    FemFunction();
    FemFunction( const Fada::FemFunction& femfunction);
    FemFunction& operator=( const Fada::FemFunction& femfunction);
    std::string getName() const;
    FemFunction* clone() const;

    int ncomp() const;
    void set_size(int ncomp);
    void reInitAndEqual(const Fada::FemFunction& other);

    const Alat::DoubleVector& operator()() const;
    Alat::DoubleVector& operator()();
    const Alat::Vector<Alat::Node>& d() const;
    Alat::Vector<Alat::Node>& d();
    
    inline const double& operator()(int icomp) const
    {
      return _u[icomp];
    }
    inline double& operator()(int icomp)
    {
      return _u[icomp];
    }
    inline const Alat::Node& d(int icomp) const
    {
      return _du[icomp];
    }
    inline Alat::Node& d(int icomp)
    {
      return _du[icomp];
    }

  };
  std::ostream& operator<<(std::ostream& os, const FemFunction& g);
}

/*--------------------------------------------------------------------------*/

#endif
