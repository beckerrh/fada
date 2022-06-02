#include  "Alat/variablematrixinterface.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
VariableMatrixInterface::~VariableMatrixInterface(){}
VariableMatrixInterface::VariableMatrixInterface() : Alat::InterfaceBase(){}
VariableMatrixInterface::VariableMatrixInterface( const Alat::VariableMatrixInterface& variablematrixinterface) : Alat::InterfaceBase(variablematrixinterface){}
VariableMatrixInterface& Alat::VariableMatrixInterface::operator=( const Alat::VariableMatrixInterface& variablematrixinterface)
{
  InterfaceBase::operator=(variablematrixinterface);
  assert(0);
  return *this;
}

std::string Alat::VariableMatrixInterface::getName() const
{
  return "VariableMatrixInterface";
}

std::string Alat::VariableMatrixInterface::getInterfaceName() const
{
  return "VariableMatrixInterface";
}

VariableMatrixInterface* Alat::VariableMatrixInterface::clone() const
{
  assert(0);
//return new Alat::VariableMatrixInterface(*this);
}

/*--------------------------------------------------------------------------*/
int Alat::VariableMatrixInterface::getNComponents() const
{
  _notWritten("getNComponents");
}

int Alat::VariableMatrixInterface::getMComponents() const
{
  _notWritten("getMComponents");
}

/*--------------------------------------------------------------------------*/
// void Alat::VariableMatrixInterface::set_size(const FadaMesh::MeshInterface* mesh, const Alat::DofInformationInterface* dofinformationi, const Alat::DofInformationInterface* dofinformationj)
// {
//   _notWritten("reInit");
// }

void Alat::VariableMatrixInterface::set_size(int n, int ntotal)
{
  _notWritten("reInit");
}

void Alat::VariableMatrixInterface::set_size(const Alat::SparsityPatternSoft& sparsitypatternsoft)
{
  _notWritten("reInit");
}

void Alat::VariableMatrixInterface::set_size(const Alat::SparsityPatternWithRowIndexSoft& sparsitypatternsoft)
{
  _notWritten("reInit");
}

/*--------------------------------------------------------------------------*/
void Alat::VariableMatrixInterface::assemble(const Alat::AssembleMatrix& Aloc, const Alat::IntVector& indices, const Alat::IntVector& jndices, const Alat::DoubleVector& scalei, const Alat::DoubleVector& scalej)
{
  _notWritten("assemble");
}
//
// /*--------------------------------------------------------------------------*/
// void Alat::VariableMatrixInterface::assembleCell(int iK, const Alat::AssembleMatrix& Aloc, const Alat::IntVector& indices, const Alat::IntVector& jndices, double scale)
// {
//   _notWritten("assembleCell");
// }
//
// /*--------------------------------------------------------------------------*/
// void Alat::VariableMatrixInterface::assembleCell(int iKL, int iKR, const Alat::AssembleMatrix& Aloc, const Alat::IntVector& indices, const Alat::IntVector& jndices, double scale)
// {
//   _notWritten("assembleCell iKL, iKR");
// }
//
// /*--------------------------------------------------------------------------*/
// void Alat::VariableMatrixInterface::assembleEdge(int iE, const Alat::AssembleMatrix& Aloc, const Alat::IntVector& indices, const Alat::IntVector& jndices, double scale)
// {
//   _notWritten("assembleEdge");
// }

/*--------------------------------------------------------------------------*/
void Alat::VariableMatrixInterface::matrixVectorProduct(VariableVectorInterface* f, const Alat::VariableVectorInterface* u, double d) const
{
  _notWritten("matrixVectorProduct");
}
void Alat::VariableMatrixInterface::matrixVectorProductTransposed(VariableVectorInterface* f, const Alat::VariableVectorInterface* u, double d) const
{
  _notWritten("matrixVectorProductTransposed");
}

/*--------------------------------------------------------------------------*/
void Alat::VariableMatrixInterface::addMatrix(const Alat::VariableMatrixInterface* sparsematrix, double d)
{
  _notWritten("addMatrix");
}

void Alat::VariableMatrixInterface::addMatrixForDirectSolver(int offsetivar, int offsetjvar, Alat::DoubleVector& matrixvalues, const Alat::SparsityPattern* sparsitypattern) const
{
  _notWritten("addMatrixForDirectSolver");
}

void Alat::VariableMatrixInterface::addEntriesForDirectSolver(int offsetivar, int offsetjvar, Alat::SparsityPatternSoft& sparsitypatternsoft) const
{
  _notWritten("addEntriesForDirectSolver");
}

