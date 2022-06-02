#include  "Fada/localdofinformation.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/femdgp12d.h"
#include  "Fada/integrationformulatriangle.h"
#include  "Fada/p12dtransformation.h"

using namespace Fada;
using namespace std;

/*-------------------------------------------------------*/
FemDgP12d::~FemDgP12d() {}
FemDgP12d::FemDgP12d() : FemPk2d<1>() {}
FemDgP12d::FemDgP12d(const FemDgP12d& orig) : FemPk2d<1>(orig)
{}
FemDgP12d& FemDgP12d::operator=(const FemDgP12d& orig)
{
  assert(0);
}

FemDgP12d* FemDgP12d::clone() const
{
  return new FemDgP12d(*this);
}

std::string FemDgP12d::getName() const
{
  return "FemDgP12d";
}

/*-------------------------------------------------------*/
void FemDgP12d::setDofDescription(Fada::LocalDofInformation& dof_description) const
{
  dof_description.cells() = 3;
}

/*-------------------------------------------------------*/
int FemDgP12d::getOrder() const
{
  return 1;
}

/*-------------------------------------------------------*/
void FemDgP12d::reInitReferencePoint(const Alat::Node& xhat)
{
  FemPk2d<1>::reInitReferencePoint(xhat);
  double x = xhat.x(), y = xhat.y();

  //Base P1 hierarchique
  phi()[0] = 1.0-x-y;
  phi()[1] = x;
  phi()[2] = y;

  dphi()[0].x() = -1.0;
  dphi()[1].x() = 1.0;
  dphi()[2].x() = 0.0;


  dphi()[0].y() = -1.0;
  dphi()[1].y() = 0.0;
  dphi()[2].y() = 1.0;


  //Grad sur l'element
  const Alat::DoubleMatrix& TIT = getTransformation()->getInvJacobian();
  for(int ii = 0; ii < 3; ii++)
  {
    double temp = dphi()[ii].x();
    dphi()[ii].x() = TIT(0, 0)*temp+TIT(0, 1)*dphi()[ii].y();
    dphi()[ii].y() = TIT(1, 0)*temp+TIT(1, 1)*dphi()[ii].y();
  }
}

/*---------------------------------------------------------*/
Fada::IntegrationFormulaInterface* FemDgP12d::newIntegrationFormulaTransfer() const
{
  return new Fada::TriangleDegree4;
}
