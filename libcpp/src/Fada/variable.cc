#include  "Fada/feminterface.h"
#include  "Fada/variable.h"
#include  "Alat/stringvector.h"
#include  <cassert>
#include  <sstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
Variable::~Variable()
{}

Variable::Variable() : VariableInterface(), _timeintegral(false)
{}

Variable::Variable( std::string varname, const Variable& variable) : VariableInterface(variable)
{
  _varname = varname;
  _femname = variable._femname;
  _ncomp = variable._ncomp;
  _visutype = variable._visutype;
  _timeintegral = variable._timeintegral;
  _visunames = variable._visunames;
}

Variable::Variable( const Variable& variable) : VariableInterface(variable)
{
  _varname = variable._varname;
  _femname = variable._femname;
  _ncomp = variable._ncomp;
  _visutype = variable._visutype;
  _timeintegral = variable._timeintegral;
  _visunames = variable._visunames;
}

Variable::Variable( std::string varname, std::string femname, int ncomp, std::string visutype) : VariableInterface()
{
  _varname = varname;
  _femname = femname;
  _ncomp = ncomp;
  _visutype = visutype;
  _postproctype = "none";
  _timeintegral = false;
  _visunames.set_size( _ncomp );
  if(_ncomp == 1)
  {
    _visunames[0] = getVarName();
  }
  else
  {
    for(int i = 0; i < _ncomp; i++)
    {
      std::stringstream ss;
      // ss<<getVarName()<<"_"<<i;
      ss<<getVarName()<<i;
      _visunames[i] = ss.str();
    }
  }
}

Variable::Variable( std::string varname, int ncomp, std::string postproctype, bool timeintegral) : VariableInterface()
{
  _varname = varname;
  _femname = "none";
  _ncomp = ncomp;
  _visutype = "none";
  _postproctype = postproctype;
  _timeintegral = timeintegral;
  _visunames.set_size( _ncomp );

  _postproctype.set_size( _ncomp );
  _postproctype = postproctype;

  if(_ncomp == 1)
  {
    _visunames[0] = getVarName();
  }
  else
  {
    for(int i = 0; i < _ncomp; i++)
    {
      std::stringstream ss;
      ss<<getVarName()<<";"<<i;
      _visunames[i] = ss.str();
    }
  }
}

/*--------------------------------------------------------------------------*/
Variable& Variable::operator=( const Variable& variable)
{
  VariableInterface::operator=(variable);
  _varname = variable._varname;
  _femname = variable._femname;
  _ncomp = variable._ncomp;
  _visutype = variable._visutype;
  _visunames = variable._visunames;
  return *this;
}

/*--------------------------------------------------------------------------*/
std::string Variable::getName() const
{
  return "Variable";
}

std::ostream& Fada::operator<<(std::ostream& os, const Variable& variable)
{
  os << "varname="<<variable.getVarName()<<" femname="<<variable.getFemName()<<" ncomp=" << variable.getNComponents()<< " _visutype="<<variable.getVisualizationType();
  return os;
}

std::ostream& Variable::write(std::ostream& os) const
{
  os << *this;
  return os;
}

/*--------------------------------------------------------------------------*/
std::string Variable::getVarName() const
{
  return _varname;
}

std::string Variable::getFemName() const
{
  return _femname;
}

int Variable::getNComponents() const
{
  return _ncomp;
}

int Variable::getN() const
{
  assert(_femname == "none");
  return 1;
}

std::string Variable::getVisualizationType() const
{
  return _visutype;
}

std::string Variable::getPostProcType(int icomp) const
{
  return _postproctype[icomp];
}

void Variable::setPostProcType(int icomp, std::string type)
{
  _postproctype[icomp] = type;
}

/*--------------------------------------------------------------------------*/
void Variable::getVisualizationNames(Alat::StringVector& visunames) const
{
  visunames.set_size( getNComponents() );

  for(int i = 0; i < getNComponents(); i++)
  {
    visunames[i] = _visunames[i];
  }
}

void Variable::setVisualizationName(int icomp, const std::string& name)
{
  assert( icomp < _visunames.size() );
  _visunames[icomp] = name;
  // _visunames[icomp] =getVarName()+'_'+name;
}

const std::string&  Variable::getVisualizationName(int icomp) const
{
  assert( icomp < _visunames.size() );
  return _visunames[icomp];
}

/*--------------------------------------------------------------------------*/
void Variable::setVisualizationNames(Alat::StringVector& visunames) const
{
  // std::cerr<<visunames.size()<<" "<<_visunames.size()<<'\n';
  assert( visunames.size() == _visunames.size() );
  assert( _visunames.size() == getNComponents() );
  for(int i = 0; i < getNComponents(); i++)
  {
    _visunames[i] = visunames[i];
  }
}

/*--------------------------------------------------------------------------*/
bool Variable::timeIntegral() const
{
  return _timeintegral;
}
