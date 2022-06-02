#ifndef __Fada_DataFunction_h
#define __Fada_DataFunction_h

#include  <string>

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class DataFunction
  {
private:
    double _eps;

protected:
public:
    ~DataFunction();
    DataFunction();
    DataFunction( const DataFunction& datafunction);
    DataFunction& operator=( const DataFunction& datafunction);
    std::string getName() const;

    virtual double operator() (double x, double y, double z, double t = 0.) const;

    virtual double d (int i, double x, double y, double z, double t = 0.) const;
    virtual double dd (int i, int j, double x, double y, double z, double t = 0.) const;
    virtual double x (double x, double y, double z, double t = 0.) const;
    virtual double y (double x, double y, double z, double t = 0.) const;
    virtual double z (double x, double y, double z, double t = 0.) const;
    virtual double xx(double x, double y, double z, double t = 0.) const;
    virtual double yy(double x, double y, double z, double t = 0.) const;
    virtual double zz(double x, double y, double z, double t = 0.) const;
    virtual double xy(double x, double y, double z, double t = 0.) const;
    virtual double yz(double x, double y, double z, double t = 0.) const;
    virtual double xz(double x, double y, double z, double t = 0.) const;
    virtual double t (double x, double y, double z, double t = 0.) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