/*--------------------------------------------------------------------------*/
const Alat::SparsityPattern* Alat::VariableMatrixInterface::getSparsityPattern() const
{
  _notWritten("getSparsityPattern");
}

Alat::SparsityPattern* Alat::VariableMatrixInterface::getSparsityPattern()
{
  _notWritten("getSparsityPattern");
}

/*--------------------------------------------------------------------------*/
int Alat::VariableMatrixInterface::getN() const
{
  _notWritten("getN");
}

bool Alat::VariableMatrixInterface::greaterThan(int pos1, int pos2) const
{
  _notWritten("greaterThan");
}

const double& Alat::VariableMatrixInterface::getValue(int icomp, int jcomp, int pos) const
{
  _notWritten("getValue");
}

double& Alat::VariableMatrixInterface::getValue(int icomp, int jcomp, int pos)
{
  _notWritten("getValue");
}

/*--------------------------------------------------------------------------*/
void Alat::VariableMatrixInterface::zeroLine(int index, int comp)
{
  _notWritten("zeroLine");
}

void Alat::VariableMatrixInterface::setElement(int index, int compi, int compj, double d)
{
  _notWritten("setElement");
}

/*--------------------------------------------------------------------------*/
VariableMatrixInterface* Alat::VariableMatrixInterface::newSchurComplementMatrix(const Alat::VariableMatrixInterface* B, const Alat::VariableMatrixInterface* C, std::string type, std::string storage) const
{
  _notWritten("newSchurComplementMatrix");
}

void Alat::VariableMatrixInterface::computeSchurSparsityPatternSoft(Alat::SparsityPatternSoft& sparsitypatternsoft, const Alat::VariableMatrixInterface* A, const Alat::VariableMatrixInterface* B, const Alat::VariableMatrixInterface* C, const Alat::VariableMatrixInterface* D, bool fullStencil) const
{
  _notWritten("computeSchurSparsityPatternSoft");
}

void Alat::VariableMatrixInterface::computeSchurComplement(const Alat::VariableMatrixInterface* A, const Alat::VariableMatrixInterface* B, const Alat::VariableMatrixInterface* C, const Alat::VariableMatrixInterface* D)
{
  _notWritten("computeSchurComplement");
}

void Alat::VariableMatrixInterface::reInitSchurMatrix(const Alat::VariableMatrixInterface* A, const Alat::VariableMatrixInterface* B, const Alat::VariableMatrixInterface* C, const Alat::VariableMatrixInterface* D, bool fullStencil)
{
  _notWritten("reInitSchurMatrix");
}

/*--------------------------------------------------------------------------*/
void Alat::VariableMatrixInterface::computeSpai(double relax, const Alat::VariableMatrixInterface* variablematrix)
{
  _notWritten("computeSpai");
}

Alat::VariableMatrixInterface* Alat::VariableMatrixInterface::newSpaiMatrixAndReinit(int ncomp, std::string type, std::string storage, const Alat::VariableMatrixInterface* matrix) const
{
  _notWritten("newSpaiMatrixAndReinit");
}

/*--------------------------------------------------------------------------*/
Alat::VariableMatrixInterface* Alat::VariableMatrixInterface::newGaussSeidelMatrixAndReinit(int ncomp, std::string type, std::string storage) const
{
  _notWritten("newGaussSeidelMatrixAndReinit");
}

void Alat::VariableMatrixInterface::computeGaussSeidel(double relax, const Alat::VariableMatrixInterface* variablematrix)
{
  _notWritten("computeGaussSeidel");
}

void Alat::VariableMatrixInterface::gaussSeidel(Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, double relax, const Alat::IntVector& p, const Alat::IntVector& pinv) const
{
  _notWritten("gaussSeidel");
}

/*--------------------------------------------------------------------------*/
Alat::VariableMatrixInterface* Alat::VariableMatrixInterface::newIluMatrixAndReinit(int ncomp, const PreconditionerInterface* preconditioner, std::string type, std::string storage) const
{
  _notWritten("newIluMatrixAndReinit");
}

void Alat::VariableMatrixInterface::computeIlu(double relax, const Alat::VariableMatrixInterface* variablematrix, const Alat::IntVector& p, const Alat::IntVector& pinv)
{
  _notWritten("computeIlu");
}

void Alat::VariableMatrixInterface::ilu(Alat::VariableVectorInterface* u, const Alat::VariableVectorInterface* f, double relax, const Alat::IntVector& p, const Alat::IntVector& pinv) const
{
  _notWritten("ilu");
}
