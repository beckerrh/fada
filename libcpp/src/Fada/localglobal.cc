#include  "Fada/dofinformationinterface.h"
#include  "Fada/dofmanagerallvariables.h"
#include  "Fada/localglobal.h"
#include  "Fada/localvectors.h"
#include  "Alat/matrixallvariables.h"
#include  "Alat/systemassemblematrix.h"
#include  "Fada/variablemanager.h"
#include  "Alat/variablevector.h"
#include  "Alat/systemvector.h"
#include  "Fada/visitorintegrationloop.h"
#include  "Alat/stringset.h"
#include  <cassert>
#include  <cmath>

using namespace Fada;

/*--------------------------------------------------------------------------*/
LocalGlobal::~LocalGlobal(){}
LocalGlobal::LocalGlobal(){}
LocalGlobal::LocalGlobal( const LocalGlobal& localglobal)
{
  assert(0);
}

LocalGlobal& LocalGlobal::operator=( const LocalGlobal& localglobal)
{
  assert(0);
  return *this;
}

std::string LocalGlobal::getName() const
{
  return "LocalGlobal";
}

LocalGlobal* LocalGlobal::clone() const
{
  assert(0);
//return new LocalGlobal(*this);
}

/*--------------------------------------------------------------------------*/
void LocalGlobal::setVariablesScales(const Alat::StringDoubleVectorMap& variablescales)
{
  _scalings = variablescales;
}

/*--------------------------------------------------------------------------*/
void LocalGlobal::basicInit(const DofManagerAllVariables* dofmanagerallvariables, const VariableManager* variable_manager, const Alat::StringDoubleVectorMap& scalings)
{
  _dofmanagerallvariables = dofmanagerallvariables;
  _variablemanager = variable_manager;
  _scalings = scalings;
  if( _scalings.size() != _variablemanager->getNUnknownVariables() )
  {
    std::cerr << "_scalings="<<_scalings<<"\n";
    assert(0);
  }
  for(Alat::StringDoubleVectorMap::const_iterator p=_scalings.begin();p!=_scalings.end();p++)
  {
    if(p->second.size()!=_variablemanager->getUnknown(p->first)->getNComponents())
    {
      std::cerr << "_scalings="<<_scalings<<"\n";
      std::cerr << "p->first="<<p->first<< " " << _variablemanager->getUnknown(p->first)->getNComponents()<<"\n";
      assert(0);
    }
  }
}

/*--------------------------------------------------------------------------*/
void LocalGlobal::initLocalPostProcess(Alat::SystemAssembleVector& floc) const
{
  Alat::StringIntMap ncompvec, nvec;
  const VariablesMap& postprocess = _variablemanager->getPostProcess();
  for(VariablesMap::const_iterator p = postprocess.begin();p!=postprocess.end();p++)
  {
    ncompvec[p->first] = p->second->getNComponents();
    const Fada::DofInformationInterface* dofinfo = _dofmanagerallvariables->getDofInformationOfVariable(p->first);
    nvec[p->first] = dofinfo->ndoflocal();
  }
  floc.set_size(ncompvec, nvec);
}

/*--------------------------------------------------------------------------*/
void LocalGlobal::initLocalOutput(Alat::SystemAssembleVector& floc) const
{
  Alat::StringIntMap ncompvec, nvec;
  const VariablesMap& unknowns = _variablemanager->getUnknowns();
  for(VariablesMap::const_iterator p = unknowns.begin();p!=unknowns.end();p++)
  {
    ncompvec[p->first] = p->second->getNComponents();
    const Fada::DofInformationInterface* dofinfo = _dofmanagerallvariables->getDofInformationOfVariable(p->first);
    nvec[p->first] = dofinfo->ndoflocal();
  }
  // std::cerr << "LocalGlobal::initLocalOutput() ncompvec="<<ncompvec<<" nvec="<<nvec<<"\n";
  floc.set_size(ncompvec, nvec);
}

/*--------------------------------------------------------------------------*/
void LocalGlobal::initLocalVectors(Fada::LocalVectors& localvectors, const VectorMap& vectormap) const
{
  if(vectormap.size() == 0)
  {
    localvectors.clear();
    return;
  }
  Alat::StringSet variables = vectormap.begin()->second->getVariables();
  for(VectorMap::const_iterator p = vectormap.begin(); p != vectormap.end(); p++)
  {
    assert(p->second->getVariables() == variables);
  }
  Alat::StringIntMap ncompvec, nvec;
  const VariablesMap& unknowns = _variablemanager->getUnknowns();
  for(VariablesMap::const_iterator p = unknowns.begin();p!=unknowns.end();p++)
  {
    ncompvec[p->first] = p->second->getNComponents();
    const Fada::DofInformationInterface* dofinfo = _dofmanagerallvariables->getDofInformationOfVariable(p->first);
    nvec[p->first] = dofinfo->ndoflocal();
  }
  Alat::StringIntMap ncompvecdata, nvecdata;
  const VariablesMap& data = _variablemanager->getData();
  for(VariablesMap::const_iterator p = data.begin();p!=data.end();p++)
  {
    ncompvecdata[p->first] = p->second->getNComponents();
    const Fada::DofInformationInterface* dofinfo = _dofmanagerallvariables->getDofInformationOfVariable(p->first);
    nvecdata[p->first] = dofinfo->ndoflocal();
  }
  localvectors.set_size(ncompvec, nvec, vectormap, ncompvecdata, nvecdata);
}

