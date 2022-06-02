#include  "Fada/femvrtm2d.h"
#include  "Fada/integrationformulaquadrilateral.h"
#include  "Fada/q12dtransformation.h"
#include  "Fada/q22dtransformation.h"

using namespace Fada;
using namespace std;

/*---------------------------------------------------------*/
//        2
//     3 -- 2
//   3 |    |  1
//     0 -- 1
//        0

/*-------------------------------------------------------*/

FemVRTM2d::~FemVRTM2d() {}
FemVRTM2d::FemVRTM2d(std::string type) : FemVRTM(type) {}
FemVRTM2d::FemVRTM2d(const FemVRTM2d& fem) : FemVRTM(fem)
{}
FemVRTM2d& FemVRTM2d::operator=(const FemVRTM2d& fem)
{
  assert(0);
}

FemVRTM2d* FemVRTM2d::clone() const
{
  return new FemVRTM2d(*this);
}

std::string FemVRTM2d::getName() const
{
  return "FemVRTM2d"+_type;
}

/*---------------------------------------------------------*/

std::string FemVRTM2d::getReferenceElementType() const
{
  return "Quadrilateral";
}

/*---------------------------------------------------------*/

Fada::TransformationInterface* FemVRTM2d::newTransformation() const
{
  return new Q12DTransformation;
}

Fada::TransformationInterface* FemVRTM2d::newTransformationCurved()
{
  return new Q22DTransformation;
}

/*---------------------------------------------------------*/

void FemVRTM2d::reInitReferencePoint(const Alat::Node& xi)
{
  FEMBase::reInitReferencePoint(xi);

  double rot = xi.x()*xi.x()-xi.y()*xi.y();
  double drot;
  if(_type == "point")
  {
    drot = 0.25;
  }
  else if(_type == "mean")
  {
    drot = 0.375;
  }
  else
  {
    std::cerr<<"*** ERROR FemVRTM2d::reInitPhysicalPoint(): unknown type "<<_type<<"\n";
    exit(1);
  }

  phi()[0] = 0.25 - 0.5*xi.y() - drot*rot;
  phi()[1] = 0.25 + 0.5*xi.x() + drot*rot;
  phi()[2] = 0.25 + 0.5*xi.y() - drot*rot;
  phi()[3] = 0.25 - 0.5*xi.x() + drot*rot;

  dphi()[0].x() = -2.0*drot*xi.x();
  dphi()[0].y() = -0.5 + 2.0*drot*xi.y();

  dphi()[1].x() = 0.5 + 2.0*drot*xi.x();
  dphi()[1].y() = -2.0* drot*xi.y();

  dphi()[2].x() = -2.0*drot*xi.x();
  dphi()[2].y() = 0.5 + 2.0*drot*xi.y();

  dphi()[3].x() = -0.5 + 2.0*drot*xi.x();
  dphi()[3].y() = -2.0*drot*xi.y();

  const Alat::DoubleMatrix& TIT = getTransformation()->getInvJacobian();
  for(int ii = 0; ii < 4; ii++)
  {
    double temp = dphi()[ii].x();
    dphi()[ii].x() = TIT(0, 0)*temp+TIT(0, 1)*dphi()[ii].y();
    dphi()[ii].y() = TIT(1, 0)*temp+TIT(1, 1)*dphi()[ii].y();
  }
}

/*---------------------------------------------------------*/

Fada::IntegrationFormulaInterface* FemVRTM2d::newIntegrationFormulaTransfer() const
{
  return new Fada::QuadRotatedTrapez;
  // return new Fada::QuadGauss4;
}

/*---------------------------------------------------------*/

void FemVRTM2d::getLocalCellIndicesPatch(int iK, Alat::IntVector& indices) const
{
  indices.set_size( ndoflocal() );
  if(iK == 0)
  {
    indices[0] = 9;
    indices[1] = 2;
    indices[2] = 7;
    indices[3] = 8;
  }
  else if(iK == 1)
  {
    indices[0] = 3;
    indices[1] = 0;
    indices[2] = 1;
    indices[3] = 2;
  }
  else if(iK == 2)
  {
    indices[0] = 1;
    indices[1] = 4;
    indices[2] = 5;
    indices[3] = 6;
  }
  else if(iK == 3)
  {
    indices[0] = 7;
    indices[1] = 6;
    indices[2] = 10;
    indices[3] = 11;
  }
}

/*---------------------------------------------------------*/

void FemVRTM2d::getReferenceCenterPoint(Alat::Node& xhat) const
{
  xhat.zeros();
}
