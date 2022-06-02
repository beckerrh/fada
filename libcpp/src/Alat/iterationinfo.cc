#include  "Alat/filescanner.h"
#include  "Alat/iterationinfo.h"
#include  <cassert>
#include  <cmath>
#include  <stdio.h>

using namespace Alat;

/*--------------------------------------------------------------------------*/

IterationInfo::~IterationInfo()
{}

/*--------------------------------------------------------------------------*/

IterationInfo::IterationInfo() : _firstresidual(0.0), _maxiter(20), _printstep(1), _rtol(1e-6), _gtol(1e-12), _reductionrate(1.0), _iteration(0), _lastresidual(1.0), _divfactor(1000.0), _printbuff("\n"), _printbufff("\n")
{}

/*--------------------------------------------------------------------------*/

IterationInfo::IterationInfo( const IterationInfo& iterationinfo)
{
  _firstresidual = iterationinfo._firstresidual;
  _maxiter = iterationinfo._maxiter;
  _printstep = iterationinfo._printstep;
  _rtol = iterationinfo._rtol;
  _gtol = iterationinfo._gtol;
  _reductionrate = iterationinfo._reductionrate;
  _iteration = iterationinfo._iteration;
  _lastresidual = iterationinfo._lastresidual;
  _divfactor = iterationinfo._divfactor;
  _printbuff = iterationinfo._printbuff;
  _printbufff = iterationinfo._printbufff;
}

/*--------------------------------------------------------------------------*/

IterationInfo& IterationInfo::operator=( const IterationInfo& iterationinfo)
{
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/

std::string IterationInfo::getName() const
{
  return "IterationInfo";
}

/*--------------------------------------------------------------------------*/

IterationInfo* IterationInfo::clone() const
{
  return new IterationInfo(*this);
}

/*-------------------------------------------------------*/

std::ostream& IterationInfo::printLoopInformation(std::ostream& os) const
{
  os << "blockname/maxiter/printstep/rtol/gtol: " << _blockname<<"/"<<_maxiter<<"/"<<_printstep<<"/"<<_rtol<<"/"<<_gtol<<" ";
  return os;
}

/*-------------------------------------------------------*/

std::ostream& Alat::operator<<(std::ostream& stream, const IterationInfo& A)
{
  stream << "rtol gtol maxiter _iteration " << A.getRTol() << " " << A.getGlobalTol() << " " << A.getMaxiter() << " " << A.getNumberOfIterations();
  return stream;
}

/*-------------------------------------------------------*/

void IterationInfo::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname)
{
  _id = blockname;
  Alat::DataFormatHandler dataformathandler;
  dataformathandler.insert("maxiter", &_maxiter, 20);
  dataformathandler.insert("printstep", &_printstep, 1);
  dataformathandler.insert("gtol", &_gtol, 1e-12);
  dataformathandler.insert("rtol", &_rtol, 1e-6);
  dataformathandler.insert("divfactor", &_divfactor, 1000.);
  if(blockname != "none")
  {
    _blockname = blockname;
  }
  else
  {
    _blockname = "IterationInfo";
  }
  Alat::FileScanner filescanner(dataformathandler, parameterfile, _blockname, 0);
}

double IterationInfo::getLastReductionRate() const
{
  return _reductionrate;
}

double IterationInfo::getPreviousReductionRate() const
{
  return _previuousreductionrate;
}

double IterationInfo::getMissingRate() const
{
  // ce qu'il reste a faire
  // soit le min entre gtol/res et rtol*firstres/res;
  return fmin(_gtol, _rtol*_firstresidual)/_lastresidual;
}

/*-------------------------------------------------------*/
void IterationInfo::printFirstIteration() const
{
  printf( "%3d %10s **=rate=** --- **%9.3e** (%6.1e %6.1e)%s", _iteration, getId().c_str(), _lastresidual, _firstresidual*_rtol, _gtol, _printbufff.c_str() );
  if(_firstresidual != _firstresidual)
  {
    assert(0);
  }
}

void IterationInfo::printIteration() const
{
  printf( "%3d %10s **%6.4f** --- **%9.3e**%s", _iteration, getId().c_str(), _reductionrate, _lastresidual, _printbuff.c_str() );
}

/*-------------------------------------------------------*/

void IterationInfo::checkIteration(AlatEnums::iterationstatus& status, double res) const
{
  return checkIteration(status, res, _iteration, true);
}

void IterationInfo::checkIteration(AlatEnums::iterationstatus& status, double residual, int iteration, bool print) const
{
  _iteration = iteration;
  if(iteration)
  {           
    _previuousreductionrate = _reductionrate;
    _reductionrate = residual/_lastresidual;
  }
  // std::cerr << "IterationInfo::checkIteration() _iteration="<<_iteration<<" _lastresidual="<<_lastresidual<<" residual="<<residual << " print="<<print << "_printstep="<<_printstep<<" id"<<_id<<"\n";
  _lastresidual = residual;
  if(_printstep && !( iteration%_printstep ) && print && iteration)
  {
    printIteration();
  }
  if(iteration == 0)
  {
    status = AlatEnums::IterationStatusRunning;
    // std::cerr << "_firstresidual="<<_firstresidual<< " " << getId() <<"\n";
    _firstresidual = residual;
    _lastresidual = residual;
    if(_printstep && !( iteration%_printstep ) && print)
    {
      printFirstIteration();
    }
  }
  // std::cerr << "IterationInfo::CheckIteration() " << getId() << " ? " << residual << " iteration=" << iteration<< " _maxiter=" << _maxiter << " status="<<AlatEnums::iterationStatusToString(status)<<"\n";
  if( ( residual > _divfactor*_firstresidual )  ||  ( residual > _divfactor*_lastresidual ) )
  {
    if(print)
    {
      std::cerr << "IterationInfo::CheckIteration() BIG RESIDUAL _firstresidual _lastresidual residual "  << getId() << " ? iteration " << iteration << " _firstresidual " << _firstresidual << " _lastresidual " << _lastresidual << " residual " << residual << " _divfactor " << _divfactor << "\n";
      // assert(0);
    }
    status = AlatEnums::IterationStatusDiverged;
    return;
  }
  else if(iteration >= _maxiter )
  {
    status = AlatEnums::IterationStatusMaxIter;
    return;
  }
  _lastresidual = residual;
  bool ok = ( residual <= _rtol*_firstresidual )||( residual <= _gtol );
  if(ok)
  {
    status = AlatEnums::IterationStatusConverged;
  }
  return;
}

/*--------------------------------------------------------------------------*/

void IterationInfo::setId(std::string id)
{
  _id = id;
}

const std::string& IterationInfo::getId() const
{
  return _id;
}

const double& IterationInfo::getGlobalTol() const
{
  return _gtol;
}

double& IterationInfo::getGlobalTol()
{
  return _gtol;
}

const double& IterationInfo::getRTol() const
{
  return _rtol;
}

double& IterationInfo::getRTol()
{
  return _rtol;
}

int& IterationInfo::getMaxiter()
{
  return _maxiter;
}

const int& IterationInfo::getMaxiter() const
{
  return _maxiter;
}

int& IterationInfo::getPrintStep()
{
  return _printstep;
}

void IterationInfo::reset() const
{
  _iteration = 0;
}

int& IterationInfo::getNumberOfIterations() const
{
  return _iteration;
}

const double& IterationInfo::getFirstResidual() const
{
  return _firstresidual;
}

const double& IterationInfo::getLastResidual() const
{
  return _lastresidual;
}

const int& IterationInfo::getPrintstep() const
{
  return _printstep;
}
