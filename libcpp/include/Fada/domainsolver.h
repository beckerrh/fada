#ifndef __Fada_DomainSolver_h
#define __Fada_DomainSolver_h

#include  "boundarymanager.h"
#include  "chronometer.h"
#include  "dirichletzero.h"
#include  "dofmanagerallvariables.h"
#include  "domainsolverinterface.h"
#include  "ghostlinearsolveragent.h"
#include  "ghostvectoragent.h"
#include  "ghostmatrixagent.h"
#include  "integratormanager.h"
#include  "variablemanager.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/timedata.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
class MatrixAllVariables;
class VariableVector;
}
namespace Fada
{
class DirichletInterface;
class MultiLevelVector;

// class DomainSolver : public virtual Fada::Solver, public virtual Fada::DomainSolverInterface
class DomainSolver : public Fada::DomainSolverInterface
{
private:
   bool _varscaleafterinitialize;

protected:
   const Alat::ParameterFile *_parameterfile;
   mutable const FadaMesh::MeshInterface *_mesh;
   mutable const FadaMesh::MeshInterface *_refinedmesh;
   const Alat::IoManager *_io_manager;
   double _time, _deltat, _deltatold;
   IntegratorManager _integratormanager;
   FadaEnums::looptype _looptype;

   double& getTime()
   {
      return(_time);
   }

   double& getDeltaT()
   {
      return(_deltat);
   }

   double& getDeltaTOld()
   {
      return(_deltatold);
   }

   // pour strong
   Fada::DirichletZero _dirichletzero;
   Alat::StringIntMap _ncompOfVars;
   Alat::Vector <Alat::StringIntMap> _nOfVars;
   GhostMatrixAgent _ghost_matrix_agent;
   GhostLinearSolverAgent _ghost_linear_solver_agent;
   GhostVectorAgent _ghost_vector_agent;
   Fada::ModelInterface *_model;
   DofManagerAllVariables _dof_manager;
   mutable FemManagerInterface *_femmanager;
   mutable FemManagerInterface *_femmanagerR;
   BoundaryManager _boundary_manager;
   VariableManager _variablemanager;

   DomainIntegrationLoopInterface *_domainintegrationloop;
   static Fada::Chronometer _chronometer;
   std::string _matrixstorage, _mgtransfer, _linalg;
   MultiLevelTransferAllVariablesInterface *_multileveltransferallvariables;

   void strongDirichletMatrix(int level, Alat::MatrixAllVariables *matrix) const;
   void strongDirichletVectorZero(int level, Alat::SystemVector *f) const;
   void strongDirichletVectorSolution(int level, Alat::SystemVector *f) const;
   void reInitVectorForInterpolation(Alat::GhostVector& u) const;
   BoundaryManager * getBoundaryManager();
   VariableManager * getVariableManager();
   Fada::ModelInterface *& getModelPointer();
   const Fada::ModelInterface * getModel() const;
   Fada::ModelInterface * getModel();

   DofManagerAllVariables * getDofManagerAllVariables();
   const DofInformationInterface * getDofInformationOfVariable(const Fada::VariableInterface *var) const;
   FemManagerInterface * getFemManager();
   Alat::VectorInterface * newVector(const Alat::GhostVector& v);
   Alat::SystemVector * newLevelVector(const Alat::GhostVector& v) const;
   Alat::SystemMatrixInterface * newLevelMatrix(int level, const Alat::GhostMatrix& gmatrix) const;
   void reInitVector(Alat::VectorInterface *v, const Alat::GhostVector& gv);
   void reInitLevelVector(int level, Alat::SystemVector *v, const Alat::GhostVector& gv) const;
   void reInitLevelMatrix(int level, Alat::SystemMatrixInterface *A, const Alat::GhostMatrix& gmatrix) const;

   void reInitMatrices();
   bool existsMatrix(Alat::GhostMatrix& matrix) const;
   void matrixZero(Alat::GhostMatrix& A) const;
   Alat::MatrixInterface * newMatrix(const Alat::GhostMatrix& gmatrix);
   void reInitMatrix(Alat::MatrixInterface *A, const Alat::GhostMatrix& gmatrix);

   // void _strongNormalDirichletMatrix(const std::string& varname, int ncomp, Alat::MatrixAllVariables* matrixallvariables, const Fada::DofInformationInterface* dofinfo, const Alat::IntVector& sides, const Alat::IntVector& cells, const Alat::IntVector& sideids) const;
   void _strongDirichletMatrix(const std::string& varname, int ncomp, Alat::MatrixAllVariables *matrixallvariables, const Fada::DofInformationInterface *dofinfo, const Alat::IntVector& sides, const Alat::IntVector& cells, const Alat::IntVector& sideids) const;

