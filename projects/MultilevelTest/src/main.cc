#include  "Alat/filescanner.h"
#include  "Fada/main.h"
#include  "solvermanager.h"
#include  "loop.h"
#include  "convectiondiffusionreaction.h"
#include  "laplace.h"
#include  "transport.h"


/*--------------------------------------------------------------------------*/
class Main : public Fada::Main
{
protected:
   Fada::SolverManager* newSolverManager() const
   {
      return(new SolverManager());
   }

   Fada::ModelInterface * newModel() const
   {
      Alat::DataFormatHandler dataformathandler;
      std::string             modelname;
      dataformathandler.insert("model", &modelname, "none");
      Alat::FileScanner FS(dataformathandler, getParameterFile(), "ModelManager", 0);

      if (modelname == "ConvectionDiffusionReaction")
      {
         return(new ConvectionDiffusionReaction());
      }
      else if (modelname == "Laplace")
      {
         return(new Laplace());
      }
      else if (modelname == "Transport")
      {
         return(new Transport());
      }
      else
      {
         _error_string("newModel", "unknown model", modelname);
      }
   }

   Fada::LoopInterface * newLoop(std::string loopname) const
   {
      if (loopname == "prolongate" or loopname == "project" or loopname == "donothing")
      {
         return(new Loop(loopname));
      }
      return(Fada::Main::newLoop(loopname));
   }
};



/*--------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
   Main main;

   main.readParameterFile(argc, argv);
   main.run();
   return(0);
}
