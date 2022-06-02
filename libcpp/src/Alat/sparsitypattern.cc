#include  "Alat/sparsitypattern.h"
#include  "Alat/set.h"
#include  <algorithm>
#include  <cassert>
#include  <fstream>

using namespace Alat;
using namespace std;

/*-------------------------------------------------------------*/
SparsityPattern::~SparsityPattern() {}
SparsityPattern::SparsityPattern() : _col(), _rowstart() {}
SparsityPattern::SparsityPattern(const SparsityPattern& S) : _col( S.col() ), _rowstart( S.rowstart() ) {}
SparsityPattern& SparsityPattern::operator=(const SparsityPattern& S)
{
  _rowstart.set_size( S.rowstart().size() );
  _rowstart = S.rowstart();
  _col.set_size( S.col().size() );
  _col = S.col();
  return *this;
}

/*-------------------------------------------------------------*/
void SparsityPattern::set_size(int n, int nt)
{
  _col.set_size(nt);
  _rowstart.set_size(n+1);
}

/*-------------------------------------------------------------*/
void SparsityPattern::set_size(const SparsityPattern& SP)
{
  set_size( SP.n(), SP.ntotal() );
  _col = SP.col();
  _rowstart = SP.rowstart();
  sort();
}

/*-------------------------------------------------------------*/
void SparsityPattern::set_size(const SparsityPatternSoft& SP)
{
  int n = SP.size();
  _col.set_size( SP.ntotal() );
  _rowstart.set_size(n+1);

  _rowstart[0] = 0;
  for(int i = 0; i < n; i++)
  {
    _rowstart[i+1] = _rowstart[i]+SP[i].size();
  }
  int pos = 0;
  for(int i = 0; i < n; i++)
  {
    for(Alat::IntSet::const_iterator p = SP[i].begin(); p != SP[i].end(); p++)
    {
      _col[pos++] = *p;
    }
  }
  sort();
}

/*-------------------------------------------------------------*/
void SparsityPattern::set_size(const Alat::Vector<Alat::IntVector>& SP)
{
  // std::cerr << "::::::" << SP << "\n";
  int n = SP.size();
  int ntotal = 0;
  for(int i = 0; i < n; i++)
  {
    ntotal += SP[i].size();
  }
  _col.set_size(ntotal);
  _rowstart.set_size(n+1);

  _rowstart[0] = 0;
  for(int i = 0; i < n; i++)
  {
    _rowstart[i+1] = _rowstart[i]+SP[i].size();
  }
  int pos = 0;
  for(int i = 0; i < n; i++)
  {
    for(Alat::IntVector::const_iterator p = SP[i].begin(); p != SP[i].end(); p++)
    {
      _col[pos++] = *p;
    }
  }
}
/*-------------------------------------------------------------*/
void SparsityPattern::print( std::ostream& os) const
{
  os << "SparsityPattern n=" << n() << " ntotal " << ntotal() << "\n";
  for(int i = 0; i < n(); i++)
  {
    for(int pos = rowstart(i); pos < rowstop(i); pos++)
    {
      os << col(pos) << " ";
    }
    os << "\n";
  }  
}

/*-------------------------------------------------------------*/
void SparsityPattern::saveFada(std::ostream& out, std::string datatype) const
{
  col().saveFada(out, datatype);
  rowstart().saveFada(out, datatype);
}

/*-------------------------------------------------------------*/
void SparsityPattern::loadFada(std::istream& in)
{
  col().loadFada(in);
  rowstart().loadFada(in);
}

/*-------------------------------------------------------------*/
void SparsityPattern::write(const std::string& filename, std::string datatype) const
{
  // std::string filename = basename + ".fadalightconnect";
  std::ofstream file( filename.c_str() );
  assert( file.is_open() );
  saveFada(file, datatype);
  // col().write(file, datatype);
  // rowstart().write(file, datatype);
  file.close();
}

/*-------------------------------------------------------------*/
void SparsityPattern::read(const std::string& filename)
{
  // std::string filename = basename + ".fadalightconnect";
  std::ifstream file( filename.c_str() );
  if( !file.is_open() )
  {
    std::cerr << "*** ERROR in Alat::SparsityPattern::read(): could not open file " << filename << "\n";
    assert(0);
    exit(1);
  }
  loadFada(file);
  // col().read(file);
  // rowstart().read(file);
  file.close();
}

/*--------------------------------------------------------------------------*/
void SparsityPattern::enlarge(int enlarge_stencil)
{
  if(enlarge_stencil == 0)
  {
    return;
  }
  Alat::SparsityPatternSoft SPS( n() );
  for(int i = 0; i < n(); i++)
  {
    for(int pos = rowstart(i); pos < rowstop(i); pos++)
    {
      SPS[i].insert( col(pos) );
    }
  }

  for(int ilarge = 0; ilarge < enlarge_stencil; ilarge++)
  {
    Alat::SparsityPatternSoft SPSadd( n() );
    for(int i = 0; i < n(); i++)
    {
      for(std::set<int>::const_iterator it = SPS[i].begin(); it != SPS[i].end(); it++)
      {
        int j = *it;
        for(std::set<int>::const_iterator it2 = SPS[j].begin(); it2 != SPS[j].end(); it2++)
        {
          SPSadd[i].insert(*it2);
        }
      }
    }
    for(int i = 0; i < n(); i++)
    {
      for(std::set<int>::const_iterator it = SPSadd[i].begin(); it != SPSadd[i].end(); it++)
      {
        SPS[i].insert(*it);
      }
    }
  }
  set_size(SPS);
}

