#include  "Alat/systemassemblematrix.h"
// #include  "Fada/dofinformationinterface.h"
#include  "Alat/gaussseidelvariablediagonalmatrix.h"
// #include  "Fada/localdofinformation.h"
// #include  "FadaMesh/meshinterface.h"
#include  "Alat/variablematrix.h"
#include  "Alat/variablevector.h"
#include  "Alat/doublevector.h"
#include  <cassert>
#include  "Alat/armadillo.h"

using namespace Alat;

/*--------------------------------------------------------------------------*/
GaussSeidelVariableDiagonalMatrix::~GaussSeidelVariableDiagonalMatrix(){}
GaussSeidelVariableDiagonalMatrix::GaussSeidelVariableDiagonalMatrix(int ncomp, std::string type) : VariableDiagonalMatrix(ncomp, ncomp, type), _variablematrix(NULL){}
GaussSeidelVariableDiagonalMatrix::GaussSeidelVariableDiagonalMatrix( const GaussSeidelVariableDiagonalMatrix& variablematrix) : VariableDiagonalMatrix(variablematrix)
{
  assert(0);
}

GaussSeidelVariableDiagonalMatrix& GaussSeidelVariableDiagonalMatrix::operator=( const GaussSeidelVariableDiagonalMatrix& variablematrix)
{
  VariableDiagonalMatrix::operator=(variablematrix);
  assert(0);
  return *this;
}

std::string GaussSeidelVariableDiagonalMatrix::getName() const
{
  if(_type == "simple")
  {
    return "GaussSeidelVariableDiagonalMatrix_Simple";
  }
  else if(_type == "diagonal")
  {
    return "GaussSeidelVariableDiagonalMatrix_Diagonal";
  }
  return "GaussSeidelVariableDiagonalMatrix_Full";
}

GaussSeidelVariableDiagonalMatrix* GaussSeidelVariableDiagonalMatrix::clone() const
{
  assert(0);
//return new GaussSeidelVariableDiagonalMatrix(*this);
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
void GaussSeidelVariableDiagonalMatrix::computeGaussSeidel(double relax, const Alat::VariableMatrixInterface* variablematrixinterface)
{
  _variablematrix = dynamic_cast<const Alat::VariableMatrix*>( variablematrixinterface );
  assert(_variablematrix);
  const Alat::SparsityPattern* sparsitypattern = _variablematrix->getSparsityPattern();
  // assert( _type == _variablematrix->getType() );
	assert(_type == "full");

  int ncomp = _variablematrix->getNComponents();
  assert(_ncomp == ncomp);
  arma::mat H(ncomp, ncomp);
  std::cerr << "sparsitypattern->n()= "<<sparsitypattern->n()<<" _fullmatrix.size()="<<_fullmatrix(0, 0).size()<<"\n";
  std::cerr << "sparsitypattern->n()= "<<sparsitypattern->n()<<" _simplematrix.size()="<<_simplematrix.size()<<"\n";
  assert( sparsitypattern->n() == _fullmatrix(0, 0).size() );
  for(int i = 0; i < sparsitypattern->n(); i++)
  {
    for(int pos = sparsitypattern->rowstart(i); pos != sparsitypattern->rowstop(i); pos++)
    {
      int j = sparsitypattern->col(pos);
      if(j == i)
      {
        if(_type == "simple")
        {
          _simplematrix[i] = 1./_variablematrix->getValue(0, 0, pos);
        }
        else
        {
          for(int icomp = 0; icomp < ncomp; icomp++)
          {
            for(int jcomp = 0; jcomp < ncomp; jcomp++)
            {
              H(icomp, jcomp) = _variablematrix->getValue(icomp, jcomp, pos);
            }
          }
          H = arma::inv(H);
          for(int icomp = 0; icomp < ncomp; icomp++)
          {
            for(int jcomp = 0; jcomp < ncomp; jcomp++)
            {
              _fullmatrix ( icomp, jcomp)[i] = H(icomp, jcomp);
            }
          }
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void GaussSeidelVariableDiagonalMatrix::gaussSeidel(Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, double relax, const Alat::IntVector& p, const Alat::IntVector& pinv) const
{
  const Alat::VariableVector* vf = dynamic_cast<const Alat::VariableVector*>( f );
  assert(vf);
  Alat::VariableVector* vu = dynamic_cast<Alat::VariableVector*>( u );
  assert(vu);
  const Alat::SparsityPattern* sparsitypattern = _variablematrix->getSparsityPattern();
  if(_type == "full")
  {
    assert( p.size() == _fullmatrix(0, 0).size() );
    assert( pinv.size() == _fullmatrix(0, 0).size() );

		// std::cerr << "p="<<p<<"\n";
		// std::cerr << "pinv="<<pinv<<"\n";

    for(int i = 0; i < p.size(); i++)
    {
      // int ip = p[i];
      int ip = pinv[i];
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        ( *vu )(icomp, ip) = ( *vf )(icomp, ip);
      }
      for(int pos = sparsitypattern->rowstart(ip); pos != sparsitypattern->rowstop(ip); pos++)
      {
        int j = sparsitypattern->col(pos);
        // int jp = pinv[j];
        // int jp = p[j];
        int jp = j;
        if(jp != ip)
        {
          for(int icomp = 0; icomp < _ncomp; icomp++)
          {
            for(int jcomp = 0; jcomp < _mcomp; jcomp++)
            {
              Alat::DoubleVector::const_iterator a = _variablematrix->getFullMatrix(icomp, jcomp).begin();
              Alat::VariableVector::iterator vup = vu->begin(icomp);
              Alat::VariableVector::const_iterator q = vf->begin(jcomp);
              *( vup+ip ) -= *( a+pos ) * *( q+jp );
            }
          }
        }
      }
      for(int icomp = 0; icomp < _ncomp; icomp++)
      {
        for(int jcomp = 0; jcomp < _mcomp; jcomp++)
        {
          Alat::DoubleVector::const_iterator a = _fullmatrix(icomp, jcomp).begin();
          Alat::VariableVector::iterator vup = vu->begin(icomp);
          // Alat::VariableVector::const_iterator q = vf->begin(jcomp);
          *( vup+ip ) = (1.0-relax)* *( vup+ip ) + relax* *( a+ip ) * *( vup+ip );
          // *( vup+ip ) =  *( a+ip ) * *( vup+ip );
        }
      }
    }
  }
  else
  {
    assert(0);
  }
}