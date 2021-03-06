#include  "Alat/matrixallvariables.h"
#include  "FadaMesh/meshinterface.h"
#include  "Alat/preconditionerwithsorting.h"
#include  "Alat/variablematrixinterface.h"
#include  "Alat/tokenize.h"
#include  "Alat/gaussseidelpreconditioner.h"
#include  "Alat/ilupreconditioner.h"
#include  "Alat/iluvariablematrix.h"
#include  "Fada/preconditionerselector.h"
#include  "Alat/sortercuthillmckee.h"
#include  "Alat/sortermatrix.h"
#include  "Alat/sortermetis.h"
#include  "Alat/spaipreconditioner.h"
#include  "Alat/trivialpreconditioner.h"
#include  "Alat/umfsolveronevariable.h"
#include  "Alat/variablematrix.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

PreconditionerSelector::~PreconditionerSelector()
{}

PreconditionerSelector::PreconditionerSelector() : Alat::InterfaceBase()
{}

PreconditionerSelector::PreconditionerSelector( const PreconditionerSelector& preconditionerselector) : Alat::InterfaceBase(preconditionerselector)
{
  assert(0);
}

PreconditionerSelector& PreconditionerSelector::operator=( const PreconditionerSelector& preconditionerselector)
{
  Alat::InterfaceBase::operator=(preconditionerselector);
  assert(0);
  return *this;
}

std::string PreconditionerSelector::getInterfaceName() const
{
  return "PreconditionerSelector";
}

std::string PreconditionerSelector::getName() const
{
  return "PreconditionerSelector";
}

PreconditionerSelector* PreconditionerSelector::clone() const
{
  assert(0);
//return new PreconditionerSelector(*this);
}

/*--------------------------------------------------------------------------*/

Alat::PreconditionerInterface* PreconditionerSelector::newPreconditioner(std::string name, std::string sorter, const Alat::VariableMatrixInterface* matrix, int level, int nlevels, int dimension) const
{
  Alat::StringVector bouts = Alat::Tokenize(name, "_");
  int nbouts = bouts.size();
  assert(nbouts);
  if(bouts[0] == "trivial")
  {
    return new Alat::TrivialPreconditioner;
  }
  Alat::PreconditionerInterface* preconditioner = NULL;
  if(bouts[0] == "direct")
  {
    preconditioner = new Alat::UmfSolverOneVariable(matrix);
  }
  else if(bouts[0] == "spai")
  {
    if(bouts.size() != 3)
    {
      _error_string("newPreconditioner", "please give type and relax in the form \"spai_type_relax\"\n");
    }
    preconditioner = new Alat::SpaiPreconditioner( matrix, bouts[1].c_str(), atof( bouts[2].c_str() ) );
  }
  else if(bouts[0] == "gs")
  {
    if(bouts.size() != 3)
    {
      _error_string("newPreconditioner", "please give type and relax in the form \"gs_type_relax\"\n");
    }
    preconditioner = new Alat::GaussSeidelPreconditioner( matrix, bouts[1].c_str(), atof( bouts[2].c_str() ) );
  }
  else if(bouts[0] == "ilu")
  {
    if(bouts.size() != 3)
    {
      _error_string("newPreconditioner", "please give type and relax in the form \"gs_type_relax\"\n");
    }
    preconditioner = new Alat::IluPreconditioner( matrix, bouts[1].c_str(), atof( bouts[2].c_str() ) );
  }
  else
  {
    assert(0);
  }

  Alat::PreconditionerWithSorting* preconditionerwithsorting = dynamic_cast<Alat::PreconditionerWithSorting*>( preconditioner );
  if(preconditionerwithsorting)
  {
    if(sorter == "matrix")
    {
      preconditionerwithsorting->newSorterPointer() = new Alat::SorterMatrix;
    }
    else if(sorter == "cuthillmckee")
    {
      preconditionerwithsorting->newSorterPointer() = new Alat::SorterCuthillMcKee;
    }
    else if(sorter == "metis")
    {
      preconditionerwithsorting->newSorterPointer() = new Alat::SorterMetis( preconditionerwithsorting->getPinv() );
    }
    else if(sorter != "none")
    {
      _error_string("newPreconditioner", "unknown sorter", sorter);
    }
  }
  if(preconditioner == NULL)
  {
    _error_string( "newPreconditioner", "unknown preconditioner", name + " for matrix " + matrix->getName() );
  }
  return preconditioner;
}
