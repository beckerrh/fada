#include  "Alat/assemblevector.h"
#include  "Alat/doublevector.h"
#include  "Alat/intvector.h"
#include  "Alat/stringvector.h"
#include  "Alat/variablevector.h"
#include  <cassert>
#include  <cmath>
#include  <iomanip>
#include  <iostream>
#include  <limits>
#include  <sstream>

using namespace Alat;

/*----------------------------------------------------------*/
VariableVector::~VariableVector() {}
VariableVector::VariableVector() : Alat::armamat(), Alat::VariableVectorInterface()
{
  // assert(0);
}
VariableVector::VariableVector(int ncomp) : Alat::armamat(ncomp, 1), Alat::VariableVectorInterface()
{
}
VariableVector::VariableVector(int ncomp, int n) : Alat::armamat(ncomp, n), Alat::VariableVectorInterface()
{
}
VariableVector::VariableVector(const VariableVector& v) : Alat::armamat(v), Alat::VariableVectorInterface(v)
{
}

VariableVector& VariableVector::operator=(const VariableVector& v)
{
  assert(0);
  return *this;
}

Alat::VariableVectorInterface* VariableVector::clone() const
{
  assert(0);
  return new VariableVector(*this);
}

std::string VariableVector::getName() const
{
  return "VariableVector";
}


/*----------------------------------------------------------*/
int VariableVector::getNComponents() const
{
  return n_rows;
}
int VariableVector::n() const
{
  return n_cols;
}

//
/*----------------------------------------------------------*/
std::ostream& VariableVector::writeAscii(std::ostream& os) const
{
  //Attention !, le résultat est lu dans le python....
  os<<getNComponents() << " " << n() << "\n";
  os << std::setprecision(12) << (*this).t();
  return os;
}

/*----------------------------------------------------------*/
VariableVector::const_iterator VariableVector::begin(int icomp) const
{
  return begin_col(icomp);
  // return Alat::DoubleVector::begin() + icomp* _n;
}

VariableVector::iterator VariableVector::begin(int icomp)
{
  return begin_col(icomp);
}

VariableVector::iterator VariableVector::end(int icomp)
{
  return end_col(icomp);
}

VariableVector::const_iterator VariableVector::end(int icomp) const
{
  return end_col(icomp);
}

/*----------------------------------------------------------*/

void VariableVector::setSingleVectorFromDirectSolver(int offset, const Alat::DoubleVector& u)
{
  int ncomp = getNComponents();
  for(int icomp = 0; icomp < ncomp; icomp++)
  {
    for(int i = 0; i < n(); i++)
    {
      // Attention : numerotation renversee
      ( *this )(icomp, i) = u[offset + i*ncomp + icomp];
    }
  }
}

/*----------------------------------------------------------*/

void VariableVector::addVectorRhsForDirectSolver(int offset, Alat::DoubleVector& f) const
{
  int ncomp = getNComponents();
  for(int icomp = 0; icomp < ncomp; icomp++)
  {
    for(int i = 0; i < n(); i++)
    {
      // Attention : numerotation renversee
      f[offset + i*ncomp + icomp] += ( *this )(icomp, i);
    }
  }
}

/*----------------------------------------------------------*/

void VariableVector::set(const Alat::AssembleVector& vloc, const Alat::IntVector& indices)
{
  int ncomp = vloc.ncomp();
  assert(getNComponents() == ncomp);
  for(int icomp = 0; icomp < ncomp; icomp++)
  {
    for(int ii = 0; ii < indices.size(); ii++)
    {
      ( *this )(icomp, indices[ii]) = vloc(icomp, ii);
    }
  }
}

/*----------------------------------------------------------*/

void VariableVector::assemble(const Alat::AssembleVector& vloc, const Alat::IntVector& indices, const Alat::DoubleVector& scale, double d)
{
  // std::cerr << "VariableVector::assemble() scale=" <<scale<< "\n";
  int ncomp = vloc.ncomp();
  if(scale.size()!=ncomp)
  {
    std::cerr << "VariableVector::assemble() scale=" <<scale<< "\n";
    std::cerr << "me: ncomp="<<getNComponents()<<"\n";
    std::cerr << "you: ncomp="<<vloc.ncomp()<<" n="<<vloc.nloc()<<"\n";
  }
  assert(scale.size()==ncomp);
  assert(getNComponents() == ncomp);
  for(int icomp = 0; icomp < ncomp; icomp++)
  {
    double s = d/scale[icomp];
    for(int ii = 0; ii < indices.size(); ii++)
    {
      ( *this )(icomp, indices[ii]) += s*vloc(icomp,ii);
    }
  }
}

/*----------------------------------------------------------*/

void VariableVector::extract(Alat::AssembleVector& uloc, const Alat::IntVector& indices, const Alat::DoubleVector& scale) const
{
  int ncomp = uloc.ncomp();
  assert(scale.size()==ncomp);
  // std::cerr << "VariableVector::extract() scale=" <<scale<< "\n";
  assert(getNComponents() == ncomp);
  for(int icomp = 0; icomp < ncomp; icomp++)
  {
    for(int ii = 0; ii < indices.size(); ii++)
    {
      uloc(icomp,ii) = scale[icomp] * ( *this )(icomp, indices[ii]);
    }
  }
}

