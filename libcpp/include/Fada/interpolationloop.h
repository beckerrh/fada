#ifndef __Fada_InterpolationLoop_h
#define __Fada_InterpolationLoop_h

#include  "loop.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class InterpolationLoop : public Loop
  {
private:
    FadaMesh::MeshInterface* _refinedmesh;
    Alat::GhostVector _uold, _uhelp;

public:
    ~InterpolationLoop();
    InterpolationLoop();
    InterpolationLoop( const InterpolationLoop& interpolationloop);
    InterpolationLoop& operator=( const InterpolationLoop& interpolationloop);
    std::string getName() const;
    InterpolationLoop* clone() const;
    FadaEnums::looptype getType() const;

    void basicInit(Fada::ModelInterface* model, SolverManager* solvermanager, const std::string& rundirectory, const Alat::ParameterFile* parameterfile = NULL);
    void run();
    FadaMesh::MeshInterface*& getRefinedMeshPointer() {return _refinedmesh;}
    const FadaMesh::MeshInterface* getRefinedMeshComposition() const{return _refinedmesh;}
  };
}

/*--------------------------------------------------------------------------*/

#endif
