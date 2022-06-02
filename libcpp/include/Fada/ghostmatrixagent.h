#ifndef __Fada_GhostMatrixAgent_h
#define __Fada_GhostMatrixAgent_h

#include  "Alat/ghostmatrix.h"
#include  "Alat/map.h"
#include  "Alat/matrixinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class GhostMatrixAgent : public Alat::Map<Alat::GhostMatrix, Alat::MatrixInterface*>
  {
public:
    typedef Alat::Map<Alat::GhostMatrix, Alat::MatrixInterface*>::const_iterator const_iterator;
    typedef Alat::Map<Alat::GhostMatrix, Alat::MatrixInterface*>::iterator iterator;

public:
    ~GhostMatrixAgent();
    GhostMatrixAgent();
    GhostMatrixAgent( const GhostMatrixAgent& ghostvectoragent);
    GhostMatrixAgent& operator=( const GhostMatrixAgent& ghostvectoragent);
    std::string getName() const;
    std::ostream& printLoopInformation(std::ostream& os) const;
    bool exists(const Alat::GhostMatrix& mg) const;
    void enrol(const Alat::GhostMatrix& mg );
    Alat::MatrixInterface* operator()(const Alat::GhostMatrix& g) const;
  };
  std::ostream& operator<<(std::ostream& os, const GhostMatrixAgent& gva);
}

/*--------------------------------------------------------------------------*/

#endif
