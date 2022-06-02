#include  "Fada/localdofinformation.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/femdgp02d.h"
#include  "Fada/integrationformulatriangle.h"
#include  "Fada/p12dtransformation.h"

using namespace Fada;
using namespace std;

/*-------------------------------------------------------*/

FemDgP02d* FemDgP02d::clone() const
{
  return new FemDgP02d(*this);
}

/*---------------------------------------------------------*/

int FemDgP02d::ndoflocal() const
{
  return 1;
}

/*---------------------------------------------------------*/

void FemDgP02d::setDofDescription(Fada::LocalDofInformation& dof_description) const
{
  dof_description.cells() = 1;
}

/*---------------------------------------------------------*/

Fada::TransformationInterface* FemDgP02d::newTransformation() const
{
  return new P12DTransformation;
}

/*---------------------------------------------------------*/

std::string FemDgP02d::getReferenceElementType() const
{
  return "Triangle";
}

/*---------------------------------------------------------*/

Fada::IntegrationFormulaInterface* FemDgP02d::newIntegrationFormulaTransfer() const
{
  return new Fada::TriangleMidPoint;
}

/*---------------------------------------------------------*/

void FemDgP02d::reInitCell(const Alat::Vector<Alat::Node>& nodes)
{
  FEMBase::reInitCell(nodes);
  assert(nodes.size() == 3);
  dphi()[0].x() = 0.0;
  dphi()[0].y() = 0.0;
  dphi()[0].z() = 0.0;
  phi()[0] = 1.0;
}

/*-------------------------------------------------------*/

int FemDgP02d::getOrder() const
{
  return 0;
}

/*-------------------------------------------------------*/

Fada::IntegrationFormulaInterface* FemDgP02d::newIntegrationFormulaMassMatrix() const
{
  return new Fada::TriangleMidPoint;
}
