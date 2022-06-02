#ifndef __Alat_Gmres_h
#define __Alat_Gmres_h

#include  "richardsonoptimal.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class Gmres : public RichardsonOptimal
  {
public:
    ~Gmres();
    Gmres(const std::string& type, int nvectors, const std::string& solutiontype);
    Gmres( const Gmres& gmres);
    Gmres& operator=( const Gmres& gmres);
    std::string getName() const;
    Gmres* clone() const;

    int getNVectors() const;
    void solve(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
