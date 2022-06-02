#ifndef __PwLinearSolution_h
#define __PwLinearSolution_h

#include  "Fada/datafunction.h"

/*--------------------------------------------------------------------------*/

// namespace FadaConvectionDiffusionReaction
// {
  class PwLinearSolution : public Fada::DataFunction
  {
public:
    ~PwLinearSolution();
    PwLinearSolution();
    PwLinearSolution( const PwLinearSolution& pwlinearsolution);
    PwLinearSolution& operator=( const PwLinearSolution& pwlinearsolution);
    std::string getName() const;
    PwLinearSolution* clone() const;

    double operator()(double x, double y, double z, double t = 0.) const;
    double x(double x, double y, double z, double t = 0.) const;
    double y(double x, double y, double z, double t = 0.) const;
    double xx(double x, double y, double z, double t = 0.) const;
    double yy(double x, double y, double z, double t = 0.) const;
  };
// }

/*--------------------------------------------------------------------------*/

#endif
