#include  "Fada/localdofinformation.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/femcgq22d.h"
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

FemCgQ22d::~FemCgQ22d() {}
FemCgQ22d::FemCgQ22d() : FemQk2d<2>() {}
FemCgQ22d::FemCgQ22d(const FemCgQ22d& orig) : FemQk2d<2>(orig)
{}
FemCgQ22d& FemCgQ22d::operator=(const FemCgQ22d& orig)
{
  assert(0);
}

FemCgQ22d* FemCgQ22d::clone() const
{
  return new FemCgQ22d(*this);
}

std::string FemCgQ22d::getName() const
{
  return "FemCgQ22d";
}

/*-------------------------------------------------------*/

int FemCgQ22d::getOrder() const
{
  return 2;
}

/*-------------------------------------------------------*/

void FemCgQ22d::setDofDescription(Fada::LocalDofInformation& dof_description) const
{
  dof_description.nodes() = 1;
  dof_description.sides() = 1;
  dof_description.cells() = 1;
}

/*-------------------------------------------------------*/

void FemCgQ22d::reInitReferencePoint(const Alat::Node& xhat)
{
  FemQk2d<2>::reInitReferencePoint(xhat);
  double x = xhat.x(), y = xhat.y();

  //Base Q2 hierarchique legendre
  phi()[0] = 0.25*( 1.0-x )*( 1.0-y );
  phi()[1] = 0.25*( 1.0+x )*( 1.0-y );
  phi()[2] = 0.25*( 1.0+x )*( 1.0+y );
  phi()[3] = 0.25*( 1.0-x )*( 1.0+y );
  phi()[4] = sqrt(6.0)*( x*x-1.0 )*( y-1.0 )*( -1.0/8.0 );
  phi()[5] = sqrt(6.0)*( y*y-1.0 )*( x+1.0 )*( 1.0/8.0 );
  phi()[6] = sqrt(6.0)*( x*x-1.0 )*( y+1.0 )*( 1.0/8.0 );
  phi()[7] = sqrt(6.0)*( y*y-1.0 )*( x-1.0 )*( -1.0/8.0 );
  phi()[8] = ( x*x-1.0 )*( y*y-1.0 );

  //grad Q2
  dphi()[0].x() = y*( 1.0/4.0 )-1.0/4.0;
  dphi()[1].x() = y*( -1.0/4.0 )+1.0/4.0;
  dphi()[2].x() = y*( 1.0/4.0 )+1.0/4.0;
  dphi()[3].x() = y*( -1.0/4.0 )-1.0/4.0;
  dphi()[4].x() = sqrt(6.0)*x*( y-1.0 )*( -1.0/4.0 );
  dphi()[5].x() = sqrt(6.0)*( y*y-1.0 )*( 1.0/8.0 );
  dphi()[6].x() = sqrt(6.0)*x*( y+1.0 )*( 1.0/4.0 );
  dphi()[7].x() = sqrt(6.0)*( y*y-1.0 )*( -1.0/8.0 );
  dphi()[8].x() = x*( y*y-1.0 )*2.0;

  dphi()[0].y() = x*( 1.0/4.0 )-1.0/4.0;
  dphi()[1].y() = x*( -1.0/4.0 )-1.0/4.0;
  dphi()[2].y() = x*( 1.0/4.0 )+1.0/4.0;
  dphi()[3].y() = x*( -1.0/4.0 )+1.0/4.0;
  dphi()[4].y() = sqrt(6.0)*( x*x-1.0 )*( -1.0/8.0 );
  dphi()[5].y() = sqrt(6.0)*y*( x+1.0 )*( 1.0/4.0 );
  dphi()[6].y() = sqrt(6.0)*( x*x-1.0 )*( 1.0/8.0 );
  dphi()[7].y() = sqrt(6.0)*y*( x-1.0 )*( -1.0/4.0 );
  dphi()[8].y() = y*( x*x-1.0 )*2.0;

  //Grad sur l'element
  const Alat::DoubleMatrix& TIT = getTransformation()->getInvJacobian();
  for(int ii = 0; ii < 9; ii++)
  {
    double temp = dphi()[ii].x();
    dphi()[ii].x() = TIT(0, 0)*temp + TIT(0, 1)*dphi()[ii].y();
    dphi()[ii].y() = TIT(1, 0)*temp + TIT(1, 1)*dphi()[ii].y();
  }
}

