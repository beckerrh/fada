#include  "Fada/localdofinformation.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/femdgq02d.h"
#include  "Fada/integrationformulaquadrilateral.h"
#include  "Fada/q12dtransformation.h"

using namespace Fada;
using namespace std;

/*-------------------------------------------------------*/

FemDgQ02d* FemDgQ02d::clone() const
{
  return new FemDgQ02d(*this);
}

/*---------------------------------------------------------*/

int FemDgQ02d::ndoflocal() const
{
  return 1;
}

/*---------------------------------------------------------*/

void FemDgQ02d::setDofDescription(Fada::LocalDofInformation& dof_description) const
{
  dof_description.cells() = 1;
}

/*---------------------------------------------------------*/

Fada::TransformationInterface* FemDgQ02d::newTransformation() const
{
  return new Q12DTransformation;
}

/*---------------------------------------------------------*/

std::string FemDgQ02d::getReferenceElementType() const
{
  return "Quadrilateral";
}

/*---------------------------------------------------------*/

Fada::IntegrationFormulaInterface* FemDgQ02d::newIntegrationFormulaTransfer() const
{
  return new Fada::QuadGauss1;
}

/*---------------------------------------------------------*/

void FemDgQ02d::reInitCell(const Alat::Vector<Alat::Node>& nodes)
{
  FEMBase::reInitCell(nodes);
  assert(nodes.size() == 4);
  dphi()[0].x() = 0.0;
  dphi()[0].y() = 0.0;
  dphi()[0].z() = 0.0;
  phi()[0] = 1.0;
}

/*-------------------------------------------------------*/

int FemDgQ02d::getOrder() const
{
  return 0;
}
