#ifndef __Fada_SimpleExactSolutions_h
#define __Fada_SimpleExactSolutions_h

#include  "Fada/datafunction.h"

/*---------------------------------------------------------*/

namespace Fada
{
  class ConstantSolution : public Fada::DataFunction
  {
protected:
    double _d;
public:
    ConstantSolution(double d = 1.2);
    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
    double x(double x, double y, double z, double t = 0.) const;
    double y(double x, double y, double z, double t = 0.) const;
    double z(double x, double y, double z, double t = 0.) const;
    double t(double x, double y, double z, double t = 0.) const;
    double xx(double x, double y, double z, double t = 0.) const;
    double yy(double x, double y, double z, double t = 0.) const;
    double zz(double x, double y, double z, double t = 0.) const;
  };
  class LinearSolution : public Fada::DataFunction
  {
private:
    double _a, _b, _c, _d;
public:
    LinearSolution(double a = 1., double b = 1.0, double c = 0.0, double d = 1.7);
    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
    double x(double x, double y, double z, double t = 0.) const;
    double y(double x, double y, double z, double t = 0.) const;
    double z(double x, double y, double z, double t = 0.) const;
    double xx(double x, double y, double z, double t = 0.) const;
    double yy(double x, double y, double z, double t = 0.) const;
    double zz(double x, double y, double z, double t = 0.) const;
  };

