#ifndef __Alat_MatrixInterface_h
#define __Alat_MatrixInterface_h

#include  "interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{        
  class DoubleVector;
  class IntVector;                  
  class SparsityPattern;
}

namespace Alat
{    
  class MatrixInterface : public Alat::InterfaceBase
  {
public:
    ~MatrixInterface();
    MatrixInterface();
    MatrixInterface( const MatrixInterface& variablematrixinterface);
    MatrixInterface& operator=( const MatrixInterface& variablematrixinterface);
    std::string getName() const;
    std::string getInterfaceName() const;
    MatrixInterface* clone() const;

    virtual void zeros() = 0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
