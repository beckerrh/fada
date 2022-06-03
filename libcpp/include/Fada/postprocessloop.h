#ifndef __Fada_PostProcessLoop_h
#define __Fada_PostProcessLoop_h

#include  "loop.h"

/*--------------------------------------------------------------------------*/

namespace Fada
{
  class PostProcessLoop : public Loop
  {
protected:
    // Alat::GhostVector _postprocess;

public:
    ~PostProcessLoop();
    PostProcessLoop();
    PostProcessLoop( const PostProcessLoop& postprocessloop);
    PostProcessLoop& operator=( const PostProcessLoop& postprocessloop);
    std::string getName() const;
    PostProcessLoop* clone() const;
    FadaEnums::looptype getType() const;

    void basicInit(Fada::ModelInterface* model, SolverManager* solvermanager, const std::string& rundirectory, const Alat::ParameterFile* parameterfile = NULL);
    void run();
  };
}

/*--------------------------------------------------------------------------*/

#endif
