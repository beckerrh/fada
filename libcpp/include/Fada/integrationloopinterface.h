#ifndef __Fada_IntegrationLoopInterface_h
#define __Fada_IntegrationLoopInterface_h

#include  "fadaenums.h"
#include  "Alat/interfacebase.h"
#include  "Alat/map.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class SparsityPattern;
}
namespace Alat
{
  class SystemMatrixInterface;
  class VariableMatrixInterface;
  class VectorInterface;
}
namespace Fada
{
  class IntegratorManager;
  class SolverInterface;

  class IntegrationLoopInterface : public virtual Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~IntegrationLoopInterface();
    IntegrationLoopInterface();
    IntegrationLoopInterface( const IntegrationLoopInterface& integrationloopinterface);
    IntegrationLoopInterface& operator=( const IntegrationLoopInterface& integrationloopinterface);

    virtual void matrixConnectivityPerLevel(const std::string& varnamei, const std::string& varnamej, const Fada::IntegratorManager* integratormanager, int level, int n, Alat::VariableMatrixInterface* matrix) const;
    virtual void matrixConnectivityPerLevel(const std::string& varnamei, const std::string& varnamej, const Fada::IntegratorManager* integratormanager, int level, int n, Alat::SparsityPattern* sparsitypattern) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
