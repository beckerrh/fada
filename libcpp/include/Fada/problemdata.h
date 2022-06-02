#ifndef __Fada_ProblemData_h
#define __Fada_ProblemData_h

#include  "dirichletinterface.h"
#include  "initialconditioninterface.h"
#include  "Alat/map.h"
#include  "neumanninterface.h"
#include  "righthandsideinterface.h"

/*--------------------------------------------------------*/

namespace Fada
{
  class ProblemData
  {
public:
    typedef Alat::Map<std::string, Fada::DataInterface*> DataMap;
    typedef Alat::Map<std::string, DataMap> MapDataMap;

private:
    MapDataMap _data;

public:
    ~ProblemData();
    ProblemData();
    ProblemData(const ProblemData& problemdata);
    ProblemData& operator=(const ProblemData& problemdata);

    std::string getName() const;
    MapDataMap::const_iterator begin() const;
    MapDataMap::const_iterator end() const;
    void clear();
    void basicInit();
    std::ostream& print(std::ostream& os) const;
    void setData(const std::string& varname, const std::string& dataname, const Fada::DataInterface* data);

    Fada::DataInterface*& setData(const std::string& varname, const std::string& dataname);
    const Fada::DataInterface* getData(const std::string& varname, const std::string& dataname ) const;
    Fada::DataInterface* getData(const std::string& varname, const std::string& dataname );
    bool hasKey(const std::string& varname, const std::string& dataname ) const;

    ///Replace a given ProblemData (problem_data) by (*this) ProblemData
    void replace(ProblemData& problem_data) const;
    ///Replace  the data of (*this) ProblemData by zero data
    void replaceByZeroData();
  };
  std::ostream& operator<<( std::ostream& os, const ProblemData& problemdata );
}


#endif
