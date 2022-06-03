#include  "Fada/fadalightenums.h"
#include  "Fada/integratorinterface.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
IntegratorInterface::~IntegratorInterface() {}
IntegratorInterface::IntegratorInterface() : Alat::InterfaceBase(){}
IntegratorInterface::IntegratorInterface( const IntegratorInterface& integratorinterface) : Alat::InterfaceBase(integratorinterface){}
IntegratorInterface& IntegratorInterface::operator=( const IntegratorInterface& integratorinterface)
{
  InterfaceBase::operator=(integratorinterface);
  assert(0);
  return *this;
}

std::string IntegratorInterface::getInterfaceName() const
{
  return "Fada::IntegratorInterface";
}

/*--------------------------------------------------------------------------*/
void IntegratorInterface::initData(){}
void IntegratorInterface::basicInit(const Fada::SolverInterface* solver){}

/*--------------------------------------------------------------------------*/
Fada::VariableInterface* IntegratorInterface::constructPostProcessVariable(std::string name, const Fada::VariableManager& variablemanager) const
{
  return NULL;
  // _notWritten("constructPostProcessVariable");
}

Fada::VariableInterface* IntegratorInterface::constructDataVariable(std::string name, const VariableManager& variablemanager, const FadaMesh::MeshInterface* mesh, const DomainSolver* solver) const
{
  return NULL;
  // _notWritten("constructDataVariable");
}

/*--------------------------------------------------------------------------*/
void IntegratorInterface::beforeIntegration(const Fada::LocalVectors& localvectorsleft, const Fada::LocalVectors& localvectorsright) const
{}

void IntegratorInterface::beforeIntegration(const Fada::LocalVectors& localvectors) const
{}
void IntegratorInterface::beforeIntegrationLoop(const FadaEnums::term& term, const FadaEnums::domain& domain, const Fada::LocalGlobal& localglobal, Fada::LocalVectors& localvectors, Fada::LocalVectors& localvectorsR, const Fada::LocalGlobal::VectorMap& vectormap, const Alat::SystemVector* datavector) const
{}
void IntegratorInterface::afterIntegrationLoop(const FadaEnums::term& term, const FadaEnums::domain& domain, Alat::SystemVector* ouput) const
{}

/*--------------------------------------------------------------------------*/

bool IntegratorInterface::needsVeryOld() const
{
  return false;
}

void IntegratorInterface::setLavrentievParameter(double parameter) const
{
  _notWritten("setLavrentievParameter");
}

/*--------------------------------------------------------------------------*/
void IntegratorInterface::postProcessCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR, double weight, double measureSmallSide) const
{
  _notWritten("postProcessCouplingSide");
}

void IntegratorInterface::postProcessCouplingSideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR ) const
{}

void IntegratorInterface::rhsCouplingSide(AlatEnums::residualstatus& status, double weight, double measureSmallSide) const
{
  _notWritten("rhsCouplingSide");
}

void IntegratorInterface::formCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR, double weight, double measureSmallSide) const
{
  _notWritten("formCouplingSide");
}

void IntegratorInterface::formCouplingSideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR ) const
{
  // _notWritten("formCouplingSideNoIntegration");
}

void IntegratorInterface::linearizationCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR, double weight, double measureSmallSide) const
{
  _notWritten("linearizationCouplingSide");
}

void IntegratorInterface::linearizationCouplingSideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR ) const
{
  _notWritten("linearizationCouplingSideNoIntegration");
}

void IntegratorInterface::matrixCouplingSide(AlatEnums::residualstatus& status,  int iKL, int iKR, double weight, double measureSmallSide) const
{
  _notWritten("matrixCouplingSide");
}

void IntegratorInterface::matrixCouplingSideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleMatrix& AlocLL, Alat::SystemAssembleMatrix& AlocLR, Alat::SystemAssembleMatrix& AlocRL, Alat::SystemAssembleMatrix& AlocRR, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const
{
  // _notWritten("matrixCouplingSideNoIntegration");
}

void IntegratorInterface::rhsTimeCouplingSide(AlatEnums::residualstatus& status, Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR, double d) const
{
  _notWritten("rhsTimeCouplingSide");
}

