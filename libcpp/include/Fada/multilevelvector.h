#ifndef __Fada_MultiLevelVector_h
#define __Fada_MultiLevelVector_h

#include  "Alat/vector.h"
#include  "Alat/vectorinterface.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class SystemVectorInterface;
  class VariableVectorInterface;
}
namespace Fada
{
  class MultiLevelVector : public Alat::Vector<Alat::SystemVectorInterface*>, public Alat::VectorInterface
  {
public:
    ~MultiLevelVector();
    MultiLevelVector();
    MultiLevelVector( const MultiLevelVector& postprocessvector);
    MultiLevelVector& operator=( const MultiLevelVector& postprocessvector);
    std::string getName() const;
    MultiLevelVector* clone() const;

    const Alat::SystemVectorInterface* getVector(int level) const;
   Alat::SystemVectorInterface* getVector(int level);
    Alat::SystemVectorInterface*& getVectorPointer(int level);

    int nlevels() const;
    void set_size(int nlevels);
    void zeros();
    double norm() const;
    double norm(int level) const;
    double scalarProduct(const MultiLevelVector* v) const;
    void equal(const MultiLevelVector* v);
    void equal(double d);
    void add(const double& d, const MultiLevelVector* v);
    void scale(const double& d);

    std::ostream& writeAscii(std::ostream& os) const;
  };
  std::ostream& operator<<(std::ostream& s, const MultiLevelVector& v);
}

/*--------------------------------------------------------------------------*/

#endif
