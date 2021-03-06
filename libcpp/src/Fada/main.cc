#include  "Alat/directoryandfileexists.h"
#include  "Fada/dynamicloop.h"
#include  "Fada/dynamicpostprocessloop.h"
#include  "Alat/filescanner.h"
#include  "Fada/interpolationloop.h"
#include  "Fada/linearloop.h"
#include  "FadaMesh/meshselector.h"
#include  "Fada/modelinterface.h"
#include  "Fada/modelinterface.h"
#include  "Alat/parameterfile.h"
#include  "Fada/postprocessloop.h"
#include  "Fada/staticloop.h"
#include  "Fada/main.h"
#include  "FadaMesh/getmeshtype.h"
#include  "Alat/tokenize.h"
#include  "Fada/solvermanager.h"
#include  "stdlib.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
Main::~Main()
{
   if (_parameterfile)
   {
      delete _parameterfile;
      _parameterfile = NULL;
   }
}

Main::Main() : Fada::MainInterface(), _parameterfile(NULL)
{
}

Main::Main(const Main& main) : Fada::MainInterface(main)
{
   assert(0);
}

Main& Main::operator=(const Main& main)
{
   Fada::MainInterface::operator=(main);
   assert(0);
   return(*this);
}

std::string Main::getName() const
{
   return("Main");
}

Main * Main::clone() const
{
   return(new Main(*this));
}

/*--------------------------------------------------------------------------*/
const Alat::ParameterFile * Main::getParameterFile() const
{
   return(_parameterfile);
}

std::string Main::getLoopName() const
{
   return(_loopname);
}

/*--------------------------------------------------------------------------*/
void Main::readParameterFile(int argc, char **argv)
{
   if (argc == 3)
   {
      _loopname     = argv[1];
      _rundirectory = argv[2];
   }
   else
   {
      std::cerr << "***ERROR in Main::basicInit() expected arguments: <looptype> <rundirectory>\n";
      assert(0);
   }
   if (!(Alat::_DirectoryExists(_rundirectory)))
   {
      std::cerr << "***ERROR in Main::basicInit() \"" << _rundirectory << "\" : rundirectory does not exist\n";
      assert(0);
      exit(1);
   }
   Alat::StringVector bouts = Alat::Tokenize(argv[0], "/");
   std::string        paramfilename(_rundirectory + "/" + bouts[bouts.size() - 1] + ".fadalightparam");

   _parameterfile = new Alat::ParameterFile(paramfilename);

   {
      Alat::DataFormatHandler dataformathandler;
      dataformathandler.insert("meshname", &_meshname, "none");
      dataformathandler.insert("meshtype", &_meshtype, "none");
      dataformathandler.insert("refinedmeshname", &_refinedmeshname, "none");
      Alat::FileScanner filescanner(dataformathandler, _parameterfile, "Mesh");
   }
}

/*--------------------------------------------------------------------------*/
Fada::SolverManager * Main::newSolverManager() const
{
   return(new Fada::SolverManager());
}

/*--------------------------------------------------------------------------*/
Fada::LoopInterface * Main::newLoop(std::string loopname) const
{
   FadaEnums::looptype looptype = FadaEnums::stringToLoopType(_loopname);

   if (looptype == FadaEnums::DynamicLoop)
   {
      return(new Fada::DynamicLoop());
   }
   else if (looptype == FadaEnums::DynamicPostProcessLoop)
   {
      return(new Fada::DynamicPostProcessLoop());
   }
   else if (looptype == FadaEnums::InterpolationLoop)
   {
      return(new Fada::InterpolationLoop());
   }
   else if (looptype == FadaEnums::LinearLoop)
   {
      return(new Fada::LinearLoop());
   }
   else if (looptype == FadaEnums::PostProcessLoop)
   {
      return(new Fada::PostProcessLoop());
   }
   else if (looptype == FadaEnums::StaticLoop)
   {
      return(new Fada::StaticLoop());
   }
   else
   {
      _error_string("newLoop", "unknown loop ", _loopname);
   }
}

/*--------------------------------------------------------------------------*/
Fada::ModelInterface * Main::newModel() const
{
   _notWritten("newModel");
}

/*--------------------------------------------------------------------------*/
void Main::run()
{
   Fada::SolverManager *solvermanager = newSolverManager();
   assert(solvermanager);

   Fada::LoopInterface *loop = newLoop(_loopname);
   assert(loop);

   FadaMeshEnums::meshtype meshtype = FadaMeshEnums::stringToMeshType(_meshtype);
   FadaMesh::MeshSelector meshselector;
   loop->getMeshPointer() = meshselector.newMesh(meshtype, _meshname);
   if (loop->getType() == FadaEnums::InterpolationLoop)
   {
      loop->getRefinedMeshPointer() = meshselector.newMesh(meshtype, _refinedmeshname);
      assert(loop->getRefinedMeshPointer());
   }

   Fada::ModelInterface *model = newModel();
   model->basicInit(_parameterfile, loop->getMesh(), loop->getType());

   loop->basicInit(model, solvermanager, _rundirectory, _parameterfile);
   loop->run();
   if (loop)
   {
      delete loop;
      loop = NULL;
   }
   if (model)
   {
      delete model;
      model = NULL;
   }
}
