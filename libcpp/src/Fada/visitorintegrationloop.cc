#include  "Fada/femmanager.h"
#include  "Fada/integratorinterface.h"
#include  "Fada/integratorinterface.h"
#include  "Fada/localglobal.h"
#include  "Fada/localglobalcoupling.h"
#include  "Alat/matrixallvariables.h"
#include  "Fada/multilevelmatrix.h"
#include  "Fada/multilevelvector.h"
#include  "Alat/systemvectorinterface.h"
#include  "Fada/variablemanager.h"
#include  "Alat/systemvector.h"
#include  "Fada/visitorintegrationloop.h"
#include  <iostream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
VisitorIntegrationLoop::VisitorIntegrationLoop(const Fada::LocalGlobal& localglobal, const Fada::VariableManager* variablemanager, Alat::SystemVectorInterface* vector) : _localglobal(localglobal), _variablemanager(variablemanager)
{
  _vector = dynamic_cast<Alat::SystemVector*>( vector );

  _localglobal.initLocalOutput(_flocL);
  _localglobal.initLocalOutput(_flocR);
  // std::cerr << "VisitorIntegrationLoop::VisitorIntegrationLoop() _flocL="<<_flocL<<"\n";
  // assert(0);
}

void VisitorIntegrationLoop::assembleOutput(int iK, const Alat::StringSet& variables)
{
  assert(_vector);
  // std::cerr << "VisitorIntegrationLoop::assembleOutput() _flocL="<<_flocL<<"\n";
  _localglobal.assembleVector(_vector, _flocL, iK, variables, 1.0);
}

Alat::SystemVector* VisitorIntegrationLoop::getOutputVector()
{
  return _vector;
}

void VisitorIntegrationLoop::assembleOutput(int iKL, int iKR, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR)
{
  assert(_vector);
  _localglobal.assembleVector(_vector, _flocL, iKL, variablesL, 1.0);
  _localglobal.assembleVector(_vector, _flocR, iKR, variablesR, 1.0);
}

void VisitorIntegrationLoop::setOuputZero() const
{
  _flocL.zeros();
}

void VisitorIntegrationLoop::setOuputZeroAll() const
{
  _flocL.zeros();
  _flocR.zeros();
}
void VisitorIntegrationLoop::setAssembleData(const Fada::IntegratorInterface* integrator) const
{
  integrator->setAssembleData(_flocL, _flocR);
}

/*--------------------------------------------------------------------------*/
VisitorIntegrationLoopComputeNormSquared::VisitorIntegrationLoopComputeNormSquared(const Fada::LocalGlobal& localglobal, const Fada::VariableManager* variablemanager, Alat::StringDoubleMap& norms) : VisitorIntegrationLoop(localglobal, variablemanager), _norms(norms)
{}

FadaEnums::term VisitorIntegrationLoopComputeNormSquared::getTerm() const
{
  return FadaEnums::ComputeNormSquared;
}

void VisitorIntegrationLoopComputeNormSquared::perCell(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts) const
{
  integrator->computeNormSquaredCell( status, _norms, weight, femfcts);
}

void VisitorIntegrationLoopComputeNormSquared::perCellBeforeIntegration(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors) const
{
}

void VisitorIntegrationLoopComputeNormSquared::assembleOutput(int iK, const Alat::StringSet& variables)
{}

void VisitorIntegrationLoopComputeNormSquared::setOuputZero() const
{}

/*--------------------------------------------------------------------------*/
VisitorIntegrationLoopPostProcess::VisitorIntegrationLoopPostProcess(const Fada::LocalGlobal& localglobal, const Fada::VariableManager* variablemanager, Alat::SystemVectorInterface* vector) : VisitorIntegrationLoop(localglobal, variablemanager, vector)
{
  _localglobal.initLocalPostProcess(_flocL);
  _localglobal.initLocalPostProcess(_flocR);
}

FadaEnums::term VisitorIntegrationLoopPostProcess::getTerm() const
{
  return FadaEnums::PostProcess;
}

void VisitorIntegrationLoopPostProcess::perCell(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts) const
{
  // integrator->postProcessCell( status, _flocL, weight, femfcts);
  integrator->postProcessCell( status, weight);
}

void VisitorIntegrationLoopPostProcess::perBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts, int color) const
{
  // integrator->postProcessBoundarySide( status, iS, iK, _flocL, weight, color, femfcts);
  integrator->postProcessBoundarySide( status, iS, iK, weight, color);
}

