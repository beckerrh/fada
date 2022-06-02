#include  "Fada/localdofinformation.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/femcgq12d.h"
#include  "Fada/integrationformulaquadrilateral.h"

using namespace Fada;
using namespace std;


//        8  --- 5 --- 4
//        |  3   |  1  |
//        7  --- 3 --- 2
//        |  2   |  0  |
//        6  --- 0 --- 1

//        3  --- - --- 2
//        |            |
//        |            |
//        |            |
//        0  --- - --- 1



/*-------------------------------------------------------*/

FemCgQ12d::~FemCgQ12d() {}
FemCgQ12d::FemCgQ12d() : FemQk2d<1>() {}
FemCgQ12d::FemCgQ12d(const FemCgQ12d& orig) : FemQk2d<1>(orig)
{}
FemCgQ12d& FemCgQ12d::operator=(const FemCgQ12d& orig)
{
  assert(0);
}

FemCgQ12d* FemCgQ12d::clone() const
{
  return new FemCgQ12d(*this);
}

std::string FemCgQ12d::getName() const
{
  return "FemQ12d";
}

/*-------------------------------------------------------*/

void FemCgQ12d::setDofDescription(Fada::LocalDofInformation& dof_description) const
{
  dof_description.nodes() = 1;
}

/*-------------------------------------------------------*/

int FemCgQ12d::getOrder() const
{
  return 1;
}

/*-------------------------------------------------------*/

void FemCgQ12d::reInitReferencePoint(const Alat::Node& xhat)
{
  FemQk2d<1>::reInitReferencePoint(xhat);
  double x = xhat.x(), y = xhat.y();

  //Base Q1 hierarchique legendre
  phi()[0] = 0.25*( 1.0-xhat.x() )*( 1.0-xhat.y() );
  phi()[1] = 0.25*( 1.0+xhat.x() )*( 1.0-xhat.y() );
  phi()[2] = 0.25*( 1.0+xhat.x() )*( 1.0+xhat.y() );
  phi()[3] = 0.25*( 1.0-xhat.x() )*( 1.0+xhat.y() );

  //grad Q1
  dphi()[0].x() = -0.25*( 1.0-xhat.y() );
  dphi()[1].x() = 0.25*( 1.0-xhat.y() );
  dphi()[2].x() = 0.25*( 1.0+xhat.y() );
  dphi()[3].x() = -0.25*( 1.0+xhat.y() );

  dphi()[0].y() = -0.25*( 1.0-xhat.x() );
  dphi()[1].y() = -0.25*( 1.0+xhat.x() );
  dphi()[2].y() = 0.25*( 1.0+xhat.x() );
  dphi()[3].y() = 0.25*( 1.0-xhat.x() );

  //Grad sur l'element
  const Alat::DoubleMatrix& TIT = getTransformation()->getInvJacobian();
  for(int ii = 0; ii < 4; ii++)
  {
    double temp = dphi()[ii].x();
    dphi()[ii].x() = TIT(0, 0)*temp+TIT(0, 1)*dphi()[ii].y();
    dphi()[ii].y() = TIT(1, 0)*temp+TIT(1, 1)*dphi()[ii].y();
    dphi()[ii].z() = 0.0;
  }
}

/*---------------------------------------------------------*/

Fada::IntegrationFormulaInterface* FemCgQ12d::newIntegrationFormulaTransfer() const
{
  return new Fada::QuadGauss4;
  // return new Fada::QuadTrapez;
}