/*--------------------------------------------------------------------------*/
void LocalGlobal::extractAll(Fada::LocalVectors& localvectors, int iK, const VectorMap& vectormap, const Alat::SystemVector* datavector) const
{
  if(vectormap.size() == 0)
  {
    localvectors.clear();
    return;
  }
  Alat::StringSet variables = vectormap.begin()->second->getVariables();
  for(VectorMap::const_iterator p = vectormap.begin(); p != vectormap.end(); p++)
  {
    assert(p->second->getVariables() == variables);
  }
  for(Alat::StringSet::const_iterator p = variables.begin(); p != variables.end(); p++)
  {
    const Fada::DofInformationInterface* dofinfo = _dofmanagerallvariables->getDofInformationOfVariable(*p);
    Alat::IntVector indices( dofinfo->ndofpercell() );
    dofinfo->getCellIndices(iK, indices);
    for(VectorMap::const_iterator q = vectormap.begin(); q != vectormap.end(); q++)
    {
      q->second->getVector(*p)->extract(localvectors.getVector(q->first)[*p], indices, _scalings[*p]);
    }
  }
}

/*--------------------------------------------------------------------------*/
void LocalGlobal::extractAll(Fada::LocalVectors& localvectors, const VectorMap& vectormap, const Alat::SystemVector* datavector, const Alat::IntVector& indicesnodes) const
{
  if(vectormap.size() == 0)
  {
    return;
  }
  Alat::StringSet variables = vectormap.begin()->second->getVariables();
  for(VectorMap::const_iterator p = vectormap.begin(); p != vectormap.end(); p++)
  {
    assert(p->second->getVariables() == variables);
  }
  for(Alat::StringSet::const_iterator p = variables.begin(); p != variables.end(); p++)
  {
    for(VectorMap::const_iterator q = vectormap.begin(); q != vectormap.end(); q++)
    {
      q->second->getVector(*p)->extract(localvectors.getVector(q->first)[*p], indicesnodes, _scalings[*p]);
    }
  }
}

/*--------------------------------------------------------------------------*/
void LocalGlobal::initLocalMatrix(Alat::SystemAssembleMatrix& Aloc) const
{
  Alat::StringIntMap ncompvec, nvec;
  const VariablesMap& unknowns = _variablemanager->getUnknowns();
  for(VariablesMap::const_iterator p = unknowns.begin();p!=unknowns.end();p++)
  {
    ncompvec[p->first] = p->second->getNComponents();
    const Fada::DofInformationInterface* dofinfo = _dofmanagerallvariables->getDofInformationOfVariable(p->first);
    nvec[p->first] = dofinfo->ndoflocal();
  }
  Aloc.set_size(ncompvec, ncompvec, nvec, nvec);
}

/*--------------------------------------------------------------------------*/
void LocalGlobal::assembleMatrixAllVariables(int iK, Alat::MatrixAllVariables* matrix, const Alat::SystemAssembleMatrix& Aloc, const Alat::StringSet& variables) const
{
  for(Alat::MatrixAllVariables::iterator p = matrix->begin(); p != matrix->end(); p++)
  {
    const Fada::DofInformationInterface* dofinfo = _dofmanagerallvariables->getDofInformationOfVariable(p->first.first);
    Alat::IntVector indices( dofinfo->ndofpercell() );
    dofinfo->getCellIndices(iK, indices);
    const Fada::DofInformationInterface* dofjnfo = _dofmanagerallvariables->getDofInformationOfVariable(p->first.second);
    Alat::IntVector jndices( dofjnfo->ndofpercell() );
    dofjnfo->getCellIndices(iK, jndices);
    p->second->assemble(Aloc(p->first.first, p->first.second), indices, jndices, _scalings[p->first.first], _scalings[p->first.second]);
  }
}

