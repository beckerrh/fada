#ifndef __Fada_Loop_h
#define __Fada_Loop_h

#include  "chronometer.h"
#include  "Alat/ghostvector.h"
#include  "Alat/iomanager.h"
#include  "loopinterface.h"

/*---------------------------------------------------------*/

namespace Fada
{
  class SolverManager;
  class TimeData;

  class Loop : public LoopInterface
  {
protected:
    Alat::IoManager _io_manager;
    const Alat::ParameterFile* _parameterfile;
    Fada::SolverManager* _solvermanager;
    FadaMesh::MeshCompositionInterface* _meshcomposition;
    mutable Chronometer _chronometer;
    std::string _vectortype;
    Alat::GhostVector _u, _data, _postprocess;
    Chronometer& getChronometer() const;

public:
    ~Loop();
    Loop();
    Loop( const Loop& loop);
    Loop& operator=( const Loop& loop);
    std::string getName() const;
    static int _debuglevel;

    void basicInit(Fada::ModelInterface* model, SolverManager* solvermanager, const std::string& rundirectory, const Alat::ParameterFile* parameterfile = NULL);
    std::ostream& printLoopInformation(std::ostream& os) const;
    void initializeSolution(Alat::GhostVector& u, TimeData& timedata);
    void initializeSolution(Alat::GhostVector& u);
    const SolverManager* getSolverManager() const;
    SolverManager* getSolverManager();
    const FadaMesh::MeshCompositionInterface* getMeshComposition() const;
    FadaMesh::MeshCompositionInterface* getMeshComposition();
    FadaMesh::MeshCompositionInterface*& getMeshCompositionPointer();
    const Alat::IoManager* getIoManager() const;
    Alat::IoManager& getIoManager();
    std::string getRunDir() const;
    const Alat::ParameterFile* getParameterFile() const;

    void setLavrentievParameter(double parameter) const;
    void computeLinearization(AlatEnums::residualstatus& status, Alat::GhostVector& y, const Alat::GhostVector& x, const Alat::GhostVector& dx) const;
    void newtonProject(Alat::GhostVector& gu) const;
  };
}

/*---------------------------------------------------------*/

#endif
