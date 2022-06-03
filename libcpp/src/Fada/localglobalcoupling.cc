#include  "Fada/dofinformationinterface.h"
#include  "Fada/dofmanagerallvariables.h"
#include  "Fada/localglobalcoupling.h"
#include  "Fada/localvectors.h"
#include  "Alat/matrixallvariables.h"
#include  "Alat/systemassemblematrix.h"
#include  "Fada/variablemanager.h"
#include  "Alat/variablevector.h"
#include  "Alat/systemvector.h"
#include  "Fada/visitorintegrationloop.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
LocalGlobalCoupling::~LocalGlobalCoupling(){}

LocalGlobalCoupling::LocalGlobalCoupling(){}

LocalGlobalCoupling::LocalGlobalCoupling( const LocalGlobalCoupling& localglobal)
{
  assert(0);
}

LocalGlobalCoupling& LocalGlobalCoupling::operator=( const LocalGlobalCoupling& localglobal)
{
  assert(0);
  return *this;
}

std::string LocalGlobalCoupling::getName() const
{
  return "LocalGlobalCoupling";
}

LocalGlobalCoupling* LocalGlobalCoupling::clone() const
{
  assert(0);
//return new LocalGlobalCoupling(*this);
}

/*--------------------------------------------------------------------------*/
void LocalGlobalCoupling::basicInit(const DofManagerAllVariables* dofmanagerallvariables_left, const VariableManager* variable_manager_left, const DofManagerAllVariables* dofmanagerallvariables_right, const VariableManager* variable_manager_right, const Alat::StringDoubleVectorMap& scalings_left, const Alat::StringDoubleVectorMap& scalings_right)
{
  _left.basicInit(dofmanagerallvariables_left, variable_manager_left, scalings_left);
  _right.basicInit(dofmanagerallvariables_right, variable_manager_right, scalings_right);
}

/*--------------------------------------------------------------------------*/
void LocalGlobalCoupling::initLocalVectors(Fada::LocalVectors& localvectorsleft, Fada::LocalVectors& localvectorsright, const LocalGlobal::VectorMap& vectormapL, const LocalGlobal::VectorMap& vectormapR) const
{
  _left.initLocalVectors(localvectorsleft, vectormapL);
  _right.initLocalVectors(localvectorsright, vectormapR);
}

/*--------------------------------------------------------------------------*/
void LocalGlobalCoupling::extractAll(Fada::LocalVectors& localvectors_left, Fada::LocalVectors& localvectorsR, const LocalGlobal::VectorMap& vectormapL, const Alat::SystemVector* datavectorL, const LocalGlobal::VectorMap& vectormapR, const Alat::SystemVector* datavectorR, const Alat::IntVector& indicesnodesleft, const Alat::IntVector& indicesnodesright) const
{
  _left.extractAll(localvectors_left, vectormapL, datavectorL, indicesnodesleft);
  _right.extractAll(localvectorsR, vectormapR, datavectorR, indicesnodesright);
}

/*--------------------------------------------------------------------------*/
void LocalGlobalCoupling::initLocalOutput(Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR) const
{
  _left.initLocalOutput(flocL);
  _right.initLocalOutput(flocR);
}
void LocalGlobalCoupling::initLocalPostProcess(Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR) const
{
  _left.initLocalPostProcess(flocL);
  _right.initLocalPostProcess(flocR);
}

