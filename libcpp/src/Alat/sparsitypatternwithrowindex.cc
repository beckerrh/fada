#include  "Alat/sparsitypatternwithrowindex.h"
#include  "Alat/set.h"
#include  <cassert>
#include  <fstream>

using namespace Alat;
using namespace std;

/*----------------------------------------------------------*/
SparsityPatternWithRowIndex::~SparsityPatternWithRowIndex() {}
SparsityPatternWithRowIndex::SparsityPatternWithRowIndex() : SparsityPattern(), _row() {}
SparsityPatternWithRowIndex::SparsityPatternWithRowIndex(const SparsityPatternWithRowIndex& S) : SparsityPattern(S), _row( S.row() ) {}
SparsityPatternWithRowIndex& SparsityPatternWithRowIndex::operator=(const SparsityPatternWithRowIndex& S)
{
  assert(0);
  return *this;
}

/*----------------------------------------------------------*/
void SparsityPatternWithRowIndex::clear()
{
  SparsityPattern::clear();
  _row.clear();
  _rowinverse.clear();
}

/*-------------------------------------------------------------*/

void SparsityPatternWithRowIndex::set_size(const SparsityPatternWithRowIndex& sparsitypattern)
{
  int n = sparsitypattern.n();
  int ntotal = sparsitypattern.ntotal();
  
  _col.set_size(ntotal);
  _rowstart.set_size(n+1);
  _row.set_size(n);
  _col= sparsitypattern.col();
  _rowstart=sparsitypattern.rowstart();
  _row=sparsitypattern.row();  
}

void SparsityPatternWithRowIndex::set_size(const SparsityPatternWithRowIndexSoft& sparsitypattern)
{
  int n = sparsitypattern.size();
  int ntotal = sparsitypattern.ntotal();
  _col.set_size(ntotal);
  _rowstart.set_size(n+1);
  _row.set_size(n);

  int count = 0;
  _rowstart[count++] = 0;
  for(SparsityPatternWithRowIndexSoft::const_iterator p = sparsitypattern.begin(); p != sparsitypattern.end(); p++)
  {
    _rowstart[count] = _rowstart[count-1] + p->second.size();
    count++;
  }

  int pos = 0;
  count = 0;
  for(SparsityPatternWithRowIndexSoft::const_iterator p = sparsitypattern.begin(); p != sparsitypattern.end(); p++)
  {
    int i = p->first;
    _row[count++] = i;
    for(Alat::IntSet::const_iterator q = p->second.begin(); q != p->second.end(); q++)
    {
      _col[pos++] = *q;
    }
  }
  sort();
  computeRowInverse();
}

/*-------------------------------------------------------------*/

void SparsityPatternWithRowIndex::set_size(const SparsityPatternWithVectorOfSet& sparsitypattern)
{
  int n = sparsitypattern.size();
  int ntotal = sparsitypattern.ntotal();
  _col.set_size(ntotal);
  _rowstart.set_size(n+1);
  _row.set_size(n);

  int count = 0;
  _rowstart[count++] = 0;
  for(SparsityPatternWithVectorOfSet::const_iterator p = sparsitypattern.begin(); p != sparsitypattern.end(); p++)
  {
    _rowstart[count] = _rowstart[count-1] + p->second.size();
    count++;
  }

  int pos = 0;
  count = 0;
  for(SparsityPatternWithVectorOfSet::const_iterator p = sparsitypattern.begin(); p != sparsitypattern.end(); p++)
  {
    int i = p->first;
    _row[count++] = i;
    for(Alat::IntSet::const_iterator q = p->second.begin(); q != p->second.end(); q++)
    {
      _col[pos++] = *q;
    }
  }
  sort();
}

/*-------------------------------------------------------------*/

int SparsityPatternWithRowIndex::rowinverse(int i) const
{
  if( _rowinverse.find(i) == _rowinverse.end() )
  {
    std::cerr << "*** ERROR in SparsityPatternWithRowIndex::rowinverse() not found value for i = " << i << " in:\n";
    std::cerr << _rowinverse << "\n";
    assert(0);
    exit(1);
  }
  return _rowinverse[i];
}

/*-------------------------------------------------------------*/

void SparsityPatternWithRowIndex::computeRowInverse()
{
  for(int i = 0; i < n(); i++)
  {
    _rowinverse[row(i)] = i;
  }
}

/*-------------------------------------------------------------*/

void SparsityPatternWithRowIndex::saveFada(std::ostream& out, std::string datatype) const
{
  col().saveFada(out, datatype);
  rowstart().saveFada(out, datatype);
  row().saveFada(out, datatype);
}

/*-------------------------------------------------------------*/

void SparsityPatternWithRowIndex::loadFada(std::istream& in)
{
  col().loadFada(in);
  rowstart().loadFada(in);
  row().loadFada(in);
}

/*-------------------------------------------------------------*/

const Alat::IntVector&  SparsityPatternWithRowIndex::row() const
{
  return _row;
}

Alat::IntVector&  SparsityPatternWithRowIndex::row()
{
  return _row;
}

Alat::IntMap& SparsityPatternWithRowIndex::rowinverse()
{
  return _rowinverse;
}

// int&  SparsityPatternWithRowIndex::row(int i)
// {
//   return _row[i];
// }

int  SparsityPatternWithRowIndex::row(int i) const
{
  return _row[i];
}

/*-------------------------------------------------------------*/

std::ostream& Alat::operator<<(std::ostream& s, const SparsityPatternWithRowIndex& A)
{
  s << "start:\n"<< A.rowstart() << std::endl;
  s << "col:\n"<< A.col() << std::endl;
  s << "row:\n"<< A.row() << std::endl;
  return s;
}
