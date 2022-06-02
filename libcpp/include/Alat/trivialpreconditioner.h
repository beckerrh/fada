#ifndef __Alat_TrivialPreconditioner_h
#define __Alat_TrivialPreconditioner_h

#include  "preconditioner.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class TrivialPreconditioner : public Alat::Preconditioner
  {
private:
protected:
public:
    ~TrivialPreconditioner();
    TrivialPreconditioner();
    TrivialPreconditioner( const TrivialPreconditioner& trivialpreconditioner);
    TrivialPreconditioner& operator=( const TrivialPreconditioner& trivialpreconditioner);
    std::string getName() const;
    TrivialPreconditioner* clone() const;

    int getNVectors() const;
    void reInit();
    void computePreconditioner();
    void solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const;
    void setsmoothtype(std::string smoothtype);
  };
}

/*--------------------------------------------------------------------------*/

#endif