  class QuadraticSolution : public Fada::DataFunction
  {
private:
    double _a, _b, _c, _d, _e, _f, _g, _h, _i;
public:
    QuadraticSolution(double a = 1.2, double b = 0.0, double c = 1.1, double d = 1.1, double e = 1.1, double f = 1.1, double g = 1.1, double h = 1.1, double i = 1.1);
    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
    double x(double x, double y, double z, double t = 0.) const;
    double y(double x, double y, double z, double t = 0.) const;
    double z(double x, double y, double z, double t = 0.) const;
    double xx(double x, double y, double z, double t = 0.) const;
    double yy(double x, double y, double z, double t = 0.) const;
    double zz(double vx, double vy, double vz, double t = 0.) const;
  };
  class Quadratic_Linear_Solution : public Fada::DataFunction
  {
private:
    double _a, _b, _c, _d, _e, _f, _g;
public:
    Quadratic_Linear_Solution(double a = 1.2, double b = 0.0, double c = 1.1, double d = 1.1, double e = 1.1, double f = 1.1, double g = 1.1);
    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.0) const;
    double x(double x, double y, double z, double t) const;
    double y(double x, double y, double z, double t) const;
    double xx(double x, double y, double z, double t) const;
    double yy(double x, double y, double z, double t) const;
    double zz(double x, double y, double z, double t = 0.) const;
    double t(double x, double y, double z, double t) const;
  };
  class CubicSolution : public Fada::DataFunction
  {
private:
    double _a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l, _m, _n, _o, _p, _r, _s, _t, _u;
public:
    CubicSolution(double a = 1.2, double b = 0.0, double c = 1.1, double d = 1.1, double e = 1.1, double f = 1.1, double g = 1.7, double h = 1.8, double i = 1.9, double j = 2.0, double k = 0.0, double l = 0.0, double m = 1.1, double n = 1.1, double o = 1.1, double p = 1.1, double r = 1.7, double s = 1.8, double t = 1.9, double u = 2.0);
    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
    double x(double x, double y, double z, double t = 0.) const;
    double y(double x, double y, double z, double t = 0.) const;
    double z(double x, double y, double z, double t = 0.) const;
    double xx(double x, double y, double z, double t = 0.) const;
    double yy(double x, double y, double z, double t = 0.) const;
    double zz(double x, double y, double z, double t = 0.) const;
  };
  class Quadratic_Quadratic_Solution : public Fada::DataFunction
  {
private:
    double _a;
public:
    Quadratic_Quadratic_Solution(double a = 1.0);
    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
    double x(double x, double y, double z, double t = 0.) const;
    double y(double x, double y, double z, double t = 0.) const;
    double xx(double x, double y, double z, double t = 0.) const;
    double yy(double x, double y, double z, double t = 0.) const;
    double zz(double x, double y, double z, double t = 0.) const;
  };
  class SinusCosinusSolution : public Fada::DataFunction
  {
private:
    double _a, _b, _c;
public:
    SinusCosinusSolution(double a = 2.0, double b = 1.0, double c = 1.);
    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
    double x(double x, double y, double z, double t = 0.) const;
    double y(double x, double y, double z, double t = 0.) const;
    double xx(double x, double y, double z, double t = 0.) const;
    double yy(double x, double y, double z, double t = 0.) const;
  };
  class CosinusSinusSolution : public Fada::DataFunction
  {
private:
    double _a, _b, _c;
public:
    CosinusSinusSolution(double a = 2.0, double b = 1.0, double c = 1.);
    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
    double x(double x, double y, double z, double t = 0.) const;
    double y(double x, double y, double z, double t = 0.) const;
    double xx(double x, double y, double z, double t = 0.) const;
    double yy(double x, double y, double z, double t = 0.) const;
  };
  class CosinusPlusCosinusSolution : public Fada::DataFunction
  {
private:
    double _a, _b, _c;
public:
    CosinusPlusCosinusSolution(double a = 2.0, double b = 1.0, double c = 1.);
    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
    double x(double x, double y, double z, double t = 0.) const;
    double y(double x, double y, double z, double t = 0.) const;
    double xx(double x, double y, double z, double t = 0.) const;
    double yy(double x, double y, double z, double t = 0.) const;
  };
  class CosinusPlusCosinusSolutionDx : public Fada::DataFunction
  {
private:
    double _a, _b, _c;
public:
    CosinusPlusCosinusSolutionDx(double a = 2.0, double b = 1.0, double c = 1.);
    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
    double x(double x, double y, double z, double t = 0.) const;
    double y(double x, double y, double z, double t = 0.) const;
    double xx(double x, double y, double z, double t = 0.) const;
    double yy(double x, double y, double z, double t = 0.) const;
  };
  class CosinusPlusCosinusSolutionDy : public Fada::DataFunction
  {
private:
    double _a, _b, _c;
public:
    CosinusPlusCosinusSolutionDy(double a = 2.0, double b = 1.0, double c = 1.);
    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
    double x(double x, double y, double z, double t = 0.) const;
    double y(double x, double y, double z, double t = 0.) const;
    double xx(double x, double y, double z, double t = 0.) const;
    double yy(double x, double y, double z, double t = 0.) const;
  };
  class ExponentialSolution : public Fada::DataFunction
  {
private:
    double _x0, _y0, _z0, _eps;
    mutable double _r, _rx, _ry;
    void _init(double x, double y, double z) const;

public:
    ExponentialSolution(double x0 = 0.5, double y0 = 0.5, double z0 = 0.5, double eps = 0.01);
    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
    double x(double x, double y, double z, double t = 0.) const;
    double y(double x, double y, double z, double t = 0.) const;
    double xx(double x, double y, double z, double t = 0.) const;
    double yy(double x, double y, double z, double t = 0.) const;
  };
  class ExponentialSolution3D : public Fada::DataFunction
  {
private:
    double _x0, _y0, _z0, _eps;
    mutable double _r, _rx, _ry, _rz;
    void _init(double x, double y, double z) const;

public:
    ExponentialSolution3D(double x0 = 0.5, double y0 = 0.5, double z0 = 0.5, double eps = 0.01);
    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
    double x(double x, double y, double z, double t = 0.) const;
    double y(double x, double y, double z, double t = 0.) const;
    double z(double x, double y, double z, double t = 0.) const;
    double xx(double x, double y, double z, double t = 0.) const;
    double yy(double x, double y, double z, double t = 0.) const;
    double zz(double x, double y, double z, double t = 0.) const;
  };

  class LDomainSolution : public Fada::DataFunction
  {
protected:
    double _theta(double x, double y, double z) const;

public:
    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
    double x(double x, double y, double z, double t = 0.) const;
    double y(double x, double y, double z, double t = 0.) const;
  };

  class SlitDomainSolution : public LDomainSolution
  {
public:
    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
    double x(double x, double y, double z, double t = 0.) const;
    double y(double x, double y, double z, double t = 0.) const;
  };
}

/*---------------------------------------------------------*/

#endif
