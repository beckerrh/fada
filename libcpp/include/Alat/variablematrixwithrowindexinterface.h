#ifndef __Alat_VariableMatrixWithRowIndexInterface_h
#define __Alat_VariableMatrixWithRowIndexInterface_h

#include  "variablematrixinterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class SparsityPatternWithRowIndex;
}
namespace Alat
{
  class VariableMatrixWithRowIndexInterface : public Alat::VariableMatrixInterface
  {
public:
    ~VariableMatrixWithRowIndexInterface();
    VariableMatrixWithRowIndexInterface();
    VariableMatrixWithRowIndexInterface( const VariableMatrixWithRowIndexInterface& sparsematrixwithrowindexinterface);
    VariableMatrixWithRowIndexInterface& operator=( const VariableMatrixWithRowIndexInterface& sparsematrixwithrowindexinterface);
    std::string getName() const;
    VariableMatrixWithRowIndexInterface* clone() const;

    virtual const Alat::SparsityPatternWithRowIndex* getSparsityPatternWithRowIndex() const=0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
