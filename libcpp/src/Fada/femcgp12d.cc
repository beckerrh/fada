#include  "Alat/assemblevector.h"
#include  "Fada/localdofinformation.h"
#include  "Fada/femcgp12d.h"
#include  "Fada/integrationformulatriangle.h"

using namespace std;
using namespace Fada;
//         2
//        | \
//        |0 \
//        0---1
//        |\3 |\
//        |1\ |2\
//        4--3---5

//
//        2
//        | \
//        |  \
//        |   \
//        0----1
/*-------------------------------------------------------*/


bool FemCgP12d::interpolationMatrixWrong() const {return true;}
void FemCgP12d::interpolate(Alat::AssembleVector& uh, const Alat::AssembleVector& uH) const
{
  for(int icomp=0; icomp<uh.ncomp();icomp++)
  {
    for(int ii=0;ii<3;ii++)
    {
      // uh[icomp][ii] = uH[icomp][ii];
      uh(icomp,ii) = uH(icomp,ii);
    }
    // uh[icomp][3] = 0.5*( uH[icomp][0] +  uH[icomp][1]);
    // uh[icomp][4] = 0.5*( uH[icomp][1] +  uH[icomp][2]);
    // uh[icomp][5] = 0.5*( uH[icomp][2] +  uH[icomp][0]);
    uh(icomp,3) = 0.5*( uH(icomp,0) +  uH(icomp,1)); 
    uh(icomp,4) = 0.5*( uH(icomp,1) +  uH(icomp,2)); 
    uh(icomp,5) = 0.5*( uH(icomp,2) +  uH(icomp,0)); 
  }
}



FemCgP12d::~FemCgP12d() {}
FemCgP12d::FemCgP12d() : FemPk2d<1>() {}
FemCgP12d::FemCgP12d(const FemCgP12d& orig) : FemPk2d<1>(orig)
{}
FemCgP12d& FemCgP12d::operator=(const FemCgP12d& orig)
{
  assert(0);
}
/*-------------------------------------------------------*/
FemCgP12d* FemCgP12d::clone() const
{
  return new FemCgP12d(*this);
}
/*-------------------------------------------------------*/
std::string FemCgP12d::getName() const
{
  return "FemP12d";
}
/*-------------------------------------------------------*/

void FemCgP12d::reInitReferencePoint(const Alat::Node& xhat)
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

/*-------------------------------------------------------*/

void FemCgP12d::setDofDescription(Fada::LocalDofInformation& dof_description) const
{
  dof_description.nodes() = 1;
}

/*-------------------------------------------------------*/

int FemCgP12d::getOrder() const
{
  return 1;
}

/*---------------------------------------------------------*/
Fada::IntegrationFormulaInterface* FemCgP12d::newIntegrationFormulaTransfer() const
{                       
  return new Fada::TriangleTrapez;
  return new Fada::TriangleDegree4;
}
