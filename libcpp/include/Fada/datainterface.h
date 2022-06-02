#ifndef __Fada_DataInterface_h
#define __Fada_DataInterface_h

#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class Node;
}
namespace FadaMesh
{
class MeshInterface;
}
namespace Fada
{
  class DataMap;

  class DataInterface : public Alat::InterfaceBase
  {
private:
protected:
public:
    ~DataInterface();
    DataInterface();
    DataInterface( const DataInterface& datainterface);
    DataInterface& operator=( const DataInterface& datainterface);

    virtual const Fada::DataInterface* getData(const std::string& name) const;
    virtual const Fada::DataMap* getData() const;
    virtual void basicInit();
    virtual double getParameter(const std::string& name) const;
    virtual void setParameter(const std::string& name, double parameter);
    virtual void setData(const std::string& name, const Fada::DataInterface* data);
    virtual void getValue(Alat::Node& betanode, const Alat::Node& v) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
