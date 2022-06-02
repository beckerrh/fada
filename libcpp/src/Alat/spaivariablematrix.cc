#include  "Alat/spaivariablematrix.h"
#include  "Alat/variablevector.h"
#include  "Alat/doublevector.h"
#include  <cassert>
#include  "Alat/armadillo.h"
#include  "Alat/map.h"

using namespace Alat;

/*--------------------------------------------------------------------------*/
SpaiVariableMatrix::~SpaiVariableMatrix()
{}

SpaiVariableMatrix::SpaiVariableMatrix(int ncomp, std::string type) : VariableMatrix(ncomp, ncomp, type)
{
}

SpaiVariableMatrix::SpaiVariableMatrix( const SpaiVariableMatrix& variablematrix) : VariableMatrix(variablematrix)
{
  assert(0);
}

SpaiVariableMatrix& SpaiVariableMatrix::operator=( const SpaiVariableMatrix& variablematrix)
{
  VariableMatrix::operator=(variablematrix);
  assert(0);
  return *this;
}

std::string SpaiVariableMatrix::getName() const
{
  if(_type == "simple")
  {
    return "SpaiVariableMatrix_Simple";
  }
  else if(_type == "diagonal")
  {
    return "SpaiVariableMatrix_Diagonal";
  }
  return "SpaiVariableMatrix_Full";
}

SpaiVariableMatrix* SpaiVariableMatrix::clone() const
{
  assert(0);
//return new SpaiVariableMatrix(*this);
}

/*--------------------------------------------------------------------------*/
void SpaiVariableMatrix::set_size(const VariableMatrix* variablematrix)
{
  const Alat::SparsityPattern* sparsitypattern = variablematrix->getSparsityPattern();
  Alat::SparsityPattern* spaisparsitypattern = getSparsityPattern();
  VariableMatrix::set_size( sparsitypattern->n(), sparsitypattern->ntotal() );
	spaisparsitypattern->set_size(*sparsitypattern);
}

/*--------------------------------------------------------------------------*/
void SpaiVariableMatrix::computeSpai(double relax, const Alat::VariableMatrixInterface* variablematrixinterface)
{
  // const Alat::SpaiVariableMatrix* variablematrix = dynamic_cast<const Alat::SpaiVariableMatrix*>( variablematrixinterface );
  const Alat::VariableMatrix* variablematrix = dynamic_cast<const Alat::VariableMatrix*>( variablematrixinterface );
  if(not variablematrix)
	{
		std::cerr << "SpaiVariableMatrix::computeSpai(d) unknown matrix " << variablematrixinterface->getName()<<"\n";
		assert(0);
	}
  if(_type == "simple")
  {
    _computeSpaiSimple(relax, variablematrix);
    // _computeSpaiSimple2(u, variablematrix);
  }
  else if(_type == "diagonal")
  {
    _computeSpaiDiagonal(relax, variablematrix);
  }
  else
  {
    _computeSpaiFull(relax, variablematrix);
  }
}

