#ifndef __Alat_DataFormatHandler_h
#define __Alat_DataFormatHandler_h

#include  <map>
#include  <set>
#include  "stringvector.h"
#include  "pair.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class DataFormatHandler
  {
private:
    typedef Alat::StringPair  NameType;

    typedef std::map<std::string, std::string*>  TypeString;
    typedef std::map<std::string, int*>     TypeInt;
    typedef std::map<std::string, bool*>     TypeBool;
    typedef std::map<std::string, double*>  TypeDouble;

    std::set<NameType>  _set_of_nametype;
    TypeString _type_string;
    TypeInt _type_int;
    TypeBool _type_bool;
    TypeDouble _type_double;

    std::string search(std::string& fo, const std::string& name);

protected:

public:
    ~DataFormatHandler();
    DataFormatHandler();
    DataFormatHandler( const DataFormatHandler& dataformathandler);
    DataFormatHandler& operator=( const DataFormatHandler& dataformathandler);
    std::string getName() const
    {
      return "DataFormatHandler";
    }

    // without default values
    void insert(const std::string&, std::string*);
    void insert(const std::string&, int*);
    void insert(const std::string&, bool*);
    void insert(const std::string&, double*);

    // with default values
    void insert(const std::string&, std::string*, const std::string&);
    void insert(const std::string&, int*, int);
    void insert(const std::string&, bool*, bool);
    void insert(const std::string&, double*, double);

    void get(std::string&, const std::string&);

    void setvalue(const std::string&, const std::string&);
    void setvalue(const std::string&, int);
    void setvalue(const std::string&, bool);
    void setvalue(const std::string&, double);

    void print(std::ostream&) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
