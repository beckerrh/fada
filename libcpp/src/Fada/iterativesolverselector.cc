#include  "Alat/bicgstab.h"
#include  "Alat/cg.h"
#include  "Alat/fgmres.h"
#include  "Alat/gmres.h"
#include  "Fada/multigrid.h"
#include  "Alat/orthomin.h"
#include  "Alat/orthominnew.h"
#include  "Alat/richardson.h"
#include  "Alat/richardsonmonotone.h"
#include  "Alat/richardsonoptimal.h"
#include  "Alat/richardsonsystem.h"
#include  "Alat/simpleiterativesolver.h"
#include  "Alat/tokenize.h"
#include  "Fada/iterativesolverselector.h"
#include  "Alat/umfsolversomevariables.h"
#include  "stdlib.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

IterativeSolverSelector::~IterativeSolverSelector()
{}

IterativeSolverSelector::IterativeSolverSelector() : Alat::InterfaceBase()
{}

IterativeSolverSelector::IterativeSolverSelector( const IterativeSolverSelector& iterativesolverselector) : Alat::InterfaceBase(iterativesolverselector)
{
  assert(0);
}

IterativeSolverSelector& IterativeSolverSelector::operator=( const IterativeSolverSelector& iterativesolverselector)
{
  Alat::InterfaceBase::operator=(iterativesolverselector);
  assert(0);
  return *this;
}

std::string IterativeSolverSelector::getInterfaceName() const
{
  return "IterativeSolverSelector";
}

std::string IterativeSolverSelector::getName() const
{
  return "IterativeSolverSelector";
}

IterativeSolverSelector* IterativeSolverSelector::clone() const
{
  assert(0);
//return new IterativeSolverSelector(*this);
}

/*--------------------------------------------------------------------------*/

Alat::LinearSolverInterface* IterativeSolverSelector::newSolver(std::string name) const
{
  Alat::StringVector bouts = Alat::Tokenize(name, "_");
  int nbouts = bouts.size();
  // std::cerr << "name " << name << " bouts " << bouts << "\n";
  if(bouts[0] == "simple")
  {
    return new Alat::SimpleIterativeSolver;
  }
  else if(bouts[0] == "direct")
  {
    return new Alat::UmfSolverSomeVariables;
  }
  else if(bouts[0] == "multigrid")
  {
    assert(0);
    // return new Fada::Multigrid;
  }
  else if(bouts[0] == "cg")
  {
    return new Alat::Cg;
  }
  else if(bouts[0] == "bicgstab")
  {
    return new Alat::BiCgStab;
  }
  else if( bouts[0] == "orthomin" )
  {
    return new Alat::Orthomin();
  }
  else if( bouts[0] == "orthominnew" )
  {
    return new Alat::OrthominNew();
  }
  else if( bouts[0] == "richardson" )
  {
    double omega = 1.0;
    if(bouts.size() > 1)
    {
      omega = atof( bouts[1].c_str() );
    }
    return new Alat::Richardson(omega);
  }
  else if( bouts[0] == "richardsonmonotone" )
  {
    return new Alat::RichardsonMonotone();
  }
  else if( bouts[0] == "richardsonoptimal" )
  {
    if(bouts.size() != 4)
    {
      _error_string("newSolver", "RichardsonOptimal needs <cyc/coeff> nvectors <ls/gal>", name);
    }
    return new Alat::RichardsonOptimal( bouts[1].c_str(), atoi( bouts[2].c_str() ), bouts[3].c_str() );
  }
  else if( bouts[0] == "gmres" )
  {
    if(bouts.size() != 4)
    {
      _error_string("newSolver", "Gmres needs <cyc/coeff> nvectors <ls/gal>", name);
    }
    return new Alat::Gmres( bouts[1].c_str(), atoi( bouts[2].c_str() ), bouts[3].c_str() );
  }
  else if( bouts[0] == "richardsonsystem" )
  {
    if(bouts.size() != 4)
    {
      _error_string("newSolver", "RichardsonSystem needs <cyc/coeff> nvectors <ls/gal>", name);
    }
    return new Alat::RichardsonSystem( bouts[1].c_str(), atoi( bouts[2].c_str() ), bouts[3].c_str() );
  }
  else if( bouts[0] == "fgmres" )
  {
    assert(bouts.size() >= 2);
    return new Alat::FGmres( atoi( bouts[1].c_str() ) );
  }
  else
  {
    _error_string("newSolver", "unknown iterative solver\"" + name + "\"");
  }
}
