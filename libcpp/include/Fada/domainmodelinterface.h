#ifndef __Fada_DomainModelInterface_h
#define __Fada_DomainModelInterface_h

#include  "modelinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class DomainModelInterface : public virtual ModelInterface
  {
protected:
    std::string getInterfaceName() const;

public:
    ~DomainModelInterface();
    DomainModelInterface();
    DomainModelInterface( const DomainModelInterface& modelinterface);
    DomainModelInterface& operator=( const DomainModelInterface& modelinterface);
    virtual DomainModelInterface* clone() const = 0;

    typedef Alat::Map<std::string, Alat::StringSet> BoundaryConditionToVariables;
    virtual void definePossibleBoundaryConditions(BoundaryConditionToVariables& bdryconditions) const = 0;
    virtual std::string matrixcoupling(const std::string& variableout, const std::string& variablsin) const=0;
    // typedef Alat::Set<BoundaryConditionToVariables> BoundaryConditionToVariablesSet;
    // virtual void definePossibleBoundaryConditions(BoundaryConditionToVariablesSet& bdryconditions) const = 0;
    // virtual void defineApplication(const Alat::StringSet& bdryconditions) = 0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
