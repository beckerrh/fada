#ifndef __Fada_Solver_h
#define __Fada_Solver_h

#include  "ghostmatrixagent.h"
#include  "integratormanager.h"
#include  "solverinterface.h"
#include  "variablemanager.h"

/*---------------------------------------------------------*/

namespace Fada
{
  class IoManager;

  class Solver : public virtual SolverInterface
  {
  protected:
    const Alat::ParameterFile* _parameterfile;
    mutable const FadaMesh::MeshInterface* _mesh;
    mutable const FadaMesh::MeshInterface* _refinedmesh;
    const Alat::IoManager* _io_manager;
    double _time, _deltat, _deltatold;
    IntegratorManager _integratormanager;

    int _index;
    std::string _couplingtype;
    // GhostMatrixAgent _ghost_matrix_agent;
    FadaEnums::looptype _looptype;

    double& getTime();
    double& getDeltaT();
    double& getDeltaTOld();

public:
    ~Solver();
    Solver();
    Solver(const Solver& S);
    Solver& operator=(const Solver& S);

    IntegratorManager* getIntegratorManager();
    FadaEnums::looptype getType() const;
    int getIndex() const;
    std::string getName() const;
    int getNLevels() const;
    const Alat::IoManager* getIoManager() const;
    void setMesh(const FadaMesh::MeshInterface* mesh ) const;
    const FadaMesh::MeshInterface* getMesh() const;
    const FadaMesh::MeshInterface* getRefinedMesh() const;
    void reInitForInterpolation(const FadaMesh::MeshInterface* mesh);
    const Alat::ParameterFile* getParameterFile() const;
    void basicInit(int index, ModelInterface* model, const FadaMesh::MeshInterface* mesh, const Alat::IoManager* io_manager, FadaEnums::looptype looptype, const Alat::ParameterFile* parameterfile = NULL);
    double getTime() const;
    double getDeltaT() const;
    double getDeltaTOld() const;
    void setTimeInfo(const Fada::TimeData& timedata, double masscoefimplicit);
    void setTimeScheme(std::string time_discretization);
    void setLavrentievParameter(double trparameter) const;
    const IntegratorManager* getIntegratorManager() const;
    std::ostream& printLoopInformation(std::ostream& os) const;
    std::string getCouplingType() const;
  };
}

/*---------------------------------------------------------*/

#endif
