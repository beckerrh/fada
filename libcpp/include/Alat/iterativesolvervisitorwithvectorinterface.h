#ifndef __Alat_IterativeSolverVisitorWithVectorInterface_h
#define __Alat_IterativeSolverVisitorWithVectorInterface_h

#include  "iterativesolvervisitorinterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class GhostVector;
}
namespace Alat
{
  class VariableMatrixInterface;
  class VariableVectorInterface;
  class SystemVector;

  class IterativeSolverVisitorWithVectorInterface : public Alat::IterativeSolverVisitorInterface
  {
public:
    ~IterativeSolverVisitorWithVectorInterface();
    IterativeSolverVisitorWithVectorInterface();
    IterativeSolverVisitorWithVectorInterface( const IterativeSolverVisitorWithVectorInterface& iterativesolvervisitorinterface);
    IterativeSolverVisitorWithVectorInterface& operator=( const IterativeSolverVisitorWithVectorInterface& iterativesolvervisitorinterface);
    std::string getName() const;
    IterativeSolverVisitorWithVectorInterface* clone() const; 
    
    virtual Alat::SystemVector* getVector(Alat::GhostVector& u) const=0;
    virtual const Alat::SystemVector* getVector(const Alat::GhostVector& u) const=0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
