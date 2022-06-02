#ifndef __Alat_GhostCouplingMatrix_h
#define __Alat_GhostCouplingMatrix_h

#include  "ghostmatrix.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class GhostCouplingMatrix : public GhostMatrix
  {
private:
    Alat::StringSet _variablesRightIn, _variablesRightOut;

public:
    ~GhostCouplingMatrix();
    GhostCouplingMatrix();
    GhostCouplingMatrix( const Alat::GhostCouplingMatrix& ghostmatrix);
    GhostCouplingMatrix( const Alat::GhostMatrix& ghostmatrix);
    GhostCouplingMatrix(const std::string name, int level=0);
    // GhostCouplingMatrix(const std::string name, const std::string& type);
    GhostCouplingMatrix& operator=( const Alat::GhostCouplingMatrix& ghostmatrix);

    void setVariablesLeftIn(const Alat::StringSet& variables);
    const Alat::StringSet& getVariablesLeftIn() const;
    void setVariablesLeftOut(const Alat::StringSet& variables);
    const Alat::StringSet& getVariablesLeftOut() const;
    void setVariablesRightIn(const Alat::StringSet& variables);
    const Alat::StringSet& getVariablesRightIn() const;
    void setVariablesRightOut(const Alat::StringSet& variables);
    const Alat::StringSet& getVariablesRightOut() const;
    // bool operator==(const Alat::GhostCouplingMatrix& v) const;
    // bool operator<(const Alat::GhostCouplingMatrix& v) const;
  };
  std::ostream& operator<<(std::ostream& os, const Alat::GhostCouplingMatrix& g);
}

/*--------------------------------------------------------------------------*/

#endif
