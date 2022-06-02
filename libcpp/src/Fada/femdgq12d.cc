#include  "Fada/localdofinformation.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/femdgq12d.h"
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
FemDgQ12d::~FemDgQ12d() {}
FemDgQ12d::FemDgQ12d() : FemQk2d<1>() {}
FemDgQ12d::FemDgQ12d(const FemDgQ12d& orig) : FemQk2d<1>(orig)
{}
FemDgQ12d& FemDgQ12d::operator=(const FemDgQ12d& orig)
{
  assert(0);
}

FemDgQ12d* FemDgQ12d::clone() const
{
  return new FemDgQ12d(*this);
}

std::string FemDgQ12d::getName() const
{
  return "FemDgQ12d";
}

/*-------------------------------------------------------*/
void FemDgQ12d::setDofDescription(Fada::LocalDofInformation& dof_description) const
{
  dof_description.cells() = 4;
}

/*-------------------------------------------------------*/
int FemDgQ12d::getOrder() const
{
  return 1;
}

/*-------------------------------------------------------*/
void FemDgQ12d::reInitReferencePoint(const Alat::Node& xhat)
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
Fada::IntegrationFormulaInterface* FemDgQ12d::newIntegrationFormulaTransfer() const
{
  return new Fada::QuadGauss4;
  // return new Fada::QuadTrapez;
}
