#include  "FadaMesh/meshinterface.h"
#include  "Fada/femdgq03d.h"
#include  "Fada/q13dtransformation.h"
#include  "Fada/integrationformulahexahedral.h"
#include  "Fada/localdofinformation.h"

using namespace Fada;
using namespace std;

/*-------------------------------------------------------*/

FemDgQ03d* FemDgQ03d::clone() const
{
  return new FemDgQ03d(*this);
}

/*---------------------------------------------------------*/

int FemDgQ03d::ndoflocal() const
{
  return 1;
}

/*---------------------------------------------------------*/

void FemDgQ03d::setDofDescription(Fada::LocalDofInformation& dof_description) const
{
  dof_description.cells() = 1;
}

/*---------------------------------------------------------*/

Fada::TransformationInterface* FemDgQ03d::newTransformation() const
{
  return new Q13DTransformation;  
}

/*---------------------------------------------------------*/

std::string FemDgQ03d::getReferenceElementType() const
{
  return "Hexahedral";  
}

/*---------------------------------------------------------*/

Fada::IntegrationFormulaInterface* FemDgQ03d::newIntegrationFormulaTransfer() const
{
  return new Fada::HexGauss1;
}

/*---------------------------------------------------------*/

void FemDgQ03d::reInitCell(const Alat::Vector<Alat::Node>& nodes)
{
  FEMBase::reInitCell(nodes);
  assert(nodes.size() == 8);
  dphi()[0].x() = 0.0;
  dphi()[0].y() = 0.0;
  dphi()[0].z() = 0.0;
  phi()[0] = 1.0;
}

/*-------------------------------------------------------*/

int FemDgQ03d::getOrder() const
{
  return 0;
}
