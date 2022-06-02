#ifndef __Ramp_h
#define __Ramp_h

#include  "Fada/application.h"
#include  "Fada/datafunction.h"
#include  "Fada/dirichletinterface.h"

/*--------------------------------------------------------------------------*/

// namespace FadaConvectionDiffusionReaction
// {
  class RampDirichlet : public Fada::DirichletInterface
  {
public:
    ~RampDirichlet() {}
    RampDirichlet() {}
    RampDirichlet(const RampDirichlet& rotstatdirichlet)
    {
      assert(0);
    }

    RampDirichlet& operator=(const RampDirichlet& rotstatdirichlet)
    {
      assert(0);
      return *this;
    }

    std::string getName() const;
    void getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t = 0.0) const;
  };

/*--------------------------------------------------------------------------*/
  class RampExactSolution : public Fada::DataFunction
  {
  private:
    Alat::Node _beta;    
public:
    ~RampExactSolution() {}
    RampExactSolution(const Alat::Node& beta) : _beta(beta) {}
    RampExactSolution( const RampExactSolution& rampexactsolution) {assert(0);}
    RampExactSolution& operator=( const RampExactSolution& rampexactsolution) {assert(0);}
    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
    double x(double x, double y, double z, double t = 0.) const;
    double y(double x, double y, double z, double t = 0.) const;
  };

/*--------------------------------------------------------------------------*/

  class Ramp : public Fada::Application
  {
private:
protected:
public:
    ~Ramp();
    Ramp();
    Ramp( const Ramp& rotstat);
    Ramp& operator=( const Ramp& rotstat);
    std::string getName() const;
    Ramp* clone() const;

    void defineProblemData(Fada::ProblemData* problemdata) const;
    Fada::DataInterface* defineData(std::string name) const;
    bool hasExactSolutions() const;
  };
// }

/*--------------------------------------------------------------------------*/

#endif
