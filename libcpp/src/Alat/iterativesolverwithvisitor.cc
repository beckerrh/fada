#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Alat/iterativesolverwithvisitor.h"
#include  <cassert>
#include  <sstream>

using namespace Alat;

/*--------------------------------------------------------------------------*/
int IterativeSolverWithVisitor::_totalids = 0;

IterativeSolverWithVisitor::~IterativeSolverWithVisitor()
{
  if(_deletevisitor && _visitor)
  {
    delete _visitor;
    _visitor = NULL;
  }
}

IterativeSolverWithVisitor::IterativeSolverWithVisitor() : LinearSolverInterface(), _visitor(NULL), _deletevisitor(true), _basicinitcalled(false), _info()
{
  _id = _totalids;
  _totalids++;
}

IterativeSolverWithVisitor::IterativeSolverWithVisitor( const IterativeSolverWithVisitor& iterativesolverwithvisitor) : LinearSolverInterface(iterativesolverwithvisitor), _deletevisitor(iterativesolverwithvisitor._deletevisitor), _info(iterativesolverwithvisitor._info)
{
  _id = _totalids;
  _totalids++;
  if(_deletevisitor)
  {
    if(not iterativesolverwithvisitor._visitor)
    {
      _visitor = NULL;
    }
    else
    {
      _visitor = iterativesolverwithvisitor._visitor->clone();
    }
  }
  else
  {
    _visitor = iterativesolverwithvisitor._visitor;
  }
}

IterativeSolverWithVisitor& IterativeSolverWithVisitor::operator=( const IterativeSolverWithVisitor& iterativesolverwithvisitor)
{
  LinearSolverInterface::operator=(iterativesolverwithvisitor);
  _visitor = iterativesolverwithvisitor._visitor;
  _deletevisitor = iterativesolverwithvisitor._deletevisitor;
  return *this;
}

std::string IterativeSolverWithVisitor::getName() const
{
  return "IterativeSolverWithVisitor";
}

IterativeSolverWithVisitor* IterativeSolverWithVisitor::clone() const
{
  return new IterativeSolverWithVisitor(*this);
}

/*--------------------------------------------------------------------------*/
std::ostream& IterativeSolverWithVisitor::printLoopInformation(std::ostream& os) const
{
  os << "\"" << getName() << "\" ";
  getVisitor()->printLoopInformation(os);
  os << " info: ";
  getIterationInfo()->printLoopInformation(os);
  os <<" ";
  return os;
}

bool IterativeSolverWithVisitor::hasIterationInfo() const
{
  return true;
}

/*--------------------------------------------------------------------------*/
Alat::IterativeSolverVisitorInterface*& IterativeSolverWithVisitor::newVisitorPointer()
{
  assert(_visitor == NULL);
  _deletevisitor = true;
  return _visitor;
}

void IterativeSolverWithVisitor::setVisitorPointer(Alat::IterativeSolverVisitorInterface* visitor)
{
  _visitor = visitor;
  _deletevisitor = false;
}

const Alat::IterativeSolverVisitorInterface* IterativeSolverWithVisitor::getVisitor() const
{
  assert(_visitor);
  return _visitor;
}

Alat::IterativeSolverVisitorInterface* IterativeSolverWithVisitor::getVisitor()
{
  assert(_visitor);
  return _visitor;
}

const IterationInfo* IterativeSolverWithVisitor::getIterationInfo() const
{
  return &_info;
}

IterationInfo* IterativeSolverWithVisitor::getIterationInfo()
{
  return &_info;
}

Alat::GhostVector& IterativeSolverWithVisitor::getMemory(int i) const
{
  if( i >= _memory.size() )
  {
    _error_string("getMemory", "", "too small memory");
  }
  return _memory[i];
}

/*--------------------------------------------------------------------------*/
void IterativeSolverWithVisitor::memory()
{
  _memory.set_size( getNVectors() );
  std::string type = getVisitor()->getVectorType();
  int level = getVisitor()->getVectorLevel();
  // std::cerr << "IterativeSolverWithVisitor::memory() in " << getName() << " visitor = "<<getVisitor()->getName() << " " << getNVectors()  << " of type " << type << "\n";
  for(int i = 0; i < _memory.size(); i++)
  {
    std::stringstream ss;
    ss<<getName()<<"_"<<_id << "_" <<getVisitor()->getName()<<"_memory_"<<i;
    _memory[i] = Alat::GhostVector( ss.str(), type, level);
    getVisitor()->newVector(&_memory[i]);
  }
}

/*--------------------------------------------------------------------------*/
void IterativeSolverWithVisitor::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname)
{
  assert(_basicinitcalled == false);
  // std::cerr << "IterativeSolver::basicInit() " << getName() << " blockname " << blockname <<"\n";
  std::string paramblockname = blockname;
  if(blockname == "none")
  {
    paramblockname = getName();
  }
  getVisitor()->basicInit(parameterfile, paramblockname);
  getIterationInfo()->basicInit(parameterfile, paramblockname);
  memory();
  _basicinitcalled = true;
}
