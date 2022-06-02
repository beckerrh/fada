#ifndef __Alat_GhostLinearSolver_h
#define __Alat_GhostLinearSolver_h

#include  "ghost.h"
#include  "ghostmatrix.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class GhostLinearSolver : public Ghost
  {
private:
    const Alat::GhostMatrix& _matrix;

public:
    ~GhostLinearSolver();
    GhostLinearSolver( const Alat::GhostLinearSolver& ghostlinearsolver);
    GhostLinearSolver(const std::string name, const Alat::GhostMatrix& matrix, int level = 0);
    GhostLinearSolver& operator=( const Alat::GhostLinearSolver& ghostlinearsolver);

    const Alat::GhostMatrix& getMatrix() const;
    std::ostream& write(std::ostream& os) const;
  };
  std::ostream& operator<<(std::ostream& os, const Alat::GhostLinearSolver& g);
}

/*--------------------------------------------------------------------------*/

#endif
