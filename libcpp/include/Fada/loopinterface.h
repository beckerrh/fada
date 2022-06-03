#ifndef __Fada_LoopInterface_h
#define __Fada_LoopInterface_h

#include  "Alat/interfacebase.h"
#include  "fadalightenums.h"
#include  "Alat/enums.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{ 
  class GhostMatrix;
  class GhostVector;
  class GhostLinearSolver;
  class IoManager;
  class ParameterFile;
}
namespace Alat
{ 
  class NewtonData;
  class NonLinearInfo;
}
namespace FadaMesh
{ 
  class MeshCompositionInterface;
}
namespace Fada
{ 
  class ModelInterface;
  class SolverManager;

  class LoopInterface : public Alat::InterfaceBase
  {
protected:
    std::string getInterfaceName() const;

public:
    ~LoopInterface();
    LoopInterface();
    LoopInterface( const LoopInterface& loop);
    LoopInterface& operator=( const LoopInterface& loop);

    virtual void basicInit(Fada::ModelInterface* model, SolverManager* solvermanager, const std::string& rundirectory, const Alat::ParameterFile* parameterfile = NULL);
    virtual FadaEnums::looptype getType() const = 0;
    virtual std::string getRunDir() const = 0;
    virtual SolverManager* getSolverManager() = 0;
    virtual const SolverManager* getSolverManager() const = 0;
    virtual FadaMesh::MeshCompositionInterface*& getMeshCompositionPointer() = 0;
    virtual const FadaMesh::MeshCompositionInterface* getMeshComposition() const = 0;
    virtual FadaMesh::MeshCompositionInterface* getMeshComposition() = 0;
    virtual const Alat::IoManager* getIoManager() const=0;
    virtual FadaMesh::MeshCompositionInterface*& getRefinedMeshCompositionPointer();
    virtual const FadaMesh::MeshCompositionInterface* getRefinedMeshComposition() const;
    virtual void computeLinearization(AlatEnums::residualstatus& status, Alat::GhostVector& y, const Alat::GhostVector& x, const Alat::GhostVector& dx) const=0;
    virtual void constructMatrixAndLinearSolvers(Alat::GhostMatrix& A,  Alat::GhostVector& u);
    virtual void constructJacobianMatrix(AlatEnums::residualstatus& status, Alat::GhostMatrix& A,  Alat::GhostVector& u);
    virtual void constructLinearSolver(Alat::GhostMatrix& A, const Alat::GhostVector& u);
    virtual void linearSolve(AlatEnums::iterationstatus& status, Alat::GhostMatrix& A, Alat::GhostVector& x, const Alat::GhostVector& b);
    virtual void residual(AlatEnums::residualstatus& status, Alat::GhostVector& y, const Alat::GhostVector& x) const;
    virtual void run() = 0;
    virtual void nonlinearSolve(Alat::NewtonData& newtondata, Alat::GhostVector& u, const Alat::GhostVector& f, int& nliter, int& liniter);
    virtual const Alat::ParameterFile* getParameterFile() const = 0;
    virtual void setLavrentievParameter(double parameter) const = 0;
    virtual void newtonProject(Alat::GhostVector& gu) const=0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
