#ifndef __Integrators_EstimatorIntegrator2_h
#define __Integrators_EstimatorIntegrator2_h

#include  "Fada/integrator.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class DataFunctionVector;
}
namespace Fada
{
  template <class T>
  class Vector;
}

  class EstimatorIntegrator2 : public Fada::Integrator
  {
private:
    mutable Alat::Map<std::string, std::string> _unknown_of_postproc;
    Alat::Map<std::string, const Fada::DataFunctionVector*> _solutions;
    int _ncomp;
    mutable Alat::Map<std::string, Alat::DoubleVector> _error;
    mutable Alat::Map<std::string, Alat::Vector<Alat::Node> >_derror;

protected:
  void postProcessCell(AlatEnums::residualstatus& status, double weight) const;

public:
    ~EstimatorIntegrator2();
    EstimatorIntegrator2();
    EstimatorIntegrator2( const EstimatorIntegrator2& errorintegrator);
    EstimatorIntegrator2& operator=( const EstimatorIntegrator2& errorintegrator);
    EstimatorIntegrator2* clone() const;
    std::string getName() const;

    void initData();
    int getOrderForIntegrationFormula(int orderin, int orderout) const;
    void setDomainsOfTerms();
    Fada::VariableInterface* constructPostProcessVariable(std::string name, const Fada::VariableManager& variablemanager) const;
  };

/*--------------------------------------------------------------------------*/

#endif