/*--------------------------------------------------------------------------*/
int SparsityPattern::find(int i, int j) const
{
  for(int pos = rowstart(i); pos < rowstop(i); pos++)
  {
    if(col(pos) == j)
    {
      return pos;
    }
  }
  std::cerr << "*** ERROR Alat::SparsityPattern::find()";
  std::cerr << "no such coupling: "<< i <<" "<<j<<std::endl;
  assert(0);
  return -1;
}

/*--------------------------------------------------------------------------*/
void SparsityPattern::sort()
{
  for(int i = 0; i < n(); i++)
  {
    std::sort(&_col[_rowstart[i]], &_col[_rowstart[i+1]]);
  }
}

/*--------------------------------------------------------------------------*/
void SparsityPattern::clear()
{
  _col.clear();
  _rowstart.clear();
}

int SparsityPattern::n() const
{
  if(_rowstart.size() == 0)
  {
    return 0;
  }
  return _rowstart.size()-1;
}

int SparsityPattern::ntotal() const
{
  return _col.size();
}

int SparsityPattern::rowsize(int i) const
{
  if(_rowstart.size() == 0)
  {
    return 0;
  }
  return _rowstart[i+1]-_rowstart[i];
}

const Alat::IntVector&  SparsityPattern::col() const
{
  return _col;
}

Alat::IntVector&  SparsityPattern::col()
{
  return _col;
}

const Alat::IntVector&  SparsityPattern::rowstart() const
{
  return _rowstart;
}

Alat::IntVector& SparsityPattern::rowstart()
{
  return _rowstart;
}

int&  SparsityPattern::col(int pos)
{
  return _col[pos];
}

int SparsityPattern::col(int pos) const
{
  return _col[pos];
}

int& SparsityPattern::rowstart(int i)
{
  return _rowstart[i];
}

int SparsityPattern::rowstart(int i) const
{
  return _rowstart[i];
}

// int&  SparsityPattern::rowstop(int i)
// {
//   return _rowstart[i+1];
// }

int  SparsityPattern::rowstop(int i) const
{
  return _rowstart[i+1];
}

int SparsityPattern::get(int i, int ii) const
{
  return _col[_rowstart[i]+ii];
}

// const int* SparsityPattern::begin(int i) const
// {
//   return & _col[_rowstart[i]];
// }
// const int* SparsityPattern::end(int i) const
// {
//   return & _col[_rowstart[i+1]];  
// }

/*--------------------------------------------------------------------------*/
std::ostream& Alat::operator<<(std::ostream& s, const SparsityPattern& A)
{
  s << "start:\n"<< A.rowstart() << std::endl;
  s << "col:\n"<< A.col() << std::endl;
  for(int i = 0; i < A.n(); i++)
  {
    s << "\nline " << i << " : ";
    for(int pos = A.rowstart(i); pos < A.rowstop(i); pos++)
    {
      s << A.col(pos) << " ";
    }
  }
  return s;
}

/*--------------------------------------------------------------------------*/
void SparsityPattern::reconstructWithNumbering(const Alat::SparsityPattern* sparsitypattern, const Alat::IntVector& p, const Alat::IntVector& pinv,  int enlarge_stencil)
{
  assert(sparsitypattern);
  // if(enlarge_stencil)
  int n = sparsitypattern->n();
  Alat::SparsityPatternSoft sparsitypatternsoft(n);

  for(int i = 0; i < n; i++)
  {
    int pi = p[i];
    sparsitypatternsoft[i].insert(i);
    for(int pos = sparsitypattern->rowstart(pi); pos < sparsitypattern->rowstop(pi); pos++)
    {
      sparsitypatternsoft[i].insert(pinv[sparsitypattern->col(pos)]);
    }
  }
  // std::cerr << "enlarge_stencil " << enlarge_stencil << "\n";
  for(int ilarge = 0; ilarge < enlarge_stencil; ilarge++)
  {
    Alat::SparsityPatternSoft sparsitypatternsoftbig(n);
    for(int i = 0; i < n; i++)
    {
      for(std::set<int>::const_iterator it = sparsitypatternsoft[i].begin(); it != sparsitypatternsoft[i].end(); it++)
      {
        int j = *it;
        for(std::set<int>::const_iterator it2 = sparsitypatternsoft[j].begin(); it2 != sparsitypatternsoft[j].end(); it2++)
        {
          sparsitypatternsoftbig[i].insert(*it2);
        }
      }
    }
    for(int i = 0; i < n; i++)
    {
      for(std::set<int>::const_iterator it = sparsitypatternsoftbig[i].begin(); it != sparsitypatternsoftbig[i].end(); it++)
      {
        sparsitypatternsoft[i].insert(*it);
      }
    }
  }
  set_size(sparsitypatternsoft);
}

/*--------------------------------------------------------------------------*/
void SparsityPattern::setDiagonal(Alat::IntVector& diagonal) const
{
  diagonal.set_size(n());
  for(int i = 0; i < n(); i++)
  {
    bool found = 0;
    for(int pos = rowstart(i); pos < rowstop(i); pos++)
    {
      if(col(pos) == i)
      {
        diagonal[i] = pos;
        found = 1;
        continue;
      }
    }
    if(!found)
    {
      std::cerr << "*** ERROR in IluBlockmatrix::_setDiag() : i = " << i << "\n";
      for(int pos = rowstart(i); pos < rowstop(i); pos++)
      {
        std::cerr  << col(pos) << " ";
      }
      std::cerr << "\n";
      assert(0);
      exit(1);
    }
  }
}
