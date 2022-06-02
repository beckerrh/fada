#ifndef  __Alat_SparsityPatternFixArray_h
#define  __Alat_SparsityPatternFixArray_h

#include  "sparsitypatternfixarraysoft.h"

/*-----------------------------------------------------------------*/

namespace Alat
{
  template<int N>
  class SparsityPatternFixArray
  {
protected:
    Alat::IntVector _rowstart;
    Alat::Vector<Alat::FixArray<N, int> >   _col;

public:
    ~SparsityPatternFixArray();
    SparsityPatternFixArray();
    SparsityPatternFixArray(const SparsityPatternFixArray& S);
    SparsityPatternFixArray& operator=(const SparsityPatternFixArray& S);

    int  n() const;
    int  ntotal() const;
    int  rowsize(int i) const;
    const Alat::Vector<Alat::FixArray<N, int> >&  col() const;
    Alat::Vector<Alat::FixArray<N, int> >&  col();
    const Alat::IntVector&  rowstart() const;
    Alat::IntVector& rowstart();
    Alat::FixArray<N, int>&  col(int pos);
    const Alat::FixArray<N, int>&  col(int pos) const;
    // int&  rowstart(int i);
    int rowstart(int i) const;
    // int&  rowstop(int i);
    int  rowstop(int i) const;
    void set_size(const SparsityPatternFixArraySoft<N>& SP);

    void write(const std::string& filename, std::string datatype = "binary") const;
    void read(const std::string& filename);
    void saveFada( std::ostream& out, std::string datatype = "binary") const;
    void loadFada(std::istream& in);
  };
  template<int N>
  std::ostream& operator<<(std::ostream& s, const SparsityPatternFixArray<N>& A);
}

#endif
