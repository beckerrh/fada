#ifndef __Fada_DynamicPostProcessLoop_h
#define __Fada_DynamicPostProcessLoop_h

#include  "Alat/doublevector.h"
#include  "postprocessloop.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class DynamicPostProcessLoop : public PostProcessLoop
  {
private:
    int _postprocessincrement;
    // Alat::DoubleVector _output_times;
    std::vector<double> _output_times;
    Alat::GhostVector _postprocesstimeintegral, _uold;

public:
    ~DynamicPostProcessLoop();
    DynamicPostProcessLoop();
    DynamicPostProcessLoop( const DynamicPostProcessLoop& dynamicpostprocessloop);
    DynamicPostProcessLoop& operator=( const DynamicPostProcessLoop& dynamicpostprocessloop);
    std::string getName() const;
    DynamicPostProcessLoop* clone() const;

    void basicInit(ModelManagerInterface* modelmanager, SolverManagerInterface* solvermanager, const std::string& rundirectory, const Alat::ParameterFile* parameterfile = NULL);
    void run();
  };
}

/*--------------------------------------------------------------------------*/

#endif
