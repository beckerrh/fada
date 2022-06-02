#ifndef __Alat_SystemMatrixInterface_h
#define __Alat_SystemMatrixInterface_h

#include  "matrixinterface.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class IntVector;
  class StringSet;
}
namespace Alat
{
  class SystemVectorInterface;
  class VariableMatrixInterface;

  class SystemMatrixInterface : public MatrixInterface
  {
public:
    ~SystemMatrixInterface();
    SystemMatrixInterface();
    SystemMatrixInterface( const SystemMatrixInterface& systemmatrixinterface);
    SystemMatrixInterface& operator=( const SystemMatrixInterface& systemmatrixinterface);
    std::string getName() const;
    std::string getInterfaceName() const;

    virtual Alat::StringSet getVariables() const=0;
    virtual Alat::StringSet getVariablesOut() const=0;
    virtual Alat::StringSet getVariablesIn() const=0;
    virtual void set_size(const Alat::StringSet& variablesOut, const Alat::StringSet& variablesIn);

    virtual const Alat::VariableMatrixInterface* getMatrix(std::string varOut, std::string varIn) const;
    virtual Alat::VariableMatrixInterface* getMatrix(std::string varOut, std::string varIn);

    virtual void matrixVectorProduct(Alat::SystemVectorInterface* out, const Alat::SystemVectorInterface* in, double d = 1.0) const=0;
    virtual std::ostream& write(std::ostream& os) const = 0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
