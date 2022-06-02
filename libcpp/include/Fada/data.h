#ifndef __Fada_Data_h
#define __Fada_Data_h

#include  "Alat/stringdoublemap.h"
#include  "datainterface.h"
#include  "datamap.h"

/*--------------------------------------------------------*/

namespace Fada
{
  class Data : public virtual Fada::DataInterface
  {
private:
    mutable DataMap _data;
    Alat::StringDoubleMap _parameters;

protected:
    std::string getInterfaceName() const;

public:
    ~Data();
    Data();
    Data(const Data& B);
    Data& operator=(const Data& B);

    std::string getName() const;
    const Fada::DataMap* getData() const;
    Fada::DataMap& getData();
    bool hasData(const std::string& name) const;
    const Fada::DataInterface* getData(const std::string& name) const;
    void setData(const std::string& name, const Fada::DataInterface* data);
    bool hasParameter(const std::string& name) const;
    double getParameter(const std::string& name) const;
    void setParameter(const std::string& name, double parameter);
  };
}


#endif
