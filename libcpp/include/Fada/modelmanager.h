#ifndef __Fada_ModelManager_h
#define __Fada_ModelManager_h

#include  "Alat/map.h"
#include  "Alat/vector.h"
#include  "modelmanagerinterface.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class ModelInterface;

  class ModelManager : public ModelManagerInterface
  {
public:
    typedef Alat::Vector<Fada::DomainModelInterface*> DomainModelVector;
    typedef Alat::Vector<Fada::ModelInterface*> CouplingModelVector;

protected:
    const Alat::ParameterFile* _parameterfile;
    DomainModelVector _domainmodels;
    CouplingModelVector _couplingmodels;
    Alat::Map<int, std::string > _modelmap;
    // const Fada::DomainModelInterface* _singlemodel;
    Alat::Map<std::string, std::pair<int, int> > _specialcouplings;
    Alat::Vector<std::pair<int, int> > _couplingorientation;
    // bool _issingle;
    IndicesOfModelMap _indicesofmodelmapdomain, _indicesofmodelmapcoupling;

    void _constructModelMap(const Alat::Vector<std::string>& modeltype_desc, const FadaMesh::MeshCompositionInterface* meshcomposition);
    void _constructSpecialCouplingMap(const Alat::Vector<std::string>& specialcouplingtype_desc);
    void _constructCouplingModel(int icoupling, int iblockl, int iblockr);
    void _constructModels(const Alat::ParameterFile* parameterfile, FadaMesh::MeshCompositionInterface* meshcomposition, FadaEnums::looptype looptype);
    void reInitCouplingModelVectors(int n);
    void reInitModelVector(int n);

public:
    ~ModelManager();
    ModelManager();
    ModelManager( const ModelManager& modelmanager);
    ModelManager& operator=( const ModelManager& modelmanager);
    std::string getName() const;
    std::ostream& printLoopInformation(std::ostream& os) const;
    void basicInit(const Alat::ParameterFile* parameterfile, FadaMesh::MeshCompositionInterface* meshcomposition, FadaEnums::looptype looptype);

    const IndicesOfModelMap& getIndicesOfDomainModelMap() const;
    const IndicesOfModelMap& getIndicesOfCouplingModelMap() const;
    int getNDomainModels() const;
    DomainModelInterface* getDomainModel(int i);
    const DomainModelInterface* getDomainModel(int i) const;
    DomainModelInterface*& getDomainModelPointer(int i);
    ModelInterface* getCouplingModel(int i);
    const ModelInterface* getCouplingModel(int i) const;
    int getNCouplingModels() const;
    ModelInterface*& getCouplingModelPointer(int i);
    int getLeftDomain(int icoupling) const;
    int getRightDomain(int icoupling) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
