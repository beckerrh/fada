#include  "Integrators/main.h"
#include  "modelmanager.h"
#include  "solvermanager.h"
#include  "loop.h"


/*--------------------------------------------------------------------------*/
class Main : public Integrators::Main
{
protected:
  Fada::SolverManagerInterface* newSolverManager(std::string meshtype) const
  {
    return new SolverManager();
  }
  Fada::ModelManagerInterface* newModelManager() const
  {
    return new ModelManager();
  }
  Fada::LoopInterface * 	newLoop (std::string loopname) const
  {
    // std::cerr << "§§§§§§§§§§§§§§§§§§§§§§ newLoop: " << loopname << "\n";
    if(loopname=="prolongate" or loopname=="project" or loopname=="donothing")
    {
      return new Loop(loopname);
    }
    return Integrators::Main::newLoop(loopname);
  }
public:
  };



/*--------------------------------------------------------------------------*/
int main(int argc, char** argv)
{
  Main main;
  main.readParameterFile(argc, argv);
  main.run();
  return 0;
}
