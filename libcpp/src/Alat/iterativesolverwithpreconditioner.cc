#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Alat/iterativesolverwithpreconditioner.h"
#include  "Alat/preconditionerinterface.h"
#include  <cassert>
#include  <sstream>

using namespace Alat;

/*--------------------------------------------------------------------------*/
IterativeSolverWithPreconditioner::~IterativeSolverWithPreconditioner()
{
  if(_deletepreconditioner && _preconditioner)
  {
    delete _preconditioner;
    _preconditioner = NULL;
  }
}

IterativeSolverWithPreconditioner::IterativeSolverWithPreconditioner() : IterativeSolverWithVisitor(), _preconditioner(NULL), _deletepreconditioner(true){}

IterativeSolverWithPreconditioner::IterativeSolverWithPreconditioner( const IterativeSolverWithPreconditioner& iterativesolverwithpreconditioner) : IterativeSolverWithVisitor(iterativesolverwithpreconditioner), _preconditioner(iterativesolverwithpreconditioner._preconditioner), _deletepreconditioner(iterativesolverwithpreconditioner._deletepreconditioner)
{
  assert(0);
}

IterativeSolverWithPreconditioner& IterativeSolverWithPreconditioner::operator=( const IterativeSolverWithPreconditioner& iterativesolverwithpreconditioner)
{
  IterativeSolverWithVisitor::operator=(iterativesolverwithpreconditioner);
  _preconditioner = iterativesolverwithpreconditioner._preconditioner;
  _deletepreconditioner = iterativesolverwithpreconditioner._deletepreconditioner;
  return *this;
}

std::string IterativeSolverWithPreconditioner::getName() const
{
  return "IterativeSolverWithPreconditioner";
}

Alat::IterativeSolverWithPreconditioner*  IterativeSolverWithPreconditioner::clone() const
{
  return new IterativeSolverWithPreconditioner(*this);
}

/*--------------------------------------------------------------------------*/
void IterativeSolverWithPreconditioner::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname)
{
  // std::cerr << "IterativeSolverWithPreconditioner::basicInit() " << getName() << " blockname " << blockname <<"\n";
  // std::cerr << "IterativeSolverWithPreconditioner::basicInit() " << getName() << " --> visitor =  " << getVisitor()->getName() << " --> preconditioner =  " << getPreconditioner()->getName() <<"\n";
  IterativeSolverWithVisitor::basicInit(parameterfile, blockname);
  assert(_preconditioner);
  getPreconditioner()->basicInit( parameterfile, blockname, getVisitor() );
  std::stringstream ss;
  ss<<"\t"<< getName() << " ("<< getVisitor()->getName() << ") : " << getPreconditioner()->getName() << " ";
  getIterationInfo()->setId( ss.str() );
}

/*--------------------------------------------------------------------------*/
void IterativeSolverWithPreconditioner::reInit()
{
  assert(_basicinitcalled==true);
  getPreconditioner()->reInit();
}

/*--------------------------------------------------------------------------*/
void IterativeSolverWithPreconditioner::compute()
{
  getPreconditioner()->computePreconditioner();
}

/*--------------------------------------------------------------------------*/
std::ostream& IterativeSolverWithPreconditioner::printLoopInformation(std::ostream& os) const
{
  IterativeSolverWithVisitor::printLoopInformation(os);
  os << " ";
  getPreconditioner()->printLoopInformation(os);
  return os;
}

/*--------------------------------------------------------------------------*/
Alat::PreconditionerInterface*& IterativeSolverWithPreconditioner::newPreconditionerPointer()
{
  assert(_preconditioner == NULL);
  _deletepreconditioner = true;
  return _preconditioner;
}

/*--------------------------------------------------------------------------*/
void IterativeSolverWithPreconditioner::setPreconditionerPointer(Alat::PreconditionerInterface* preconditioner)
{
  _preconditioner = preconditioner;
  _deletepreconditioner = false;
}

/*--------------------------------------------------------------------------*/
const Alat::PreconditionerInterface* IterativeSolverWithPreconditioner::getPreconditioner() const
{
  assert(_preconditioner);
  return _preconditioner;
}

/*--------------------------------------------------------------------------*/
Alat::PreconditionerInterface* IterativeSolverWithPreconditioner::getPreconditioner()
{
  assert(_preconditioner);
  return _preconditioner;
}

/*--------------------------------------------------------------------------*/
void IterativeSolverWithPreconditioner::addUpdate(AlatEnums::iterationstatus& status, const Alat::GhostVector& w, Alat::GhostVector& u) const
{
  getVisitor()->vectorAdd( u, 1.0, w );
}
