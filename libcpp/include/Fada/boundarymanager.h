#ifndef __Fada_BoundaryManager_h
#define __Fada_BoundaryManager_h

#include  "Alat/interfacebase.h"
#include  "Alat/map.h"
#include  "Alat/stringset.h"
#include  "Alat/stringvector.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class ParameterFile;
}
namespace FadaMesh
{
  class BoundaryInfo;
}
namespace Fada
{
  class ApplicationInterface;
  class ModelInterface;
  class VariablesMap;
  class BoundaryManager : Alat::InterfaceBase
  {
public:
    typedef Alat::Map<std::string, Alat::StringSet> BoundaryConditionToVariables;

protected:
    static std::string _none;
    static Alat::StringSet _emptyset;
    int _numberofboundaryconditions;
    bool _init;
    Alat::Map<std::string, Alat::StringSet> _strongconds, _strongnormalconds;
    Alat::Map<int, Alat::StringSet> _bdrycondofcolor;
    Alat::Map<std::string, Alat::StringSet> _varsofbdrycond;

public:
    ~BoundaryManager();
    BoundaryManager();
    BoundaryManager(const BoundaryManager& boundarymanager);
    BoundaryManager& operator=(const BoundaryManager& boundarymanager);

    void basicInit(const Alat::ParameterFile* parameterfile, const Fada::ModelInterface* model);
    std::ostream& writeBoundaryConditions(std::ostream& os) const;
    std::string getName() const;
    std::string getInterfaceName() const;
    void setData(const BoundaryConditionToVariables& boundaryconditiontovariables, const Fada::ApplicationInterface* application);
    void checkBoundaryConditions(const Fada::VariablesMap& unknowns, const FadaMesh::BoundaryInfo* boundaryinfo) const;
    const Alat::StringSet& getBoundaryConditionsOfColor(int color) const;
    const Alat::StringSet& getVariablesOfBoundaryCondition(std::string bdrycond) const;
    bool boundaryConditionIs(std::string bdrycond, int color) const;
    bool boundaryConditionIsStrong(std::string varname, int color) const;
    bool boundaryConditionIsStrongNormal(std::string varname, int color) const;
    const std::string& boundaryCondition(std::string varname, int color) const;
    const Alat::StringSet& boundaryConditions(int color) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