/*--------------------------------------------------------------------------*/
void IntegratorInterface::sortCriterion(AlatEnums::residualstatus& status, double&  value, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const
{
  _notWritten("sortCriterion");
}

/*--------------------------------------------------------------------------*/
void IntegratorInterface::postProcessCell(AlatEnums::residualstatus& status, double weight) const
{
  _notWritten("postProcessCell");
}

void IntegratorInterface::postProcessBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, double weight, int color) const
{
  _notWritten("postProcessBoundarySide");
}
 void IntegratorInterface::postProcessCellNoIntegration(AlatEnums::residualstatus& status, int iK, Alat::SystemAssembleVector& flocL, const Fada::LocalVectors& localvectors) const
 {}
 void IntegratorInterface::postProcessBoundarySideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleVector& flocL, const Fada::LocalVectors& localvectors, int color) const
 {}

/*--------------------------------------------------------------------------*/
void IntegratorInterface::rhsCell(AlatEnums::residualstatus& status, double weight) const
{
  _notWritten("rhsCell");
}

void IntegratorInterface::rhsCellNoIntegration(AlatEnums::residualstatus& status, int iK, Alat::SystemAssembleVector& floc) const
{
}

void IntegratorInterface::rhsBoundarySide(AlatEnums::residualstatus& status, int iS, double weight, int color) const
{
  _notWritten("rhsBoundarySide");
}

void IntegratorInterface::rhsBoundarySideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleVector& floc, int color) const
{
}

/*--------------------------------------------------------------------------*/
void IntegratorInterface::formCell(AlatEnums::residualstatus& status, int iK, double weight) const
{
  _notWritten("formCell");
}

void IntegratorInterface::formCellNoIntegration(AlatEnums::residualstatus& status, int iK, Alat::SystemAssembleVector& floc, const Fada::LocalVectors& localvectors) const
{
}

void IntegratorInterface::formBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, double weight, int color) const
{
  _notWritten("formBoundarySide");
}

void IntegratorInterface::formBoundarySideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleVector& floc, const Fada::LocalVectors& localvectors, int color) const
{
}

/*--------------------------------------------------------------------------*/
void IntegratorInterface::linearizationCell(AlatEnums::residualstatus& status, int iK, double weight) const
{
  _notWritten("linearizationCell");
}

void IntegratorInterface::linearizationCellNoIntegration(AlatEnums::residualstatus& status, int iK, Alat::SystemAssembleVector& floc, const Fada::LocalVectors& localvectors) const
{
}

void IntegratorInterface::linearizationBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, double weight, int color) const
{
  _notWritten("linearizationBoundarySide");
}

void IntegratorInterface::linearizationBoundarySideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleVector& floc, const Fada::LocalVectors& localvectors, int color) const
{
}

/*--------------------------------------------------------------------------*/
void IntegratorInterface::matrixCell(AlatEnums::residualstatus& status, int iK, double weight) const
{
  _notWritten("matrixCell");
}

void IntegratorInterface::matrixCellNoIntegration(AlatEnums::residualstatus& status, int iK, Alat::SystemAssembleMatrix& Aloc, const Fada::LocalVectors& localvectors) const
{
}

void IntegratorInterface::matrixBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, double weight, int color) const
{
  _notWritten("matrixBoundarySide");
}

void IntegratorInterface::matrixBoundarySideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleMatrix& Aloc, const Fada::LocalVectors& localvectors, int color) const
{
}

/*--------------------------------------------------------------------------*/
void IntegratorInterface::rhsTimeCell(AlatEnums::residualstatus& status, double weight, double d) const
{
  _notWritten("rhsTimeCell");
}

void IntegratorInterface::rhsTimeCellNoIntegration(AlatEnums::residualstatus& status, int iK, Alat::SystemAssembleVector& floc, const Fada::LocalVectors& localvectors, double d) const
{
}

void IntegratorInterface::rhsTimeBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, Alat::SystemAssembleVector& floc, double weight, int color, const Fada::FemFunctionsMap& femfcts, double d) const
{
  _notWritten("rhsTimeBoundarySide");
}

void IntegratorInterface::rhsTimeBoundarySideNoIntegration(AlatEnums::residualstatus& status, int iS, Alat::SystemAssembleVector& floc, const Fada::LocalVectors& localvectors, int color, double d) const
{
}

/*--------------------------------------------------------------------------*/
void IntegratorInterface::computeNormSquaredCell(AlatEnums::residualstatus& status, Alat::StringDoubleMap& norms, double weight, const Fada::FemFunctionsMap& femfcts) const
{
  _notWritten("computeNormSquaredCell");
}