/*----------------------------------------------------------*/

void VariableVector::set_size(int ncomp, int n)
{
  Alat::armamat::set_size(ncomp, n);
}

/*----------------------------------------------------------*/
void VariableVector::scaleIntVector(const Alat::IntVector& count)
{
  int ndof = count.size();
  for(int idof = 0; idof < ndof; idof++)
  {
    for(int icomp = 0; icomp < getNComponents(); icomp++)
    {
      operator()(icomp, idof) /= (double) count[idof];
    }
  }
}

/*----------------------------------------------------------*/
void VariableVector::scale(double s)
{
  // Alat::DoubleVector::operator*=(s);
  Alat::armamat::operator*=(s);
}

/*----------------------------------------------------------*/
void VariableVector::scale(const Alat::DoubleVector& scale)
{
  for(int icomp = 0; icomp < getNComponents(); icomp++)
  {
    double s = scale[icomp];
    iterator p = begin(icomp);
    while( p < end(icomp) )
    {
      *p++ *= s;
    }
  }
}

/*----------------------------------------------------------*/
void VariableVector::scaleinv(const Alat::DoubleVector& scale)
{
  for(int icomp = 0; icomp < getNComponents(); icomp++)
  {
    double s = 1.0/scale[icomp];
    iterator p = begin(icomp);
    while( p < end(icomp) )
    {
      *p++ *= s;
    }
  }
}

/*----------------------------------------------------------*/

void VariableVector::zeros()
{
  Alat::armamat::zeros();
}

/*----------------------------------------------------------*/

void VariableVector::zero(int icomp)
{
  iterator p = begin(icomp);
  while( p < end(icomp) )
  {
    *p++ = 0.0;
  }
}

/*----------------------------------------------------------*/
double VariableVector::min(int icomp) const
{
  double d = std::numeric_limits<double>::max();
  const_iterator p = begin(icomp);
  while( p < end(icomp) )
  {
    d = fmin(d, *p++);
  }
  return d;
}
double VariableVector::max(int icomp) const
{
  double d = std::numeric_limits<double>::lowest();
  const_iterator p = begin(icomp);
  while( p < end(icomp) )
  {
    d = fmax(d, *p++);
  }
  return d;
}
double VariableVector::maxnorm(int icomp) const
{
  double d = 0.0;
  const_iterator p = begin(icomp);
  while( p < end(icomp) )
  {
    d = fmax(d, fabs(*p++));
  }
  return d;
}
double VariableVector::sum(int icomp) const
{
  // result = arma::sum(static_cast<const Alat::armamat&>(*this), 1);
  arma::Col<double> result = arma::sum(static_cast<const Alat::armamat&>(*this), 1);
  return result[icomp];
  assert(0);
  double d = 0.0;
  const_iterator p = begin(icomp);
  while( p < end(icomp) )
  {
    d += *p++;
  }
  return d;
}

/*----------------------------------------------------------*/
double VariableVector::norm() const
{
  return arma::norm(static_cast<const Alat::armamat&>(*this));
}
/*----------------------------------------------------------*/
double VariableVector::norm(int icomp) const
{
  return arma::norm(static_cast<const Alat::armamat&>(*this).row(icomp));
}

/*----------------------------------------------------------*/

double VariableVector::scalarProduct(const Alat::VariableVectorInterface* v) const
{
  const arma::mat* vv = dynamic_cast<const arma::mat*>( v );
  assert(vv);
  return arma::dot(*vv, static_cast<const Alat::armamat&>(*this));
}

/*----------------------------------------------------------*/

void VariableVector::equal(double d)
{
  fill(d);
}

void VariableVector::equal(const Alat::VariableVectorInterface* v)
{
  const Alat::armamat* vv = dynamic_cast<const Alat::armamat*>( v );
  assert(vv);
  static_cast<Alat::armamat&>(*this)=*vv;
}

/*----------------------------------------------------------*/
void VariableVector::add(const double& d, const Alat::VariableVectorInterface* v)
{
  const Alat::armamat* vv = dynamic_cast<const Alat::armamat*>( v );
  Alat::armamat& tarma =static_cast<Alat::armamat&>(*this);
  tarma += d* (*vv);
}

/*----------------------------------------------------------*/
void VariableVector::savehdf5(const std::string& filename, const Alat::StringVector& blocknames) const
{
  arma::hdf5_name spec(filename, blocknames[0], arma::hdf5_opts::append);
  save(spec);
  // int ncomp = getNComponents();
  // assert(blocknames.size()==ncomp);
  // for(int icomp = 0; icomp < ncomp; icomp++)
  // {
  //   arma::hdf5_name spec(filename, blocknames[icomp], arma::hdf5_opts::append);
  //   row(icomp).save(spec);
  // }
}
void VariableVector::loadhdf5(const std::string& filename, const Alat::StringVector& blocknames)
{
  load(filename);
}


/*----------------------------------------------------------*/
std::ostream& VariableVector::saveFada(std::ostream& out, const std::string datatype) const
{
  if(datatype == "ascii")
  {
    save(out, arma::arma_ascii);
  }
  else
  {
    save(out, arma::arma_binary);
  }
  return out;
}
std::istream& VariableVector::loadFada(std::istream& in)
{
  load(in);
  return in;
}