   // void _strongNormalDirichletVector(const std::string& varname, int ncomp, int color, Alat::VariableVector* vvector, const Fada::DofInformationInterface* dofinfo, const Alat::IntVector& sides, const Alat::IntVector& cells, const Alat::IntVector& sideids, const Fada::DirichletInterface* dirichlet) const;
   void _strongDirichletVector(const std::string& varname, int ncomp, int color, Alat::VariableVector *vvector, const Fada::DofInformationInterface *dofinfo, const Alat::IntVector& sides, const Alat::IntVector& cells, const Alat::IntVector& sideids, const Fada::DirichletInterface *dirichlet) const;
   void _strongNormalDirichletVectorZero(const std::string& varname, int ncomp, Alat::VariableVector *vvector, const Fada::DofInformationInterface *dofinfo, const Alat::IntVector& sides, const Alat::IntVector& cells, const Alat::IntVector& sideids) const;
   void _strongDirichletVectorZero(const std::string& varname, int ncomp, Alat::VariableVector *vvector, const Fada::DofInformationInterface *dofinfo, const Alat::IntVector& sides, const Alat::IntVector& cells, const Alat::IntVector& sideids) const;
   Fada::DofInformationInterface * newDofInformationWithFem(const Fada::FemInterface *fem) const;
   Alat::LinearSolverInterface * newLinearSolver(const Alat::GhostLinearSolver& linearsolver);

   Fada::DomainIntegrationLoopInterface * newDomainIntegrationLoop() const;
   Alat::VariableVectorInterface * newVariableVector(const Fada::VariableInterface *var) const;
   Alat::VariableMatrixInterface * newVariableMatrix(int level, const std::string& storage, const Fada::VariableInterface *vari, const Fada::VariableInterface *varj) const;
   void reInitVariableVector(int level, Alat::VariableVectorInterface *v, const Fada::VariableInterface *var) const;
   void reInitVariableMatrix(int level, Alat::VariableMatrixInterface *matrix, const Fada::VariableInterface *vari, const Fada::VariableInterface *varj) const;
   void reInitVariableVectorForInterpolation(int level, Alat::VariableVectorInterface *u, const Fada::VariableInterface *var, const FadaMesh::MeshInterface *mesh) const;

   void _interpolateP1(Alat::VariableVectorInterface *unew, const Alat::VariableVectorInterface *uold) const;
   void interpolateOneVariable(const DofInformationInterface *dofinformation, Alat::VariableVectorInterface *unew, const Alat::VariableVectorInterface *uold) const;
   void _writeUnknownVariables(Alat::SystemVector *h, const Alat::SystemVector *vs, std::string filename) const;
   void _readUnknownVariables(Alat::SystemVector *v, std::string filename);
   Fada::MultiLevelTransferAllVariablesInterface * newMultiLevelTransferAllVariables();
   Fada::MultiLevelTransferSingleFemInterface * newMultiLevelTransferSingleFem(const Fada::FemInterface *fem) const;

public:
   ~DomainSolver();
   DomainSolver();
   DomainSolver(const DomainSolver& domainsolver);
   DomainSolver& operator=(const DomainSolver& domainsolver);
   std::string getName() const;
   DomainSolver * clone() const;


   IntegratorManager * getIntegratorManager()
   {
      return(&_integratormanager);
   }

   const IntegratorManager * getIntegratorManager() const
   {
      return(&_integratormanager);
   }

   FadaEnums::looptype getType() const
   {
      return(_looptype);
   }

   int getNLevels() const
   {
      return(_mesh->getNLevels());
   }

   const Alat::IoManager * getIoManager() const
   {
      return(_io_manager);
   }

   void setMesh(const FadaMesh::MeshInterface *mesh) const
   {
     _mesh = mesh;
   }

   const FadaMesh::MeshInterface * getMesh() const
   {
      return(_mesh);
   }

   const FadaMesh::MeshInterface * getRefinedMesh() const
   {
      return(_refinedmesh);
   }

   const Alat::ParameterFile * getParameterFile() const
   {
      return(_parameterfile);
   }

   // void basicInit(ModelInterface* model, const FadaMesh::MeshInterface* mesh, const Alat::IoManager* io_manager, FadaEnums::looptype looptype, const Alat::ParameterFile* parameterfile = NULL);
   double getTime() const
   {
      return(_time);
   }