/*--------------------------------------------------------------------------*/
void SpaiVariableMatrix::_computeSpaiSimple(double relax, const Alat::VariableMatrix* variablematrix)
{
  std::string matrixtype = variablematrix->getType();
  assert(matrixtype == "simple" or matrixtype == "full");

  const Alat::SparsityPattern* sparsitypattern = variablematrix->getSparsityPattern();
  Alat::SparsityPattern* spaisparsitypattern = getSparsityPattern();
  zeros();

  int n = spaisparsitypattern->n();
  int ncomp = variablematrix->getNComponents();
  double dncomp = 1.0/(double)ncomp;
  assert( variablematrix->getMComponents() == ncomp);

  Alat::DoubleVector& mysimplematrix = getSimpleMatrix();
  const Alat::DoubleVector* simplematrix;
  const Alat::Matrix<Alat::DoubleVector>* fullmatrix;
  if(matrixtype == "simple")
  {
    simplematrix = &variablematrix->getSimpleMatrix();
  }
  else
  {
    fullmatrix = &variablematrix->getFullMatrix();
  }
  arma::mat smallA, armaQ;
  for(int i = 0; i < n; i++)
  {
    int ni = spaisparsitypattern->rowsize(i);
    Alat::IntMap jmap, kmap;
    Alat::IntVector kvec(ni);

    int count = 0;
    for(int pos = spaisparsitypattern->rowstart(i); pos != spaisparsitypattern->rowstop(i); pos++)
    {
      int k = spaisparsitypattern->col(pos);
      kvec[count++] = k;
    }
    for(int ki = 0; ki < ni; ki++)
    {
      kmap[kvec[ki]] = ki;
    }
    count = 0;
    for(int ki = 0; ki < ni; ki++)
    {
      int k = kvec[ki];
      for(int pos = sparsitypattern->rowstart(k); pos != sparsitypattern->rowstop(k); pos++)
      {
        int j = sparsitypattern->col(pos);
        if( jmap.find(j) == jmap.end() )
        {
          jmap[j] = count++;
        }
      }
    }
    int nj = jmap.size();
    smallA.resize(ni, nj);
    smallA.fill(arma::fill::zeros);
    for(int ki = 0; ki < ni; ki++)
    {
      int k = kvec[ki];
      for(int pos = sparsitypattern->rowstart(k); pos != sparsitypattern->rowstop(k); pos++)
      {
        int j = sparsitypattern->col(pos);
        if(j == k)
        {
          if(matrixtype == "simple")
          {
            smallA(ki, jmap[j]) += ( *simplematrix )[pos];
          }
          else
          {
            for(int icomp = 0; icomp < ncomp; icomp++)
            {
              smallA(ki, jmap[j]) += dncomp*( *fullmatrix )(icomp, icomp)[pos];
            }
          }
        }
        else
        {
          if(matrixtype == "simple")
          {
            smallA(ki, jmap[j]) += relax*( *simplematrix )[pos];
          }
          else
          {
            for(int icomp = 0; icomp < ncomp; icomp++)
            {
              smallA(ki, jmap[j]) += relax*dncomp*( *fullmatrix )(icomp, icomp)[pos];
            }
          }
        }
        // for(int pos2 = sparsitypattern->rowstart(j); pos2 != sparsitypattern->rowstop(j); pos2++)
        // {
        //   if(sparsitypattern->col(pos2) == k)
        //   {
        //     smallA(ki, jmap[j]) += relax*( *simplematrix )[pos2];
        //   }
        // }
      }
    }
    // std::cerr << "smallA="<<smallA<<"\n";
    armaQ = arma::pinv(smallA);
    // std::cerr << "armaQ="<<armaQ<<"\n";
    for(int pos = spaisparsitypattern->rowstart(i); pos != spaisparsitypattern->rowstop(i); pos++)
    {
      int j = spaisparsitypattern->col(pos);
      int jlocal = jmap[j];
      mysimplematrix[pos] = armaQ(jlocal, kmap[i]);
    }
  }
  // std::cerr << "SpaiPreconditioner::_computePreconditionerSpaiVariableMatrixSimple() simplematrix="<<*simplematrix<<"\n";
  // std::cerr << "SpaiPreconditioner::_computePreconditionerSpaiVariableMatrixSimple() mysimplematrix="<<mysimplematrix<<"\n";
}

/*--------------------------------------------------------------------------*/
void SpaiVariableMatrix::_computeSpaiSimple2(double relax, const Alat::VariableMatrix* variablematrix)
{
  std::string matrixtype = variablematrix->getType();

  const Alat::SparsityPattern* sparsitypattern = variablematrix->getSparsityPattern();
  Alat::SparsityPattern* spaisparsitypattern = getSparsityPattern();
  zeros();

  int n = spaisparsitypattern->n();
  int ncomp = variablematrix->getNComponents();
  assert( variablematrix->getMComponents() == ncomp);

  Alat::DoubleVector& mysimplematrix = getSimpleMatrix();
  const Alat::DoubleVector& simplematrix = variablematrix->getSimpleMatrix();

  assert(matrixtype == "simple");

  arma::mat armaAIJ;
  arma::mat U;
  arma::vec s;
  arma::mat V;
  arma::vec c, a0;

  // std::cerr << "type="<<_type<<"\n";

  for(int i = 0; i < n; i++)
  {
    int ni = spaisparsitypattern->rowsize(i);
    Alat::IntMap jmap, kmap;
    Alat::IntVector kvec(ni);

    int count = 0;
    for(int pos = spaisparsitypattern->rowstart(i); pos != spaisparsitypattern->rowstop(i); pos++)
    {
      int k = spaisparsitypattern->col(pos);
      kvec[count++] = k;
    }
    for(int ki = 0; ki < ni; ki++)
    {
      kmap[kvec[ki]] = ki;
    }

    count = 0;
    for(int ki = 0; ki < ni; ki++)
    {
      int k = kvec[ki];
      for(int pos = sparsitypattern->rowstart(k); pos != sparsitypattern->rowstop(k); pos++)
      {
        int j = sparsitypattern->col(pos);
        if(j == i)
        {
          continue;
        }
        if( jmap.find(j) == jmap.end() )
        {
          jmap[j] = count++;
        }
      }
    }

    int nj = jmap.size();
    armaAIJ.resize( ni, nj );
    armaAIJ.fill(arma::fill::zeros);
    a0.resize(ni);
    a0.fill(arma::fill::zeros);
    for(int ki = 0; ki < ni; ki++)
    {
      int k = kvec[ki];
      for(int pos = sparsitypattern->rowstart(k); pos != sparsitypattern->rowstop(k); pos++)
      {
        int j = sparsitypattern->col(pos);
        if(j == i)
        {
          a0[ki] = simplematrix[pos];
        }
        else
        {
          armaAIJ(ki, jmap[j]) += simplematrix[pos];
          // for(int pos2 = sparsitypattern->rowstart(j); pos2 != sparsitypattern->rowstop(j); pos2++)
          // {
          //   if(sparsitypattern->col(pos2) == k)
          //   {
          //     armaAIJ(ki, jmap[j]) += simplematrix[pos2];
          //   }
          // }
        }
      }
    }

    assert( arma::svd(U, s, V, armaAIJ) );
    c.resize(ni);
    c.fill(arma::fill::zeros);
    for(int ii = 0; ii < ni; ii++)
    {
      for(int jj = 0; jj < ni; jj++)
      {
        for(int kk = 0; kk < ni; kk++)
        {
          if(fabs(s[jj]) > 1e-12)
          {
            c[ii] += a0[kk]*U(ii, jj) * U(kk, jj)/ ( s[jj]*s[jj] );
          }
        }
      }
    }
    double lambda = arma::dot(c, a0);
    if(fabs(lambda) < 1e-20)
    {
      std::cerr << "a0="<<a0<<"c="<<c<< "lambda="<<lambda << "\n";
      std::cerr << "armaAIJ="<<armaAIJ << "U="<<U<<" s="<<s<<"\n";
      assert(0);
    }
    count = 0;
    lambda = 1.0/lambda;
    for(int pos = spaisparsitypattern->rowstart(i); pos != spaisparsitypattern->rowstop(i); pos++)
    {
      mysimplematrix[pos] = lambda*c[count++];
    }
  }
}