void LocalGlobalCoupling::initLocalMatrices(Alat::SystemAssembleMatrix& AlocLL, Alat::SystemAssembleMatrix& AlocLR, Alat::SystemAssembleMatrix& AlocRL, Alat::SystemAssembleMatrix& AlocRR) const
{
  Alat::StringIntMap ncompL, ncompR, nL, nR;
  const Fada::VariablesMap& unknownsL = _left._variablemanager->getUnknowns();
  for(Fada::VariablesMap::const_iterator p=unknownsL.begin();p!=unknownsL.end();p++)
  {
    ncompL[p->first] = p->second->getNComponents();
    const Fada::DofInformationInterface* dofinfo = _left._dofmanagerallvariables->getDofInformationOfVariable(p->first);
    nL[p->first] = dofinfo->ndoflocal();
  }
  const Fada::VariablesMap& unknownsR = _right._variablemanager->getUnknowns();
  for(Fada::VariablesMap::const_iterator p=unknownsR.begin();p!=unknownsR.end();p++)
  {
    ncompR[p->first] = p->second->getNComponents();
    const Fada::DofInformationInterface* dofinfo = _right._dofmanagerallvariables->getDofInformationOfVariable(p->first);
    nR[p->first] = dofinfo->ndoflocal();
  }

  // int nvarL = _left._variablemanager->getNUnknownVariables();
  // int nvarR = _right._variablemanager->getNUnknownVariables();
  // Alat::IntVector ncompL(nvarL), nL(nvarL), ncompR(nvarR), nR(nvarR);
  // for(int ivar = 0; ivar < nvarL; ivar++)
  // {
  //   const Fada::VariableInterface* variable = _left._variablemanager->getUnknown(ivar);
  //   ncompL[ivar] = variable->getNComponents();
  //   std::string varname = _left._variablemanager->getNameOfUnknown(ivar);
  //   const Fada::DofInformationInterface* dofinfo = _left._dofmanagerallvariables->getDofInformationOfVariable(varname);
  //   nL[ivar] = dofinfo->ndoflocal();
  // }
  // for(int ivar = 0; ivar < nvarR; ivar++)
  // {
  //   const Fada::VariableInterface* variable = _right._variablemanager->getUnknown(ivar);
  //   ncompR[ivar] = variable->getNComponents();
  //   std::string varname = _right._variablemanager->getNameOfUnknown(ivar);
  //   const Fada::DofInformationInterface* dofinfo = _right._dofmanagerallvariables->getDofInformationOfVariable(varname);
  //   nR[ivar] = dofinfo->ndoflocal();
  // }
  // std::cerr << "LocalGlobalCoupling::initLocalMatrices() ncompL="<<ncompL<<"\n";
  // std::cerr << "LocalGlobalCoupling::initLocalMatrices() ncompR="<<ncompR<<"\n";
  // std::cerr << "LocalGlobalCoupling::initLocalMatrices() nL="<<nL<<"\n";
  // std::cerr << "LocalGlobalCoupling::initLocalMatrices() nR="<<nR<<"\n";
  AlocLL.set_size(ncompL, ncompL, nL, nL);
  AlocLR.set_size(ncompL, ncompR, nL, nR);
  AlocRL.set_size(ncompR, ncompL, nR, nL);
  AlocRR.set_size(ncompR, ncompR, nR, nR);
  // std::cerr << "LocalGlobalCoupling::initLocalMatrices() AlocLL="<<AlocLL<<"\n";
  // std::cerr << "LocalGlobalCoupling::initLocalMatrices() AlocLR="<<AlocLR<<"\n";
  // std::cerr << "LocalGlobalCoupling::initLocalMatrices() AlocRL="<<AlocRL<<"\n";
  // std::cerr << "LocalGlobalCoupling::initLocalMatrices() AlocRR="<<AlocRR<<"\n";
}

/*--------------------------------------------------------------------------*/
void LocalGlobalCoupling::assembleVector(Alat::SystemVector* favL, Alat::SystemVector* favR, const Alat::SystemAssembleVector& flocL, const Alat::SystemAssembleVector& flocR, const Alat::IntVector& indicesnodesleft, const Alat::IntVector& indicesnodesright, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR, double d) const
{
  _left.assembleVector(favL, flocL, indicesnodesleft, variablesL, d);
  _right.assembleVector(favR, flocR, indicesnodesright, variablesR, d);
}

/*--------------------------------------------------------------------------*/
void LocalGlobalCoupling::assembleVectorPostProcess(Alat::SystemVector* favL, Alat::SystemVector* favR, const Alat::SystemAssembleVector& flocL, const Alat::SystemAssembleVector& flocR, int iKL, int iKR, const Fada::VariableManager* variablemanagerL, const Fada::VariableManager* variablemanagerR, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR, double d) const
{
  _left.assembleVectorPostProcess(favL, flocL, iKL, variablesL, d);
  _right.assembleVectorPostProcess(favR, flocR, iKR, variablesR, d);
}
