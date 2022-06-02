#ifndef __Fada_DataMap_h
#define __Fada_DataMap_h

#include  "datainterface.h"
#include  "Alat/map.h"
#include  <string>

/*---------------------------------------------------------*/

namespace Fada
{
  class DataMap : public Alat::Map<std::string, const DataInterface*>
  {
public:
    ~DataMap();
    DataMap();
    DataMap(const DataMap& D);
    DataMap& operator=(const DataMap& D);

    std::string getName() const;
    void setData(const std::string& name, const Fada::DataInterface* data);
    const Fada::DataInterface* getData(const std::string& name) const;
  };
}

/*---------------------------------------------------------*/

#endif
