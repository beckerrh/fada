#include  "Fada/q13dtransformation.h"

using namespace Fada;

/*---------------------------------------------------------*/

Q13DTransformation::~Q13DTransformation() {}

/*---------------------------------------------------------*/

Q13DTransformation::Q13DTransformation() : TransformationBase() {}

/*---------------------------------------------------------*/

Q13DTransformation::Q13DTransformation(const Q13DTransformation& q13dtransformation) : TransformationBase(q13dtransformation) {}

/*---------------------------------------------------------*/

Q13DTransformation& Q13DTransformation::operator=(const Q13DTransformation& q13dtransformation)
{
  assert(0);
  return *this;
}

/*---------------------------------------------------------*/

Q13DTransformation* Q13DTransformation::clone() const
{
  return new Q13DTransformation(*this);
}

/*---------------------------------------------------------*/

std::string Q13DTransformation::getName() const
{
  return "Q13DTransformation";
}

/*---------------------------------------------------------*/

int Q13DTransformation::getNNodes() const
{
  return 8;
}

/*---------------------------------------------------------*/

double Q13DTransformation::getSurfaceJacobianHat(int iiS) const
{
  return 1.0;
}

/*--------------------------------------------------------------------------*/

void Q13DTransformation::dtrans(const Alat::Node& xhat)
{
  // std::cerr << "Q13DTransformation::dtrans() " << xhat<< "\n";
  getDTransformation()[0].x() = -0.125*( 1.0-xhat.y() )*( 1.0-xhat.z() );
  getDTransformation()[1].x() = 0.125*( 1.0-xhat.y() )*( 1.0-xhat.z() );
  getDTransformation()[2].x() = 0.125*( 1.0+xhat.y() )*( 1.0-xhat.z() );
  getDTransformation()[3].x() = -0.125*( 1.0+xhat.y() )*( 1.0-xhat.z() );
  getDTransformation()[4].x() = -0.125*( 1.0-xhat.y() )*( 1.0+xhat.z() );
  getDTransformation()[5].x() = 0.125*( 1.0-xhat.y() )*( 1.0+xhat.z() );
  getDTransformation()[6].x() = 0.125*( 1.0+xhat.y() )*( 1.0+xhat.z() );
  getDTransformation()[7].x() = -0.125*( 1.0+xhat.y() )*( 1.0+xhat.z() );

  getDTransformation()[0].y() = -0.125*( 1.0-xhat.x() )*( 1.0-xhat.z() );
  getDTransformation()[1].y() = -0.125*( 1.0+xhat.x() )*( 1.0-xhat.z() );
  getDTransformation()[2].y() = 0.125*( 1.0+xhat.x() )*( 1.0-xhat.z() );
  getDTransformation()[3].y() = 0.125*( 1.0-xhat.x() )*( 1.0-xhat.z() );
  getDTransformation()[4].y() = -0.125*( 1.0-xhat.x() )*( 1.0+xhat.z() );
  getDTransformation()[5].y() = -0.125*( 1.0+xhat.x() )*( 1.0+xhat.z() );
  getDTransformation()[6].y() = 0.125*( 1.0+xhat.x() )*( 1.0+xhat.z() );
  getDTransformation()[7].y() = 0.125*( 1.0-xhat.x() )*( 1.0+xhat.z() );

  getDTransformation()[0].z() = -0.125*( 1.0-xhat.x() )*( 1.0-xhat.y() );
  getDTransformation()[1].z() = -0.125*( 1.0+xhat.x() )*( 1.0-xhat.y() );
  getDTransformation()[2].z() = -0.125*( 1.0+xhat.x() )*( 1.0+xhat.y() );
  getDTransformation()[3].z() = -0.125*( 1.0-xhat.x() )*( 1.0+xhat.y() );
  getDTransformation()[4].z() = 0.125*( 1.0-xhat.x() )*( 1.0-xhat.y() );
  getDTransformation()[5].z() = 0.125*( 1.0+xhat.x() )*( 1.0-xhat.y() );
  getDTransformation()[6].z() = 0.125*( 1.0+xhat.x() )*( 1.0+xhat.y() );
  getDTransformation()[7].z() = 0.125*( 1.0-xhat.x() )*( 1.0+xhat.y() );
}