void VisitorIntegrationLoopPostProcess::perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const
{
  // assert(0);
  // integrator->postProcessCouplingSide( status, iKL, iKR, _flocL, _flocR, weight, measureSmallSide, femfctsL, femfctsR);
  integrator->postProcessCouplingSide( status, iKL, iKR, weight, measureSmallSide);
}

void VisitorIntegrationLoopPostProcess::perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const
{
  // assert(0);
}

void VisitorIntegrationLoopPostProcess::assembleOutput(int iK, const Alat::StringSet& variables)
{
  assert( _variablemanager->getNPostprocessVariables() == _vector->getNVars() );
  if( _variablemanager->getNPostprocessVariables() != _flocL.size() )
  {
    std::cerr << "_flocL="<<_flocL<<"\n";
    assert(0);
  }
  _localglobal.assembleVectorPostProcess(_vector, _flocL, iK, variables, 1.0);
}

void VisitorIntegrationLoopPostProcess::assembleOutput(int iKL, int iKR, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR)
{
  // std::cerr << "_flocL" << _flocL;
  // std::cerr << "_flocR" << _flocR;
  _localglobal.assembleVectorPostProcess(_vector, _flocL, iKL, variablesL, 1.0);
  _localglobal.assembleVectorPostProcess(_vector, _flocR, iKR, variablesR, 1.0);
  // std::cerr << "_vector" << *_vector;
  // assert(0);
}

void VisitorIntegrationLoopPostProcess::perCellBeforeIntegration(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors) const
{
  integrator->postProcessCellNoIntegration(status, iK, _flocL, localvectors);
}

void VisitorIntegrationLoopPostProcess::perBoundarySideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors, int color) const
{
  integrator->postProcessBoundarySideNoIntegration(status, iS, _flocL, localvectors, color);
}

/*--------------------------------------------------------------------------*/
VisitorIntegrationLoopRightHandSide::VisitorIntegrationLoopRightHandSide(const Fada::LocalGlobal& localglobal, const Fada::VariableManager* variablemanager, Alat::SystemVectorInterface* vector) : VisitorIntegrationLoop(localglobal, variablemanager, vector) {}

FadaEnums::term VisitorIntegrationLoopRightHandSide::getTerm() const
{
  return FadaEnums::RHS;
}

void VisitorIntegrationLoopRightHandSide::perCell(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts) const
{
  // integrator->rhsCell(status, _flocL, weight);
  integrator->rhsCell(status, weight);
}

void VisitorIntegrationLoopRightHandSide::perBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts, int color) const
{
  // integrator->rhsBoundarySide(status, iS, _flocL, weight, color);
  integrator->rhsBoundarySide(status, iS, weight, color);
}

void VisitorIntegrationLoopRightHandSide::perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const
{
  // integrator->rhsCouplingSide(status, _flocL, _flocR, weight, measureSmallSide);
  integrator->rhsCouplingSide(status, weight, measureSmallSide);
}

void VisitorIntegrationLoopRightHandSide::perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const
{
  assert(0);
}

void VisitorIntegrationLoopRightHandSide::perCellBeforeIntegration(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors) const
{
  integrator->rhsCellNoIntegration(status, iK, _flocL);
}

void VisitorIntegrationLoopRightHandSide::perBoundarySideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors, int color) const
{
  integrator->rhsBoundarySideNoIntegration(status, iS, _flocL, color);
}

/*--------------------------------------------------------------------------*/
VisitorIntegrationLoopTimeRhs::VisitorIntegrationLoopTimeRhs(const Fada::LocalGlobal& localglobal, const Fada::VariableManager* variablemanager, Alat::SystemVectorInterface* vector, double d) : VisitorIntegrationLoop(localglobal, variablemanager, vector), _d(d) {}

FadaEnums::term VisitorIntegrationLoopTimeRhs::getTerm() const
{
  return FadaEnums::TimeRHS;
}

void VisitorIntegrationLoopTimeRhs::perCell(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts) const
{
  // integrator->rhsTimeCell(status, _flocL, weight, femfcts, _d);
  integrator->rhsTimeCell(status, weight, _d);
}

void VisitorIntegrationLoopTimeRhs::perBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts, int color) const
{
  integrator->rhsTimeBoundarySide(status, iS, iK, _flocL, weight, color, femfcts, _d);
}

