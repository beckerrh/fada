#ifndef __Fada_FemManagerInterface_h
#define __Fada_FemManagerInterface_h

#include  "Alat/interfacebase.h"
#include  "localglobal.h"
#include  "integrationloopinterface.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  template <class T, class S>
  class Map;
  template <class T>
  class Vector;
  class StringVector;
  class Node;
}
namespace FadaMesh
{
  class MeshInterface;
}
namespace Fada
{
  class FemFunctionsMap;
  class FemInterface;
  class LocalVectors;
  class VariableManager;
  class VariableInterface;
  class TransformationInterface;
  class ProblemData;

  class FemManagerInterface : public Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~FemManagerInterface();
    FemManagerInterface();
    FemManagerInterface( const FemManagerInterface& femmanagerinterface);
    FemManagerInterface& operator=( const FemManagerInterface& femmanagerinterface);
    std::string getName() const;
    virtual FemManagerInterface* clone() const;

    virtual void basicInit(const ProblemData* problemdata, const VariableManager* variablemanager, const FadaMesh::MeshInterface* mesh) = 0;

    virtual FemInterface* newFem(std::string femname) = 0;
    virtual TransformationInterface* newTransformation(const FadaMesh::MeshInterface* mesh, const FemInterface* fem) = 0;
    virtual TransformationInterface* newTransformationCurved(const FadaMesh::MeshInterface* mesh, const FemInterface* fem) = 0;

    virtual FemInterface* getFem(std::string varname) const=0;
    // virtual const FemInterface* getFem(std::string varname) const=0;
    // virtual FemInterface* getFem(std::string varname) = 0;
    virtual Alat::Vector<FemInterface*>& getFems() const = 0;
    virtual void initCell(const Alat::Vector<Alat::Node>& F, bool curved) const = 0;
    virtual void initCellAndSide(const Alat::Vector<Alat::Node>& F, int il, bool curved) const = 0;
    virtual const Alat::StringIntMap& getVarname2Index() const = 0;
    virtual const Alat::Node& x() const = 0;
    virtual double reInitReferencePoint(const Alat::Node& xhat) const = 0;
    virtual double reInitReferencePoint(Fada::FemFunctionsMap& femfctmap, const Alat::Node& xhat, const Fada::LocalVectors& localvectors) const = 0;
    virtual void reInitPhysicalPoint(Fada::FemFunctionsMap& femfctmap, const Alat::Node& xphys, const Fada::LocalVectors& localvectors) const = 0;
    virtual double reInitReferencePointBoundary(Fada::FemFunctionsMap& femfctmap, const Alat::Node& xhat, const Fada::LocalVectors& localvectors) const = 0;
    virtual double reInitCenterPoint(Fada::FemFunctionsMap& femfctmap, const Fada::LocalVectors& localvectors) const=0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
