#include  "Fada/boundarymanager.h"
#include  "Fada/femmanager.h"
#include  "Fada/feminterface.h"
#include  "Fada/localvectors.h"
#include  "Fada/neumanninterface.h"
#include  "Fada/righthandsideinterface.h"
#include  "Alat/systemassemblevector.h"
#include  "Fada/variablemanager.h"
#include  "Integrators/righthandsidedomainintegrator.h"
#include  "Fada/varinfointegrator.h"
#include  <cassert>

using namespace Integrators;

/*--------------------------------------------------------------------------*/
RightHandSideDomainIntegrator::~RightHandSideDomainIntegrator(){}
RightHandSideDomainIntegrator::RightHandSideDomainIntegrator() : Fada::Integrator(){}
RightHandSideDomainIntegrator::RightHandSideDomainIntegrator( const RightHandSideDomainIntegrator& righthandsideintegrator) : Fada::Integrator(righthandsideintegrator)
{}

RightHandSideDomainIntegrator* RightHandSideDomainIntegrator::clone() const
{
  return new RightHandSideDomainIntegrator(*this);
}
RightHandSideDomainIntegrator& RightHandSideDomainIntegrator::operator=( const RightHandSideDomainIntegrator& righthandsideintegrator)
{
  Fada::Integrator::operator=(righthandsideintegrator);
  assert(0);
  return *this;
}
std::string RightHandSideDomainIntegrator::getName() const
{
  return "RightHandSideDomainIntegrator";
}

/*--------------------------------------------------------------------------*/

void RightHandSideDomainIntegrator::setDomainsOfTerms()
{
  addDomainsOfTerm(FadaEnums::RHS, FadaEnums::Cells);
}

/*--------------------------------------------------------------------------*/

void RightHandSideDomainIntegrator::initData()
{
  assert(getVarInfo()->getNVarInput() == 0);
  const Alat::StringSet& outvars = getVarInfo()->getVarNamesOutput();
  for(Alat::StringSet::const_iterator p = outvars.begin(); p!=outvars.end();p++)
  {
    const Fada::DataInterface* data = getProblemData(*p, "RightHandSide");
    _righthandside[*p] = dynamic_cast<const Fada::RightHandSideInterface*>( data );
    _urhs[*p].set_size(getVarInfo()->getNCompOut(*p));
  }
}

/*-------------------------------------------------------*/

void RightHandSideDomainIntegrator::rhsCell(AlatEnums::residualstatus& status, double weight) const
{
  const Alat::StringSet& outvars = getVarInfo()->getVarNamesOutput();
  for(Alat::StringSet::const_iterator p = outvars.begin(); p!=outvars.end();p++)
  {
    if(_righthandside[*p] == NULL)
    {
      _urhs[*p].zeros();
    }
    else
    {
      _righthandside[*p]->getValue( _urhs[*p], fem(*p)->x().x(), fem(*p)->x().y(), fem(*p)->x().z(), getTime() );
    }
    // std::cerr << "RightHandSideDomainIntegrator::rhsCell() ivar="<<ivar << "  "<< _urhs[ivar] << "\n";
  }
  for(Alat::StringSet::const_iterator p = outvars.begin(); p!=outvars.end();p++)
  {
    Alat::AssembleVector& floc = (*_flocL)[*p];
    for(int icomp = 0; icomp < getVarInfo()->getNCompOut(*p); icomp++)
    {
      for(int ii = 0; ii < getVarInfo()->getNDofLocalOut(*p); ii++)
      {
        // floc[*p](icomp,ii) += weight* _urhs[*p][icomp]*fem(*p)->phi( ii);
        floc(icomp,ii) += weight* _urhs[*p][icomp]*fem(*p)->phi( ii);
      }
    }
  }
}