void VisitorIntegrationLoopTimeRhs::perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const
{
  integrator->rhsTimeCouplingSide(status, _flocL, _flocR, weight, measureSmallSide, femfctsL, femfctsR, _d );
}

void VisitorIntegrationLoopTimeRhs::perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const
{
  assert(0);
}

void VisitorIntegrationLoopTimeRhs::perCellBeforeIntegration(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors) const
{
  integrator->rhsTimeCellNoIntegration(status, iK, _flocL, localvectors, _d);
}

void VisitorIntegrationLoopTimeRhs::perBoundarySideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors, int color) const
{
  integrator->rhsTimeBoundarySideNoIntegration(status, iS, _flocL, localvectors, color, _d);
}

/*--------------------------------------------------------------------------*/
VisitorIntegrationLoopForm::VisitorIntegrationLoopForm(const Fada::LocalGlobal& localglobal, const Fada::VariableManager* variablemanager, Alat::SystemVectorInterface* vector) : VisitorIntegrationLoop(localglobal, variablemanager, vector) {}

FadaEnums::term VisitorIntegrationLoopForm::getTerm() const
{
  return FadaEnums::Form;
}

void VisitorIntegrationLoopForm::assembleOutput(int iK, const Alat::StringSet& variables)
{
  _localglobal.assembleVector(_vector, _flocL, iK, variables, -1.0);
}

void VisitorIntegrationLoopForm::assembleOutput(int iKL, int iKR, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR)
{
  _localglobal.assembleVector(_vector, _flocL, iKL, variablesL, -1.0);
  _localglobal.assembleVector(_vector, _flocR, iKR, variablesR, -1.0);
}

void VisitorIntegrationLoopForm::perCell(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts) const
{
  // integrator->formCell( status, iK, _flocL, weight, femfcts);
  integrator->formCell( status, iK, weight);
}

void VisitorIntegrationLoopForm::perBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts, int color) const
{
  // integrator->formBoundarySide( status, iS, iK, _flocL, weight, color, femfcts);
  integrator->formBoundarySide( status, iS, iK, weight, color);
}

void VisitorIntegrationLoopForm::perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const
{
  // integrator->formCouplingSide( status, iKL, iKR, _flocL, _flocR, weight, measureSmallSide, femfctsL, femfctsR);
  integrator->formCouplingSide( status, iKL, iKR, weight, measureSmallSide);
}

void VisitorIntegrationLoopForm::perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const
{
  integrator->formCouplingSideNoIntegration( status, iS, _flocL, _flocR, localvectorsL, localvectorsR );
}

void VisitorIntegrationLoopForm::perCellBeforeIntegration(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors) const
{
  integrator->formCellNoIntegration(status, iK, _flocL, localvectors );
}

void VisitorIntegrationLoopForm::perBoundarySideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors, int color) const
{
  integrator->formBoundarySideNoIntegration(status, iS, _flocL, localvectors, color );
}

/*--------------------------------------------------------------------------*/
VisitorIntegrationLoopLinearization::VisitorIntegrationLoopLinearization(const Fada::LocalGlobal& localglobal, const Fada::VariableManager* variablemanager, Alat::SystemVectorInterface* vector) : VisitorIntegrationLoopForm(localglobal, variablemanager, vector) {}

FadaEnums::term VisitorIntegrationLoopLinearization::getTerm() const
{
  return FadaEnums::Form;
}

void VisitorIntegrationLoopLinearization::assembleOutput(int iK, const Alat::StringSet& variables)
{
  _localglobal.assembleVector(_vector, _flocL, iK, variables, 1.0);
}

void VisitorIntegrationLoopLinearization::assembleOutput(int iKL, int iKR, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR)
{
  _localglobal.assembleVector(_vector, _flocL, iKL, variablesL, 1.0);
  _localglobal.assembleVector(_vector, _flocR, iKR, variablesR, 1.0);
}

void VisitorIntegrationLoopLinearization::perCell(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts) const
{
  // integrator->linearizationCell( status, iK, _flocL, weight, femfcts);
  integrator->linearizationCell( status, iK, weight);
}

void VisitorIntegrationLoopLinearization::perBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts, int color) const
{
  // integrator->linearizationBoundarySide( status, iS, iK, _flocL, weight, color, femfcts);
  integrator->linearizationBoundarySide( status, iS, iK, weight, color);
}

