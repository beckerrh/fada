#ifndef __Fada_DomainIntegrationLoopInterface_h
#define __Fada_DomainIntegrationLoopInterface_h

#include  "integrationloopinterface.h"
#include  "Alat/enums.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class StringDoubleMap;
  class StringDoubleVectorMap;
}
namespace Alat
{
  class MatrixInterface;
  class VectorInterface;
}
namespace Fada
{
  class DomainSolverInterface;
  class MultiLevelVector;

  class DomainIntegrationLoopInterface : public virtual IntegrationLoopInterface
  {
protected:
    std::string getInterfaceName() const;

public:
    ~DomainIntegrationLoopInterface();
    DomainIntegrationLoopInterface();
    DomainIntegrationLoopInterface( const DomainIntegrationLoopInterface& domainintegrationloopinterface);
    DomainIntegrationLoopInterface& operator=( const DomainIntegrationLoopInterface& domainintegrationloopinterface);
    std::string getName() const;
    DomainIntegrationLoopInterface* clone() const;

    virtual void basicInit(const Fada::DomainSolverInterface* solver) = 0;
    virtual void setVariablesScales(const Alat::StringDoubleVectorMap& variablescales) = 0;

    virtual void constructRightHandSide(AlatEnums::residualstatus& status, Alat::VectorInterface* f, int level=0) const=0;
    virtual void constructJacobianMatrix(AlatEnums::residualstatus& status, Alat::MatrixInterface* A, const Alat::VectorInterface* u, const Alat::VectorInterface* uold=NULL, const Alat::VectorInterface* uveryold=NULL, int level=0) const=0;
    virtual void constructForm(AlatEnums::residualstatus& status, Alat::VectorInterface* f, const Alat::VectorInterface* u, const Alat::VectorInterface* uold, const Alat::VectorInterface* uveryold, int level=0) const=0;
    virtual void computeLinearization(AlatEnums::residualstatus& status, Alat::VectorInterface* f, const Alat::VectorInterface* u, const Alat::VectorInterface* du, const Alat::VectorInterface* uold, const Alat::VectorInterface* uveryold, int level=0) const=0;
    virtual void postProcess(AlatEnums::residualstatus& status, Alat::VectorInterface* pp, const Alat::VectorInterface* u, const Alat::VectorInterface* uold, int level=0) const=0;
    virtual void initSolution(Alat::VectorInterface* u, int level=0) const=0;
    virtual void setDataVector(Alat::VectorInterface* data)=0;
    virtual void integrateTimeRhs(AlatEnums::residualstatus& status, Alat::VectorInterface* f, const Alat::VectorInterface* u, const Alat::VectorInterface* bdf, double d, int level=0) const=0;
    virtual void computeNormSquared(AlatEnums::residualstatus& status, Alat::StringDoubleMap& norms, const Alat::VectorInterface* u, const Alat::VectorInterface* du, int level=0) const=0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
