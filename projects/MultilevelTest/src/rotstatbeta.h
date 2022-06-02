#ifndef __RotStatBeta_h
#define __RotStatBeta_h

#include  "Fada/application.h"
#include  "Fada/dirichletinterface.h"

/*--------------------------------------------------------------------------*/

// namespace FadaConvectionDiffusionReaction
// {
  class RotStatBetax : public Fada::DataFunction
  {
public:
    ~RotStatBetax() {}
    RotStatBetax() {}
    RotStatBetax(const RotStatBetax& rotstatdirichlet)
    {
      assert(0);
    }

    RotStatBetax& operator=(const RotStatBetax& rotstatdirichlet)
    {
      assert(0);
      return *this;
    }

    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
  };

/*--------------------------------------------------------------------------*/

  class RotStatBetay : public Fada::DataFunction
  {
public:
    ~RotStatBetay() {}
    RotStatBetay() {}
    RotStatBetay(const RotStatBetay& rotstatdirichlet)
    {
      assert(0);
    }

    RotStatBetay& operator=(const RotStatBetay& rotstatdirichlet)
    {
      assert(0);
      return *this;
    }

    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
  };

/*--------------------------------------------------------------------------*/

  class RotStatBetaz : public Fada::DataFunction
  {
public:
    ~RotStatBetaz() {}
    RotStatBetaz() {}
    RotStatBetaz(const RotStatBetaz& rotstatdirichlet)
    {
      assert(0);
    }

    RotStatBetaz& operator=(const RotStatBetaz& rotstatdirichlet)
    {
      assert(0);
      return *this;
    }

    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
  };
// }

/*--------------------------------------------------------------------------*/

#endif