void VisitorIntegrationLoopLinearization::perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const
{
  // integrator->linearizationCouplingSide( status, iKL, iKR, _flocL, _flocR, weight, measureSmallSide, femfctsL, femfctsR);
  integrator->linearizationCouplingSide( status, iKL, iKR, weight, measureSmallSide);
}

void VisitorIntegrationLoopLinearization::perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const
{
  integrator->linearizationCouplingSideNoIntegration( status, iS, _flocL, _flocR, localvectorsL, localvectorsR );
}

void VisitorIntegrationLoopLinearization::perCellBeforeIntegration(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors) const
{
  integrator->linearizationCellNoIntegration(status, iK, _flocL, localvectors );
}

void VisitorIntegrationLoopLinearization::perBoundarySideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors, int color) const
{
  integrator->linearizationBoundarySideNoIntegration(status, iS, _flocL, localvectors, color );
}

/*--------------------------------------------------------------------------*/
VisitorIntegrationLoopMatrix::VisitorIntegrationLoopMatrix(const LocalGlobal& localglobal, const VariableManager* variablemanager, Alat::SystemMatrixInterface* matrix, std::string couplingtype) : _localglobal(localglobal), _variablemanager(variablemanager), _couplingtype(couplingtype)
{
  _matrix = dynamic_cast<Alat::MatrixAllVariables*>( matrix );
  assert(_matrix);
  _localglobal.initLocalMatrix(_AlocLL);
  _localglobal.initLocalMatrix(_AlocLR);
  _localglobal.initLocalMatrix(_AlocRL);
  _localglobal.initLocalMatrix(_AlocRR);
}

Alat::SystemVector* VisitorIntegrationLoopMatrix::getOutputVector()
{
  return NULL;
}

void VisitorIntegrationLoopMatrix::setOuputZero() const
{
  _AlocLL.zeros();
}

void VisitorIntegrationLoopMatrix::setOuputZeroAll() const
{
  _AlocLL.zeros();
  _AlocLR.zeros();
  _AlocRL.zeros();
  _AlocRR.zeros();
}

FadaEnums::term VisitorIntegrationLoopMatrix::getTerm() const
{
  return FadaEnums::Matrix;
}

void VisitorIntegrationLoopMatrix::assembleOutput(int iK, const Alat::StringSet& variables)
{
  _localglobal.assembleMatrixAllVariables(iK, _matrix, _AlocLL, variables);
}

void VisitorIntegrationLoopMatrix::assembleOutput(int iKL, int iKR, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR)
{
  _localglobal.assembleMatrixAllVariables(iKL, iKR, _matrix, _AlocLL, _AlocLR, _AlocRL, _AlocRR, variablesL, variablesR);
}

void VisitorIntegrationLoopMatrix::perCell(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts) const
{
  // integrator->matrixCell( status, iK, _AlocLL, weight, femfcts);
  integrator->matrixCell( status, iK, weight);
}

void VisitorIntegrationLoopMatrix::perBoundarySide(AlatEnums::residualstatus& status, int iS, int iK, const Fada::IntegratorInterface* integrator, double weight, const Fada::FemFunctionsMap& femfcts, int color) const
{
  // integrator->matrixBoundarySide( status, iS, iK, _AlocLL, weight, color, femfcts);
  integrator->matrixBoundarySide( status, iS, iK, weight, color);
}

void VisitorIntegrationLoopMatrix::perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const
{
  // integrator->matrixCouplingSide( status, iKL, iKR, _AlocLL, _AlocLR, _AlocRL, _AlocRR, weight, measureSmallSide, femfctsL, femfctsR);
  integrator->matrixCouplingSide( status, iKL, iKR, weight, measureSmallSide);
}

void VisitorIntegrationLoopMatrix::perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const
{
  integrator->matrixCouplingSideNoIntegration( status, iS, _AlocLL, _AlocLR, _AlocRL, _AlocRR, localvectorsL, localvectorsR );
}

void VisitorIntegrationLoopMatrix::perCellBeforeIntegration(AlatEnums::residualstatus& status, int iK, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors) const
{
  integrator->matrixCellNoIntegration(status, iK, _AlocLL, localvectors );
}

void VisitorIntegrationLoopMatrix::perBoundarySideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectors, int color) const
{
  integrator->matrixBoundarySideNoIntegration(status, iS, _AlocLL, localvectors, color );
}
void VisitorIntegrationLoopMatrix::setAssembleData(const Fada::IntegratorInterface* integrator) const
{
  integrator->setAssembleData(_AlocLL, _AlocLR, _AlocRL, _AlocRR);
}

