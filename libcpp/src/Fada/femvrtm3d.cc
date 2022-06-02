#include  "Fada/femvrtm3d.h"
#include  "Fada/integrationformulahexahedral.h"
#include  "Fada/q13dtransformation.h"

using namespace Fada;
using namespace std;

/*---------------------------------------------------------*/
//!            4                              4
//!      3 ----------2                  7 ----------6
//!      |           |                  |           |
//!      |           |                  |           |
//!   2  |     0     |   3           2  |     5     |   3
//!      |           |                  |           |
//!      |           |                  |           |
//!      0 --------- 1                  4 --------- 5
//!            1                              1
/*---------------------------------------------------------*/
//


/*-------------------------------------------------------*/

FemVRTM3d::~FemVRTM3d() {}
FemVRTM3d::FemVRTM3d(std::string type) : FemVRTM(type) {}
FemVRTM3d::FemVRTM3d(const FemVRTM3d& fem) : FemVRTM(fem)
{}
FemVRTM3d& FemVRTM3d::operator=(const FemVRTM3d& fem)
{
  assert(0);
}

FemVRTM3d* FemVRTM3d::clone() const
{
  return new FemVRTM3d(*this);
}

std::string FemVRTM3d::getName() const
{
  return "FemVRTM3d"+_type;
}

/*---------------------------------------------------------*/

std::string FemVRTM3d::getReferenceElementType() const
{
  return "Hexhedral";
}

/*---------------------------------------------------------*/

Fada::TransformationInterface* FemVRTM3d::newTransformation() const
{
  return new Q13DTransformation;
}

Fada::TransformationInterface* FemVRTM3d::newTransformationCurved()
{
  return new Q13DTransformation;
}

/*---------------------------------------------------------*/

void FemVRTM3d::reInitReferencePoint(const Alat::Node& xi)
{
  FEMBase::reInitReferencePoint(xi);

  double rotx = 2.0*xi.x()*xi.x()-xi.z()*xi.z()-xi.y()*xi.y();
  double roty = 2.0*xi.y()*xi.y()-xi.x()*xi.x()-xi.z()*xi.z();
  double rotz = 2.0*xi.z()*xi.z()-xi.x()*xi.x()-xi.y()*xi.y();
  double drot;
  if(_type == "point")
  {
    drot = 1.0/6.0;
  }
  else if(_type == "mean")
  {
    drot = 1.0/4.0;
  }
  else
  {
    std::cerr<<"*** ERROR FemVRTM2d::reInitPhysicalPoint(): unknown type "<<_type<<"\n";
    exit(1);
  }

  phi()[0] = 1.0/6.0 - 0.5*xi.z() + drot*rotz;
  phi()[5] = 1.0/6.0 + 0.5*xi.z() + drot*rotz;

  phi()[2] = 1.0/6.0 - 0.5*xi.x() + drot*rotx;
  phi()[3] = 1.0/6.0 + 0.5*xi.x() + drot*rotx;

  phi()[1] = 1.0/6.0 - 0.5*xi.y() + drot*roty;
  phi()[4] = 1.0/6.0 + 0.5*xi.y() + drot*roty;


  dphi()[0].x() = -2.0* drot* xi.x();
  dphi()[0].y() = -2.0* drot* xi.y();
  dphi()[0].z() = -0.5 + 4.0* drot* xi.z();

  dphi()[5].x() = -2.0* drot* xi.x();
  dphi()[5].y() = -2.0* drot* xi.y();
  dphi()[5].z() = 0.5 + 4.0* drot* xi.z();


  dphi()[2].x() = -0.5 + 4.0* drot* xi.x();
  dphi()[2].y() = -2.0* drot* xi.y();
  dphi()[2].z() = -2.0* drot* xi.z();

  dphi()[3].x() = 0.5 + 4.0* drot* xi.x();
  dphi()[3].y() = -2.0* drot* xi.y();
  dphi()[3].z() = -2.0* drot* xi.z();


  dphi()[1].x() = -2.0* drot* xi.x();
  dphi()[1].y() = -0.5 + 4.0* drot* xi.y();
  dphi()[1].z() = -2.0* drot* xi.z();

  dphi()[4].x() = -2.0* drot* xi.x();
  dphi()[4].y() = 0.5 + 4.0* drot* xi.y();
  dphi()[4].z() = -2.0* drot* xi.z();

  const Alat::DoubleMatrix& TIT = getTransformation()->getInvJacobian();
  for(int ii = 0; ii < 6; ii++)
  {
    double tempx = dphi()[ii].x();
    double tempy = dphi()[ii].y();
    dphi()[ii].x() = TIT(0, 0)*tempx + TIT(0, 1)*tempy + TIT(0, 2)*dphi()[ii].z();
    dphi()[ii].y() = TIT(1, 0)*tempx + TIT(1, 1)*tempy + TIT(1, 2)*dphi()[ii].z();
    dphi()[ii].z() = TIT(2, 0)*tempx + TIT(2, 1)*tempy + TIT(2, 2)*dphi()[ii].z();
  }
}

/*---------------------------------------------------------*/

Fada::IntegrationFormulaInterface* FemVRTM3d::newIntegrationFormulaTransfer() const
{
  // return new Fada::HexTrapez;
  return new Fada::HexGauss8;
}

/*---------------------------------------------------------*/

void FemVRTM3d::getLocalCellIndicesPatch(int iK, Alat::IntVector& indices) const
{
  indices.set_size( ndoflocal() );
  if(iK == 0)
  {
    indices[0] = 14;
    indices[1] = 13;
    indices[2] = 12;
    indices[3] =  2;
    indices[4] = 11;
    indices[5] = 15;
  }
  else if(iK == 1)
  {
    /*indices[0] = 4;
       indices[1] = 2;
       indices[2] = 0;
       indices[3] = 1;
       indices[4] = 3;
       indices[5] = 5;*/
    indices[0] = 4;
    indices[1] = 3;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 1;
    indices[5] = 5;
  }
  else if(iK == 2)
  {
    indices[0] = 9;
    indices[1] = 1;
    indices[2] = 8;
    indices[3] = 6;
    indices[4] = 7;
    indices[5] = 10;
  }
  else if(iK == 3)
  {
    indices[0] = 18;
    indices[1] = 11;
    indices[2] = 17;
    indices[3] =  8;
    indices[4] = 16;
    indices[5] = 19;
  }
  else if(iK == 4)
  {
    indices[0] = 15;
    indices[1] = 31;
    indices[2] = 30;
    indices[3] = 22;
    indices[4] = 29;
    indices[5] = 32;
  }
  else if(iK == 5)
  {
    indices[0] =  5;
    indices[1] = 23;
    indices[2] = 22;
    indices[3] = 20;
    indices[4] = 21;
    indices[5] = 24;
  }
  else if(iK == 6)
  {
    indices[0] = 10;
    indices[1] = 21;
    indices[2] = 27;
    indices[3] = 25;
    indices[4] = 26;
    indices[5] = 28;
  }
  else if(iK == 7)
  {
    indices[0] = 19;
    indices[1] = 29;
    indices[2] = 34;
    indices[3] = 27;
    indices[4] = 33;
    indices[5] = 35;
  }
}

/*---------------------------------------------------------*/

void FemVRTM3d::getReferenceCenterPoint(Alat::Node& xhat) const
{
  xhat.zeros();
}
