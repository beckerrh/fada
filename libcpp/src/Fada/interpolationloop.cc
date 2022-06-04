#include  "Fada/interpolationloop.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/solvermanager.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
InterpolationLoop::~InterpolationLoop()
{
}

InterpolationLoop::InterpolationLoop() : Loop(), _refinedmesh(NULL), _uold("uold", "unknowns"), _uhelp("uhelp", "unknowns")
{
}

InterpolationLoop::InterpolationLoop(const InterpolationLoop& interpolationloop) : Loop(interpolationloop), _refinedmesh(interpolationloop._refinedmesh)
{
   assert(0);
}

InterpolationLoop& InterpolationLoop::operator=(const InterpolationLoop& interpolationloop)
{
   Loop::operator=(interpolationloop);

   assert(0);
   return(*this);
}

std::string InterpolationLoop::getName() const
{
   return("InterpolationLoop");
}

FadaEnums::looptype InterpolationLoop::getType() const
{
   return(FadaEnums::InterpolationLoop);
}

InterpolationLoop * InterpolationLoop::clone() const
{
   return(new InterpolationLoop(*this));
}

/*--------------------------------------------------------------------------*/
void InterpolationLoop::basicInit(Fada::ModelInterface *model, SolverManager *solvermanager, const std::string& rundirectory, const Alat::ParameterFile *parameterfile)
{
   // std::cerr << "InterpolationLoop::basicInit() DEBUT\n";
   Loop::basicInit(model, solvermanager, rundirectory, parameterfile);

   if (getMesh() == NULL)
   {
      std::cerr << "***ERROR in InterpolationLoop::basicInit(): no Mesh set !\n";
      assert(0);
      exit(1);
   }
   if (_vectortype == "ml")
   {
      _uold.setLevel(-1);
      _uhelp.setLevel(-1);
   }
   getSolverManager()->registerVector(_uold);
   getSolverManager()->registerVector(_uhelp);

   assert(_refinedmesh);
   getMesh()->setResolution(0);
   getSolverManager()->reInitForInterpolation(_refinedmesh);
   getSolverManager()->reInitVectorForInterpolation(_uhelp);
   getSolverManager()->reInitVectorForInterpolation(_u);
   getIoManager().setReadUnknownVariables(true);
   // std::cerr << "InterpolationLoop::basicInit() FIN\n";
}

/*--------------------------------------------------------------------------*/
void InterpolationLoop::run()
{
   assert(getRefinedMeshComposition());
   getSolverManager()->readUnknownVariables(_uold);
   getSolverManager()->interpolateSolution(_u, _uold);
   getSolverManager()->setMesh(getRefinedMeshComposition());
   getSolverManager()->writeUnknownVariables(_uhelp, _u);

   // getRefinedMeshComposition()->writeH5( getIoManager().getFileNameOut(Alat::IoManager::MeshVisu, "Mesh") );
   // getRefinedMeshComposition()->writeMeshInfo( getIoManager().getFileNameOut(Alat::IoManager::MeshVisu, "MeshInfo") );
   // getSolverManager()->writeVariablesInfo( );
}
