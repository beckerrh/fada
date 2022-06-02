#ifndef __RotatingCone_h
#define __RotatingCone_h

#include  "Fada/application.h"
#include  "Fada/datafunction.h"
#include  "Fada/dirichletinterface.h"
#include  "Fada/initialconditioninterface.h"

/*--------------------------------------------------------------------------*/

// namespace FadaConvectionDiffusionReaction
// {
  class RotatingConeDirichlet2d : public Fada::DirichletInterface
  {
public:
    ~RotatingConeDirichlet2d() {}
    RotatingConeDirichlet2d() {}
    RotatingConeDirichlet2d(const RotatingConeDirichlet2d& rotstatdirichlet)
    {
      assert(0);
    }

    RotatingConeDirichlet2d& operator=(const RotatingConeDirichlet2d& rotstatdirichlet)
    {
      assert(0);
      return *this;
    }

    std::string getName() const;
    void getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t = 0.0) const;
  };

/*--------------------------------------------------------------------------*/

  class RotatingConeDirichlet3d : public Fada::DirichletInterface
  {
public:
    ~RotatingConeDirichlet3d() {}
    RotatingConeDirichlet3d() {}
    RotatingConeDirichlet3d(const RotatingConeDirichlet3d& rotstatdirichlet)
    {
      assert(0);
    }

    RotatingConeDirichlet3d& operator=(const RotatingConeDirichlet3d& rotstatdirichlet)
    {
      assert(0);
      return *this;
    }

    std::string getName() const;
    void getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t = 0.0) const;
  };

/*--------------------------------------------------------------------------*/

  class RotatingConeInitial : public Fada::InitialConditionInterface
  {
public:
    ~RotatingConeInitial() {}
    RotatingConeInitial() {}
    RotatingConeInitial(const RotatingConeInitial& initial)
    {
      assert(0);
    }

    RotatingConeInitial& operator=(const RotatingConeInitial& initial)
    {
      assert(0);
      return *this;
    }

    std::string getName() const
    {
      return "RotatingConeInitial";
    }

    void getValue(Alat::DoubleVector& g, double x, double y, double z) const;
  };

  class RotatingConeVelocity1 : public Fada::DataFunction
  {
public:
    ~RotatingConeVelocity1() {}
    RotatingConeVelocity1() {}
    RotatingConeVelocity1(const RotatingConeVelocity1& rotatingconevelocity1)
    {
      assert(0);
    }

    RotatingConeVelocity1& operator=(const RotatingConeVelocity1& rotatingconevelocity1)
    {
      assert(0);
      return *this;
    }

    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
  };

/*--------------------------------------------------------------------------*/

  class RotatingConeVelocity2 : public Fada::DataFunction
  {
public:
    ~RotatingConeVelocity2() {}
    RotatingConeVelocity2() {}
    RotatingConeVelocity2(const RotatingConeVelocity2& rotatingconevelocity2)
    {
      assert(0);
    }

    RotatingConeVelocity2& operator=(const RotatingConeVelocity2& rotatingconevelocity2)
    {
      assert(0);
      return *this;
    }

    std::string getName() const;
    double operator()(double x, double y, double z, double t = 0.) const;
  };

/*--------------------------------------------------------------------------*/

  class RotatingCone : public Fada::Application
  {
private:
protected:
public:
    ~RotatingCone();
    RotatingCone();
    RotatingCone( const RotatingCone& rotstat);
    RotatingCone& operator=( const RotatingCone& rotstat);
    std::string getName() const;
    RotatingCone* clone() const;

    void defineProblemData(Fada::ProblemData* problemdata) const;
    Fada::DataInterface* defineData(std::string name) const;
  };
// }

/*--------------------------------------------------------------------------*/

#endif
