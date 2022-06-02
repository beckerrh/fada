#ifndef __Fada_LinearConstraintsOneVariable_h
#define __Fada_LinearConstraintsOneVariable_h

#include  "Alat/sparsematrixwithrowindex.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class IntVector;
}
namespace Alat
{
  class SparseMatrixWithRowIndex;
}
namespace Fada
{
class LinearConstraintsOneVariable
{
protected:
  Alat::SparseMatrixWithRowIndex _smwi;
  Alat::IntVector _index;

public:
  ~LinearConstraintsOneVariable();
  LinearConstraintsOneVariable();
  LinearConstraintsOneVariable( const LinearConstraintsOneVariable& linearconstraintsonevariable);
  LinearConstraintsOneVariable& operator=( const LinearConstraintsOneVariable& linearconstraintsonevariable);
  std::string getName() const;
  LinearConstraintsOneVariable* clone() const;

  void clear();
  Alat::IntVector& getIndices();
  const Alat::SparseMatrixWithRowIndex& getConstraints() const;
  Alat::SparseMatrixWithRowIndex& getConstraints();
  const Alat::IntVector& getIndices() const;
  void read(const std::string& filename);
  void write(const std::string& filename, const std::string& type) const;
  void constructIndices(int n);
  bool isConstrained(int i) const;
  int getIndex(int i) const;
};
std::ostream& operator<<( std::ostream& s, const LinearConstraintsOneVariable& A );
}

/*--------------------------------------------------------------------------*/
#endif
