#ifndef __Fada_InterpolationLoop_h
#define __Fada_InterpolationLoop_h

#include  "loop.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class InterpolationLoop : public Loop
  {
private:
    FadaMesh::MeshCompositionInterface* _refinedmeshcomposition;
    Alat::GhostVector _uold, _uhelp;

public:
    ~InterpolationLoop();
    InterpolationLoop();
    InterpolationLoop( const InterpolationLoop& interpolationloop);
    InterpolationLoop& operator=( const InterpolationLoop& interpolationloop);
    std::string getName() const;
    InterpolationLoop* clone() const;
    FadaEnums::looptype getType() const;

    void basicInit(ModelManagerInterface* modelmanager, SolverManagerInterface* solvermanager, const std::string& rundirectory, const Alat::ParameterFile* parameterfile = NULL);
    void run();
    FadaMesh::MeshCompositionInterface*& getRefinedMeshCompositionPointer();
    const FadaMesh::MeshCompositionInterface* getRefinedMeshComposition() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
