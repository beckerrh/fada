#ifndef __Fada_IntegratorSingleVariable_h
#define __Fada_IntegratorSingleVariable_h

#include  "Fada/integrator.h"
#include  "FadaMesh/fadameshenums.h"
#include  "Alat/node.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class DirichletInterface;

  class IntegratorSingleVariable : public Fada::Integrator
  {
protected:
    bool _initdatacalled;
    int _ncompin, _ncompout, _nlocal;
    std::string _varnamein, _varnameout;
    mutable Alat::DoubleVector _udir;
    const Fada::DirichletInterface* _dirichlet;
    FadaMeshEnums::meshtype _meshtype;
    const FadaMesh::MeshInterface* _mesh;
    mutable Alat::Node _normal, _vhat, _x;

    const Fada::FemInterface* _femL, *_femR;

    mutable const Fada::FemFunction* _UL, *_UR;
    mutable const Fada::FemFunction* _UOLDL, *_UOLDR;
    mutable const Fada::FemFunction* _DUL, *_DUR;
    mutable const Fada::FemFunction* _BDFL, *_BDFR;

    mutable Alat::AssembleVector* _fL, *_fR;
    mutable Alat::AssembleMatrix* _ALL, *_ALR, *_ARL, *_ARR;

public:
    ~IntegratorSingleVariable();
    IntegratorSingleVariable();
    IntegratorSingleVariable( const IntegratorSingleVariable& integratorsinglevariable);
    IntegratorSingleVariable& operator=( const IntegratorSingleVariable& integratorsinglevariable);
    std::string getName() const;
    void basicInit(const Fada::SolverInterface* solver);
    void initData(std::string bdrycond="");
    void setFemFunctions(const Fada::FemFunctionsMap& femfctmap) const;
    void setFemFunctions(const Fada::FemFunctionsMap& femfctmapL, const Fada::FemFunctionsMap& femfctmapR) const;
    void setAssembleData(Alat::SystemAssembleMatrix& AlocLL, Alat::SystemAssembleMatrix& AlocLR, Alat::SystemAssembleMatrix& AlocRL, Alat::SystemAssembleMatrix& AlocRR) const;
    void setAssembleData(Alat::SystemAssembleVector& flocL, Alat::SystemAssembleVector& flocR) const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
