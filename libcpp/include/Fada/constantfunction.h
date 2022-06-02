#ifndef __Fada_ConstantFunction_h
#define __Fada_ConstantFunction_h

#include  "datafunction.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class ConstantFunction : public DataFunction
  {
private:
    double _d;

public:
    ~ConstantFunction();
    ConstantFunction(double d);
    ConstantFunction( const ConstantFunction& constantfunction);
    ConstantFunction& operator=( const ConstantFunction& constantfunction);
    std::string getName() const;
    ConstantFunction* clone() const;

    double operator() (double x, double y, double z, double t = 0.) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
