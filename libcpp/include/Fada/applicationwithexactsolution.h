#ifndef __Fada_ApplicationWithExactSolution_h
#define __Fada_ApplicationWithExactSolution_h

#include  "application.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class DataFunctionVector;

  class ApplicationWithExactSolution : public virtual Application
  {
protected:
    const std::string& getExactSolutionName() const;
    std::string _exactsolutionname;

public:
    ~ApplicationWithExactSolution();
    ApplicationWithExactSolution();
    ApplicationWithExactSolution( const ApplicationWithExactSolution& applicationwithexactsolution);
    ApplicationWithExactSolution& operator=( const ApplicationWithExactSolution& applicationwithexactsolution);
    std::string getName() const;

    void basicInit(const Alat::ParameterFile* parameterfile, const Fada::ModelInterface* model, const Alat::StringSet& possibleboundaryconditions);
    bool hasExactSolutions() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
