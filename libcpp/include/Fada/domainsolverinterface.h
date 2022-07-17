#ifndef __Fada_DomainSolverInterface_h
#define __Fada_DomainSolverInterface_h

#include  "Alat/interfacebase.h"
#include  "Alat/enums.h"
#include  "fadaenums.h"

/*--------------------------------------------------------------------------*/
namespace FadaMesh
{
class MeshInterface;
}
namespace Alat
{
class DoubleVector;
class IntVector;
template <class T, class S>
class Map;
class StringDoubleMap;
class StringDoubleVectorMap;
class StringVector;
class GhostMatrix;
class GhostLinearSolver;
class GhostVector;
class IoManager;
class ParameterFile;
}
namespace Alat
{
class LinearSolverInterface;
class SystemVector;
class VariableVectorInterface;
class VectorInterface;
class SystemMatrixInterface;
class VariableMatrixInterface;
}
namespace Fada
{
class BoundaryManager;
class DomainIntegrationLoopInterface;
class ModelInterface;
class FemInterface;
class MultiLevelTransferAllVariablesInterface;
class MultiLevelTransferSingleFemInterface;
class DofManagerAllVariables;
class DofInformationInterface;
class FemManagerInterface;
class FemInterface;
class FormIntegrationLoopInterface;
class IntegratorInterface;
class IntegratorManager;
class LinearInfo;
class NonLinearInfo;
class TimeData;
class VariableInterface;
class VariableManager;

// class DomainSolverInterface : public virtual SolverInterface
class DomainSolverInterface : public virtual Alat::InterfaceBase
{
protected:
   std::string getInterfaceName() const;
   virtual Alat::LinearSolverInterface * newLinearSolver(const Alat::GhostLinearSolver& linearsolver) = 0;

   virtual Fada::DomainIntegrationLoopInterface * newDomainIntegrationLoop() const
   {
   }

   virtual Alat::VariableMatrixInterface * newVariableMatrix(int level, const std::string& storage, const Fada::VariableInterface *vari, const Fada::VariableInterface *varj) const = 0;

   virtual void reInitVariableMatrix(int level, Alat::VariableMatrixInterface *matrix, const Fada::VariableInterface *vari, const Fada::VariableInterface *varj) const
   {
   }

   virtual void reInitLevelMatrix(int level, Alat::SystemMatrixInterface *A, const Alat::GhostMatrix& gmatrix) const = 0;
   virtual Alat::SystemMatrixInterface * newLevelMatrix(int level, const Alat::GhostMatrix& gmatrix) const           = 0;

public:
   ~DomainSolverInterface();
   DomainSolverInterface();
   DomainSolverInterface(const DomainSolverInterface& domainsolverinterface);
   DomainSolverInterface& operator=(const DomainSolverInterface& domainsolverinterface);
   std::string getName() const;



   virtual IntegratorManager * getIntegratorManager()           = 0;
   virtual void registerMatrix(const Alat::GhostMatrix& matrix) = 0;
   virtual int getNLevels() const = 0;
   virtual FadaEnums::looptype getType() const = 0;

   virtual std::ostream& printLoopInformation(std::ostream& os) const = 0;
   virtual const FadaMesh::MeshInterface * getMesh() const            = 0;
   virtual double getTime() const   = 0;
   virtual double getDeltaT() const = 0;
   virtual void setTimeInfo(const Fada::TimeData& timedata, double masscoefimplicit) = 0;
   virtual void setTimeScheme(std::string time_discretization) = 0;
   virtual void reInit()         = 0;
   virtual void reInitMatrices() = 0;

   // virtual void reInitInterpolation();
   virtual void constructJacobianMatrix(AlatEnums::residualstatus& status, Alat::GhostMatrix& A, const Alat::GhostVector& u) = 0;
   virtual void constructRightHandSide(AlatEnums::residualstatus& status, Alat::GhostVector& gf) const = 0;
   virtual void integrateTimeRhs(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& bdf, double d) const = 0;

   virtual void constructForm(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u) const = 0;
   virtual void computeLinearization(AlatEnums::residualstatus& status, Alat::GhostVector& y, const Alat::GhostVector& x, const Alat::GhostVector& dx) const = 0;
   virtual void postProcess(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& uold, int level    = 0) const=0;

