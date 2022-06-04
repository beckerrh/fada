#ifndef __Fada_SolverInterface_h
#define __Fada_SolverInterface_h

#include  "fadalightenums.h"
#include  "Alat/interfacebase.h"
#include  "Alat/vector.h"
#include  "Alat/enums.h"
#include  <set>

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class GhostMatrix;
  class GhostVector;
  class GhostLinearSolver;
  class IoManager;
  class StringVector;
  class ParameterFile;
}
namespace Alat
{
  class SystemMatrixInterface;
  class VariableMatrixInterface;
}
namespace FadaMesh
{
  class MeshInterface;
}
namespace Fada
{
  class DofManagerAllVariables;
  class DofInformationInterface;
  class FemManagerInterface;
  class FemInterface;
  class FormIntegrationLoopInterface;
  class IntegratorInterface;
  class IntegratorManager;
  class LinearInfo;
  class MultiLevelTransferAllVariablesInterface;
  class NonLinearInfo;
  class TimeData;
  class VariableInterface;
  class VariableManager;

  class SolverInterface : public virtual Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;
    virtual Alat::VariableMatrixInterface* newVariableMatrix(int level, const std::string& storage, const Fada::VariableInterface* vari, const Fada::VariableInterface* varj) const = 0;
    virtual void reInitVariableMatrix(int level, Alat::VariableMatrixInterface* matrix, const Fada::VariableInterface* vari, const Fada::VariableInterface* varj) const;
    virtual void reInitLevelMatrix(int level, Alat::SystemMatrixInterface* A, const Alat::GhostMatrix& gmatrix) const = 0;
    virtual Alat::SystemMatrixInterface* newLevelMatrix(int level, const Alat::GhostMatrix& gmatrix) const = 0;

public:
    ~SolverInterface();
    SolverInterface();
    SolverInterface( const SolverInterface& solverinterface);
    SolverInterface& operator=( const SolverInterface& solverinterface);

    virtual IntegratorManager* getIntegratorManager()= 0;
    virtual void registerMatrix(const Alat::GhostMatrix& matrix) = 0;
    virtual int getNLevels() const = 0;
    virtual FadaEnums::looptype getType() const=0;

    virtual std::ostream& printLoopInformation(std::ostream& os) const = 0;
    virtual const FadaMesh::MeshInterface* getMesh() const = 0;
    virtual double getTime() const = 0;
    virtual double getDeltaT() const = 0;
    virtual void setTimeInfo(const Fada::TimeData& timedata, double masscoefimplicit) = 0;
    virtual void setTimeScheme(std::string time_discretization) = 0;
    virtual void reInit() = 0;
    virtual void reInitMatrices() = 0;
    // virtual void reInitInterpolation();
    virtual void constructJacobianMatrix(AlatEnums::residualstatus& status, Alat::GhostMatrix& A, const Alat::GhostVector& u) = 0;
    virtual void constructRightHandSide(AlatEnums::residualstatus& status, Alat::GhostVector& gf) const = 0;
    virtual void integrateTimeRhs(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& bdf, double d) const=0;

    virtual void constructForm(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u) const = 0;
    virtual void computeLinearization(AlatEnums::residualstatus& status, Alat::GhostVector& y, const Alat::GhostVector& x, const Alat::GhostVector& dx) const = 0;
    virtual void postProcess(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& uold, int level=0) const;

    virtual void matrixZero(Alat::GhostMatrix& A) const = 0;
    virtual void matrixVectorProduct(int level, const Alat::GhostMatrix& A, Alat::GhostVector& r, const Alat::GhostVector& u, double d) const=0;

    virtual void restrict(int level, Alat::GhostVector& gu, const Alat::GhostVector& gf ) const;
    virtual void prolongate(int level, Alat::GhostVector& gu, const Alat::GhostVector& gf, double d = 1.0) const;

    virtual const Alat::IoManager* getIoManager() const = 0;
    virtual const Alat::ParameterFile* getParameterFile() const = 0;
    virtual const IntegratorManager* getIntegratorManager() const = 0;

    virtual void setLavrentievParameter(double parameter) const;
    virtual void reInitForInterpolation(const FadaMesh::MeshInterface* mesh);
    virtual Fada::DofInformationInterface* newDofInformation(const Fada::FemInterface* fem) const;
    virtual void setMesh(const FadaMesh::MeshInterface* mesh) const;

    virtual void integrateInTimePostProcess(Alat::GhostVector& ptime, const Alat::GhostVector& p) const;
    virtual std::string getCouplingType() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
