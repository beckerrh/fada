#include  "Fada/q12dtransformation.h"

using namespace Fada;

/*---------------------------------------------------------*/

Q12DTransformation::~Q12DTransformation() {}

/*---------------------------------------------------------*/

Q12DTransformation::Q12DTransformation() : TransformationBase() {}

/*---------------------------------------------------------*/

Q12DTransformation::Q12DTransformation(const Q12DTransformation& q12dtransformation) : TransformationBase(q12dtransformation) {}

/*---------------------------------------------------------*/

Q12DTransformation& Q12DTransformation::operator=(const Q12DTransformation& q12dtransformation)
{
  assert(0);
  return *this;
}

/*---------------------------------------------------------*/

Q12DTransformation* Q12DTransformation::clone() const
{
  return new Q12DTransformation(*this);
}

/*---------------------------------------------------------*/

std::string Q12DTransformation::getName() const
{
  return "Q12DTranformation";
}

/*---------------------------------------------------------*/

int Q12DTransformation::getNNodes() const
{
  return 4;
}

/*---------------------------------------------------------*/


void Q12DTransformation::correctIntegrationPoint(Alat::Node& shat) const
{
  shat.x() = 2.0*shat.x() -1;
}

/*---------------------------------------------------------*/

double Q12DTransformation::getSurfaceJacobianHat(int iiS) const
{
  // par ce que formule d'integration 1D sur [0,1] !!!
  return 2.0;
}

/*---------------------------------------------------------*/

void Q12DTransformation::dtrans(const Alat::Node& xhat)
{
  assert( donebasicInit() );
  getDTransformation()[0].x() = -0.25*( 1.0-xhat.y() );
  getDTransformation()[1].x() = 0.25*( 1.0-xhat.y() );
  getDTransformation()[2].x() = 0.25*( 1.0+xhat.y() );
  getDTransformation()[3].x() = -0.25*( 1.0+xhat.y() );
  getDTransformation()[0].y() = -0.25*( 1.0-xhat.x() );
  getDTransformation()[1].y() = -0.25*( 1.0+xhat.x() );
  getDTransformation()[2].y() = 0.25*( 1.0+xhat.x() );
  getDTransformation()[3].y() = 0.25*( 1.0-xhat.x() );
}

/*---------------------------------------------------------*/

void Q12DTransformation::trans(const Alat::Node& xhat)
{
  assert( donebasicInit() );
  getTransformation()[0] = 0.25*( 1.0-xhat.x() )*( 1.0-xhat.y() );
  getTransformation()[1] = 0.25*( 1.0+xhat.x() )*( 1.0-xhat.y() );
  getTransformation()[2] = 0.25*( 1.0+xhat.x() )*( 1.0+xhat.y() );
  getTransformation()[3] = 0.25*( 1.0-xhat.x() )*( 1.0+xhat.y() );
}

/*---------------------------------------------------------*/

void Q12DTransformation::setNormalHat( int iiS, Alat::Node& normalhat ) const
{
  assert( iiS > -1 );
  assert( iiS < 4 );
  if( iiS == 0 )
  {
    normalhat.x() = 0.0;
    normalhat.y() = -1.0;
  }
  else if( iiS == 1 )
  {
    normalhat.x() = 1.0;
    normalhat.y() = 0.0;
  }
  else if( iiS == 2 )
  {
    normalhat.x() = 0.0;
    normalhat.y() = 1.0;
  }
  else
  {
    normalhat.x() = -1.0;
    normalhat.y() = 0.0;
  }
  normalhat.z() = 0.0;
}

/*---------------------------------------------------------*/

void Q12DTransformation::ShatToXhat( Alat::Node& xhat, const Alat::Node& s, int iiS)
{
  if(iiS == 0)
  {
    xhat.x() = s.x();
    xhat.y() = -1.0;
  }
  else if(iiS == 1)
  {
    xhat.x() = 1.0;
    xhat.y() = s.x();
  }
  else if(iiS == 2)
  {
    // xhat.x() = s.x();
    xhat.x() = -s.x();
    xhat.y() = 1.0;
  }
  else
  {
    xhat.x() = -1.0;
    // xhat.y() = s.x();
    xhat.y() = -s.x();
  }
}