/*--------------------------------------------------------------------------*/
void LocalGlobal::assembleMatrixAllVariables(int iKL, int iKR, Alat::MatrixAllVariables* matrix, const Alat::SystemAssembleMatrix& AlocLL, const Alat::SystemAssembleMatrix& AlocLR, const Alat::SystemAssembleMatrix& AlocRL, const Alat::SystemAssembleMatrix& AlocRR, const Alat::StringSet& variablesL, const Alat::StringSet& variablesR) const
{
  for(Alat::MatrixAllVariables::iterator p = matrix->begin(); p != matrix->end(); p++)
  {
    const Fada::DofInformationInterface* dofinfo = _dofmanagerallvariables->getDofInformationOfVariable(p->first.first);
    Alat::IntVector indicesL( dofinfo->ndofpercell() ), indicesR( dofinfo->ndofpercell() );
    dofinfo->getCellIndices(iKL, indicesL);
    dofinfo->getCellIndices(iKR, indicesR);
    const Fada::DofInformationInterface* dofjnfo = _dofmanagerallvariables->getDofInformationOfVariable(p->first.second);
    Alat::IntVector jndicesL( dofjnfo->ndofpercell() ), jndicesR( dofjnfo->ndofpercell() );
    dofjnfo->getCellIndices(iKL, jndicesL);
    dofjnfo->getCellIndices(iKR, jndicesR);
    p->second->assemble(AlocLL(p->first.first, p->first.second), indicesL, jndicesL, _scalings[p->first.first], _scalings[p->first.second]);
    p->second->assemble(AlocLR(p->first.first, p->first.second), indicesL, jndicesR, _scalings[p->first.first], _scalings[p->first.second]);
    p->second->assemble(AlocRL(p->first.first, p->first.second), indicesR, jndicesL, _scalings[p->first.first], _scalings[p->first.second]);
    p->second->assemble(AlocRR(p->first.first, p->first.second), indicesR, jndicesR, _scalings[p->first.first], _scalings[p->first.second]);
  }
}

/*--------------------------------------------------------------------------*/
void LocalGlobal::assembleVector(Alat::SystemVector* fav, const Alat::SystemAssembleVector& floc, int iK, const Alat::StringSet& variables, double d) const
{
  for(Alat::StringSet::const_iterator p = variables.begin(); p != variables.end(); p++)
  {
    const Fada::DofInformationInterface* dofinfo = _dofmanagerallvariables->getDofInformationOfVariable(*p);
    Alat::IntVector indices( dofinfo->ndofpercell() );
    dofinfo->getCellIndices(iK, indices);
    int ncomp = _variablemanager->getUnknown(*p)->getNComponents();
    fav->getVector(*p)->assemble(floc[*p], indices, _scalings[*p], d);
  }
}

/*--------------------------------------------------------------------------*/
void LocalGlobal::assembleVector(Alat::SystemVector* fav, const Alat::SystemAssembleVector& floc, const Alat::IntVector& indicesnodes, const Alat::StringSet& variables, double d) const
{
  for(Alat::StringSet::const_iterator p = variables.begin(); p != variables.end(); p++)
  {
    int ncomp = _variablemanager->getUnknown(*p)->getNComponents();
    fav->getVector(*p)->assemble(floc[*p], indicesnodes, _scalings[*p], d);
  }
}

/*--------------------------------------------------------------------------*/
void LocalGlobal::assembleVectorPostProcess(Alat::SystemVector* fav, const Alat::SystemAssembleVector& floc, int iK, const Alat::StringSet& variables, double d) const
{
  for(Alat::StringSet::const_iterator p = variables.begin(); p != variables.end(); p++)
  {
    const Fada::VariableInterface* variable = _variablemanager->getPostProcess(*p);
    // std::cerr << "variable->getFemName() "<<variable->getFemName()<< " variable->getVarName() " << variable->getVarName() << "\n";
    if(variable->getFemName() == "none")
    {
      Alat::VariableVector* vv = dynamic_cast<Alat::VariableVector*>( fav->getVector(*p) );
      assert(vv);
      assert( vv->getNComponents() == variable->getNComponents() );
      for(int icomp = 0; icomp < variable->getNComponents(); icomp++)
      {
        std::string type = variable->getPostProcType(icomp);
        if( ( type == "L2" ) || ( type == "H1" ) || ( type == "L1" ) || ( type == "W1" )|| ( type == "sum" )|| ( type == "sep" ) )
        {
          // ( *vv )( icomp, 0 ) += floc[*p](icomp,0);
          ( *vv )( icomp, 0 ) += floc[*p](icomp,0);
        }
        else if(type == "C0")
        {
          // ( *vv )( icomp, 0 ) = fmax( ( *vv )( icomp, 0 ), floc[*p](icomp,0) );
          ( *vv )( icomp, 0 ) = fmax( ( *vv )( icomp, 0 ), floc[*p](icomp,0) );
        }
        else if(type == "US")
        {
          // ( *vv )( icomp, 0 ) = fmin( ( *vv )( icomp, 0 ), floc[*p](icomp,0) );
          ( *vv )( icomp, 0 ) = fmin( ( *vv )( icomp, 0 ), floc[*p](icomp,0) );
        }
        else if(type == "OS")
        {
          // ( *vv )( icomp, 0 ) = fmax( ( *vv )( icomp, 0 ), floc[*p](icomp,0) );
          ( *vv )( icomp, 0 ) = fmax( ( *vv )( icomp, 0 ), floc[*p](icomp,0) );
        }
        else
        {
          assert(0);
        }
      }
    }
    else
    {
      const Fada::DofInformationInterface* dofinfo = _dofmanagerallvariables->getDofInformationOfVariable(*p);
      Alat::IntVector indices( dofinfo->ndofpercell() );
      dofinfo->getCellIndices(iK, indices);
      Alat::DoubleVector scaling(variable->getNComponents(), 1.0);
      fav->getVector(*p)->assemble(floc[*p], indices, scaling, d);
    }
  }
}
