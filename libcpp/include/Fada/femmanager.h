#ifndef __Fada_FemManager_h
#define __Fada_FemManager_h

#include  "Alat/node.h"
#include  "Alat/doublevector.h"
#include  "Alat/intvector.h"
#include  "Alat/map.h"
#include  "Alat/stringvector.h"
#include  "femmanagerinterface.h"
#include  "femfunctions.h"

/*--------------------------------------------------------------------------*/
namespace Fada
{
  class FemInterface;
  class LocalVectors;
  class VariableInterface;
  class VariableManager;

  class FemManager : public FemManagerInterface
  {
private:
    int _ivarfornormal;
    Alat::StringIntMap _varnametofemindex;
    mutable Alat::Vector<FemInterface*> _fems;
    const Fada::VariableManager* _variablemanager;

    void _pointFemFunctions(Fada::FemFunctionsMap& femfctmap, const Fada::LocalVectors& localvectors) const;

public:
    ~FemManager();
    FemManager();
    FemManager( const FemManager& femmanager);
    FemManager& operator=( const FemManager& femmanager);
    FemManager* clone() const;
    std::string getName() const;

    // const FemInterface* getFem(std::string ivar) const;
    FemInterface* getFem(std::string ivar) const;

    void basicInit(const ProblemData* problemdata, const VariableManager* variablemanager, const FadaMesh::MeshInterface* mesh);
    Alat::Vector<FemInterface*>& getFems() const;
    void initCell(const Alat::Vector<Alat::Node>& F, bool curved) const;
    void initCellAndSide(const Alat::Vector<Alat::Node>& F, int il, bool curved) const;
    const Alat::StringIntMap& getVarname2Index() const;
    // const Alat::StringVector& getIndex2Varname() const;
    // const std::string& getIndex2Varname(int index) const;

    const Alat::Node& x() const;
    double reInitReferencePoint(const Alat::Node& xhat) const;
    double reInitReferencePoint(Fada::FemFunctionsMap& femfctmap, const Alat::Node& xhat, const Fada::LocalVectors& localvectors) const;
    void reInitPhysicalPoint(Fada::FemFunctionsMap& femfctmap, const Alat::Node& xphys, const Fada::LocalVectors& localvectors) const;
    double reInitCenterPoint(Fada::FemFunctionsMap& femfctmap, const Fada::LocalVectors& localvectors) const;
    double reInitReferencePointBoundary(Fada::FemFunctionsMap& femfctmap, const Alat::Node& xhat, const Fada::LocalVectors& localvectors) const;
    Fada::FemInterface* newFem(std::string femname);
    Fada::TransformationInterface* newTransformation(const FadaMesh::MeshInterface* mesh, const Fada::FemInterface* fem);
    Fada::TransformationInterface* newTransformationCurved(const FadaMesh::MeshInterface* mesh, const Fada::FemInterface* fem);
  };
  std::ostream& operator<<(std::ostream& os, const FemManager& g);
}

/*--------------------------------------------------------------------------*/

#endif
