#include  "Fada/p12dtransformation.h"
#include  <cmath>

using namespace Fada;


/*---------------------------------------------------------*/

P12DTransformation::P12DTransformation() : TransformationBase() {}

/*---------------------------------------------------------*/

P12DTransformation::P12DTransformation(const P12DTransformation& p12dtransformation) : TransformationBase(p12dtransformation) {}

/*---------------------------------------------------------*/

P12DTransformation& P12DTransformation::operator=(const P12DTransformation& p12dtransformation)
{
  assert(0);
  return *this;
}

/*---------------------------------------------------------*/

P12DTransformation* P12DTransformation::clone() const
{
  return new P12DTransformation(*this);
}

/*--------------------------------------------------------------------------*/

void P12DTransformation::dtrans(const Alat::Node& xhat)
{
  getDTransformation()[0].x() = -1.0;
  getDTransformation()[1].x() = 1.0;
  getDTransformation()[2].x() = 0.0;
  getDTransformation()[0].y() = -1.0;
  getDTransformation()[1].y() = 0.0;
  getDTransformation()[2].y() = 1.0;
}

/*---------------------------------------------------------*/

void P12DTransformation::trans(const Alat::Node& xhat)
{
  getTransformation()[0] = 1.0-xhat.x()-xhat.y();
  getTransformation()[1] = xhat.x();
  getTransformation()[2] = xhat.y();
}

/*---------------------------------------------------------*/

void P12DTransformation::setNormalHat( int iiS, Alat::Node& normalhat ) const
{
  assert( iiS > -1 );
  assert( iiS < 3 );
  if( iiS == 1 )
  {
    normalhat.x() = -1.0;
    normalhat.y() = 0.0;
  }
  else if( iiS == 0 )
  {
    normalhat.x() = 1.0/sqrt(2.0) ;
    normalhat.y() = 1.0/sqrt(2.0);
  }
  else if( iiS == 2 )
  {
    normalhat.x() = 0.0 ;
    normalhat.y() = -1.0;
  }
}

/*---------------------------------------------------------*/

void P12DTransformation::ShatToXhat( Alat::Node& xhat, const Alat::Node& s, int iiS)
{
  if(iiS == 2)
  {
    xhat.x() = s.x();
    xhat.y() = 0;
  }
  else if(iiS == 0)
  {
    xhat.x() = 1.0-s.x();
    xhat.y() = s.x();
  }
  else
  {
    xhat.x() = 0.0;
    // xhat.y() = s.x();
    xhat.y() = 1.0-s.x();
  }
}

/*---------------------------------------------------------*/

double P12DTransformation::getSurfaceJacobianHat(int iiS) const
{
  if(iiS == 0)
  {
    return sqrt(2.0);
  }
  else
  {
    return 1.0;
  }
}

