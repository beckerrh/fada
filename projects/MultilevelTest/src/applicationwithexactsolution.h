#ifndef __ApplicationWithExactSolution_h
#define __ApplicationWithExactSolution_h

#include  "Fada/applicationwithexactsolution.h"

/*--------------------------------------------------------------------------*/

// namespace FadaConvectionDiffusionReaction
// {
  class ApplicationWithExactSolution : public Fada::ApplicationWithExactSolution
  {
protected:
  void constructExactSolution(Fada::DataFunctionVector& uex) const;
  
public:
    ~ApplicationWithExactSolution();
    ApplicationWithExactSolution();
    ApplicationWithExactSolution( const ApplicationWithExactSolution& applicationwithexactsolution);
    ApplicationWithExactSolution& operator=( const ApplicationWithExactSolution& applicationwithexactsolution);
    std::string getName() const;
    Fada::ApplicationInterface* clone() const;

    void defineProblemData(Fada::ProblemData* problemdata) const;
    Fada::DataInterface* defineData(std::string name) const;
  };
// }

/*--------------------------------------------------------------------------*/

#endif
