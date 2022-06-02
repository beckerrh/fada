#include  "Fada/datafunction.h"
#include  "Alat/node.h"
#include  <cassert>
#include  <iostream>
#include  <stdlib.h>

using namespace Fada;

/*--------------------------------------------------------------------------*/
DataFunction::~DataFunction(){}
DataFunction::DataFunction() : _eps(1.e-6){}
DataFunction::DataFunction( const DataFunction& datafunction)
{
  _eps = datafunction._eps;
  assert(0);
}
DataFunction& DataFunction::operator=( const DataFunction& datafunction)
{
  _eps = datafunction._eps;
  assert(0);
  return *this;
}
std::string DataFunction::getName() const
{
  return "DataFunction";
}
double DataFunction::operator() (double x, double y, double z, double t) const
{
  std::cerr<<"\"DataFunction::operator()\" not written!"<<std::endl;
  assert(0);
  exit(1);
}

/*----------------------------------------------------------*/
double DataFunction::d (int i, double xv, double yv, double zv, double t) const
{
  if(i == 0)
  {
    return x(xv, yv, zv, t);
  }
  else if(i == 1)
  {
    return y(xv, yv, zv, t);
  }
  return z(xv, yv, zv, t);
}

double DataFunction::dd (int i, int j, double xv, double yv, double zv, double t) const
{
  if(i == 0)
  {
    if(j == 0)
    {
      return xx(xv, yv, zv, t);
    }
    else if(j == 1)
    {
      return xy(xv, yv, zv, t);
    }
    return xz(xv, yv, zv, t);
  }
  else if(i == 1)
  {
    if(j == 0)
    {
      return xy(xv, yv, zv, t);
    }
    else if(j == 1)
    {
      return yy(xv, yv, zv, t);
    }
    return yz(xv, yv, zv, t);
  }
  if(j == 0)
  {
    return xz(xv, yv, zv, t);
  }
  else if(j == 1)
  {
    return yz(xv, yv, zv, t);
  }
  return zz(xv, yv, zv, t);
}

double DataFunction::x(double vx, double vy, double vz, double t) const
{
  Alat::Node vl(vx, vy, vz), vr(vx, vy, vz);
  vr.x() += _eps;
  vl.x() -= _eps;
  return ( ( *this )( vr.x(), vr.y(), vr.z(), t )-( *this )( vl.x(), vl.y(), vl.z(), t ) )/( 2.*_eps );
}

double DataFunction::y(double vx, double vy, double vz, double t) const
{
  Alat::Node vl(vx, vy, vz), vr(vx, vy, vz);
  vr.y() += _eps;
  vl.y() -= _eps;
  return ( ( *this )( vr.x(), vr.y(), vr.z(), t )-( *this )( vl.x(), vl.y(), vl.z(), t ) )/( 2.*_eps );
}

double DataFunction::z(double vx, double vy, double vz, double t) const
{
  Alat::Node vl(vx, vy, vz), vr(vx, vy, vz);
  vr.z() += _eps;
  vl.z() -= _eps;
  // std::cerr << "DataFunction::z() " << ( *this )( vr.x(), vr.y(), vr.z(), t ) << " " << ( *this )( vl.x(), vl.y(), vl.z(), t ) << " " << ( ( *this )( vr.x(), vr.y(), vr.z(), t )-( *this )( vl.x(), vl.y(), vl.z(), t ) )/( 2.*_eps );
  return ( ( *this )( vr.x(), vr.y(), vr.z(), t )-( *this )( vl.x(), vl.y(), vl.z(), t ) )/( 2.*_eps );
}

double DataFunction::t(double vx, double vy, double vz, double t) const
{
  return ( ( *this )( vx, vy, vz, t+_eps )-( *this )( vx, vy, vz, t-_eps ) )/( 2.*_eps );
}

double DataFunction::xx(double vx, double vy, double vz, double t) const
{
  Alat::Node vl(vx, vy, vz), vr(vx, vy, vz);
  vr.x() += _eps;
  vl.x() -= _eps;
  return ( -2.*( *this )( vx, vy, vz, t )+( *this )( vr.x(), vr.y(), vr.z(), t )+( *this )( vl.x(), vl.y(), vl.z(), t ) )/( _eps*_eps );
}

double DataFunction::yy(double vx, double vy, double vz, double t) const
{
  Alat::Node vl(vx, vy, vz), vr(vx, vy, vz);
  vr.y() += _eps;
  vl.y() -= _eps;
  return ( -2.*( *this )( vx, vy, vz, t )+( *this )( vr.x(), vr.y(), vr.z(), t )+( *this )( vl.x(), vl.y(), vl.z(), t ) )/( _eps*_eps );
}

double DataFunction::zz(double vx, double vy, double vz, double t) const
{
  Alat::Node vl(vx, vy, vz), vr(vx, vy, vz);
  vr.z() += _eps;
  vl.z() -= _eps;
  return ( -2.*( *this )( vx, vy, vz, t )+( *this )( vr.x(), vr.y(), vr.z(), t )+( *this )( vl.x(), vl.y(), vl.z(), t ) )/( _eps*_eps );
}

double DataFunction::xy(double vx, double vy, double vz, double t) const
{
  Alat::Node vl(vx, vy, vz), vr(vx, vy, vz);
  vr.y() += _eps;
  vl.y() -= _eps;
  return ( x(vr.x(), vr.y(), vr.z(), t)-x(vl.x(), vl.y(), vl.z(), t) )/( 2.*_eps );
}

double DataFunction::xz(double vx, double vy, double vz, double t) const
{
  Alat::Node vl(vx, vy, vz), vr(vx, vy, vz);
  vr.z() += _eps;
  vl.z() -= _eps;
  return ( x( vr.x(), vr.y(), vr.z() )-x( vl.x(), vl.y(), vl.z() ) )/( 2.*_eps );
}

double DataFunction::yz(double vx, double vy, double vz, double t) const
{
  Alat::Node vl(vx, vy, vz), vr(vx, vy, vz);
  vr.z() += _eps;
  vl.z() -= _eps;
  return ( y(vr.x(), vr.y(), vr.z(), t)-y(vl.x(), vl.y(), vl.z(), t) )/( 2.*_eps );
}
