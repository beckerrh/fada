#ifndef __Alat_armadillo_h
#define __Alat_armadillo_h

#define ARMA_NO_DEBUG
#include  <armadillo>

/*--------------------------------------------------------------------------*/
namespace Alat
{
  typedef arma::Col<int> armaivec;
  typedef arma::Mat<int> armaimat;
  typedef arma::Cube<int> armaicube;
  typedef arma::Col<double> armavec;
  typedef arma::Mat<double> armamat;
  typedef arma::Cube<double> armacube;
}

/*--------------------------------------------------------------------------*/

#endif
