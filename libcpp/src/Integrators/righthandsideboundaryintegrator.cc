#include  "Fada/boundarymanager.h"
#include  "Fada/femmanager.h"
#include  "Fada/feminterface.h"
#include  "Fada/localvectors.h"
#include  "Fada/neumanninterface.h"
#include  "Fada/righthandsideinterface.h"
#include  "Alat/systemassemblevector.h"
#include  "Fada/variablemanager.h"
#include  "Integrators/righthandsideboundaryintegrator.h"
#include  "Fada/varinfointegrator.h"
#include  <cassert>

using namespace Integrators;

/*--------------------------------------------------------------------------*/

RightHandSideBoundaryIntegrator::~RightHandSideBoundaryIntegrator()
{}

/*--------------------------------------------------------------------------*/

RightHandSideBoundaryIntegrator::RightHandSideBoundaryIntegrator(const Alat::Map<std::string, std::string>& bdrycondofvar) : Fada::Integrator(), _bdrycondofvar(bdrycondofvar)
{}

/*--------------------------------------------------------------------------*/

RightHandSideBoundaryIntegrator::RightHandSideBoundaryIntegrator( const RightHandSideBoundaryIntegrator& righthandsideintegrator) : Fada::Integrator(righthandsideintegrator), _bdrycondofvar(righthandsideintegrator._bdrycondofvar)
{}

/*--------------------------------------------------------------------------*/

RightHandSideBoundaryIntegrator* RightHandSideBoundaryIntegrator::clone() const
{
  return new RightHandSideBoundaryIntegrator(*this);
}

/*--------------------------------------------------------------------------*/

RightHandSideBoundaryIntegrator& RightHandSideBoundaryIntegrator::operator=( const RightHandSideBoundaryIntegrator& righthandsideintegrator)
{
  Fada::Integrator::operator=(righthandsideintegrator);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string RightHandSideBoundaryIntegrator::getName() const
{
  return "RightHandSideBoundaryIntegrator";
}

/*--------------------------------------------------------------------------*/

void RightHandSideBoundaryIntegrator::setDomainsOfTerms()
{
  addDomainsOfTerm(FadaEnums::RHS, FadaEnums::BoundarySides);
}

/*--------------------------------------------------------------------------*/

void RightHandSideBoundaryIntegrator::initData()
{
  assert(getVarInfo()->getNVarInput() == 0);
  const Alat::StringSet& outvars = getVarInfo()->getVarNamesOutput();
  for(Alat::StringSet::const_iterator p = outvars.begin(); p!=outvars.end();p++)
  {
    const Fada::DataInterface* data =  getProblemData(*p, _bdrycondofvar[*p]);
    const Fada::NeumannInterface* neumann = dynamic_cast<const Fada::NeumannInterface*>( data );
    _neumann[*p] = neumann;
    _uneumann[*p].set_size(getVarInfo()->getNCompOut(*p));
  }
}

/*-------------------------------------------------------*/

void RightHandSideBoundaryIntegrator::rhsBoundarySide(AlatEnums::residualstatus& status, int iS, double weight, int color) const
{
  const Alat::StringSet& outvars = getVarInfo()->getVarNamesOutput();
  for(Alat::StringSet::const_iterator p = outvars.begin(); p!=outvars.end();p++)
  {
    if( not _boundarymanager->boundaryConditionIs(_bdrycondofvar[*p], color) )
    {
      continue;
    }
    if(not _neumann[*p])
    {
      continue;
    }
    Alat::AssembleVector& floc = (*_flocL)[*p];    
    _neumann[*p]->getValue( _uneumann[*p], color, fem(*p)->getNormal(), fem(*p)->x().x(), fem(*p)->x().y(), fem(*p)->x().z(), getTime() );
    for(int icomp = 0; icomp < getVarInfo()->getNCompOut(*p); icomp++)
    {
      for(int ii = 0; ii < getVarInfo()->getNDofLocalOut(*p); ii++)
      {
        floc(icomp,ii) += weight* _uneumann[*p][icomp]*fem(*p)->phi( ii);
      }
    }
  }
}
