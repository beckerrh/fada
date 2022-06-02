#include  "Alat/filescanner.h"
#include  "Alat/ghostvector.h"
#include  "Alat/matrixallvariables.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/multilevelmatrix.h"
#include  "Fada/multilevelvector.h"
#include  "Fada/solver.h"
#include  "Fada/timedata.h"
#include  "Alat/variablevector.h"
#include  "Alat/systemvector.h"
#include  "Alat/tokenize.h"
#include  <cmath>

using namespace Fada;
using namespace std;

/*---------------------------------------------------------*/
Solver::~Solver()
{}

Solver::Solver() : SolverInterface(), _parameterfile(NULL), _mesh(NULL), _io_manager(NULL), _time(0.0), _deltat(0.0), _deltatold(0.0), _refinedmesh(NULL)
{}

Solver::Solver(const Solver& S) : SolverInterface(S)
{
  assert(0);
}

Solver& Solver::operator=(const Solver& S)
{
  assert(0);
  return *this;
}

std::string Solver::getName() const
{
  return "Solver";
}

/*--------------------------------------------------------------------------*/
std::ostream& Solver::printLoopInformation(std::ostream& os) const
{
  os << "|>===Solver="<<getName()<<"===<|\n";
  _integratormanager.printLoopInformation(os);
  // os << "\"couplingtype\"=" << _couplingtype << "\n";
  return os;
}

/*--------------------------------------------------------------------------*/
FadaEnums::looptype Solver::getType() const
{
  return _looptype;
}

int Solver::getIndex() const
{
  return _index;
}

int Solver::getNLevels() const
{
  return getMesh()->getNLevels();
}

void Solver::setLavrentievParameter(double trparameter) const
{
  getIntegratorManager()->setLavrentievParameter( trparameter);
}

std::string Solver::getCouplingType() const
{
  return _couplingtype;
}

/*---------------------------------------------------------*/
void Solver::basicInit(int index, ModelInterface* model, const FadaMesh::MeshInterface* mesh, const Alat::IoManager* io_manager, FadaEnums::looptype looptype, const Alat::ParameterFile* parameterfile)
{
  _looptype = looptype;
  _index = index;
  Alat::DataFormatHandler dataformathandler;
  dataformathandler.insert("couplingtype", &_couplingtype, "cg");
  Alat::FileScanner filescanner(dataformathandler, parameterfile, "Solver", 0);

  assert(mesh);
  _mesh = mesh;
  _parameterfile = parameterfile;
  _io_manager = io_manager;
}

/*--------------------------------------------------------------------------*/
void Solver::setMesh(const FadaMesh::MeshInterface* mesh) const
{
  _mesh = mesh;
}

/*--------------------------------------------------------------------------*/

double Solver::getTime() const
{
  return _time;
}

double Solver::getDeltaT() const
{
  return _deltat;
}

double Solver::getDeltaTOld() const
{
  return _deltatold;
}

/*---------------------------------------------------------*/

const Alat::ParameterFile* Solver::getParameterFile() const
{
  return _parameterfile;
}

/*---------------------------------------------------------*/

const Alat::IoManager* Solver::getIoManager() const
{
  assert(_io_manager);
  return _io_manager;
}

/*---------------------------------------------------------*/

const FadaMesh::MeshInterface* Solver::getMesh() const
{
  return _mesh;
}

/*---------------------------------------------------------*/

const FadaMesh::MeshInterface* Solver::getRefinedMesh() const
{
  return _refinedmesh;
}

/*---------------------------------------------------------*/

void Solver::reInitForInterpolation(const FadaMesh::MeshInterface* mesh)
{
  _refinedmesh = mesh;
}

/*---------------------------------------------------------*/
IntegratorManager* Solver::getIntegratorManager()
{
  return &_integratormanager;
}

const IntegratorManager* Solver::getIntegratorManager() const
{
  return &_integratormanager;
}

/*---------------------------------------------------------*/
double& Solver::getTime()
{
  return _time;
}

double& Solver::getDeltaT()
{
  return _deltat;
}

double& Solver::getDeltaTOld()
{
  return _deltatold;
}

void Solver::setTimeInfo(const Fada::TimeData& timedata, double masscoefimplicit)
{
  getTime() = timedata.time;
  getDeltaT() = timedata.deltat;
  getDeltaTOld() =  timedata.deltatold;
  // getTime() = time;
  // getDeltaT() = deltat;
  // getDeltaTOld() = deltatold;
  getIntegratorManager()->setTimeInfo( timedata, masscoefimplicit);
}

void Solver::setTimeScheme(std::string time_discretization)
{
  getIntegratorManager()->setTimeScheme( time_discretization);
}
