#include  "Alat/doublevector.h"
#include  "Alat/doublematrix.h"
#include  "reaction.h"

/*--------------------------------------------------------------------------*/
void Reaction::reaction(Alat::DoubleVector& omega, const Alat::DoubleVector& u, double x, double y, double z, double t) const
{
   omega(0) = 32*u[0];
}

/*--------------------------------------------------------------------------*/
void Reaction::reactionPrime(Alat::DoubleMatrix& omegaprime, const Alat::DoubleVector& u, double x, double y, double z, double t) const
{
   omegaprime(0,0) = 32;
}
