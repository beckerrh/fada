#ifndef __RotStat_h
#define __RotStat_h

#include  "Fada/application.h"
#include  "Fada/dirichletinterface.h"

/*--------------------------------------------------------------------------*/

// namespace FadaConvectionDiffusionReaction
// {
  class RotStatDirichlet2d : public Fada::DirichletInterface
  {
public:
    ~RotStatDirichlet2d() {}
    RotStatDirichlet2d() {}
    RotStatDirichlet2d(const RotStatDirichlet2d& rotstatdirichlet)
    {
      assert(0);
    }

    RotStatDirichlet2d& operator=(const RotStatDirichlet2d& rotstatdirichlet)
    {
      assert(0);
      return *this;
    }

    std::string getName() const;
    void getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t = 0.0) const;
  };

/*--------------------------------------------------------------------------*/

  class RotStatDirichlet3d : public Fada::DirichletInterface
  {
public:
    ~RotStatDirichlet3d() {}
    RotStatDirichlet3d() {}
    RotStatDirichlet3d(const RotStatDirichlet3d& rotstatdirichlet)
    {
      assert(0);
    }

    RotStatDirichlet3d& operator=(const RotStatDirichlet3d& rotstatdirichlet)
    {
      assert(0);
      return *this;
    }

    std::string getName() const;
    void getValue(Alat::DoubleVector& g, int color, const Alat::Node& normal, double x, double y, double z, double t = 0.0) const;
  };

/*--------------------------------------------------------------------------*/

  class RotStat : public Fada::Application
  {
private:
protected:
public:
    ~RotStat();
    RotStat();
    RotStat( const RotStat& rotstat);
    RotStat& operator=( const RotStat& rotstat);
    std::string getName() const;
    RotStat* clone() const;

    void defineProblemData(Fada::ProblemData* problemdata) const;
    Fada::DataInterface* defineData(std::string name) const;
  };
// }

/*--------------------------------------------------------------------------*/

#endif
