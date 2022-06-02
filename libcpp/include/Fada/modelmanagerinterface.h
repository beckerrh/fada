#ifndef __Fada_ModelManagerInterface_h
#define __Fada_ModelManagerInterface_h

#include  "Fada/fadalightenums.h"
#include  "Alat/interfacebase.h"
#include  "Alat/set.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class IntVector;
  template<class K, class T>
  class Map;
  class IoManager;
  class ParameterFile;
}
namespace FadaMesh
{
  class MeshCompositionInterface;
}
namespace Fada
{
  class CouplingModelInterface;
  class DomainModelInterface;
  class ModelInterface;

  class ModelManagerInterface : public Alat::InterfaceBase
  {
public:
    typedef Alat::Map<std::string, Alat::IntSet> IndicesOfModelMap;

protected:
    std::string getInterfaceName() const;
    virtual Fada::DomainModelInterface* constructModelByName(const std::string& modelname) const;

public:
    ~ModelManagerInterface();
    ModelManagerInterface();
    ModelManagerInterface( const ModelManagerInterface& modelmanagerinterface);
    ModelManagerInterface& operator=( const ModelManagerInterface& modelmanagerinterface);
    virtual std::string getName() const = 0;
    virtual std::ostream& printLoopInformation(std::ostream& os) const = 0;
    virtual void basicInit(const Alat::ParameterFile* parameterfile, FadaMesh::MeshCompositionInterface* meshcomposition, FadaEnums::looptype looptype) = 0;
    virtual DomainModelInterface* getDomainModel(int i) = 0;
    virtual const DomainModelInterface* getDomainModel(int i) const = 0;
    virtual DomainModelInterface*& getDomainModelPointer(int i) = 0;
    virtual ModelInterface* getCouplingModel(int i) = 0;
    virtual const ModelInterface* getCouplingModel(int i) const = 0;
    virtual ModelInterface*& getCouplingModelPointer(int i) = 0;
    virtual int getNDomainModels() const = 0;
    virtual int getNCouplingModels() const = 0;
    virtual int getLeftDomain(int icoupling) const;
    virtual int getRightDomain(int icoupling) const;
    virtual const IndicesOfModelMap& getIndicesOfDomainModelMap() const = 0;
    virtual const IndicesOfModelMap& getIndicesOfCouplingModelMap() const = 0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
