#include  "Fada/localdofinformation.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/femcgq13d.h"
#include  "Fada/integrationformulahexahedral.h"

using namespace Fada;
using namespace std;

/*-------------------------------------------------------*/

FemCgQ13d::~FemCgQ13d() {}

/*-------------------------------------------------------*/

FemCgQ13d::FemCgQ13d() : FemQk3d<1>() {}

/*-------------------------------------------------------*/

FemCgQ13d::FemCgQ13d(const FemCgQ13d& orig) : FemQk3d<1>(orig) {}

/*-------------------------------------------------------*/

FemCgQ13d& FemCgQ13d::operator=(const FemCgQ13d& orig)
{
  assert(0);
}

/*-------------------------------------------------------*/

FemCgQ13d* FemCgQ13d::clone() const
{
  return new FemCgQ13d(*this);
}

/*-------------------------------------------------------*/

std::string FemCgQ13d::getName() const
{
  return "FemCgQ13d";
}

/*-------------------------------------------------------*/

void FemCgQ13d::setDofDescription(Fada::LocalDofInformation& dof_description) const
{
  dof_description.nodes() = 1;
}

/*-------------------------------------------------------*/

int FemCgQ13d::getOrder() const
{
  return 1;
}

/*---------------------------------------------------------*/

void FemCgQ13d::reInitReferencePoint(const Alat::Node& xhat)
{          
  FemQk3d<1>::reInitReferencePoint(xhat);
  double x = xhat.x(), y = xhat.y();

  phi()[0] = 0.125*( 1.0-xhat.x() )*( 1.0-xhat.y() )*( 1.0-xhat.z() );
  phi()[1] = 0.125*( 1.0+xhat.x() )*( 1.0-xhat.y() )*( 1.0-xhat.z() );
  phi()[2] = 0.125*( 1.0+xhat.x() )*( 1.0+xhat.y() )*( 1.0-xhat.z() );
  phi()[3] = 0.125*( 1.0-xhat.x() )*( 1.0+xhat.y() )*( 1.0-xhat.z() );
  phi()[4] = 0.125*( 1.0-xhat.x() )*( 1.0-xhat.y() )*( 1.0+xhat.z() );
  phi()[5] = 0.125*( 1.0+xhat.x() )*( 1.0-xhat.y() )*( 1.0+xhat.z() );
  phi()[6] = 0.125*( 1.0+xhat.x() )*( 1.0+xhat.y() )*( 1.0+xhat.z() );
  phi()[7] = 0.125*( 1.0-xhat.x() )*( 1.0+xhat.y() )*( 1.0+xhat.z() );


  dphi()[0].x() = -0.125*( 1.0-xhat.y() )*( 1.0-xhat.z() );
  dphi()[1].x() =  0.125*( 1.0-xhat.y() )*( 1.0-xhat.z() );
  dphi()[2].x() =  0.125*( 1.0+xhat.y() )*( 1.0-xhat.z() );
  dphi()[3].x() = -0.125*( 1.0+xhat.y() )*( 1.0-xhat.z() );
  dphi()[4].x() = -0.125*( 1.0-xhat.y() )*( 1.0+xhat.z() );
  dphi()[5].x() =  0.125*( 1.0-xhat.y() )*( 1.0+xhat.z() );
  dphi()[6].x() =  0.125*( 1.0+xhat.y() )*( 1.0+xhat.z() );
  dphi()[7].x() = -0.125*( 1.0+xhat.y() )*( 1.0+xhat.z() );

  dphi()[0].y() = -0.125*( 1.0-xhat.x() )*( 1.0-xhat.z() );
  dphi()[1].y() = -0.125*( 1.0+xhat.x() )*( 1.0-xhat.z() );
  dphi()[2].y() =  0.125*( 1.0+xhat.x() )*( 1.0-xhat.z() );
  dphi()[3].y() =  0.125*( 1.0-xhat.x() )*( 1.0-xhat.z() );
  dphi()[4].y() = -0.125*( 1.0-xhat.x() )*( 1.0+xhat.z() );
  dphi()[5].y() = -0.125*( 1.0+xhat.x() )*( 1.0+xhat.z() );
  dphi()[6].y() =  0.125*( 1.0+xhat.x() )*( 1.0+xhat.z() );
  dphi()[7].y() =  0.125*( 1.0-xhat.x() )*( 1.0+xhat.z() );

  dphi()[0].z() = -0.125*( 1.0-xhat.x() )*( 1.0-xhat.y() );
  dphi()[1].z() = -0.125*( 1.0+xhat.x() )*( 1.0-xhat.y() );
  dphi()[2].z() = -0.125*( 1.0+xhat.x() )*( 1.0+xhat.y() );
  dphi()[3].z() = -0.125*( 1.0-xhat.x() )*( 1.0+xhat.y() );
  dphi()[4].z() =  0.125*( 1.0-xhat.x() )*( 1.0-xhat.y() );
  dphi()[5].z() =  0.125*( 1.0+xhat.x() )*( 1.0-xhat.y() );
  dphi()[6].z() =  0.125*( 1.0+xhat.x() )*( 1.0+xhat.y() );
  dphi()[7].z() =  0.125*( 1.0-xhat.x() )*( 1.0+xhat.y() );

  // std::cerr << "FemCgQ13d::reInitReferencePoint() xhat =" << xhat << "\n";
  // std::cerr << "FemCgQ13d::reInitReferencePoint() phi() =" << phi() << "\n";
  // std::cerr << "FemCgQ13d::reInitReferencePoint() dphi() =" << dphi() << "\n";
  // assert(0);
  // std::cerr << "FemCgQ13d::reInitReferencePoint() T =" << getTransformation()->getJacobian() << "\n";
  // std::cerr << "FemCgQ13d::reInitReferencePoint() TI=" << getTransformation()->getInvJacobian() << "\n";


  //Grad sur l'element
  const Alat::DoubleMatrix& TIT = getTransformation()->getInvJacobian();


  for(int ii = 0; ii < 8; ii++)
  {
    double tempx = dphi()[ii].x();
    double tempy = dphi()[ii].y();
    dphi()[ii].x() = TIT(0, 0)*tempx + TIT(0, 1)*tempy + TIT(0, 2)*dphi()[ii].z();
    dphi()[ii].y() = TIT(1, 0)*tempx + TIT(1, 1)*tempy + TIT(1, 2)*dphi()[ii].z();
    dphi()[ii].z() = TIT(2, 0)*tempx + TIT(2, 1)*tempy + TIT(2, 2)*dphi()[ii].z();
    // dphi()[ii].x() = TIT(0, 0)*tempx + TIT(1, 0)*tempy + TIT(2, 0)*dphi()[ii].z();
    // dphi()[ii].y() = TIT(0, 1)*tempx + TIT(1, 1)*tempy + TIT(2, 1)*dphi()[ii].z();
    // dphi()[ii].z() = TIT(0, 2)*tempx + TIT(1, 2)*tempy + TIT(2, 2)*dphi()[ii].z();
  }
  // assert(0);
}

/*---------------------------------------------------------*/

Fada::IntegrationFormulaInterface* FemCgQ13d::newIntegrationFormulaTransfer() const
{
  return new Fada::HexTrapez;
  // return new Fada::HexGauss8;
}
