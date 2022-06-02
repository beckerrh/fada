#include  "Fada/simpleexactsolutions.h"
#include  <math.h>

using namespace Fada;
using namespace std;

/*---------------------------------------------------------*/

ConstantSolution::ConstantSolution(double d) : _d(d) {}
string ConstantSolution::getName() const
{
  return "ConstantSolution";
}

double ConstantSolution::operator()(double x, double y, double z, double t) const
{
  return _d;
}

double ConstantSolution::x(double x, double y, double z, double t) const
{
  return 0.0;
}

double ConstantSolution::y(double x, double y, double z, double t) const
{
  return 0.0;
}

double ConstantSolution::z(double x, double y, double z, double t) const
{
  return 0.0;
}

double ConstantSolution::t(double x, double y, double z, double t) const
{
  return 0.0;
}

double ConstantSolution::xx(double x, double y, double z, double t) const
{
  return 0.0;
}

double ConstantSolution::yy(double x, double y, double z, double t) const
{
  return 0.0;
}

double ConstantSolution::zz(double x, double y, double z, double t) const
{
  return 0.;
}

/*---------------------------------------------------------*/

LinearSolution::LinearSolution(double a, double b, double c, double d) : _a(a), _b(b), _c(c), _d(d) {}
string LinearSolution::getName() const
{
  return "LinearSolution";
}

double LinearSolution::operator()(double x, double y, double z, double t) const
{
  return _a+_b*x+_c*y+_d*z;
}

double LinearSolution::x(double x, double y, double z, double t) const
{
  return _b;
}

double LinearSolution::y(double x, double y, double z, double t) const
{
  return _c;
}

double LinearSolution::z(double x, double y, double z, double t) const
{
  return _d;
}

double LinearSolution::xx(double x, double y, double z, double t) const
{
  return 0.0;
}

double LinearSolution::yy(double x, double y, double z, double t) const
{
  return 0.0;
}

double LinearSolution::zz(double x, double y, double z, double t) const
{
  return 0.;
}

/*---------------------------------------------------------*/

QuadraticSolution::QuadraticSolution(double a, double b, double c, double d, double e, double f, double g, double h, double i) : _a(a), _b(b), _c(c), _d(d), _e(e), _f(f), _g(g), _h(h), _i(i) {}
string QuadraticSolution::getName() const
{
  return "QuadraticSolution";
}

double QuadraticSolution::operator()(double x, double y, double z, double t) const
{
  return _a+_b*x+_c*y+_d*x*x+_e*y*y+_f*x*y + _g*x*z + _h*y*z + _i*z*z;
}

double QuadraticSolution::x(double x, double y, double z, double t) const
{
  return _b+2.0*_d*x+_f*y + _g*z;
}

double QuadraticSolution::y(double x, double y, double z, double t) const
{
  return _c+2.0*_e*y+_f*x + _h*z;
}

double QuadraticSolution::z(double x, double y, double z, double t) const
{
  return _g*x + _h*y + 2.0*_i*z;
}

double QuadraticSolution::xx(double x, double y, double z, double t) const
{
  return 2.0*_d;
}

double QuadraticSolution::yy(double x, double y, double z, double t) const
{
  return 2.0*_e;
}

double QuadraticSolution::zz(double vx, double vy, double vz, double t) const
{
  return 2.0*_i;
}

/*---------------------------------------------------------*/

Quadratic_Linear_Solution::Quadratic_Linear_Solution(double a, double b, double c, double d, double e, double f, double g) : _a(a), _b(b), _c(c), _d(d), _e(e), _f(f), _g(g) {}
string Quadratic_Linear_Solution::getName() const
{
  return "Quadratic_linear_Solution";
}

double Quadratic_Linear_Solution::operator()(double x, double y, double z, double t) const
{
  return ( _a+_b*x+_c*y+_d*x*x+_e*y*y+_f*x*y )*_g*t;
}

