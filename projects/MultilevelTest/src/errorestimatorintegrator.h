#ifndef ___ErrorEstimatorIntegrator_hpp
#define ___ErrorEstimatorIntegrator_hpp

#include  "Fada/integrator.h"
#include  "Fada/righthandsideinterface.h"

/*--------------------------------------------------------------------------*/
  class ErrorEstimatorIntegrator : public Fada::Integrator
  {
  protected:
    mutable std::string _varnamein, _varnameout;
    const Fada::RightHandSideInterface* _rhs;
    // mutable Alat::armavec _frhs;
    mutable Alat::DoubleVector _frhs;

  public:
    ~ErrorEstimatorIntegrator();
    ErrorEstimatorIntegrator();
    ErrorEstimatorIntegrator( const ErrorEstimatorIntegrator& errorestimatorintegrator);
    ErrorEstimatorIntegrator& operator=( const ErrorEstimatorIntegrator& errorestimatorintegrator);
    std::string getName() const;
    std::string getClassName() const;

    void initData();
    void setDomainsOfTerms();
    Fada::VariableInterface* constructPostProcessVariable(std::string name, const Fada::VariableManager& variablemanager) const;
    void postProcessCell(AlatEnums::residualstatus& status, double weight) const;
    void postProcessCouplingSide(AlatEnums::residualstatus& status, int iKL, int iKR, double weight, double measureSmallSide) const;
  };

/*--------------------------------------------------------------------------*/
#endif
