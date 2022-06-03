#ifndef __Fada_PreconditionerSelector_h
#define __Fada_PreconditionerSelector_h

#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class PreconditionerInterface;
  class SystemMatrixInterface;
}
namespace Fada
{
  class PreconditionerSelector : public Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~PreconditionerSelector();
    PreconditionerSelector();
    PreconditionerSelector( const PreconditionerSelector& preconditionerselector);
    PreconditionerSelector& operator=( const PreconditionerSelector& preconditionerselector);
    std::string getName() const;
    PreconditionerSelector* clone() const;
    Alat::PreconditionerInterface* newPreconditioner(std::string name, std::string sorter, const Alat::VariableMatrixInterface* matrix, int level, int nlevels, int dimension) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