double Quadratic_Linear_Solution::x(double x, double y, double z, double t) const
{
  return ( _b+2.0*_d*x+_f*y )*_g*t;
}

double Quadratic_Linear_Solution::y(double x, double y, double z, double t) const
{
  return ( _c+2.0*_e*y+_f*x )*_g*t;
}

double Quadratic_Linear_Solution::xx(double x, double y, double z, double t) const
{
  return 2.0*_d*_g*t;
}

double Quadratic_Linear_Solution::yy(double x, double y, double z, double t) const
{
  return 2.0*_e*_g*t;
}

double Quadratic_Linear_Solution::zz(double x, double y, double z, double t) const
{
  return 0.;
}

double Quadratic_Linear_Solution::t(double x, double y, double z, double t) const
{
  return ( _a+_b*x+_c*y+_d*x*x+_e*y*y+_f*x*y )*_g;
}

/*---------------------------------------------------------*/

CubicSolution::CubicSolution(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j, double k, double l, double m, double n, double o, double p, double r, double s, double t, double u) : _a(a), _b(b), _c(c), _d(d), _e(e), _f(f), _g(g), _h(h), _i(i), _j(j), _k(k), _l(l), _m(m), _n(n), _o(o), _p(p), _r(r), _s(s), _t(t), _u(u) {}

string CubicSolution::getName() const
{
  return "CubicSolution";
}

double CubicSolution::operator()(double x, double y, double z, double t) const
{
  return _a+_b*x+_c*y+_d*x*x+_e*y*y+_f*x*y+_g*x*x*x+_h*x*x*y+_i*x*y*y+_j*y*y*y+_k*z+_l*z*z+_m*x*z+_n*y*z+_o*z*z*z+_p*x*x*z+_r*y*y*z+_s*x*y*z+_t*x*z*z+_u*y*z*z;
}

double CubicSolution::x(double x, double y, double z, double t) const
{
  return _b+2.0*_d*x+_f*y+3.0*_g*x*x+2.0*_h*x*y+_i*y*y+_m*z+2.0*_p*x*z+_s*y*z+_t*z*z;
}

double CubicSolution::y(double x, double y, double z, double t) const
{
  return _c+2.0*_e*y+_f*x+_h*x*x+2.0*_i*x*y+3.0*_j*y*y+_n*z+2.0*_r*y*z+_s*x*z+_u*z*z;
}

double CubicSolution::z(double x, double y, double z, double t) const
{
  return _k+2.0*_l*z+_m*x+_n*y+3.0*_o*z*z+_p*x*x+_r*y*y+_s*x*y+2.0*_t*x*z+2.0*_u*y*z;
}

double CubicSolution::xx(double x, double y, double z, double t) const
{
  return 2.0*_d+6.0*_g*x+2.0*_h*y+2.0*_p*z;
}

double CubicSolution::yy(double x, double y, double z, double t) const
{
  return 2.0*_e+2.0*_i*x+6.0*_j*y+2.0*_r*z;
}

double CubicSolution::zz(double x, double y, double z, double t) const
{
  return 2.0*_l+6.0*_o*z+2.0*_t*x+2.0*_u*y;
}

/*---------------------------------------------------------*/

Quadratic_Quadratic_Solution::Quadratic_Quadratic_Solution(double a) : _a(a) {}
string Quadratic_Quadratic_Solution::getName() const
{
  return "Quadratic_Quadratic_Solution";
}

double Quadratic_Quadratic_Solution::operator()(double x, double y, double z, double t) const
{
  return ( 1-x*x )*( 1-y*y );
}

double Quadratic_Quadratic_Solution::x(double x, double y, double z, double t) const
{
  return -2.0*x*( 1-y*y );
}

double Quadratic_Quadratic_Solution::y(double x, double y, double z, double t) const
{
  return -2.0*y*( 1-x*x );
}

