#include  "FadaMesh/curvedboundarydescriptioninterface.h"
#include  <cassert>

using namespace FadaMesh;

/*--------------------------------------------------------------------------*/

CurvedBoundaryDescriptionInterface::~CurvedBoundaryDescriptionInterface()
{}

/*--------------------------------------------------------------------------*/

CurvedBoundaryDescriptionInterface::CurvedBoundaryDescriptionInterface() : Alat::InterfaceBase()
{}

/*--------------------------------------------------------------------------*/

CurvedBoundaryDescriptionInterface::CurvedBoundaryDescriptionInterface( const CurvedBoundaryDescriptionInterface& curvedboundarydescriptioninterface) : Alat::InterfaceBase(curvedboundarydescriptioninterface)
{}

/*--------------------------------------------------------------------------*/

CurvedBoundaryDescriptionInterface& CurvedBoundaryDescriptionInterface::operator=( const CurvedBoundaryDescriptionInterface& curvedboundarydescriptioninterface)
{
  InterfaceBase::operator=(curvedboundarydescriptioninterface);
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string CurvedBoundaryDescriptionInterface::getInterfaceName() const
{
  return "CurvedBoundaryDescriptionInterface";
}

/*--------------------------------------------------------------------------*/

double CurvedBoundaryDescriptionInterface::operator()(double x, double y, double z) const
{
  _notWritten("operator");
  return 0.0;
}

/*--------------------------------------------------------------------------*/


void CurvedBoundaryDescriptionInterface::write(std::ostream& out, std::string type) const
{
  _notWritten("write");
}

/*--------------------------------------------------------------------------*/

void CurvedBoundaryDescriptionInterface::read(std::istream& in)
{
  _notWritten("read");
}