   virtual void matrixZero(Alat::GhostMatrix& A) const = 0;
   virtual void matrixVectorProduct(int level, const Alat::GhostMatrix& A, Alat::GhostVector& r, const Alat::GhostVector& u, double d) const = 0;

   virtual void restrict (int level, Alat::GhostVector & gu, const Alat::GhostVector & gf) const=0;
   virtual void prolongate(int level, Alat::GhostVector& gu, const Alat::GhostVector& gf, double d = 1.0) const=0;

   virtual const Alat::IoManager * getIoManager() const           = 0;
   virtual const Alat::ParameterFile * getParameterFile() const   = 0;
   virtual const IntegratorManager * getIntegratorManager() const = 0;

   virtual void setLavrentievParameter(double parameter) const = 0;
   virtual void reInitForInterpolation(const FadaMesh::MeshInterface *mesh) = 0;
   virtual Fada::DofInformationInterface * newDofInformation(const Fada::FemInterface *fem) const = 0;
   virtual void setMesh(const FadaMesh::MeshInterface *mesh) const = 0;

   virtual void integrateInTimePostProcess(Alat::GhostVector& ptime, const Alat::GhostVector& p) const = 0;



   virtual void basicInit(Fada::ModelInterface *model, const FadaMesh::MeshInterface *mesh, const Alat::IoManager *io_manager, FadaEnums::looptype looptype, const Alat::ParameterFile *parameterfile = NULL) = 0;

   virtual void registerVector(const Alat::GhostVector& v)             = 0;
   virtual void registerLinearSolver(const Alat::GhostLinearSolver& v) = 0;

   virtual Alat::VariableVectorInterface * newVariableVector(const Fada::VariableInterface *var) const = 0;
   virtual Fada::FemManagerInterface * newFemManager() const = 0;
   virtual Fada::MultiLevelTransferAllVariablesInterface * newMultiLevelTransferAllVariables();
   virtual Fada::MultiLevelTransferSingleFemInterface * newMultiLevelTransferSingleFem(const Fada::FemInterface *fem) const;

   virtual void reInitVariableVector(int level, Alat::VariableVectorInterface *v, const Fada::VariableInterface *var) const = 0;
   virtual void reInitVariableVectorForInterpolation(int level, Alat::VariableVectorInterface *u, const Fada::VariableInterface *var, const FadaMesh::MeshInterface *mesh) const;
   virtual void reInitLinearSolvers();
   virtual void reInitLinearSolver(const Alat::GhostLinearSolver& linearsolver) = 0;

   virtual const Alat::StringDoubleVectorMap& getVariablesScales() const = 0;
   virtual const VariableManager * getVariableManager() const            = 0;
   virtual const FemManagerInterface * getFemManager() const             = 0;
   virtual const FemManagerInterface * getFemManagerRight() const        = 0;
   virtual const Fada::ModelInterface * getModel() const = 0;
   virtual const DofManagerAllVariables * getDofManagerAllVariables() const = 0;
   virtual const BoundaryManager * getBoundaryManager() const = 0;
   virtual void initSolution(Alat::GhostVector& u)            = 0;
   virtual void addDiagonalCoupling(int level, Alat::GhostMatrix& gA, const Alat::SystemMatrixInterface *matrix) const;
   virtual void reInitCouplingSolvers(int n);
   virtual int getNcompOfVar(std::string varname) const        = 0;
   virtual int getNOfVar(int level, std::string varname) const = 0;

   // virtual void linearSolve(AlatEnums::iterationstatus& status, Alat::GhostMatrix& A, Alat::GhostLinearSolver& linearsolver, Alat::GhostVector& x, const Alat::GhostVector& b) = 0;
   virtual void constructLinearSolver(const Alat::GhostLinearSolver& linearsolver) = 0;
   virtual const DomainIntegrationLoopInterface * getDomainIntegrationLoop() const = 0;
   virtual void readUnknownVariables(Alat::GhostVector& v, int number = -1) = 0;
   virtual void writeUnknownVariables(Alat::GhostVector& help, const Alat::GhostVector& gv, int number = -1) const = 0;
   virtual void writeVariablesInfo() const = 0;
   virtual void writePostProcessVariables(Alat::DoubleVector& est, const Alat::GhostVector& v, int number = -1) const = 0;
   virtual void reInitVectorForInterpolation(Alat::GhostVector& u) const = 0;
   virtual void interpolateSolution(Alat::GhostVector& ufine, const Alat::GhostVector& ucoarse) const;
   virtual void projectToAllLevels(Alat::GhostVector& gu) const = 0;
   virtual void interpolateOneVariable(const DofInformationInterface *dofinformation, Alat::VariableVectorInterface *unew, const Alat::VariableVectorInterface *uold) const;