   double getDeltaT() const
   {
      return(_deltat);
   }

   double getDeltaTOld() const
   {
      return(_deltatold);
   }

   void setTimeInfo(const Fada::TimeData& timedata, double masscoefimplicit)
   {
      getTime()      = timedata.time;
      getDeltaT()    = timedata.deltat;
      getDeltaTOld() = timedata.deltatold;
      getIntegratorManager()->setTimeInfo(timedata, masscoefimplicit);
   }

   void setTimeScheme(std::string time_discretization)
   {
      getIntegratorManager()->setTimeScheme(time_discretization);
   }

   void setLavrentievParameter(double trparameter) const
   {
      getIntegratorManager()->setLavrentievParameter(trparameter);
   }

   // std::ostream& printLoopInformation(std::ostream& os) const;


   static void initChronometer();
   static void printChronometer(std::ostream& os);
   void basicInit(Fada::ModelInterface *model, const FadaMesh::MeshInterface *mesh, const Alat::IoManager *io_manager, FadaEnums::looptype looptype, const Alat::ParameterFile *parameterfile = NULL);
   void reInit();
   void reInitForInterpolation(const FadaMesh::MeshInterface *mesh);
   void reInitMatrixAndLinearSolver();
   void reInitLinearSolvers();

   void registerMatrix(const Alat::GhostMatrix& v);
   void registerVector(const Alat::GhostVector& v);
   void registerLinearSolver(const Alat::GhostLinearSolver& v);
   bool existsVector(const Alat::GhostVector& v) const;
   bool existsLinearSolver(Alat::GhostLinearSolver& v) const;

   const Alat::StringDoubleVectorMap& getVariablesScales() const;
   int getNcompOfVar(std::string varname) const;
   int getNOfVar(int level, std::string varname) const;
   const VariableManager * getVariableManager() const;
   const BoundaryManager * getBoundaryManager() const;
   const DofManagerAllVariables * getDofManagerAllVariables() const;
   const DomainIntegrationLoopInterface * getDomainIntegrationLoop() const;
   DomainIntegrationLoopInterface * getDomainIntegrationLoop();
   const FemManagerInterface * getFemManager() const;
   const FemManagerInterface * getFemManagerRight() const;
   const Alat::IntVector& getNcompOfVars() const;
   const Alat::IntVector& getNOfVars(int level) const;
   Alat::LinearSolverInterface * getLinearSolver(const Alat::GhostLinearSolver& v) const;

   //new
   const Alat::VariableMatrixInterface * getMatrix(int level, const Alat::GhostMatrix& ghostmatrix, std::string outvar, std::string invar) const;
   Alat::VariableMatrixInterface * getMatrix(int level, Alat::GhostMatrix& ghostmatrix, std::string outvar, std::string invar) const;
   Alat::SystemVector * getVector(int level, Alat::GhostVector& ghostvector) const;
   const Alat::SystemVector * getVector(int level, const Alat::GhostVector& ghostvector) const;
   const Alat::MatrixInterface * getMatrix(const Alat::GhostMatrix& ghostmatrix) const;
   Alat::MatrixInterface * getMatrix(Alat::GhostMatrix& ghostmatrix) const;
   const Alat::SystemMatrixInterface * getMatrix(int level, const Alat::GhostMatrix& ghostmatrix) const;
   Alat::SystemMatrixInterface * getMatrix(int level, Alat::GhostMatrix& ghostmatrix) const;
   const Alat::VectorInterface * getVector(const Alat::GhostVector& v) const;
   Alat::VectorInterface * getVector(Alat::GhostVector& v) const;
   bool vectorExists(const Alat::GhostVector& v) const;

   // void linearSolve(AlatEnums::iterationstatus& status, Alat::GhostMatrix& A, Alat::GhostLinearSolver& linearsolver, Alat::GhostVector& x, const Alat::GhostVector& b);
   void constructRightHandSide(AlatEnums::residualstatus& status, Alat::GhostVector& f) const;
   void constructForm(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u) const;
   void computeLinearization(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& du) const;
   void postProcess(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& uold, int level = 0) const;
   void constructJacobianMatrix(AlatEnums::residualstatus& status, Alat::GhostMatrix& gA, const Alat::GhostVector& gu);
   void addDiagonalCoupling(int level, Alat::GhostMatrix& gA, const Alat::SystemMatrixInterface *matrix) const;
   void initSolution(Alat::GhostVector& u);
   void readUnknownVariables(Alat::GhostVector& v, int number = -1);
   void writeUnknownVariables(Alat::GhostVector& help, const Alat::GhostVector& gv, int number = -1) const;
   void writeVariablesInfo() const;
   void writePostProcessVariables(Alat::DoubleVector& est, const Alat::GhostVector& v, int number = -1) const;
   void integrateInTimePostProcess(Alat::GhostVector& ptime, const Alat::GhostVector& p) const;
   Fada::DofInformationInterface * newDofInformation(const Fada::FemInterface *fem) const;
   std::ostream& printLoopInformation(std::ostream& os) const;