double Quadratic_Quadratic_Solution::xx(double x, double y, double z, double t) const
{
  return -2.0*( 1-y*y );
}

double Quadratic_Quadratic_Solution::yy(double x, double y, double z, double t) const
{
  return -2.0*( 1-x*x );
}

double Quadratic_Quadratic_Solution::zz(double x, double y, double z, double t) const
{
  return 0.;
}

/*---------------------------------------------------------*/
SinusCosinusSolution::SinusCosinusSolution(double a, double b, double c) : _a(a), _b(b), _c(c) {}
string SinusCosinusSolution::getName() const
{
  return "SinusCosinusSolution";
}

double SinusCosinusSolution::operator()(double x, double y, double z, double t) const
{
  return _a*sin(_b*M_PI*x)*cos(_c*M_PI*y);
}

double SinusCosinusSolution::x(double x, double y, double z, double t) const
{
  return _a*_b*M_PI*cos(_b*M_PI*x)*cos(_c*M_PI*y);
}

double SinusCosinusSolution::xx(double x, double y, double z, double t) const
{
  return -_a* _b* _b* M_PI* M_PI* sin(_b* M_PI* x)*cos(_c*M_PI*y);
}

double SinusCosinusSolution::y(double x, double y, double z, double t) const
{
  return -_a* _c* M_PI* sin(_b* M_PI* x)*sin(_c*M_PI*y);
}

double SinusCosinusSolution::yy(double x, double y, double z, double t) const
{
  return -_a* _c* _c* M_PI* M_PI* sin(_b* M_PI* x)*cos(_c*M_PI*y);
}

/*---------------------------------------------------------*/
CosinusSinusSolution::CosinusSinusSolution(double a, double b, double c) : _a(a), _b(b), _c(c) {}
string CosinusSinusSolution::getName() const
{
  return "CosinusSinusSolution";
}

double CosinusSinusSolution::operator()(double x, double y, double z, double t) const
{
  return _a*cos(_b*M_PI*x)*sin(_c*M_PI*y);
}

double CosinusSinusSolution::x(double x, double y, double z, double t) const
{
  return -_a* _b* M_PI* sin(_b* M_PI* x)*sin(_c*M_PI*y);
}

double CosinusSinusSolution::y(double x, double y, double z, double t) const
{
  return _a*_c*M_PI*cos(_b*M_PI*x)*cos(_c*M_PI*y);
}

double CosinusSinusSolution::xx(double x, double y, double z, double t) const
{
  return -_a* _b* _b* M_PI* M_PI* cos(_b* M_PI* x)*sin(_c*M_PI*y);
}

double CosinusSinusSolution::yy(double x, double y, double z, double t) const
{
  return -_a* _c* _c* M_PI* M_PI* cos(_b* M_PI* x)*sin(_c*M_PI*y);
}

/*---------------------------------------------------------*/
CosinusPlusCosinusSolution::CosinusPlusCosinusSolution(double a, double b, double c) : _a(a), _b(b), _c(c) {}
string CosinusPlusCosinusSolution::getName() const
{
  return "CosinusPlusCosinusSolution";
}

double CosinusPlusCosinusSolution::operator()(double x, double y, double z, double t) const
{
  return _a*( cos(_b*M_PI*x)+cos(_c*M_PI*y) );
}

double CosinusPlusCosinusSolution::x(double x, double y, double z, double t) const
{
  return -_a* M_PI* _b* sin(_b* M_PI* x);
}

double CosinusPlusCosinusSolution::y(double x, double y, double z, double t) const
{
  return -_a* M_PI* _c* sin(_c* M_PI* y);
}

double CosinusPlusCosinusSolution::xx(double x, double y, double z, double t) const
{
  return -_a* M_PI* _b* M_PI* _b* cos(_b* M_PI* x);
}

double CosinusPlusCosinusSolution::yy(double x, double y, double z, double t) const
{
  return -_a* M_PI* _c* M_PI* _c* cos(_c* M_PI* y);
}

