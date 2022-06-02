#ifndef __Alat_SystemVectorInterface_h
#define __Alat_SystemVectorInterface_h

#include  "vectorinterface.h"
#include  "map.h"

/*--------------------------------------------------------------------------*/
namespace Alat
{
  class DoubleVector;
  class IntVector;
  class StringDoubleVectorMap;
  class StringSet;
  class StringVector;
  // template<class T, class S>
  // class Map;
}

namespace Alat
{
  class AssembleVector;
  class VariableVectorInterface;

  class SystemVectorInterface : public virtual VectorInterface
  {
protected:
    std::string getInterfaceName() const;

public:
    ~SystemVectorInterface();
    SystemVectorInterface();
    SystemVectorInterface( const Alat::SystemVectorInterface& vectorinterface);
    SystemVectorInterface& operator=( const Alat::SystemVectorInterface& vectorinterface);
    virtual SystemVectorInterface* clone() const;

    virtual bool hasVector(std::string name) const = 0;
    virtual Alat::StringSet getVariables() const = 0;
    virtual const Alat::VariableVectorInterface* getVector(std::string name) const = 0;
    virtual Alat::VariableVectorInterface* getVector(std::string name) = 0;

    virtual void scalePerVariables(const Alat::StringDoubleVectorMap&) = 0;
    virtual void scalePerVariablesInverse(const Alat::StringDoubleVectorMap& scales)=0;
    virtual std::ostream& writeAscii(std::ostream& os) const = 0;
    virtual void savehdf5(const std::string& filename, const Alat::Map<std::string, Alat::StringVector>& blocknames) const =0;
    virtual void loadhdf5(const std::string& filename, const Alat::Map<std::string, Alat::StringVector>& blocknames)=0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
