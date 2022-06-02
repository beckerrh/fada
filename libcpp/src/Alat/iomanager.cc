#include  "Alat/filescanner.h"
#include  "Alat/iomanager.h"
#include  "Alat/tokenize.h"
#include  <cassert>
#include  <stdio.h>
#include  <stdlib.h>
#include  <iomanip>
#include  <sstream>

using namespace Alat;

/*---------------------------------------------------------*/

IoManager::~IoManager()
{}

/*---------------------------------------------------------*/

IoManager::IoManager() : _parameterfile(NULL)
{
  _rundir = "none";
  _datatype = "none";
}

/*---------------------------------------------------------*/

IoManager::IoManager(const Alat::IoManager& I)
{
  assert(0);
}

/*---------------------------------------------------------*/

IoManager& IoManager::operator=(const Alat::IoManager& I)
{
  assert(0);
  return *this;
}

/*---------------------------------------------------------*/

std::ostream& IoManager::printLoopInformation(std::ostream& os) const
{
  os << "|>~~~ IoManager\n";
  os << "|>~~~ \t datatype \"" << _datatype << "\"\n";
  os << "|>~~~ \t rundir \"" << _rundir << "\"\n";
  os << "|>~~~ \t readunknownvariables \"" << _readunknownvariables << "\"\n";
  return os;
}

/*---------------------------------------------------------*/

void IoManager::basicInit(const std::string& rundirectory, const Alat::ParameterFile* parameterfile)
{
  _rundir = rundirectory;
  _parameterfile = parameterfile;
  Alat::DataFormatHandler dataformathandler;
  dataformathandler.insert("readunknownvariables", &_readunknownvariables, "no");
  dataformathandler.insert("precision", &_output_precision, "double");
  dataformathandler.insert("resultsdir", &_resultsdir, "Results");
  dataformathandler.insert("inputdir", &_inputdir, "none");
  dataformathandler.insert("inputfile", &_inputfile, "U");

  if(getDataType() == "none")
  {
    dataformathandler.insert("datatype", &_datatype, "binary");
  }

  Alat::FileScanner filescanner(dataformathandler, parameterfile, "IoManager", 0);
  if(_inputdir == "none")
  {
    _inputdir = _rundir+"/"+_resultsdir+"/Unknowns";
  }

  std::string command;
  command = "mkdir -p "+_rundir;
  system( command.c_str() );
  command = "mkdir -p "+_rundir+"/"+_resultsdir;
  system( command.c_str() );
  command = "mkdir -p "+_rundir+"/"+_resultsdir+"/Unknowns";
  system( command.c_str() );
  command = "mkdir -p "+_rundir+"/"+_resultsdir+"/PostProcess";
  system( command.c_str() );
  command = "mkdir -p "+_rundir+"/"+_resultsdir+"/RunInfo";
  system( command.c_str() );
  command = "mkdir -p "+_rundir+"/"+_resultsdir+"/MeshVisu";
  system( command.c_str() );
}

/*---------------------------------------------------------*/

std::string IoManager::getDirectoryName( resultsubdir subdir) const
{
  std::string out = _rundir+"/"+_resultsdir+"/"+resultSubDirToString(subdir);
  return out;
}

/*---------------------------------------------------------*/

std::string IoManager::getFileNameOut( resultsubdir subdir, const std::string& name,  int number) const
{
  std::string out = _rundir+"/"+_resultsdir+"/"+resultSubDirToString(subdir)+"/"+name;

  if(number != -1)
  {
    std::stringstream ss;
    ss<< std::setfill('0') << std::setw(7) << number;
    out += "_timestep_"+ss.str();
  }

  return out;
}

/*---------------------------------------------------------*/

std::string IoManager::getFileNameIn(int number) const
{
  std::string in;
  if(number == -1)
  {
    in = _inputdir+"/"+_inputfile;
  }
  else
  {
    std::stringstream ss;
    ss<< std::setfill('0') << std::setw(7) << number;
    in = _inputdir+"/"+_inputfile;
    in += "_timestep_"+ss.str();
  }
  return in;
}

/*---------------------------------------------------------*/

std::string IoManager::getName()
{
  return "IoManager";
}

/*---------------------------------------------------------*/

const std::string& IoManager::getRunDir() const
{
  return _rundir;
}

// /*---------------------------------------------------------*/
//
// const std::string& IoManager::getInputDir() const
// {
//   return _inputdir;
// }
//
// /*---------------------------------------------------------*/
//
// std::string& IoManager::getInputDir()
// {
//   return _inputdir;
// }

/*---------------------------------------------------------*/

std::string IoManager::getResultsDir() const
{
  return _rundir+"/"+_resultsdir;
}

// /*---------------------------------------------------------*/
//
// std::string& IoManager::getResultsDir()
// {
//   return _resultsdir;
// }

/*---------------------------------------------------------*/

const std::string& IoManager::getOutputPrecision() const
{
  return _output_precision;
}

/*---------------------------------------------------------*/

std::string& IoManager::getOutputPrecision()
{
  return _output_precision;
}

/*---------------------------------------------------------*/

const Alat::ParameterFile* IoManager::getParameterFile()
{
  return _parameterfile;
}

/*---------------------------------------------------------*/

const std::string& IoManager::getDataType() const
{
  return _datatype;
}

//
// /*---------------------------------------------------------*/
//
// std::string& IoManager::getDataType()
// {
//   return _datatype;
// }

/*---------------------------------------------------------*/

std::string IoManager::getReadUnknownVariables() const
{
  return _readunknownvariables;
}

/*---------------------------------------------------------*/

void IoManager::setReadUnknownVariables(bool read)
{
  _readunknownvariables = "no";
  if(read)
  {
    _readunknownvariables = "yes";
  }
}

/*---------------------------------------------------------*/

std::string IoManager::resultSubDirToString(resultsubdir subdir) const
{
  if(subdir == Unknowns)
  {
    return "Unknowns";
  }
  else if(subdir == PostProcess)
  {
    return "PostProcess";
  }
  else if(subdir == RunInfo)
  {
    return "RunInfo";
  }
  else if(subdir == MeshVisu)
  {
    return "MeshVisu";
  }

  std::cerr<<"****IoManager::ResultSubDirToString: subdirectory not defined "<<subdir<<'\n';
  exit(1);
  assert(0);
}

/*---------------------------------------------------------*/

std::string IoManager::getFileNameOnBlock(const std::string& basefilename, int iblock, int level)
{
  std::stringstream ss;
  ss<< "_block_" << std::setfill('0') << std::setw(4) << iblock<< "_level_" << std::setfill('0') << std::setw(2) << level;
  return basefilename+ss.str();
  char st[5];
  sprintf(st, "%04d", iblock);
  char st2[3];
  sprintf(st2, "%02d", level);
  return basefilename+"_block_"+std::string(st)+"_level_"+st2;
}