/*--------------------------------------------------------------------------*/

void Q13DTransformation::trans(const Alat::Node& xhat)
{
  getTransformation()[0] = 0.125*( 1.0-xhat.x() )*( 1.0-xhat.y() )*( 1.0-xhat.z() );
  getTransformation()[1] = 0.125*( 1.0+xhat.x() )*( 1.0-xhat.y() )*( 1.0-xhat.z() );
  getTransformation()[2] = 0.125*( 1.0+xhat.x() )*( 1.0+xhat.y() )*( 1.0-xhat.z() );
  getTransformation()[3] = 0.125*( 1.0-xhat.x() )*( 1.0+xhat.y() )*( 1.0-xhat.z() );
  getTransformation()[4] = 0.125*( 1.0-xhat.x() )*( 1.0-xhat.y() )*( 1.0+xhat.z() );
  getTransformation()[5] = 0.125*( 1.0+xhat.x() )*( 1.0-xhat.y() )*( 1.0+xhat.z() );
  getTransformation()[6] = 0.125*( 1.0+xhat.x() )*( 1.0+xhat.y() )*( 1.0+xhat.z() );
  getTransformation()[7] = 0.125*( 1.0-xhat.x() )*( 1.0+xhat.y() )*( 1.0+xhat.z() );
}

/*---------------------------------------------------------*/

void Q13DTransformation::setNormalHat( int iiS, Alat::Node& normalhat ) const
{
  assert( iiS > -1 );
  assert( iiS < 6 );
  if( iiS == 0 )
  {
    normalhat.x() = 0.0;
    normalhat.y() = 0.0;
    normalhat.z() = -1.0;
  }
  else if( iiS == 1 )
  {
    normalhat.x() = 0.0;
    normalhat.y() = -1.0;
    normalhat.z() = 0.0;
  }
  else if( iiS == 2 )
  {
    normalhat.x() = -1.0;
    normalhat.y() = 0.0;
    normalhat.z() = 0.0;
  }
  else if(iiS == 3 )
  {
    normalhat.x() = 1.0;
    normalhat.y() = 0.0;
    normalhat.z() = 0.0;
  }
  else if( iiS == 4 )
  {
    normalhat.x() = 0.0;
    normalhat.y() = 1.0;
    normalhat.z() = 0.0;
  }
  else if(iiS == 5 )
  {
    normalhat.x() = 0.0;
    normalhat.y() = 0.0;
    normalhat.z() = 1.0;
  }
}

/*---------------------------------------------------------*/

void Q13DTransformation::ShatToXhat( Alat::Node& xhat, const Alat::Node& s, int iiS)
{
  if(iiS == 0)
  {
    xhat.x() = s.x();
    xhat.y() = s.y();
    xhat.z() = -1.0;
  }
  else if(iiS == 1)
  {
    xhat.x() = s.y();
    xhat.y() = -1.0;
    xhat.z() = s.x();
  }
  else if(iiS == 2)
  {
    xhat.x() = -1.0;
    xhat.y() = s.x();
    xhat.z() = s.y();
  }
  else if(iiS == 3)
  {
    xhat.x() = 1.0;
    xhat.y() = s.y();
    xhat.z() = s.x();
  }
  else if(iiS == 4)
  {
    xhat.x() = s.x();
    xhat.y() = 1.0;
    xhat.z() = s.y();
  }
  else if(iiS == 5)
  {
    xhat.x() = s.y();
    xhat.y() = s.x();
    xhat.z() = 1.0;
  }
  // std::cerr << "Q13DTransformation::ShatToXhat() s="<<s<<" xhat="<<xhat<<"\n";
}
