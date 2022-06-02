#include  "Fada/domainsolverinterface.h"
#include  "Fada/femmanager.h"
#include  "Fada/multileveltransferallvariables.h"
#include  "Alat/stringset.h"
#include  "Alat/systemvectorinterface.h"
#include  "Alat/systemvectorinterface.h"
#include  "Fada/variablemanager.h"
#include  "FadaMesh/multilevelmesh.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
MultiLevelTransferAllVariables::~MultiLevelTransferAllVariables() 
{
}

MultiLevelTransferAllVariables::MultiLevelTransferAllVariables(): MultiLevelTransferAllVariablesInterface()
{
}

MultiLevelTransferAllVariables::MultiLevelTransferAllVariables( const MultiLevelTransferAllVariables& multileveltransferallvariables): MultiLevelTransferAllVariablesInterface(multileveltransferallvariables)
{
(*this).operator=(multileveltransferallvariables);
}

MultiLevelTransferAllVariables& MultiLevelTransferAllVariables::operator=( const MultiLevelTransferAllVariables& multileveltransferallvariables) 
{
MultiLevelTransferAllVariablesInterface::operator=(multileveltransferallvariables);
assert(0);
return *this;
}

std::string MultiLevelTransferAllVariables::getName() const 
{
return "MultiLevelTransferAllVariables";
}

MultiLevelTransferAllVariables* MultiLevelTransferAllVariables::clone() const 
{
return new MultiLevelTransferAllVariables(*this);
}

/*--------------------------------------------------------------------------*/
void MultiLevelTransferAllVariables::basicInit(const Fada::DomainSolverInterface* domainsolver, const FadaMesh::MeshInterface* mesh, const Fada::VariableManager& variablemanager, const Fada::DofManagerAllVariables* dofmanagerallvariables)
{
  const FadaMesh::MultiLevelMesh* mlmesh = dynamic_cast<const FadaMesh::MultiLevelMesh*>( mesh );
  assert(mlmesh);
  const Fada::VariablesMap& unknowns = domainsolver->getVariableManager()->getUnknowns();
  Alat::Map<std::string, const Fada::FemInterface*> fems;
  for(Fada::VariableManager::const_iterator p = unknowns.begin(); p != unknowns.end(); p++)
  {
    std::string femname = p->second->getFemName();
    _varnametofem[p->first] = femname;
    fems[femname] = domainsolver->getFemManager()->getFem(p->first);
  }
  for(Alat::Map<std::string, const Fada::FemInterface*>::const_iterator p=fems.begin(); p!=fems.end(); p++)
  {
    _mltransfersinglefem[p->first] = domainsolver->newMultiLevelTransferSingleFem(p->second);
    _mltransfersinglefem[p->first]->basicInit(mlmesh, p->second);
  }
}
void MultiLevelTransferAllVariables::prolongate(int level, Alat::SystemVectorInterface* ufine, const Alat::SystemVectorInterface* ucoarse, double d) const
{
  Alat::StringSet variables = 	ufine->getVariables();
  for(Alat::StringSet::const_iterator p=variables.begin(); p!=variables.end(); p++ )
  {
    std::string fem = _varnametofem[*p];
    _mltransfersinglefem[fem]->prolongate(level, ufine->getVector(*p), ucoarse->getVector(*p));  
  } 
}
void MultiLevelTransferAllVariables::restrict(int level, Alat::SystemVectorInterface* ucoarse, const Alat::SystemVectorInterface* ufine ) const
{
  Alat::StringSet variables = 	ufine->getVariables();
  for(Alat::StringSet::const_iterator p=variables.begin(); p!=variables.end(); p++ )
  {
    std::string fem = _varnametofem[*p];
    _mltransfersinglefem[fem]->restrict(level, ucoarse->getVector(*p), ufine->getVector(*p));  
  } 
}
void MultiLevelTransferAllVariables::project(int level, Alat::SystemVectorInterface* ucoarse, const Alat::SystemVectorInterface* ufine) const
{
  Alat::StringSet variables = 	ufine->getVariables();
  for(Alat::StringSet::const_iterator p=variables.begin(); p!=variables.end(); p++ )
  {
    std::string fem = _varnametofem[*p];
    _mltransfersinglefem[fem]->project(level, ucoarse->getVector(*p), ufine->getVector(*p));  
  } 
}