   virtual const Alat::VariableMatrixInterface * getMatrix(int level, const Alat::GhostMatrix& matrix, std::string outvar, std::string invar) const = 0;
   virtual Alat::VariableMatrixInterface * getMatrix(int level, Alat::GhostMatrix& matrix, std::string outvar, std::string invar) const             = 0;
   virtual const Alat::SystemVector * getVector(int level, const Alat::GhostVector& v) const = 0;
   virtual Alat::SystemVector * getVector(int level, Alat::GhostVector& v) const             = 0;
   virtual const Alat::SystemMatrixInterface * getMatrix(int level, const Alat::GhostMatrix& matrix) const = 0;
   virtual const Alat::VectorInterface * getVector(const Alat::GhostVector& v) const = 0;
   virtual Alat::VectorInterface * getVector(Alat::GhostVector& v) const             = 0;
   virtual bool vectorExists(const Alat::GhostVector& v) const = 0;

   virtual void vectorZero(Alat::GhostVector& gu) const = 0;
   virtual void vectorAdd(Alat::GhostVector& p, double d, const Alat::GhostVector& q) const = 0;
   virtual void vectorEqual(Alat::GhostVector& r, const Alat::GhostVector& f) const         = 0;
   virtual double vectorScalarProduct(const Alat::GhostVector& r, const Alat::GhostVector& rtilde) const = 0;
   virtual double vectorNorm(const Alat::GhostVector& gu) const = 0;
   virtual std::ostream& vectorWrite(std::ostream& os, const Alat::GhostVector& r) const = 0;

   virtual void strongDirichletVectorSolution(Alat::GhostVector& u) const;
   virtual void strongDirichletVectorZero(Alat::GhostVector& f) const;
   virtual void strongDirichletMatrix(Alat::GhostMatrix& gA) const;
   virtual void newtonProject(Alat::GhostVector& gu) const;
   virtual void beforeTimestep(Alat::GhostVector& gu) const;
   virtual void computeNormSquared(AlatEnums::residualstatus& status, Alat::StringDoubleMap& norms, const Alat::GhostVector& u, const Alat::GhostVector& du) const = 0;

   virtual void vectorScalarProductPerVariables(int level, Alat::StringDoubleMap& sp, const Alat::GhostVector& p, const Alat::GhostVector& q) const = 0;
   virtual Alat::LinearSolverInterface * getLinearSolver(const Alat::GhostLinearSolver& v) const = 0;
   virtual void constructMultigridTransfer(const Alat::GhostMatrix& ghostmatrix) = 0;

   virtual void setMultiGridVector(int level, Alat::GhostVector& mgf, const Alat::GhostVector& gf) const = 0;
   virtual void setVector(int level, Alat::GhostVector& gf, const Alat::GhostVector& mgf) const          = 0;
   virtual void setVectorFromAllVariables(Alat::GhostVector& gu, const Alat::GhostVector& guall) const   = 0;
   virtual void setVectorToAllVariables(Alat::GhostVector& guall, const Alat::GhostVector& gu) const     = 0;
   virtual void addPostProcessVariable(Fada::VariableInterface *variable) = 0;
   virtual void defineIntegratorsAndVariables(Fada::ModelInterface *model, const FadaMesh::MeshInterface *mesh) = 0;

   virtual void setVariableVectorToAll(Alat::GhostVector& u, const Alat::GhostVector& ui) const   = 0;
   virtual void setVariableVectorFromAll(Alat::GhostVector& ui, const Alat::GhostVector& u) const = 0;
};
}

/*--------------------------------------------------------------------------*/

#endif