/*--------------------------------------------------------------------------*/
VisitorIntegrationLoopPostProcessCoupling::VisitorIntegrationLoopPostProcessCoupling(const Fada::LocalGlobalCoupling& localglobal, const Fada::VariableManager* variablemanagerL, const Fada::VariableManager* variablemanagerR, Alat::SystemVectorInterface* vectorL, Alat::SystemVectorInterface* vectorR) : _localglobal(localglobal), _variablemanagerL(variablemanagerL), _variablemanagerR(variablemanagerR)
{
  _vectorL = dynamic_cast<Alat::SystemVector*>( vectorL );
  assert(_vectorL);
  _vectorR = dynamic_cast<Alat::SystemVector*>( vectorR );
  assert(_vectorR);
  // _localglobal.initLocalOutput(_flocL, _flocR);
  _localglobal.initLocalPostProcess(_flocL, _flocR);
  // std::cerr << "VisitorIntegrationLoopPostProcessCoupling() _flocL0="<< _flocL[0].size()<<"\n";
  // std::cerr << "VisitorIntegrationLoopPostProcessCoupling() _flocL1="<< _flocL[1].size()<<"\n";
  // assert(0);
}

FadaEnums::term VisitorIntegrationLoopPostProcessCoupling::getTerm() const
{
  return FadaEnums::PostProcess;
}

void VisitorIntegrationLoopPostProcessCoupling::perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const
{
  // integrator->postProcessCouplingSide( status, iKL, iKR, _flocL, _flocR, weight, measureSmallSide, femfctsL, femfctsR);
  integrator->postProcessCouplingSide( status, iKL, iKR, weight, measureSmallSide);
}

void VisitorIntegrationLoopPostProcessCoupling::perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const
{
  integrator->postProcessCouplingSideNoIntegration( status, iS, _flocL, _flocR, localvectorsL, localvectorsR );
}

void VisitorIntegrationLoopPostProcessCoupling::assembleOutput(int iKl, int iKr, const Alat::IntVector& indicesnodesleft, const Alat::IntVector& indicesnodesright, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR)
{
  _localglobal.assembleVectorPostProcess(_vectorL, _vectorR, _flocL, _flocR, iKl, iKr, _variablemanagerL, _variablemanagerR, variablesL, variablesR, 1.0);
}

void VisitorIntegrationLoopPostProcessCoupling::setOuputZeroAll() const
{
  _flocL.zeros();
  _flocR.zeros();
}

/*--------------------------------------------------------------------------*/
VisitorIntegrationLoopTimeRhsCoupling::VisitorIntegrationLoopTimeRhsCoupling(const Fada::LocalGlobalCoupling& localglobal, Alat::SystemVectorInterface* vectorL, Alat::SystemVectorInterface* vectorR, double d) : _localglobal(localglobal), _d(d)
{
  _vectorL = dynamic_cast<Alat::SystemVector*>( vectorL );
  assert(_vectorL);
  _vectorR = dynamic_cast<Alat::SystemVector*>( vectorR );
  assert(_vectorR);
  _localglobal.initLocalOutput(_flocL, _flocR);
}

FadaEnums::term VisitorIntegrationLoopTimeRhsCoupling::getTerm() const
{
  return FadaEnums::TimeRHS;
}

void VisitorIntegrationLoopTimeRhsCoupling::perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const
{
  integrator->rhsTimeCouplingSide(status, _flocL, _flocR, weight, measureSmallSide, femfctsL, femfctsR, _d );
}

void VisitorIntegrationLoopTimeRhsCoupling::perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const
{
  assert(0);
}

void VisitorIntegrationLoopTimeRhsCoupling::assembleOutput(int iKl, int iKr, const Alat::IntVector& indicesnodesleft, const Alat::IntVector& indicesnodesright, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR)
{
  _localglobal.assembleVector(_vectorL, _vectorR, _flocL, _flocR, indicesnodesleft, indicesnodesright, variablesL, variablesR, 1.0);
}

void VisitorIntegrationLoopTimeRhsCoupling::setOuputZeroAll() const
{
  _flocL.zeros();
  _flocR.zeros();
}

