#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Alat/preconditioner.h"
#include  <cassert>
#include  <sstream>

using namespace Alat;

/*--------------------------------------------------------------------------*/
int Preconditioner::_totalids = 0;

Preconditioner::~Preconditioner()
{}

Preconditioner::Preconditioner() : PreconditionerInterface(), _visitor(NULL)
{
  _id = _totalids;
  _totalids++;
}

Preconditioner::Preconditioner( const Preconditioner& preconditioner) : PreconditionerInterface(preconditioner), _visitor(preconditioner._visitor)
{
  assert(0);
}

Preconditioner& Preconditioner::operator=( const Preconditioner& preconditioner)
{
  PreconditionerInterface::operator=(preconditioner);
  assert(0);
  return *this;
}

std::string Preconditioner::getName() const
{
  return "constructLinearSolver";
}

Preconditioner* Preconditioner::clone() const
{
  assert(0);
//return new Preconditioner(*this);
}

/*--------------------------------------------------------------------------*/
void Preconditioner::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor)
{
  // std::cerr << "Preconditioner::basicInit() " << getName() << "\n";
  _visitor = visitor;
  assert(_visitor);
  memory();
}

/*--------------------------------------------------------------------------*/
void Preconditioner::setsmoothtype(std::string smoothtype)
{
  _smoothtype = smoothtype;
}

/*--------------------------------------------------------------------------*/
int Preconditioner::getNVectors() const
{
  return 0;
}

/*--------------------------------------------------------------------------*/
const Alat::IterativeSolverVisitorInterface* Preconditioner::getVisitor() const
{
  assert(_visitor);
  return _visitor;
}

Alat::IterativeSolverVisitorInterface* Preconditioner::getVisitor()
{
  assert(_visitor);
  return _visitor;
}

/*--------------------------------------------------------------------------*/
Alat::GhostVector& Preconditioner::getMemory(int i) const
{
  if( i >= _memory.size() )
  {
    _error_string("getMemory", "", "too small memory");
  }
  return _memory[i];
}

/*--------------------------------------------------------------------------*/
void Preconditioner::memory()
{
  int nvectors = getNVectors();
  if(nvectors == 0)
  {
    return;
  }
  if(_visitor == NULL)
  {
    _error_string("memory", "no visitor !");
  }
  _memory.set_size( getNVectors() );
  std::string type = getVisitor()->getVectorType();
  int level = getVisitor()->getVectorLevel();
  // std::cerr << "Preconditioner::memory() in " << getName() << " visitor = "<<getVisitor()->getName() << " " << getNVectors()  << " of type " << type << "\n";
  // assert(0);
  for(int i = 0; i < _memory.size(); i++)
  {
    std::stringstream ss;
    ss<<getName()<<"_"<<_id<<"_"<<getVisitor()->getName()<<"_memory_"<<i;
    _memory[i] = Alat::GhostVector( ss.str(), type, level );
    // _memory[i].setVariables( variablesOfVector(i) );
    getVisitor()->newVector(&_memory[i]);
  }
}
