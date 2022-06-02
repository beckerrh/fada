#ifndef  __Alat_SparsityPatternWithRowIndex_h
#define  __Alat_SparsityPatternWithRowIndex_h

#include  "sparsitypattern.h"
#include  "sparsitypatternwithrowindexsoft.h"
#include  "sparsitypatternwithvectorofset.h"

/*-----------------------------------------------------------------*/

namespace Alat
{
  class SparsityPatternWithRowIndex : public SparsityPattern
  {
protected:
    Alat::IntVector _row;
    Alat::IntMap _rowinverse;

public:
    ~SparsityPatternWithRowIndex();
    SparsityPatternWithRowIndex();
    SparsityPatternWithRowIndex(const SparsityPatternWithRowIndex& S);
    SparsityPatternWithRowIndex& operator=(const SparsityPatternWithRowIndex& S);
    void clear();

    const Alat::IntVector&  row() const;
    Alat::IntVector&  row();
    Alat::IntMap& rowinverse();
    int rowinverse(int i) const;
    void computeRowInverse();
    // int&  row(int i);
    int  row(int i) const;

    void set_size(const SparsityPatternWithRowIndex& sparsitypattern);
    void set_size(const SparsityPatternWithRowIndexSoft& sparsitypattern);
    void set_size(const SparsityPatternWithVectorOfSet& sparsitypattern);
    void saveFada(std::ostream& out, std::string datatype = "binary") const;
    void loadFada(std::istream& in);
  };
  std::ostream& operator<<(std::ostream& s, const SparsityPatternWithRowIndex& A);
}

#endif
