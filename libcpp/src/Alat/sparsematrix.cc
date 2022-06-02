#include  "Alat/sparsematrix.h"
#include  "Alat/variablevector.h"
#include  "Alat/doublematrix.h"
#include  <cassert>
#include  <fstream>

using namespace Alat;
using namespace std;

/*-------------------------------------------------------------*/
SparseMatrix::~SparseMatrix(){}
SparseMatrix::SparseMatrix(){}
SparseMatrix::SparseMatrix(const SparseMatrix& S){}
SparseMatrix& SparseMatrix::operator=(const SparseMatrix& S)
{
  assert(0);
  return *this;
}
std::string SparseMatrix::getName() const
{
  return "SparseMatrix";
}
/*-------------------------------------------------------------*/
void SparseMatrix::read(std::string filename)
{
  std::ifstream infile( filename.c_str() );
  getSparsityPattern()->loadFada(infile);
  getValue().loadFada(infile);
}
void SparseMatrix::write(std::string filename, std::string datatype) const
{
  std::ofstream ofile( filename.c_str() );
  getSparsityPattern()->saveFada(ofile, datatype);
  getValue().saveFada(ofile, datatype);
}
std::ostream& SparseMatrix::write(std::ostream& os) const
{
  const Alat::SparsityPattern& SP =  *getSparsityPattern();
  int n = SP.n();
  for(int i = 0; i < n; i++)
  {
    os<<i<<endl;
    for(int pos = SP.rowstart(i); pos < SP.rowstop(i); pos++)
    {
      os<<SP.col(pos)<<": "<<_value[pos]<<", ";
    }
    os<<endl;
  }
  return os;
}

/*-------------------------------------------------------------*/
Alat::SparsityPattern* SparseMatrix::getSparsityPattern()
{
  return &_sparsitypattern;
}

const Alat::SparsityPattern* SparseMatrix::getSparsityPattern() const
{
  return &_sparsitypattern;
}

const Alat::DoubleVector& SparseMatrix::getValue() const
{
  return _value;
}

Alat::DoubleVector& SparseMatrix::getValue()
{
  return _value;
}

const double& SparseMatrix::getValue(int pos) const
{
  return _value[pos ];
}

double& SparseMatrix::getValue(int pos)
{
  return _value[pos ];
}

void SparseMatrix::zeros()
{
  _value.zeros();
}

/*-------------------------------------------------------------*/
void SparseMatrix::set_size(int n, int nt)
{
  _sparsitypattern.set_size(n, nt);
  _value.set_size(nt);
}

void SparseMatrix::set_size(const Alat::SparsityPattern& sparsitypattern)
{
  _sparsitypattern.set_size(sparsitypattern);
  int ntotal = sparsitypattern.ntotal();
  _value.set_size(ntotal);
}

void SparseMatrix::set_size(const Alat::SparsityPatternSoft& sparsitypatternsoft)
{
  _sparsitypattern.set_size(sparsitypatternsoft);
  int ntotal = sparsitypatternsoft.ntotal();
  _value.set_size(ntotal);
}

void SparseMatrix::set_size(const Alat::SparseMatrix& matrix)
{
  set_size(*matrix.getSparsityPattern());
  _value.set_size(_sparsitypattern.ntotal());  
}

/*-------------------------------------------------------------*/
double& SparseMatrix::operator() (int i, int j)
{
  int posstart = _sparsitypattern.rowstart(i);
  int posend = _sparsitypattern.rowstop(i);
  for(int pos = posstart; pos < posend; pos++)
  {
    if(_sparsitypattern.col(pos) == j)
    {
      return _value[pos];
    }
  }
  std::cerr<<"Position ("<<i<<","<<j<<")"<<"doesn't exist in the pattern of the matrix\n";
  assert(0);
}

/*-------------------------------------------------------------*/

bool SparseMatrix::greaterThan(int pos1, int pos2) const
{
  return _value[pos1] > _value[pos2];
}

/*-------------------------------------------------------------*/
void SparseMatrix::productVector(std::vector<double>::iterator fi, std::vector<double>::const_iterator ui, double d) const
{
  const Alat::SparsityPattern& SP =  *getSparsityPattern();
  int n = SP.n();
  for(int i = 0; i < n; i++)
  {
    for(int pos = SP.rowstart(i); pos < SP.rowstop(i); pos++)
    {
      int j = SP.col(pos);
      // y[i] += d*_value[pos]*x[j];
      *fi += d*_value[pos]**( ui+j );
    }
    fi++;
  }
}

void SparseMatrix::productVectorTransposed(std::vector<double>::iterator fi, std::vector<double>::const_iterator ui, double d) const
{
  const Alat::SparsityPattern& SP =  *getSparsityPattern();
  int n = SP.n();
  for(int i = 0; i < n; i++)
  {
    for(int pos = SP.rowstart(i); pos < SP.rowstop(i); pos++)
    {
      int j = SP.col(pos);
      // y[j] += d*_value[pos]*x[i];
      *( fi+j ) += d*_value[pos]**ui;
    }
    ui++;
  }
}

