#ifndef __Fada_FunctionalInterface_h
#define __Fada_FunctionalInterface_h

#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class ParameterFile;
}
namespace Fada
{
  class IntegratorInterface;
  template <class T>
  class ParameterMap;
  class ProblemData;

  class FunctionalInterface : public Alat::InterfaceBase
  {
public:
    ~FunctionalInterface();
    FunctionalInterface();
    FunctionalInterface( const FunctionalInterface& functionalinterface);
    FunctionalInterface& operator=( const FunctionalInterface& functionalinterface);
    std::string getName() const;

    virtual void basicInit(const Alat::ParameterFile* parameterfile, const Fada::ProblemData* model_problemdata, const Fada::ParameterMap<double>& phys_params, const Fada::ParameterMap<double>& num_params);
    virtual Fada::IntegratorInterface* getIntegrator() const;
    virtual Fada::IntegratorInterface*& getIntegratorPointer() const  = 0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
