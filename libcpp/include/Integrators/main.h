#ifndef __Integrators_Main_h
#define __Integrators_Main_h

#include  "Fada/maininterface.h"
#include  "Fada/fadalightenums.h"

/*--------------------------------------------------------------------------*/

namespace Alat
{
  class ParameterFile;
}
namespace Fada
{
  class DomainModelInterface;
  class LoopInterface;
}
namespace Integrators
{
  class Main : public Fada::MainInterface
  {
protected:
    std::string _meshname, _meshtype, _refinedmeshname, _rundirectory, _loopname;
    Alat::ParameterFile* _parameterfile;

    Fada::SolverManager* newSolverManager(std::string meshtype) const;
    Fada::LoopInterface* newLoop(std::string loopname) const;
    Fada::ModelManagerInterface* newModelManager() const;

public:
    ~Main();
    Main();
    Main( const Main& main);
    Main& operator=( const Main& main);
    std::string getName() const;
    Main* clone() const;
    void readParameterFile(int argc, char** argv);
    void run();
    std::string getMeshType() const;
    const Alat::ParameterFile* getParameterFile() const;
    std::string getLoopName() const;
  };
}

/*--------------------------------------------------------------------------*/

#endif
