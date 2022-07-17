#ifndef __Fada_DomainIntegrationLoop_h
#define __Fada_DomainIntegrationLoop_h

#include  "Alat/systemassemblematrix.h"
#include  "Fada/domainintegrationloopinterface.h"
#include  "Fada/integratorinterface.h"
#include  "Fada/localglobal.h"
#include  "Fada/localvectors.h"
#include  "FadaMesh/meshinterface.h"
// #include  "integrationloop.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
class ParameterFile;
class SystemAssembleVector;
class SystemAssembleMatrix;
}
namespace Alat
{
class MatrixInterface;
class VectorInterface;
class SysetmMatrixInterface;
class SystemVector;
}
namespace Fada
{
class FemManagerInterface;
class IntegrationFormulaInterface;
class IntegratorManager;
class DomainSolverInterface;
class VariableManager;
}

namespace Fada
{
// class DomainIntegrationLoop : public virtual Fada::DomainIntegrationLoopInterface, public virtual IntegrationLoop
class DomainIntegrationLoop : public virtual Fada::DomainIntegrationLoopInterface
{
protected:
   const Alat::ParameterFile *_parameterfile;
   const Fada::IntegratorManager *_integratormanager;
   const Fada::FemManagerInterface *_femmanager, *_femmanagerR;
   mutable Fada::LocalVectors _localvectors, _localvectorsR, _localvectors_edge;
   const FadaMesh::MeshInterface *_mesh;

   Alat::Map <std::string, const Fada::IntegrationFormulaInterface *> _formulae;
   typedef Alat::Set <const Fada::IntegratorInterface *>                              IntegratorSet;
   typedef Alat::Map <Fada::TermAndDomain, Alat::Map <std::string, IntegratorSet> >   IntegratorsForTermAndDomainAndFormula;
   IntegratorsForTermAndDomainAndFormula _integrators_for_term_and_domain_and_formula;
   Alat::Map <Fada::TermAndDomain, Alat::StringSet> _integrationformulaefortermanddomain;

   const Alat::StringSet& getIntegrationFormulaeForTermAndDomain(FadaEnums::term t, FadaEnums::domain d) const;
   const IntegratorSet& getIntegratorsForTermAndDomain(FadaEnums::term t, FadaEnums::domain d, const std::string& formulaname) const;

   Fada::LocalGlobal _localglobal;
   const Fada::VariableManager *_variablemanager;
   const Fada::DofManagerAllVariables *_dofmanagerallvariables;
   const Fada::DomainSolverInterface *_solver;
   mutable const Alat::SystemVector *_datavector;
   const Alat::VectorInterface *_data;

   // mutable Alat::SystemAssembleMatrix _AlocLL, _AlocLR, _AlocRL, _AlocRR;

   template <class VISITOR>
   void integratePerLevelCells(AlatEnums::residualstatus& status, VISITOR& visitor, int level, const Fada::LocalGlobal::VectorMap& vectormap, Fada::FemFunctionsMap& femfctmap) const;

   template <class VISITOR>
   void integratePerLevelBoundarySides(AlatEnums::residualstatus& status, VISITOR& visitor, int level, const Fada::LocalGlobal::VectorMap& vectormap, Fada::FemFunctionsMap& femfctmap) const;

   template <class VISITOR>
   void integratePerLevelInteriorSides(AlatEnums::residualstatus& status, VISITOR& visitor, int level, const Fada::LocalGlobal::VectorMap& vectormap, Fada::FemFunctionsMap& femfctmapL, Fada::FemFunctionsMap& femfctmapR) const;

   const Fada::DomainSolverInterface * getSolver() const;
   const Fada::VariableManager * getVariableManager() const;
   void _fillVectorMap(Fada::LocalGlobal::VectorMap& vectormap, FadaEnums::term term, const Alat::SystemVector *u, const Alat::SystemVector *uold, const Alat::SystemVector *uveryold) const;

   void constructJacobianMatrix(int level, AlatEnums::residualstatus& status, Alat::SystemMatrixInterface *A, const Alat::SystemVector *u, const Alat::SystemVector *uold, const Alat::SystemVector *uveryold) const;

   void _basicInit(const FadaMesh::MeshInterface* mesh, const Fada::IntegratorManager* integratormanager, const Fada::FemManagerInterface* femmanager, const Fada::FemManagerInterface* femmanager_right, const Alat::ParameterFile* parameterfile);

public:
   ~DomainIntegrationLoop();
   DomainIntegrationLoop();
   DomainIntegrationLoop(const DomainIntegrationLoop& domainintegrationloop);
   DomainIntegrationLoop& operator=(const DomainIntegrationLoop& domainintegrationloop);
   DomainIntegrationLoop * clone() const;
   std::string getName() const;
   void basicInit(const Fada::DomainSolverInterface *solver);
   void setVariablesScales(const Alat::StringDoubleVectorMap& variablescales);

   void constructRightHandSide(AlatEnums::residualstatus& status, Alat::VectorInterface *f, int level = 0) const;
   void constructJacobianMatrix(AlatEnums::residualstatus& status, Alat::MatrixInterface *A, const Alat::VectorInterface *u, const Alat::VectorInterface *uold = NULL, const Alat::VectorInterface *uveryold = NULL, int level = 0) const;
   void constructForm(AlatEnums::residualstatus& status, Alat::VectorInterface *f, const Alat::VectorInterface *u, const Alat::VectorInterface *uold, const Alat::VectorInterface *uveryold, int level = 0) const;
   void computeLinearization(AlatEnums::residualstatus& status, Alat::VectorInterface *f, const Alat::VectorInterface *u, const Alat::VectorInterface *du, const Alat::VectorInterface *uold, const Alat::VectorInterface *uveryold, int level = 0) const;
   void initSolution(Alat::VectorInterface *u, int level = 0) const;
   void postProcess(AlatEnums::residualstatus& status, Alat::VectorInterface *pp, const Alat::VectorInterface *u, const Alat::VectorInterface *uold, int level = 0) const;
   void setDataVector(Alat::VectorInterface *data);
   void integrateTimeRhs(AlatEnums::residualstatus& status, Alat::VectorInterface *f, const Alat::VectorInterface *u, const Alat::VectorInterface *bdf, double d, int level = 0) const;
   void computeNormSquared(AlatEnums::residualstatus& status, Alat::StringDoubleMap& norms, const Alat::VectorInterface *u, const Alat::VectorInterface *du, int level = 0) const;
};
}

/*--------------------------------------------------------------------------*/

#endif