/*---------------------------------------------------------*/
CosinusPlusCosinusSolutionDx::CosinusPlusCosinusSolutionDx(double a, double b, double c) : _a(a), _b(b), _c(c) {}
string CosinusPlusCosinusSolutionDx::getName() const
{
  return "CosinusPlusCosinusSolutionDx";
}

double CosinusPlusCosinusSolutionDx::operator()(double x, double y, double z, double t) const
{
  return _a*_b*M_PI*( -sin(_b*M_PI*x) );
}

double CosinusPlusCosinusSolutionDx::x(double x, double y, double z, double t) const
{
  return -_a* M_PI*_b* M_PI* _b* cos(_b* M_PI* x);
}

double CosinusPlusCosinusSolutionDx::y(double x, double y, double z, double t) const
{
  return 0.0;
}

double CosinusPlusCosinusSolutionDx::xx(double x, double y, double z, double t) const
{
  return _a* M_PI* _b* M_PI* _b*_b* M_PI* sin(_b* M_PI* x);
}

double CosinusPlusCosinusSolutionDx::yy(double x, double y, double z, double t) const
{
  return 0.0;
}

/*---------------------------------------------------------*/
CosinusPlusCosinusSolutionDy::CosinusPlusCosinusSolutionDy(double a, double b, double c) : _a(a), _b(b), _c(c) {}
string CosinusPlusCosinusSolutionDy::getName() const
{
  return "CosinusPlusCosinusSolutionDy";
}

double CosinusPlusCosinusSolutionDy::operator()(double x, double y, double z, double t) const
{
  return _a*_c*M_PI*( -sin(_c*M_PI*y) );
}

double CosinusPlusCosinusSolutionDy::x(double x, double y, double z, double t) const
{
  return 0.0;
}

double CosinusPlusCosinusSolutionDy::y(double x, double y, double z, double t) const
{
  return -_a* M_PI* _c*_c* M_PI* cos(_c* M_PI* y);
}

double CosinusPlusCosinusSolutionDy::xx(double x, double y, double z, double t) const
{
  return 0.0;
}

double CosinusPlusCosinusSolutionDy::yy(double x, double y, double z, double t) const
{
  return _a* M_PI* _c* M_PI* _c*_c* M_PI* sin(_c* M_PI* y);
}

/*---------------------------------------------------------*/
void ExponentialSolution::_init(double x, double y, double z) const
{
  double xm = x-_x0;
  double ym = y-_y0;
  _r = 0.5*( xm*xm+ym*ym )/_eps;
  _rx = xm;
  _ry = ym;
}

ExponentialSolution::ExponentialSolution(double x0, double y0, double z0, double eps) : Fada::DataFunction(), _x0(x0), _y0(y0), _z0(z0), _eps(eps) {}
string ExponentialSolution::getName() const
{
  return "ExponentialSolution";
}

double ExponentialSolution::operator()(double x, double y, double z, double t) const
{
  _init(x, y, z);
  return exp(-_r);
}

double ExponentialSolution::x(double x, double y, double z, double t) const
{
  _init(x, y, z);
  return -_rx* exp(-_r)/_eps;
}

double ExponentialSolution::y(double x, double y, double z, double t) const
{
  _init(x, y, z);
  return -_ry* exp(-_r)/_eps;
}

double ExponentialSolution::xx(double x, double y, double z, double t) const
{
  _init(x, y, z);
  return ( _rx*_rx/_eps-1.0 )*exp(-_r)/_eps;
}

double ExponentialSolution::yy(double x, double y, double z, double t) const
{
  _init(x, y, z);
  return ( _ry*_ry/_eps-1.0 )*exp(-_r)/_eps;
}

/*---------------------------------------------------------*/

void ExponentialSolution3D::_init(double x, double y, double z) const
{
  double xm = x-_x0;
  double ym = y-_y0;
  double zm = z-_z0;
  _r = 0.5*( xm*xm+ym*ym+zm*zm )/_eps;
  _rx = xm;
  _ry = ym;
  _rz = zm;
}

