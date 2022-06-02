#ifndef __Fada_IterativeSolverSelector_h
#define __Fada_IterativeSolverSelector_h

#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class LinearSolverInterface;
}
namespace Fada
{
  class IterativeSolverSelector : public Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;
public:
    ~IterativeSolverSelector();
    IterativeSolverSelector();
    IterativeSolverSelector( const IterativeSolverSelector& iterativesolverselector);
    IterativeSolverSelector& operator=( const IterativeSolverSelector& iterativesolverselector);
    std::string getName() const;
    IterativeSolverSelector* clone() const;
    Alat::LinearSolverInterface* newSolver(std::string name) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
