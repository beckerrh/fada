#include  "Fada/femvrtm.h"
#include  "Fada/localdofinformation.h"

using namespace Fada;
using namespace std;

/*-------------------------------------------------------*/

FemVRTM::~FemVRTM() {}
FemVRTM::FemVRTM(std::string type) : FEMBase(), _type(type) {}
FemVRTM::FemVRTM(const FemVRTM& fem) : FEMBase(fem)
{
  _type = fem._type;
}
FemVRTM& FemVRTM::operator=(const FemVRTM& fem)
{
  assert(0);
}

std::string FemVRTM::getName() const
{
  return "FemVRTM_"+_type;
}

/*-------------------------------------------------------*/

int FemVRTM::getOrder() const
{
  return 1;
}

/*-------------------------------------------------------*/

void FemVRTM::setDofDescription(Fada::LocalDofInformation& dof_description) const
{
  dof_description.sides() = 1;
}
