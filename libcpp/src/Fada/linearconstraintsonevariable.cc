#include  "Fada/linearconstraintsonevariable.h"
#include  <cassert>
#include  <fstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
LinearConstraintsOneVariable::~LinearConstraintsOneVariable()
{
}

LinearConstraintsOneVariable::LinearConstraintsOneVariable()
{
}

LinearConstraintsOneVariable::LinearConstraintsOneVariable( const LinearConstraintsOneVariable& linearconstraintsonevariable)
{
(*this).operator=(linearconstraintsonevariable);
}

LinearConstraintsOneVariable& LinearConstraintsOneVariable::operator=( const LinearConstraintsOneVariable& linearconstraintsonevariable)
{assert(0);
return *this;
}

std::string LinearConstraintsOneVariable::getName() const
{
return "LinearConstraintsOneVariable";
}

LinearConstraintsOneVariable* LinearConstraintsOneVariable::clone() const
{
return new LinearConstraintsOneVariable(*this);
}

/*--------------------------------------------------------------------------*/
Alat::SparseMatrixWithRowIndex& LinearConstraintsOneVariable::getConstraints()
{
  return _smwi;
}
void LinearConstraintsOneVariable::clear()
{
  getConstraints().clear();
  getIndices().clear();
}

Alat::IntVector& LinearConstraintsOneVariable::getIndices()
{
  return _index;
}

const Alat::SparseMatrixWithRowIndex& LinearConstraintsOneVariable::getConstraints() const
{
  return _smwi;
}

const Alat::IntVector& LinearConstraintsOneVariable::getIndices() const
{
  return _index;
}

void LinearConstraintsOneVariable::read(const std::string& filename)
{
  getConstraints().read(filename);
  std::string filename2 = filename+".vector";
  // std::cerr << "***  LinearConstraintsOneVariable::read() " << filename2 << "\n";
  std::ifstream file(filename2.c_str());
  assert(file.is_open());
  getIndices().loadFada(file);
  file.close();
}
void LinearConstraintsOneVariable::write(const std::string& filename, const std::string& type) const
{
  getConstraints().write(filename, type);
  std::string filename2 = filename+".vector";
  // std::cerr << "***  LinearConstraintsOneVariable::write() " << filename2 << "\n";
  std::ofstream file(filename2.c_str());
  assert(file.is_open());
  getIndices().saveFada(file, type);
  file.close();
}


std::ostream& Fada::operator<<( std::ostream& s, const LinearConstraintsOneVariable& A )
{
  s<<A.getConstraints()<<"\n"<<A.getIndices()<<"\n";
  return s;
}

void LinearConstraintsOneVariable::constructIndices(int n)
{
  Alat::IntVector& indices = getIndices();
  indices.set_size(n);
  indices = -1;
  // const SparsityPatternWithRowIndex& SP = getConstraints().getSparsityPatternWithRowIndex();
  const Alat::SparsityPatternWithRowIndex* SP = _smwi.getSparsityPatternWithRowIndex();
  for(int ii = 0; ii < SP->n(); ii++)
  {
    int i = SP->row(ii);
    indices[i] = ii;
  }
}
bool LinearConstraintsOneVariable::isConstrained(int i) const
{
  return _index[i] != -1;
}

int LinearConstraintsOneVariable::getIndex(int i) const
{
  return _index[i];
}
