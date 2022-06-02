#include  "Fada/dofinformationinterface.h"
#include  "Fada/dofmanagerallvariables.h"
#include  "Fada/domainsolverinterface.h"
#include  "Fada/femmanagerinterface.h"
#include  "Fada/feminterface.h"
#include  "Fada/variablemanager.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
DofManagerAllVariables::~DofManagerAllVariables()
{
  for(int i = 0; i < _dofs.size(); i++)
  {
    if(_dofs[i])
    {
      delete _dofs[i];
      _dofs[i] = NULL;
    }
  }
}

/*--------------------------------------------------------------------------*/
DofManagerAllVariables::DofManagerAllVariables()
{}

/*--------------------------------------------------------------------------*/
DofManagerAllVariables::DofManagerAllVariables( const DofManagerAllVariables& dofmanagerallvariables)
{
  assert(0);
}

/*--------------------------------------------------------------------------*/

DofManagerAllVariables& DofManagerAllVariables::operator=( const DofManagerAllVariables& dofmanagerallvariables)
{
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/
std::string DofManagerAllVariables::getName() const
{
  return "DofManagerAllVariables";
}

/*--------------------------------------------------------------------------*/
DofManagerAllVariables* DofManagerAllVariables::clone() const
{
  return new DofManagerAllVariables(*this);
}

/*--------------------------------------------------------------------------*/
void DofManagerAllVariables::basicInit(const DomainSolverInterface* solver, const FadaMesh::MeshInterface* mesh)
{
  _femmanager = solver->getFemManager();
  _varname2index = _femmanager->getVarname2Index();
  const Alat::Vector<FemInterface*>& fems = _femmanager->getFems();
  _dofs.set_size( fems.size() );
  for(int i = 0; i < _dofs.size(); i++)
  {
    _dofs[i] = solver->newDofInformation(fems[i]);
    _dofs[i]->basicInit( fems[i], mesh );
  }
}

/*--------------------------------------------------------------------------*/
const DofInformationInterface* DofManagerAllVariables::getDofInformationOfVariable(std::string varname) const
{
  return _dofs[_varname2index[varname]];
}

/*--------------------------------------------------------------------------*/
int DofManagerAllVariables::getNDofInformations() const
{
  return _dofs.size();
}

/*--------------------------------------------------------------------------*/
const DofInformationInterface* DofManagerAllVariables::getDofInformationOfIndex(int ivar) const
{
  return _dofs[ivar];
}

/*--------------------------------------------------------------------------*/
const Alat::StringIntMap& DofManagerAllVariables::getVarnameToIndex() const
{
  return _varname2index;
}

/*--------------------------------------------------------------------------*/
std::ostream& operator<<(std::ostream& os, const DofManagerAllVariables& dofmanagerallvariables)
{
  const Alat::StringIntMap& varnametoindex = dofmanagerallvariables.getVarnameToIndex();

  for(Alat::StringIntMap::const_iterator p = varnametoindex.begin(); p != varnametoindex.end(); p++)
  {
    os << p->first << " ===> " << p->second << "\n";
  }
  return os;
}

/*--------------------------------------------------------------------------*/
void DofManagerAllVariables::setMeshLevel(int level) const
{
  // std::cerr << "DofManagerAllVariables::setMeshLevel() level="<<level<<"\n";
  for(int i = 0; i < _dofs.size(); i++)
  {
    _dofs[i]->setMeshLevel(level);
  }
}
