#ifndef __Alat_GhostMatrix_h
#define __Alat_GhostMatrix_h

#include  "ghost.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class GhostMatrix : public Ghost
  {
private:
    Alat::StringSet _variablesIn;

public:
    ~GhostMatrix();
    GhostMatrix();
    GhostMatrix( const Alat::GhostMatrix& ghostmatrix);
    GhostMatrix(const std::string name, int level=0);
    Alat::GhostMatrix& operator=( const Alat::GhostMatrix& ghostmatrix);

    void setVariablesIn(const Alat::StringSet& variables);
    const Alat::StringSet& getVariablesIn() const;
    void setVariablesOut(const Alat::StringSet& variables);
    const Alat::StringSet& getVariablesOut() const;
    // bool operator==(const Alat::GhostMatrix& v) const;
    // bool operator<(const Alat::GhostMatrix& v) const;
  };
  std::ostream& operator<<(std::ostream& os, const Alat::GhostMatrix& g);
}

/*--------------------------------------------------------------------------*/

#endif