// void SparseMatrix::productVectorByComponent(int icomp, int jcomp, VariableVector& y, const VariableVector& x, double d) const
// {
//   productVector(y.begin(icomp), x.begin(jcomp), d);
// }
//
// void SparseMatrix::productVectorByComponentTransposed(int icomp, int jcomp, VariableVector& y, const VariableVector& x, double d) const
// {
//   productVectorTransposed(y.begin(icomp), x.begin(icomp), d);
// }
//
// void SparseMatrix::productVectorByComponentTransposedDiagonal(int icomp, int jcomp, VariableVector& y, const VariableVector& x, double d) const
// {
//   std::vector<double>::iterator fi = y.begin(icomp);
//   std::vector<double>::const_iterator ui = x.begin(icomp);
//   const Alat::SparsityPattern& SP =  *getSparsityPattern();
//   int n = SP.n();
//   for(int i = 0; i < n; i++)
//   {
//     for(int pos = SP.rowstart(i); pos < SP.rowstop(i); pos++)
//     {
//       int j = SP.col(pos);
//       if(i == j)
//       {
//         *( fi+j ) += d*_value[pos]* *( ui+j );
//       }
//     }
//   }
// }

void SparseMatrix::productVector(Alat::DoubleVector& y, const Alat::DoubleVector& x, double d) const
{
  // const SparsityPattern& sparsitypattern =  *getSparsityPattern();
  int n = _sparsitypattern.n();
  /*if( ( n != y.size() ) ||( n != x.size() ) )
     {
     std::cerr<<"*** ERROR SparseMatrix::productVector(): n, y.size(), x.size() = "<<n<<" "<<y.size()<<" "<<x.size()<<"\n";
     assert(0);
     }*/

  Alat::DoubleVector::iterator py = y.begin();
  for(int i = 0; i < n; i++)
  {
    for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
    {
      int j = _sparsitypattern.col(pos);
      // y[i] += d*_value[pos]*x[j];
      *py += d*_value[pos]*x[j];
    }
    py++;
  }
}

void SparseMatrix::productVectorTransposed(Alat::DoubleVector& y, const Alat::DoubleVector& x, double d) const
{
  // const SparsityPattern& SP =  *getSparsityPattern();
  int n = _sparsitypattern.n();
  assert( n == y.size() );
  assert( n == x.size() );

  Alat::DoubleVector::const_iterator px = x.begin();
  for(int i = 0; i < n; i++)
  {
    for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
    {
      int j = _sparsitypattern.col(pos);
      //          y[j] += d*_value[pos]*x[i];
      y[j] += d*_value[pos]**px;
    }
    px++;
  }
}
// 
// /*-------------------------------------------------------------*/
// 
// void SparseMatrix::dirichlet(int i, double d)
// {
//   for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
//   {
//     int j = _sparsitypattern.col(pos);
//     if(j == i)
//     {
//       _value[pos] = d;
//     }
//     else
//     {
//       _value[pos] = 0.;
//     }
//   }
// }

/*-------------------------------------------------------------*/

void SparseMatrix::zeroLine(int i)
{
  for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
  {
    int j = _sparsitypattern.col(pos);
    _value[pos] = 0.;
  }
}

/*-------------------------------------------------------------*/
void SparseMatrix::assemble(const Alat::IntVector& ind, const Alat::DoubleMatrix& Aloc, double coeff)
{
  SparseMatrix::assemble(ind, ind, Aloc, coeff);
}

void SparseMatrix::assemble(const Alat::IntVector& ind, const Alat::IntVector& jnd, const Alat::DoubleMatrix& Aloc, double coeff)
{
  for(int ii = 0; ii < ind.size(); ii++)
  {
    int i = ind[ii];
    if( i >= _sparsitypattern.n() )
    {
      continue;
    }
    int posstart = _sparsitypattern.rowstart(i);
    int posend = _sparsitypattern.rowstop(i);
    for(int jj = 0; jj < jnd.size(); jj++)
    {
      int j = jnd[jj];
      for(int pos = posstart; pos < posend; pos++)
      {
        if(_sparsitypattern.col(pos) == j)
        {
          _value[pos] += coeff*Aloc(ii, jj);
          break;
        }
      }
    }
  }
}

/*-------------------------------------------------------------*/

const double& SparseMatrix::getEntry(int pos) const
{
  return _value[pos];
}

double& SparseMatrix::getEntry(int pos)
{
  return _value[pos];
}

/*-------------------------------------------------------------*/
void SparseMatrix::suppressZeros(double epsilon)
{
  int n = _sparsitypattern.n();
  Alat::SparsityPatternSoft sparsitypatternsoft(n);

  for(int i = 0; i < n; i++)
  {
    for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
    {
      if(_value[pos] >= epsilon)
      {
        sparsitypatternsoft[i].insert( _sparsitypattern.col(pos) );
      }
    }
  }
  // std::cerr << "SparseMatrix::suppressZeros() " << _sparsitypattern.ntotal() << " ==> " << sparsitypatternsoft.ntotal() << "\n";
  Alat::DoubleVector newvalue( sparsitypatternsoft.ntotal() );
  int count = 0;
  for(int i = 0; i < n; i++)
  {
    for(int pos = _sparsitypattern.rowstart(i); pos < _sparsitypattern.rowstop(i); pos++)
    {
      if(_value[pos] >= epsilon)
      {
        newvalue[count++] = _value[pos];
      }
    }
  }
  // std::cerr << "_value " << _value << "\n";
  _value = newvalue;
  // std::cerr << "newvalue " << newvalue << "\n";
  // std::cerr << "_value " << _value << "\n";
  _sparsitypattern.set_size(sparsitypatternsoft);
}

/*-------------------------------------------------------------*/

std::ostream& Alat::operator<<( std::ostream& os, const SparseMatrix& matrix )
{
  os << "Sparsitypattern = " << *matrix.getSparsityPattern() <<"\n";
  os << "Values = " <<matrix.getValue() <<"\n";
  return os;
}
