#include  "discretizationinterface.h"
#include  <cassert>

/*--------------------------------------------------------------------------*/
DiscretizationInterface::~DiscretizationInterface() {}
DiscretizationInterface::DiscretizationInterface(): Fada::DomainIntegrationLoopInterface(){}
DiscretizationInterface::DiscretizationInterface( const DiscretizationInterface& discretizationinterface): Fada::DomainIntegrationLoopInterface(discretizationinterface)
{
(*this).operator=(discretizationinterface);
}
DiscretizationInterface& DiscretizationInterface::operator=( const DiscretizationInterface& discretizationinterface) 
{
Fada::DomainIntegrationLoopInterface::operator=(discretizationinterface);
assert(0);
return *this;
}
std::string DiscretizationInterface::getName() const 
{
return "DiscretizationInterface";
}
DiscretizationInterface* DiscretizationInterface::clone() const 
{
  assert(0);
// return new DiscretizationInterface(*this);
}

/*--------------------------------------------------------------------------*/
void DiscretizationInterface::basicInit(const Fada::DomainSolverInterface* solver)
{
  assert(0);
}
void DiscretizationInterface::computeLinearization(AlatEnums::residualstatus& status, Alat::VectorInterface* f, const Alat::VectorInterface* u, const Alat::VectorInterface* du, const Alat::VectorInterface* uold, const Alat::VectorInterface* uveryold) const
{
  assert(0);
}
void DiscretizationInterface::postProcess(AlatEnums::residualstatus& status, Alat::VectorInterface* pp, const Alat::VectorInterface* u, const Alat::VectorInterface* uold) const
{
  assert(0);
}
void DiscretizationInterface::initSolution(Alat::VectorInterface* u) const
{
  assert(0);
}
void DiscretizationInterface::setDataVector(Alat::VectorInterface* data)
{
  assert(0);
}
void DiscretizationInterface::integrateTimeRhs(AlatEnums::residualstatus& status, Alat::VectorInterface* f, const Alat::VectorInterface* u, const Alat::VectorInterface* bdf, double d) const
{
  assert(0);
}
void DiscretizationInterface::computeNormSquared(AlatEnums::residualstatus& status, Alat::StringDoubleMap& norms, const Alat::VectorInterface* u, const Alat::VectorInterface* du) const
{
  assert(0);
}