   void matrixVectorProduct(int level, const Alat::GhostMatrix& A, Alat::GhostVector& r, const Alat::GhostVector& u, double d) const;

   void prolongate(int level, Alat::GhostVector& gu, const Alat::GhostVector& gf, double d = 1.0) const;

   void restrict (int level, Alat::GhostVector & gu, const Alat::GhostVector & gf) const;
   void projectToAllLevels(Alat::GhostVector& gu, const Alat::GhostVector& gufine) const;
   void projectToAllLevels(Alat::GhostVector& gu) const;
   void interpolateSolution(Alat::GhostVector& ufine, const Alat::GhostVector& ucoarse) const;

   // void projectSolution( Alat::GhostVector& u, const Alat::GhostVector& f) const;

   double vectorNorm(const Alat::GhostVector& gu) const;
   void vectorZero(Alat::GhostVector& gu) const;
   void vectorEqual(Alat::GhostVector& r, const Alat::GhostVector& f) const;
   void vectorAdd(Alat::GhostVector& p, double d, const Alat::GhostVector& q) const;
   void vectorScale(Alat::GhostVector& r, double d) const;
   double vectorScalarProduct(const Alat::GhostVector& r, const Alat::GhostVector& rtilde) const;

   double vectorNorm(int level, const Alat::GhostVector& gu) const;
   void vectorZero(int level, Alat::GhostVector& gu) const;
   void vectorEqual(int level, Alat::GhostVector& r, const Alat::GhostVector& f) const;
   void vectorAdd(int level, Alat::GhostVector& p, double d, const Alat::GhostVector& q) const;
   void vectorScale(int level, Alat::GhostVector& r, double d) const;
   double vectorScalarProduct(int level, const Alat::GhostVector& r, const Alat::GhostVector& rtilde) const;

   std::ostream& vectorWrite(std::ostream& os, const Alat::GhostVector& r) const;

   void integrateTimeRhs(AlatEnums::residualstatus& status, Alat::GhostVector& f, const Alat::GhostVector& u, const Alat::GhostVector& bdf, double d) const;
   void strongDirichletVectorSolution(Alat::GhostVector& u) const;
   void strongDirichletVectorZero(Alat::GhostVector& f) const;
   void strongDirichletMatrix(Alat::GhostMatrix& gA) const;
   void computeNormSquared(AlatEnums::residualstatus& status, Alat::StringDoubleMap& norms, const Alat::GhostVector& u, const Alat::GhostVector& du) const;

   void vectorScalarProductPerVariables(int level, Alat::StringDoubleMap& sp, const Alat::GhostVector& p, const Alat::GhostVector& q) const;

   void setMultiGridVector(int level, Alat::GhostVector& mgf, const Alat::GhostVector& gf) const;
   void setVector(int level, Alat::GhostVector& gf, const Alat::GhostVector& mgf) const;
   void setVectorFromAllVariables(Alat::GhostVector& gu, const Alat::GhostVector& guall) const;
   void setVectorToAllVariables(Alat::GhostVector& guall, const Alat::GhostVector& gu) const;

   Fada::FemManagerInterface * newFemManager() const;
   void reInitLinearSolver(const Alat::GhostLinearSolver& linearsolver);
   void constructLinearSolver(const Alat::GhostLinearSolver& linearsolver);
   void constructMultigridTransfer(const Alat::GhostMatrix& ghostmatrix);
   void addPostProcessVariable(Fada::VariableInterface *variable);
   void defineIntegratorsAndVariables(Fada::ModelInterface *model, const FadaMesh::MeshInterface *mesh);

   void setVariableVectorToAll(Alat::GhostVector& u, const Alat::GhostVector& ui) const;
   void setVariableVectorFromAll(Alat::GhostVector& ui, const Alat::GhostVector& u) const;
};
}

/*--------------------------------------------------------------------------*/

#endif