ExponentialSolution3D::ExponentialSolution3D(double x0, double y0, double z0, double eps) : Fada::DataFunction(), _x0(x0), _y0(y0), _z0(z0), _eps(eps) {}
string ExponentialSolution3D::getName() const
{
  return "ExponentialSolution3D";
}

double ExponentialSolution3D::operator()(double x, double y, double z, double t) const
{
  _init(x, y, z);
  return exp(-_r);
}

double ExponentialSolution3D::x(double x, double y, double z, double t) const
{
  _init(x, y, z);
  return -_rx* exp(-_r)/_eps;
}

double ExponentialSolution3D::y(double x, double y, double z, double t) const
{
  _init(x, y, z);
  return -_ry* exp(-_r)/_eps;
}

double ExponentialSolution3D::z(double x, double y, double z, double t) const
{
  _init(x, y, z);
  return -_rz* exp(-_r)/_eps;
}

double ExponentialSolution3D::xx(double x, double y, double z, double t) const
{
  _init(x, y, z);
  return ( _rx*_rx/_eps-1.0 )*exp(-_r)/_eps;
}

double ExponentialSolution3D::yy(double x, double y, double z, double t) const
{
  _init(x, y, z);
  return ( _ry*_ry/_eps-1.0 )*exp(-_r)/_eps;
}

double ExponentialSolution3D::zz(double x, double y, double z, double t) const
{
  _init(x, y, z);
  return ( _rz*_rz/_eps-1.0 )*exp(-_r)/_eps;
}

/*---------------------------------------------------------*/

double LDomainSolution::_theta(double x, double y, double z) const
{
  double angle = atan2(y, x);
  if (angle < 0) { angle += 2 * M_PI; }
  return angle;
  if(y >= 0.0)
  {
    return atan2(y, x);
  }
  else
  {
    return 2.0*M_PI-atan2(y, x);
  }
}

string LDomainSolution::getName() const
{
  return "LDomainSolution";
}

double LDomainSolution::operator()(double x, double y, double z, double t) const
{
  double r2 = x*x+y*y;
  return pow(r2, 1.0/3.0)*sin(2.0*_theta(x, y, z)/3.0);
}

double LDomainSolution::x(double x, double y, double z, double t) const
{
  double r2 = x*x+y*y;
  return ( 2.0/3.0 )*pow(r2, -2.0/3.0)*( sin(2.0*_theta(x, y, z)/3.0)*x-cos(2.0*_theta(x, y, z)/3.0)*y );
}

double LDomainSolution::y(double x, double y, double z, double t) const
{
  double r2 = x*x+y*y;
  return ( 2.0/3.0 )*pow(r2, -2.0/3.0)*( sin(2.0*_theta(x, y, z)/3.0)*y+cos(2.0*_theta(x, y, z)/3.0)*x );
}

/*---------------------------------------------------------*/

string SlitDomainSolution::getName() const
{
  return "SlitDomainSolution";
}

double SlitDomainSolution::operator()(double x, double y, double z, double t) const
{
  double r2 = x*x+y*y;
  return pow(r2, 1.0/4.0)*sin(_theta(x, y, z)/2.0);
}

double SlitDomainSolution::x(double x, double y, double z, double t) const
{
  double r2 = x*x+y*y;
  return ( 1.0/2.0 )*pow(r2, -3.0/4.0)*( sin(_theta(x, y, z)/2.0)*x-cos(_theta(x, y, z)/2.0)*y );
}

double SlitDomainSolution::y(double x, double y, double z, double t) const
{
  double r2 = x*x+y*y;
  return ( 1.0/2.0 )*pow(r2, -3.0/4.0)*( sin(_theta(x, y, z)/2.0)*y+cos(_theta(x, y, z)/2.0)*x );
}
