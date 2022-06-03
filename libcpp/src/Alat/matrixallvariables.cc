#include  "Alat/matrixallvariables.h"
#include  "Alat/systemassemblematrix.h"
#include  "Alat/systemvector.h"
#include  "Alat/stringset.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
MatrixAllVariables::~MatrixAllVariables()
{
  for(MatrixAllVariables::iterator p = this->begin(); p != this->end(); p++)
  {
    if( p->second )
    {
      delete p->second;
      p->second = NULL;
    }
  }
}
MatrixAllVariables::MatrixAllVariables() : Alat::Map<Alat::StringPair, Alat::VariableMatrixInterface*>(), SystemMatrixInterface(){}
MatrixAllVariables::MatrixAllVariables( const MatrixAllVariables& matrixallvariables) : Alat::Map<Alat::StringPair, Alat::VariableMatrixInterface*>(matrixallvariables), SystemMatrixInterface(matrixallvariables)
{
  assert(0);
}

MatrixAllVariables& MatrixAllVariables::operator=( const MatrixAllVariables& matrixallvariables)
{
  SystemMatrixInterface::operator=(matrixallvariables);
  assert(0);
  return *this;
}

std::string MatrixAllVariables::getName() const
{
  return "MatrixAllVariables";
}

MatrixAllVariables* MatrixAllVariables::clone() const
{
  assert(0);
}

/*--------------------------------------------------------------------------*/
Alat::StringSet MatrixAllVariables::getVariables() const
{
  Alat::StringSet variables;
  for(MatrixAllVariables::const_iterator p = this->begin(); p != this->end(); p++)
  {
    assert(p->first.first == p->first.second);
    variables.insert(p->first.first);
  }
  return variables;
}

Alat::StringSet MatrixAllVariables::getVariablesOut() const
{
  Alat::StringSet variables;
  for(MatrixAllVariables::const_iterator p = this->begin(); p != this->end(); p++)
  {
    variables.insert(p->first.first);
  }
  return variables;
}

Alat::StringSet MatrixAllVariables::getVariablesIn() const
{
  Alat::StringSet variables;
  for(MatrixAllVariables::const_iterator p = this->begin(); p != this->end(); p++)
  {
    variables.insert(p->first.second);
  }
  return variables;
}

/*--------------------------------------------------------------------------*/
const Alat::VariableMatrixInterface*  MatrixAllVariables::getMatrix(std::string varOut, std::string varIn) const
{
  return ( *this )[ Alat::makePair(varOut, varIn) ];
}

Alat::VariableMatrixInterface*  MatrixAllVariables::getMatrix(std::string varOut, std::string varIn)
{
  return ( *this )[ Alat::makePair(varOut, varIn) ];
}

Alat::VariableMatrixInterface*&  MatrixAllVariables::getMatrixPointer(std::string varOut, std::string varIn)
{
  return ( *this )[ Alat::makePair(varOut, varIn) ];
}

/*--------------------------------------------------------------------------*/
void MatrixAllVariables::set_size(const Alat::StringSet& variables)
{
  for(Alat::StringSet::const_iterator p = variables.begin(); p != variables.end(); p++)
  {
    getMatrixPointer(*p, *p) = NULL;
  }
}

/*--------------------------------------------------------------------------*/
void MatrixAllVariables::set_size(const Alat::StringSet& variablesOut, const Alat::StringSet& variablesIn)
{
  for(Alat::StringSet::const_iterator p = variablesOut.begin(); p != variablesOut.end(); p++)
  {
    for(Alat::StringSet::const_iterator q = variablesIn.begin(); q != variablesIn.end(); q++)
    {
      getMatrixPointer(*p, *q) = NULL;
    }
  }
  // std::cerr << " MatrixAllVariables::reInit( nvars mvars " << nvars << " " << mvars << "\n";
  // Alat::Matrix<VariableMatrixInterface*>::set_size(nvars, mvars);
}

/*--------------------------------------------------------------------------*/
std::ostream& Alat::operator<<(std::ostream& os, const MatrixAllVariables& v)
{
  Alat::StringSet variablesOut = v.getVariablesOut();
  Alat::StringSet variablesIn = v.getVariablesIn();
  for(Alat::StringSet::const_iterator p = variablesOut.begin(); p != variablesOut.end(); p++)
  {
    for(Alat::StringSet::const_iterator q = variablesIn.begin(); q != variablesIn.end(); q++)
    {
      os << *p << "-" << *q << ":\n";
      v.getMatrix(*p, *q)->write(os);
    }
  }
  return os;
}

/*--------------------------------------------------------------------------*/
std::ostream& MatrixAllVariables::write(std::ostream& os) const
{
  os << *this;
  return os;
}

/*--------------------------------------------------------------------------*/
void MatrixAllVariables::zeros()
{
  for(MatrixAllVariables::iterator p = this->begin(); p != this->end(); p++)
  {
    p->second->zeros();
  }
}

/*--------------------------------------------------------------------------*/
void MatrixAllVariables::addMatrix(const SystemMatrixInterface* matrix, double d)
{
  Alat::StringSet variablesOut = getVariablesOut();
  Alat::StringSet variablesIn = getVariablesIn();
  assert( variablesOut == matrix->getVariablesOut() );
  assert( variablesIn == matrix->getVariablesIn() );
  for(Alat::StringSet::iterator p = variablesOut.begin(); p != variablesOut.end(); p++)
  {
    for(Alat::StringSet::iterator q = variablesIn.begin(); q != variablesIn.end(); q++)
    {
      getMatrix(*p, *q)->addMatrix(matrix->getMatrix(*p, *q), d);
    }
  }
}

/*--------------------------------------------------------------------------*/
void MatrixAllVariables::matrixVectorProduct(Alat::SystemVector* out, const Alat::SystemVector* in, double d) const
{
  Alat::StringSet varsout = out->getVariables();
  Alat::StringSet varsin = in->getVariables();
  for(Alat::StringSet::const_iterator p = varsout.begin(); p != varsout.end(); p++)
  {
    for(Alat::StringSet::const_iterator q = varsin.begin(); q != varsin.end(); q++)
    {
      // std::cerr << "MatrixAllVariables::matrixVectorProduct() " << *p << " <-- " << *q << "\n";
      getMatrix(*p, *q)->matrixVectorProduct(out->getVector(*p), in->getVector(*q), d);
    }
  }
}
