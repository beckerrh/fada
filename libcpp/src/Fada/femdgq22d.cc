#include  "Fada/localdofinformation.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/femdgq22d.h"
#include  "Fada/integrationformulaquadrilateral.h"
#include  <math.h>

using namespace Fada;
using namespace std;

/*---------------------------------------------------------*/
//!            6
//!      3 ----------2
//!      |           |
//!      |           |
//!   7  |     8     |   5
//!      |           |
//!      |           |
//!      0 --------- 1
//!            4



/*-------------------------------------------------------*/

FemDgQ22d::~FemDgQ22d() {}
FemDgQ22d::FemDgQ22d() : FemQk2d<2>() {}
FemDgQ22d::FemDgQ22d(const FemDgQ22d& orig) : FemQk2d<2>(orig)
{}
FemDgQ22d& FemDgQ22d::operator=(const FemDgQ22d& orig)
{
  assert(0);
}

FemDgQ22d* FemDgQ22d::clone() const
{
  return new FemDgQ22d(*this);
}

std::string FemDgQ22d::getName() const
{
  return "FemDgQ22d";
}

/*-------------------------------------------------------*/

void FemDgQ22d::setDofDescription(Fada::LocalDofInformation& dof_description) const
{
  dof_description.cells() = 9;
}

/*-------------------------------------------------------*/

int FemDgQ22d::getOrder() const
{
  return 2;
}

/*-------------------------------------------------------*/

void FemDgQ22d::reInitReferencePoint(const Alat::Node& xhat)
{
  FemQk2d<2>::reInitReferencePoint(xhat);
  double x = xhat.x(), y = xhat.y();

  //Base Q2 hierarchique legendre
  phi()[0]=0.25*(1.0-xhat.x())*(1.0-xhat.y());
  phi()[1]=0.25*(1.0+xhat.x())*(1.0-xhat.y());
  phi()[2]=0.25*(1.0+xhat.x())*(1.0+xhat.y());
  phi()[3]=0.25*(1.0-xhat.x())*(1.0+xhat.y());
  phi()[4] = sqrt(6.0)*(x*x-1.0)*(y-1.0)*(-1.0/8.0);
  phi()[5] = sqrt(6.0)*(y*y-1.0)*(x+1.0)*(1.0/8.0);
  phi()[6] = sqrt(6.0)*(x*x-1.0)*(y+1.0)*(1.0/8.0);
  phi()[7] = sqrt(6.0)*(y*y-1.0)*(x-1.0)*(-1.0/8.0);
  phi()[8] = (x*x-1.0)*(y*y-1.0);
  
  //grad Q2
  dphi()[0].x() = y*(1.0/4.0)-1.0/4.0;
  dphi()[1].x() = y*(-1.0/4.0)+1.0/4.0;
  dphi()[2].x() = y*(1.0/4.0)+1.0/4.0;
  dphi()[3].x() = y*(-1.0/4.0)-1.0/4.0;
  dphi()[4].x() = sqrt(6.0)*x*(y-1.0)*(-1.0/4.0);
  dphi()[5].x() = sqrt(6.0)*(y*y-1.0)*(1.0/8.0);
  dphi()[6].x() = sqrt(6.0)*x*(y+1.0)*(1.0/4.0);
  dphi()[7].x() = sqrt(6.0)*(y*y-1.0)*(-1.0/8.0);
  dphi()[8].x() = x*(y*y-1.0)*2.0;

  dphi()[0].y() = x*(1.0/4.0)-1.0/4.0;
  dphi()[1].y() = x*(-1.0/4.0)-1.0/4.0;
  dphi()[2].y() = x*(1.0/4.0)+1.0/4.0;
  dphi()[3].y() = x*(-1.0/4.0)+1.0/4.0;
  dphi()[4].y() = sqrt(6.0)*(x*x-1.0)*(-1.0/8.0);
  dphi()[5].y() = sqrt(6.0)*y*(x+1.0)*(1.0/4.0);
  dphi()[6].y() = sqrt(6.0)*(x*x-1.0)*(1.0/8.0);
  dphi()[7].y() = sqrt(6.0)*y*(x-1.0)*(-1.0/4.0);
  dphi()[8].y() = y*(x*x-1.0)*2.0;

  //Grad sur l'element
  const Alat::DoubleMatrix& TIT = getTransformation()->getInvJacobian();
  for(int ii = 0; ii < 9; ii++)
  {
    double temp = dphi()[ii].x();
    dphi()[ii].x() = TIT(0, 0)*temp+TIT(0, 1)*dphi()[ii].y();
    dphi()[ii].y() = TIT(1, 0)*temp+TIT(1, 1)*dphi()[ii].y();
    dphi()[ii].z() = 0.0;
  }
}

/*---------------------------------------------------------*/

Fada::IntegrationFormulaInterface* FemDgQ22d::newIntegrationFormulaTransfer() const
{
  return new Fada::QuadGauss9;
}
