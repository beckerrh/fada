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
    FadaMesh::MeshInterface* _mesh;
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
    const SolverManager* getSolverManager() const {return _solvermanager;}
    SolverManager* getSolverManager() {return _solvermanager;}
    const FadaMesh::MeshInterface* getMesh() const {return _mesh;}
    FadaMesh::MeshInterface* getMesh() {return _mesh;}
    FadaMesh::MeshInterface*& getMeshPointer() {return _mesh;}
    const Alat::IoManager* getIoManager() const{return &_io_manager;}
    Alat::IoManager& getIoManager(){return _io_manager;}
    std::string getRunDir() const;
    const Alat::ParameterFile* getParameterFile() const {return _parameterfile;}

    void setLavrentievParameter(double parameter) const;
    void computeLinearization(AlatEnums::residualstatus& status, Alat::GhostVector& y, const Alat::GhostVector& x, const Alat::GhostVector& dx) const;
    void newtonProject(Alat::GhostVector& gu) const;
  };
}

/*---------------------------------------------------------*/

#endif
