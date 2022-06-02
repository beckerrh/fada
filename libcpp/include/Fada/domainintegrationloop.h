#ifndef __Fada_DomainIntegrationLoop_h
#define __Fada_DomainIntegrationLoop_h

#include  "Fada/domainintegrationloopinterface.h"
#include  "Fada/localglobal.h"
#include  "Alat/systemassemblematrix.h"
#include  "integrationloop.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class SystemAssembleVector;
  class SystemAssembleMatrix;
}
namespace Alat
{
  class MatrixInterface;
  class VectorInterface;
  class SysetmMatrixInterface;
  class SystemVectorInterface;
}
namespace Fada
{
  class DomainSolverInterface;
  class VariableManager;
}

namespace Fada
{
  class DomainIntegrationLoop : public virtual Fada::DomainIntegrationLoopInterface, public virtual IntegrationLoop
  {
protected:
    Fada::LocalGlobal _localglobal;
    const Fada::VariableManager* _variablemanager;
    const Fada::DofManagerAllVariables* _dofmanagerallvariables;
    const Fada::DomainSolverInterface* _solver;
    mutable const Alat::SystemVectorInterface* _datavector;
    const Alat::VectorInterface* _data;

    // mutable Alat::SystemAssembleMatrix _AlocLL, _AlocLR, _AlocRL, _AlocRR;

    template<class VISITOR>
    void integratePerLevelCells(AlatEnums::residualstatus& status, VISITOR& visitor, int level, const Fada::LocalGlobal::VectorMap& vectormap, Fada::FemFunctionsMap& femfctmap) const;
    template<class VISITOR>
    void integratePerLevelBoundarySides(AlatEnums::residualstatus& status, VISITOR& visitor, int level, const Fada::LocalGlobal::VectorMap& vectormap, Fada::FemFunctionsMap& femfctmap) const;
    template<class VISITOR>
    void integratePerLevelInteriorSides(AlatEnums::residualstatus& status, VISITOR& visitor, int level, const Fada::LocalGlobal::VectorMap& vectormap, Fada::FemFunctionsMap& femfctmapL, Fada::FemFunctionsMap& femfctmapR) const;

    const Fada::DomainSolverInterface* getSolver() const;
    const Fada::VariableManager* getVariableManager() const;
    void _fillVectorMap(Fada::LocalGlobal::VectorMap& vectormap, FadaEnums::term term, const Alat::SystemVectorInterface* u, const Alat::SystemVectorInterface* uold, const Alat::SystemVectorInterface* uveryold) const;

    void constructJacobianMatrix(int level, AlatEnums::residualstatus& status, Alat::SystemMatrixInterface* A, const Alat::SystemVectorInterface* u, const Alat::SystemVectorInterface* uold, const Alat::SystemVectorInterface* uveryold) const;

public:
    ~DomainIntegrationLoop();
    DomainIntegrationLoop();
    DomainIntegrationLoop( const DomainIntegrationLoop& domainintegrationloop);
    DomainIntegrationLoop& operator=( const DomainIntegrationLoop& domainintegrationloop);
    DomainIntegrationLoop* clone() const;
    std::string getName() const;
    void basicInit(const Fada::DomainSolverInterface* solver);
    void setVariablesScales(const Alat::StringDoubleVectorMap& variablescales);

    void constructRightHandSide(AlatEnums::residualstatus& status, Alat::VectorInterface* f, int level=0) const;
    void constructJacobianMatrix(AlatEnums::residualstatus& status,  Alat::MatrixInterface* A, const Alat::VectorInterface* u, const Alat::VectorInterface* uold = NULL, const Alat::VectorInterface* uveryold = NULL, int level=0) const;
    void constructForm(AlatEnums::residualstatus& status, Alat::VectorInterface* f, const Alat::VectorInterface* u, const Alat::VectorInterface* uold, const Alat::VectorInterface* uveryold, int level=0) const;
    void computeLinearization(AlatEnums::residualstatus& status, Alat::VectorInterface* f, const Alat::VectorInterface* u, const Alat::VectorInterface* du, const Alat::VectorInterface* uold, const Alat::VectorInterface* uveryold, int level=0) const;
    void initSolution(Alat::VectorInterface* u, int level=0) const;
    void postProcess(AlatEnums::residualstatus& status, Alat::VectorInterface* pp, const Alat::VectorInterface* u, const Alat::VectorInterface* uold, int level=0) const;
    void setDataVector(Alat::VectorInterface* data);
    void integrateTimeRhs(AlatEnums::residualstatus& status, Alat::VectorInterface* f, const Alat::VectorInterface* u, const Alat::VectorInterface* bdf, double d, int level=0) const;
    void computeNormSquared(AlatEnums::residualstatus& status, Alat::StringDoubleMap& norms, const Alat::VectorInterface* u, const Alat::VectorInterface* du, int level=0) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
