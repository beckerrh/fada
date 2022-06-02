#ifndef __Fada_MultiLevelMatrix_h
#define __Fada_MultiLevelMatrix_h

#include  "Alat/vector.h"
#include  "Alat/matrixinterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class SystemMatrixInterface;
  class SystemVectorInterface;
}
namespace Fada
{
  class MultiLevelVector;
  
  class MultiLevelMatrix : public Alat::Vector<Alat::SystemMatrixInterface*>, public Alat::MatrixInterface
  {
public:
    ~MultiLevelMatrix();
    MultiLevelMatrix();
    MultiLevelMatrix( const MultiLevelMatrix& multilevelmatrix);
    MultiLevelMatrix& operator=( const MultiLevelMatrix& multilevelmatrix);
    std::string getName() const;
    MultiLevelMatrix* clone() const;

    void set_size(int nlevels);
    Alat::SystemMatrixInterface*& getMatrixPointer(int level);
    Alat::SystemMatrixInterface* getMatrix(int level);
    const Alat::SystemMatrixInterface* getMatrix(int level) const;
    int nlevels() const;
    void zeros();
    std::ostream& write(std::ostream& os) const;
    void matrixVectorProduct(int level, Alat::SystemVectorInterface* f, const Alat::SystemVectorInterface* u, double d = 1.0) const;
    void matrixVectorProduct(Fada::MultiLevelVector* f, const Fada::MultiLevelVector* u, double d = 1.0) const;
  };
  std::ostream& operator<<(std::ostream& s, const MultiLevelMatrix& A);
}

/*--------------------------------------------------------------------------*/

#endif
