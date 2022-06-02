#include  "Fada/q22dtransformation.h"

using namespace Fada;

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

/*---------------------------------------------------------*/

Q22DTransformation::~Q22DTransformation() {}

/*---------------------------------------------------------*/

Q22DTransformation::Q22DTransformation() : Q12DTransformation() {}

/*---------------------------------------------------------*/

Q22DTransformation::Q22DTransformation(const Q22DTransformation& q22dtransformation) : Q12DTransformation(q22dtransformation) {}

/*---------------------------------------------------------*/

Q22DTransformation& Q22DTransformation::operator=(const Q22DTransformation& q22dtransformation)
{
  assert(0);
  return *this;
}

/*---------------------------------------------------------*/

Q22DTransformation* Q22DTransformation::clone() const
{
  return new Q22DTransformation(*this);
}

/*---------------------------------------------------------*/

std::string Q22DTransformation::getName() const
{
  return "Q22DTransformation";
}

/*---------------------------------------------------------*/

int Q22DTransformation::getNNodes() const
{
  return 9;
}

/*---------------------------------------------------------*/

void Q22DTransformation::dtrans(const Alat::Node& xhat)
{
  double x = xhat.x(), y = xhat.y();
  assert( donebasicInit() );
  getDTransformation()[0].x() =  0.25*y*( 1.0-y )*( 1.0-2.0*x );
  getDTransformation()[1].x() = -0.25*y*( 1.0-y )*( 1.0+2.0*x );
  getDTransformation()[2].x() =  0.25*y*( 1.0+y )*( 1.0+2.0*x );
  getDTransformation()[3].x() = -0.25*y*( 1.0+y )*( 1.0-2.0*x );
  getDTransformation()[4].x() =  x*y*( 1.0-y );
  getDTransformation()[5].x() =  0.5*( 1.0-y )*( 1.0+y )*( 1.0+2.0*x );
  getDTransformation()[6].x() = -x*y*( 1.0+y );
  getDTransformation()[7].x() = -0.5*( 1.0-y )*( 1.0+y )*( 1.0-2.0*x );
  getDTransformation()[8].x() = x*( y*y-1.0 )*2.0;

  getDTransformation()[0].y() =  0.25*x*( 1.0-x )*( 1.0-2.0*y );
  getDTransformation()[1].y() = -0.25*x*( 1.0+x )*( 1.0-2.0*y );
  getDTransformation()[2].y() =  0.25*x*( 1.0+x )*( 1.0+2.0*y );
  getDTransformation()[3].y() = -0.25*x*( 1.0-x )*( 1.0+2.0*y );
  getDTransformation()[4].y() = -0.5*( 1.0+x )*( 1.0-x )*( 1.0-2.0*y );
  getDTransformation()[5].y() = -x*y*( 1.0+x );
  getDTransformation()[6].y() = 0.5*( 1.0+x )*( 1.0-x )*( 1.0+2.0*y );
  getDTransformation()[7].y() = x*y*( 1.0-x );
  getDTransformation()[8].y() = y*( x*x-1.0 )*2.0;
}

/*---------------------------------------------------------*/

void Q22DTransformation::trans(const Alat::Node& xhat)
{
  double x = xhat.x(), y = xhat.y();
  assert( donebasicInit() );
  getTransformation()[0] =  0.25*x*y*( 1.0-x )*( 1.0-y );
  getTransformation()[1] = -0.25*x*y*( 1.0+x )*( 1.0-y );
  getTransformation()[2] =  0.25*x*y*( 1.0+x )*( 1.0+y );
  getTransformation()[3] = -0.25*x*y*( 1.0-x )*( 1.0+y );
  getTransformation()[4] = -0.5*( 1.0+x )*( 1.0-x )*y*( 1.0-y );
  getTransformation()[5] =  0.5*x*( 1.0+x )*( 1.0-y )*( 1.0+y );
  getTransformation()[6] =  0.5*( 1.0-x )*( 1.0+x )*y*( 1.0+y );
  getTransformation()[7] = -0.5*x*( 1.0-x )*( 1.0-y )*( 1.0+y );
  getTransformation()[8] = ( 1.0-x*x )*( 1.0-y*y );
}

/*-------------------------------------------------------*/

void Q22DTransformation::reInitCell(const Alat::Vector<Alat::Node>& nodes)
{
  for(int ii = 0; ii < 9; ii++)
  {
    getNodes()[ii] = nodes[ii];
  }
  // std::cerr << "# Q22DTransformation::reInitCell() " << getV() << "\n";
}
