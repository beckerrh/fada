#ifndef __Fada_VariableInterface_h
#define __Fada_VariableInterface_h

#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class StringVector;
}
namespace Fada
{
  class FemInterface;

  class VariableInterface : public virtual Alat::InterfaceBase
  {
private:
protected:
    std::string getInterfaceName() const;

public:
    ~VariableInterface();
    VariableInterface();
    VariableInterface( const VariableInterface& variableinterface);
    VariableInterface& operator=( const VariableInterface& variableinterface);
    virtual std::ostream& write(std::ostream& os) const = 0;

    virtual std::string getVarName() const = 0;
    virtual int getNComponents() const = 0;
    virtual int getN() const;

    virtual std::string getFemName() const = 0;
    virtual std::string getVisualizationType() const = 0;
    virtual std::string getPostProcType(int icomp) const = 0;
    virtual void getVisualizationNames(Alat::StringVector& visunames) const = 0;
    virtual void setVisualizationNames(Alat::StringVector& visunames) const = 0;
    virtual bool timeIntegral() const = 0;
    virtual void setVisualizationName(int icomp, const std::string& name)=0;
    virtual const std::string&  getVisualizationName(int icomp) const=0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
