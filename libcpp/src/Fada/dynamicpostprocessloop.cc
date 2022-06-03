#include  "Alat/dataformathandler.h"
#include  "Fada/dynamicpostprocessloop.h"
#include  "Alat/filescanner.h"
#include  "Alat/alatio.h"
#include  "Fada/solvermanager.h"
#include  <cassert>
#include  <fstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
DynamicPostProcessLoop::~DynamicPostProcessLoop()
{}
DynamicPostProcessLoop::DynamicPostProcessLoop() : PostProcessLoop(), _postprocesstimeintegral("postprocesstimeintegral", "postprocess"), _uold("uold", "unknowns")
{}
DynamicPostProcessLoop::DynamicPostProcessLoop( const DynamicPostProcessLoop& dynamicpostprocessloop) : PostProcessLoop(dynamicpostprocessloop)
{
  assert(0);
}

DynamicPostProcessLoop& DynamicPostProcessLoop::operator=( const DynamicPostProcessLoop& dynamicpostprocessloop)
{
  PostProcessLoop::operator=(dynamicpostprocessloop);
  assert(0);
  return *this;
}

std::string DynamicPostProcessLoop::getName() const
{
  return "DynamicPostProcessLoop";
}

DynamicPostProcessLoop* DynamicPostProcessLoop::clone() const
{
  return new DynamicPostProcessLoop(*this);
}

/*--------------------------------------------------------------------------*/
void DynamicPostProcessLoop::basicInit(ModelManagerInterface* modelmanager, SolverManager* solvermanager, const std::string& rundirectory, const Alat::ParameterFile* parameterfile)
{
  solvermanager->registerVector(_uold);
  solvermanager->registerVector(_postprocesstimeintegral);
  Alat::DataFormatHandler dataformathandler;
  dataformathandler.insert("postprocessincrement", &_postprocessincrement, 1);
  Alat::FileScanner filescanner(dataformathandler, parameterfile, "Loop", 0);
  PostProcessLoop::basicInit(modelmanager, solvermanager, rundirectory, parameterfile);
  std::string filenametimeinfo = getIoManager().getFileNameOut(Alat::IoManager::RunInfo, "DynamicLoopInfo");
  std::ifstream filetimeinfo( filenametimeinfo.c_str() );
  if( not filetimeinfo.is_open() )
  {
    _error_string("basicInit", "cannot open file", filenametimeinfo);
  }
  Alat::loadAlat(filetimeinfo, _output_times);
  // std::istream_iterator<double> iter(filetimeinfo);
  // std::copy(iter.begin(), iter.end(), std::back_inserter(_output_times));
  // _output_times.read(filetimeinfo);
  filetimeinfo.close();
}

/*--------------------------------------------------------------------------*/
void DynamicPostProcessLoop::run()
{
  // std::cerr << "DynamicPostProcessLoop::run() " << _output_times << "\n";
  AlatEnums::residualstatus status;
  for(int iteration = 0; iteration < _output_times.size(); iteration++)
  {
    if( ( iteration%( _postprocessincrement ) ) != 0 )
    {
      continue;
    }
    // std::cerr<< "@@@@@@@  DynamicPostProcessLoop::run() iteration " << iteration << " time="<<_output_times[iteration]<<" _postprocessincrement= "<<_postprocessincrement<<"\n";
    getSolverManager()->vectorEqual(_uold, _u);
    getSolverManager()->readUnknownVariables(_u, iteration);
    getSolverManager()->postProcess(status, _postprocess, _u, _uold);
    bool first = (iteration==0);
    getSolverManager()->integrateInTimePostProcess(_postprocesstimeintegral, _postprocess);
    // std::cerr<< "@@@@@@@  DynamicPostProcessLoop::run() _postprocess " << _postprocess<<"\n";
    // std::cerr<< "@@@@@@@  DynamicPostProcessLoop::run() _postprocesstimeintegral " << _postprocesstimeintegral<<"\n";
    getSolverManager()->writePostProcessVariablesDynamic(_postprocesstimeintegral, iteration);
  }
  // getSolverManager()->writeVariablesInfo( );
}
