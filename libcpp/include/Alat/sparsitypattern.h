#ifndef  __Alat_SparsityPattern_h
#define  __Alat_SparsityPattern_h

#include  "intvector.h"
#include  "sparsitypatternsoft.h"

/*-----------------------------------------------------------------*/

namespace Alat
{
  class SparsityPattern
  {
protected:
    Alat::IntVector _col, _rowstart;

public:
    ~SparsityPattern();
    SparsityPattern();
    SparsityPattern(const SparsityPattern& S);
    SparsityPattern& operator=(const SparsityPattern& S);

    void clear();
    int n() const;
    int ntotal() const;
    int rowsize(int i) const;
    const Alat::IntVector&  col() const;
    Alat::IntVector&  col();
    const Alat::IntVector&  rowstart() const;
    Alat::IntVector& rowstart();
    int&  col(int pos);
    int col(int pos) const;
    int& rowstart(int i);
    int rowstart(int i) const;
    // int&  rowstop(int i);
    int  rowstop(int i) const;
    int get(int i, int ii) const;
    
    // const int* begin(int i) const;
    // const int* end(int i) const;

    void set_size(int n, int nt);
    void set_size(const SparsityPattern& SP);
    void set_size(const SparsityPatternSoft& SP);
    void set_size(const Alat::Vector<Alat::IntVector>& SP);

    int find(int i, int j) const;
    void sort();
    void print( std::ostream& os) const;
    void saveFada(std::ostream& out, std::string datatype = "binary") const;
    void loadFada(std::istream& in);
    void write(const std::string& filename, std::string datatype = "binary") const;
    void read(const std::string& filename);
    void enlarge(int enlarge_stencil);
    void reconstructWithNumbering(const Alat::SparsityPattern* sparsitypattern, const Alat::IntVector& p, const Alat::IntVector& pinv,  int enlarge_stencil = 0);
    void setDiagonal(Alat::IntVector& diagonal) const;    
  };
  std::ostream& operator<<(std::ostream& s, const SparsityPattern& A);
}

#endif
