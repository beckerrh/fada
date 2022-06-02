#ifndef __Fada_GhostLinearSolverAgent_h
#define __Fada_GhostLinearSolverAgent_h

#include  "Alat/ghostlinearsolver.h"
#include  "Alat/linearsolverinterface.h"
#include  "Alat/map.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class GhostLinearSolverAgent : public Alat::Map<Alat::GhostLinearSolver, Alat::LinearSolverInterface*>
  {
public:
    typedef Alat::Map<Alat::GhostLinearSolver, Alat::LinearSolverInterface*>::const_iterator const_iterator;
    typedef Alat::Map<Alat::GhostLinearSolver, Alat::LinearSolverInterface*>::iterator iterator;

public:
    ~GhostLinearSolverAgent();
    GhostLinearSolverAgent();
    GhostLinearSolverAgent( const GhostLinearSolverAgent& ghostvectoragent);
    GhostLinearSolverAgent& operator=( const GhostLinearSolverAgent& ghostvectoragent);
    std::string getName() const;
    std::ostream& printLoopInformation(std::ostream& os) const;
    bool exists(const Alat::GhostLinearSolver& mg) const;
    void enrol(const Alat::GhostLinearSolver& mg );
    Alat::LinearSolverInterface* operator()(const Alat::GhostLinearSolver& g) const;
  };
  std::ostream& operator<<(std::ostream& os, const GhostLinearSolverAgent& gva);
}

/*--------------------------------------------------------------------------*/

#endif