/*--------------------------------------------------------------------------*/
VisitorIntegrationLoopFormCoupling::VisitorIntegrationLoopFormCoupling(const Fada::LocalGlobalCoupling& localglobal, Alat::SystemVectorInterface* vectorL, Alat::SystemVectorInterface* vectorR) : _localglobal(localglobal)
{
  _vectorL = dynamic_cast<Alat::SystemVector*>( vectorL );
  assert(_vectorL);
  _vectorR = dynamic_cast<Alat::SystemVector*>( vectorR );
  assert(_vectorR);
  _localglobal.initLocalOutput(_flocL, _flocR);
}

FadaEnums::term VisitorIntegrationLoopFormCoupling::getTerm() const
{
  return FadaEnums::Form;
}

void VisitorIntegrationLoopFormCoupling::perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const
{
  // integrator->formCouplingSide( status, iKL, iKR, _flocL, _flocR, weight, measureSmallSide, femfctsL, femfctsR);
  integrator->formCouplingSide( status, iKL, iKR, weight, measureSmallSide);
}

void VisitorIntegrationLoopFormCoupling::perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const
{
  integrator->formCouplingSideNoIntegration(status, iS, _flocL, _flocR, localvectorsL, localvectorsR);
}

void VisitorIntegrationLoopFormCoupling::assembleOutput(int iKl, int iKr, const Alat::IntVector& indicesnodesleft, const Alat::IntVector& indicesnodesright, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR)
{
  _localglobal.assembleVector(_vectorL, _vectorR, _flocL, _flocR, indicesnodesleft, indicesnodesright, variablesL, variablesR, -1.0);
}

void VisitorIntegrationLoopFormCoupling::setOuputZeroAll() const
{
  _flocL.zeros();
  _flocR.zeros();
}

/*--------------------------------------------------------------------------*/
VisitorIntegrationLoopLinearizationCoupling::VisitorIntegrationLoopLinearizationCoupling(const Fada::LocalGlobalCoupling& localglobal, Alat::SystemVectorInterface* vectorL, Alat::SystemVectorInterface* vectorR) : VisitorIntegrationLoopFormCoupling(localglobal, vectorL, vectorR)
{}

void VisitorIntegrationLoopLinearizationCoupling::perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const
{
  // integrator->linearizationCouplingSide( status, iKL, iKR, _flocL, _flocR, weight, measureSmallSide, femfctsL, femfctsR);
  integrator->linearizationCouplingSide( status, iKL, iKR, weight, measureSmallSide);
}

void VisitorIntegrationLoopLinearizationCoupling::perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const
{
  integrator->linearizationCouplingSideNoIntegration(status, iS, _flocL, _flocR, localvectorsL, localvectorsR );
}

void VisitorIntegrationLoopLinearizationCoupling::assembleOutput(int iKl, int iKr, const Alat::IntVector& indicesnodesleft, const Alat::IntVector& indicesnodesright, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR)
{
  _localglobal.assembleVector(_vectorL, _vectorR, _flocL, _flocR, indicesnodesleft, indicesnodesright, variablesL, variablesR, 1.0);
}

/*--------------------------------------------------------------------------*/
VisitorIntegrationLoopSortCriterion::VisitorIntegrationLoopSortCriterion(const Fada::LocalGlobalCoupling& localglobal) : _localglobal(localglobal), _value(0.0)
{}

FadaEnums::term VisitorIntegrationLoopSortCriterion::getTerm() const
{
  return FadaEnums::SortCriterion;
}

void VisitorIntegrationLoopSortCriterion::perCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR,const Fada::IntegratorInterface* integrator, double weight, double measureSmallSide, const Fada::FemFunctionsMap& femfctsL, const Fada::FemFunctionsMap& femfctsR) const
{
  integrator->sortCriterion( status, _value, weight, measureSmallSide, femfctsL, femfctsR);
}

void VisitorIntegrationLoopSortCriterion::perCouplingSideBeforeIntegration(AlatEnums::residualstatus& status, int iS, const Fada::IntegratorInterface* integrator, const Fada::LocalVectors& localvectorsL, const Fada::LocalVectors& localvectorsR) const
{
  assert(0);
}

void VisitorIntegrationLoopSortCriterion::assembleOutput(int iKl, int iKr, const Alat::IntVector& indicesnodesleft, const Alat::IntVector& indicesnodesright, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR)
{
//  std::cerr<<"value "<<_value<<'\n';
  return;
}

bool VisitorIntegrationLoopSortCriterion::getCriterion() const
{
  return _value < 0.0;
}

void VisitorIntegrationLoopSortCriterion::setOuputZeroAll() const
{
  _value = 0.0;
}