/*--------------------------------------------------------------------------*/
void SpaiVariableMatrix::_computeSpaiDiagonal(double relax, const Alat::VariableMatrix* variablematrix)
{
  std::string matrixtype = variablematrix->getType();
  assert(matrixtype != "simple");
  assert(0);
}

/*--------------------------------------------------------------------------*/
void SpaiVariableMatrix::_computeSpaiFull(double relax, const Alat::VariableMatrix* variablematrix)
{
  std::string matrixtype = variablematrix->getType();
  assert(matrixtype == "full");

  const Alat::SparsityPattern* sparsitypattern = variablematrix->getSparsityPattern();
  Alat::SparsityPattern* spaisparsitypattern = getSparsityPattern();
  zeros();

  int n = spaisparsitypattern->n();
  int ncomp = variablematrix->getNComponents();
  assert( variablematrix->getMComponents() == ncomp);

  arma::mat armaAIJ, armaQ;
  for(int i = 0; i < n; i++)
  {
    int ni = spaisparsitypattern->rowsize(i);
    Alat::IntMap jmap, kmap;
    Alat::IntVector kvec(ni);

    int count = 0;
    for(int pos = spaisparsitypattern->rowstart(i); pos != spaisparsitypattern->rowstop(i); pos++)
    {
      int k = spaisparsitypattern->col(pos);
      kvec[count++] = k;
    }
    for(int ki = 0; ki < ni; ki++)
    {
      kmap[kvec[ki]] = ki;
    }

    count = 0;
    for(int ki = 0; ki < ni; ki++)
    {
      int k = kvec[ki];
      for(int pos = sparsitypattern->rowstart(k); pos != sparsitypattern->rowstop(k); pos++)
      {
        int j = sparsitypattern->col(pos);
        if( jmap.find(j) == jmap.end() )
        {
          jmap[j] = count++;
        }
      }
    }
    int nj = jmap.size();
    armaAIJ.resize(ni*ncomp, nj*ncomp);
    armaAIJ.fill(arma::fill::zeros);
    for(int ki = 0; ki < ni; ki++)
    {
      int k = kvec[ki];
      for(int pos = sparsitypattern->rowstart(k); pos != sparsitypattern->rowstop(k); pos++)
      {
        int j = sparsitypattern->col(pos);
        for(int icomp = 0; icomp < ncomp; icomp++)
        {
          for(int jcomp = 0; jcomp < ncomp; jcomp++)
          {
            // armaAIJ(ki*ncomp+icomp, jmap[j]*ncomp+jcomp) += variablematrix->getEntry(pos, icomp, jcomp);
            armaAIJ(ki*ncomp+icomp, jmap[j]*ncomp+jcomp) += variablematrix->getValue(icomp, jcomp, pos);
          }
        }
      }
    }
    armaQ = arma::pinv(armaAIJ);
    for(int pos = spaisparsitypattern->rowstart(i); pos != spaisparsitypattern->rowstop(i); pos++)
    {
      int j = spaisparsitypattern->col(pos);
      for(int icomp = 0; icomp < ncomp; icomp++)
      {
        for(int jcomp = 0; jcomp < ncomp; jcomp++)
        {
          getValue(icomp, jcomp, pos) = armaQ(jmap[j]*ncomp+jcomp, kmap[i]*ncomp+icomp);
        }
      }
    }
  }
}