// /*-------------------------------------------------------*/
// void FemCgQ22d::computeHatSecondDerivatives(Alat::Vector<Alat::FixArray<6, double> >& d2phihat,const Alat::Node& xhat)
// {
//     double x = xhat.x(), y = xhat.y();
//     //d2phihat/dxdx
//     d2phihat[0][0] = 0.0;
//     d2phihat[1][0] = 0.0;
//     d2phihat[2][0] = 0.0;
//     d2phihat[3][0] = 0.0;
//     d2phihat[4][0] = sqrt(6.0)*(y-1.0)*(-1.0/4.0);
//     d2phihat[5][0] = 0.0;
//     d2phihat[6][0] = sqrt(6.0)*(y+1.0)*(1.0/4.0);
//     d2phihat[7][0] = 0.0;
//     d2phihat[8][0] = (y*y-1.0)*2.0;
//
//     //d2phihat/dxdy
//     d2phihat[0][1] = 0.25;
//     d2phihat[1][1] = -0.25;
//     d2phihat[2][1] = 0.25;
//     d2phihat[3][1] = -0.25;
//     d2phihat[4][1] = sqrt(6.0)*x*(-1.0/4.0);
//     d2phihat[5][1] = sqrt(6.0)*y*(1.0/4.0);
//     d2phihat[6][1] = sqrt(6.0)*x*(1.0/4.0);
//     d2phihat[7][1] = sqrt(6.0)*y*(-1.0/4.0);
//     d2phihat[8][1] = y*x*4.0;
//
//     //d2phihat/dydy
//     d2phihat[0][3] = 0.0;
//     d2phihat[1][3] = 0.0;
//     d2phihat[2][3] = 0.0;
//     d2phihat[3][3] = 0.0;
//     d2phihat[4][3] = 0.0;
//     d2phihat[5][3] = sqrt(6.0)*(x+1.0)*(1.0/4.0);
//     d2phihat[6][3] = 0.0;
//     d2phihat[7][3] = sqrt(6.0)*(x-1.0)*(-1.0/4.0);
//     d2phihat[8][3] = (x*x-1.0)*2.0;
// }


/*---------------------------------------------------------*/

Fada::IntegrationFormulaInterface* FemCgQ22d::newIntegrationFormulaTransfer() const
{
  return new Fada::QuadGauss16;
}

// /*---------------------------------------------------------*/
//
// void FemCgQ22d::getLocalCellIndicesPatch(int iK, Alat::IntVector& indices) const
// {
//     //        8 --19-- 5 --14-- 4
//     //        |        |        |
//     //       20  24/3  15 22/2  13
//     //        |        |        |
//     //        7 --17-- 3 --11-- 2
//     //        |        |        |
//     //       18  23/0  12 21/1  10
//     //        |        |        |
//     //        6 --16-- 0 ---9-- 1
//     //
//   indices.set_size( ndoflocal() );
//   if(iK == 0)
//   {
//     indices[0] = 6;
//     indices[1] = 0;
//     indices[2] = 3;
//     indices[3] = 7;
//     indices[4] = 16;
//     indices[5] = 12;
//     indices[6] = 17;
//     indices[7] = 18;
//     indices[8] = 23;
//   }
//   else if(iK == 1)
//   {
//     indices[0] = 0;
//     indices[1] = 1;
//     indices[2] = 2;
//     indices[3] = 3;
//     indices[4] = 9;
//     indices[5] = 10;
//     indices[6] = 11;
//     indices[7] = 12;
//     indices[8] = 21;
//   }
//   else if(iK == 2)
//   {
//     indices[0] = 3;
//     indices[1] = 2;
//     indices[2] = 4;
//     indices[3] = 5;
//     indices[4] = 11;
//     indices[5] = 13;
//     indices[6] = 14;
//     indices[7] = 15;
//     indices[8] = 22;
//   }
//   else if(iK == 3)
//   {
//     indices[0] = 7;
//     indices[1] = 3;
//     indices[2] = 5;
//     indices[3] = 8;
//     indices[4] = 17;
//     indices[5] = 15;
//     indices[6] = 19;
//     indices[7] = 20;
//     indices[8] = 24;
//   }
//
// }
