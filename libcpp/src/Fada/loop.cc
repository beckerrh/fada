#include  "Alat/filescanner.h"
#include  "Fada/loop.h"
#include  "Fada/domainsolver.h"
#include  "Fada/solvermanager.h"
#include  "Fada/timedata.h"
#include  "Alat/userandsysteminfo.h"
#include  <fstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
int Loop::_debuglevel;
/*--------------------------------------------------------------------------*/

/*---------------------------------------------------------*/
Loop::Loop() : _parameterfile(NULL), _solvermanager(NULL), _mesh(NULL), _io_manager(), _chronometer(), _u("u", "unknowns"), _data("data", "data"), _postprocess("postprocess", "postprocess")
{
  getChronometer().enrol("basicInit", true);
  getChronometer().enrol("InPutOutPut", true);
  getChronometer().enrol("Jacobian", true);
  getChronometer().enrol("Residual", true);
  getChronometer().enrol("constructLinearSolver", true);
  getChronometer().enrol("LinearSolve", true);
  getChronometer().enrol("VectorOperations", true);
  getChronometer().enrol("nonlinearSolve");
  _debuglevel = 1;
}

Loop::~Loop()
{
  if(_solvermanager)
  {
    delete _solvermanager;
    _solvermanager = NULL;
  }
  if(_mesh)
  {
    delete _mesh;
    _mesh = NULL;
  }
}

Loop::Loop( const Loop& loop)
{
  assert(0);
}

Loop& Loop::operator=( const Loop& loop)
{
  assert(0);
  return *this;
}

/*---------------------------------------------------------*/
void Loop::basicInit(Fada::ModelInterface* model, SolverManager* solvermanager, const std::string& rundirectory, const Alat::ParameterFile* parameterfile)
{
  _solvermanager=solvermanager;
  // std::cerr << "Loop::basicInit() DEBUT\n";
  getChronometer().start("basicInit");
  getChronometer().setClassName( getName() );
  Alat::DataFormatHandler dataformathandler;
  int debug;
  dataformathandler.insert("debug", &debug, 0);
  dataformathandler.insert("vectortype", &_vectortype, "none");
  Alat::FileScanner filescanner(dataformathandler, parameterfile, "Loop", 0);
  Loop::_debuglevel = debug;
  if(_solvermanager == NULL)
  {
    std::cerr<<"\"Loop::basicInit()\": ERROR: no SolverManager set !\n";
    assert(0);
  }
  if(_mesh == NULL)
  {
    std::cerr<<"\"Loop::basicInit()\": ERROR: no Mesh set !\n";
    assert(0);
  }
  _parameterfile = parameterfile;
  _io_manager.basicInit(rundirectory, parameterfile);

  if(_vectortype=="ml")
  {
    _u.setLevel(-1);
    _data.setLevel(-1);
    _postprocess.setLevel(-1);
  }
  getSolverManager()->registerVector(_u);
  getSolverManager()->registerVector(_data);
  getSolverManager()->registerVector(_postprocess);

  getSolverManager()->basicInit(model, getMesh(), getIoManager(), getType(), parameterfile);

  std::string filename = getIoManager().getDirectoryName(Alat::IoManager::RunInfo)+"/LoopInformation";
  std::ofstream file;
  file.open( filename.c_str(), std::ios_base::app);
  if (!file.is_open())
  {
    file.open( filename.c_str());
  }
  printLoopInformation(file);
  file.close();


  getChronometer().stop("basicInit");
  // std::cerr << "Loop::basicInit() FIN\n";
}

/*---------------------------------------------------------*/
std::ostream& Loop::printLoopInformation(std::ostream& os) const
{
  // std::string blank("    ");
  os << "|>~~~~~~-----------------------------------------------------------------------------------------~~~~~~<|\n";
  os << "|>~~~~~~-------------------------------------->Fada<--------------------------------------~~~~~~<|\n";
  os << "|>~~~~~~-----------------------------------------------------------------------------------------~~~~~~<|\n";
  Alat::UserAndSystemInfo userandsysteminfo(os);
  os << "|>~~~ Loop="<<getName()<<"\n";
  os << "|>~~~ SolverManager="<<getSolverManager()->getName()<<"\n";
  os << "|>~~~ Mesh="<<getMesh()->getName()<<" ("<<getMesh()->getInfo()<<")\n";
  // os << "|>~~~ RunDir="<<getIoManager()->getRunDir()<<"\n"<<getIoManager()->printLoopInformation(os)<<"\n";
  getIoManager()->printLoopInformation(os);
  os << "|>~~~~~~-----------------------------------------------------------------------------------------~~~~~~<|\n";
  getSolverManager()->printLoopInformation(os);
  os << "|>~~~~~~-----------------------------------------------------------------------------------------~~~~~~<|\n";
  return os;
}

/*---------------------------------------------------------*/
void Loop::initializeSolution(Alat::GhostVector& u, TimeData& timedata)
{
  getChronometer().start("InPutOutPut");
  if(getIoManager().getReadUnknownVariables() == "yes")
  {
    getSolverManager()->readUnknownVariables( u);
    timedata.time = getSolverManager()->getTime();
    timedata.deltat = getSolverManager()->getDeltaT();
  }
  else
  {
    // Fada::TimeData timedata;
    // timedata.time = 0.0;
    // timedata.deltat = 0.0;
    // timedata.deltatold = 0.0;
    double masscoefimplicit = 0.0;
    getSolverManager()->setTimeInfo(timedata, masscoefimplicit);
    getSolverManager()->initSolution(u);
  }
  getChronometer().stop("InPutOutPut");
}

/*---------------------------------------------------------*/
void Loop::initializeSolution(Alat::GhostVector& u)
{
  getChronometer().start("InPutOutPut");
  if(getIoManager().getReadUnknownVariables() == "yes")
  {
    getSolverManager()->readUnknownVariables( u);
  }
  else
  {
    getSolverManager()->initSolution(u);
  }
  getChronometer().stop("InPutOutPut");
}

/*---------------------------------------------------------*/
// const Alat::ParameterFile* Loop::getParameterFile() const
// {
//   return _parameterfile;
// }

Chronometer& Loop::getChronometer() const
{
  return _chronometer;
}

std::string Loop::getName() const
{
  return "Loop";
}

std::string Loop::getRunDir() const
{
  return _io_manager.getRunDir();
}

void Loop::computeLinearization(AlatEnums::residualstatus& status, Alat::GhostVector& y, const Alat::GhostVector& x, const Alat::GhostVector& dx) const
{
  getChronometer().start("Residual");
  getSolverManager()->computeLinearization(status, y, x, dx);
  getChronometer().stop("Residual");
}

void Loop::setLavrentievParameter(double parameter) const
{
  getChronometer().start("VectorOperations");
  getSolverManager()->setLavrentievParameter(parameter);
  getChronometer().stop("VectorOperations");
}

/*--------------------------------------------------------------------------*/
void Loop::newtonProject(Alat::GhostVector& gu) const
{
  getSolverManager()->newtonProject(gu);
}
