#ifndef __Fada_DirichletWithExactSolution_h
#define __Fada_DirichletWithExactSolution_h

#include  "dirichletinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{ 
  class DataFunctionVector;
  
  class DirichletWithExactSolution : public DirichletInterface
  {
protected:
    std::string _varname;
    const Fada::DataFunctionVector& _solution;

public:
    ~DirichletWithExactSolution();
    DirichletWithExactSolution(std::string varname, const Fada::DataFunctionVector& solution);
    DirichletWithExactSolution( const DirichletWithExactSolution& dirichletwithexactsolution);
    DirichletWithExactSolution& operator=( const DirichletWithExactSolution& dirichletwithexactsolution);
    std::string getName() const;
    void getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t = 0.0) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
