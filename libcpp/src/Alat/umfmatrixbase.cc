#include  "Alat/sparsitypattern.h"
#include  "Alat/doublevector.h"
#include  "Alat/umfmatrixbase.h"
#include  "stdlib.h"
#include  <cassert>
#include  <fstream>

#define UMFPACK_OK       0
#define UMFPACK_INFO    90
#define UMFPACK_CONTROL 20

#define UMFPACK_A       ( 0 )     /* Ax=b		*/
#define UMFPACK_At      ( 1 )     /* A'x=b	*/

using namespace Alat;

/*--------------------------------------------------------------------------*/

extern "C" int umfpack_di_symbolic
(
  int n,
  int m,
  const int Ap [],
  const int Ai [],
  const double Ax [],
  void** Symbolic,
  const double Control [UMFPACK_CONTROL],
  double Info [UMFPACK_INFO]
);
extern "C" int umfpack_di_numeric
(
  const int Ap [],
  const int Ai [],
  const double Ax [],
  void* Symbolic,
  void** Numeric,
  const double Control [UMFPACK_CONTROL],
  double Info [UMFPACK_INFO]
);
extern "C" int umfpack_di_solve
(
  int sys,
  const int Ap [],
  const int Ai [],
  const double Ax [],
  double X [],
  const double B [],
  void* Numeric,
  const double Control [UMFPACK_CONTROL],
  double Info [UMFPACK_INFO]
);
extern "C" void umfpack_di_free_symbolic(void** Symbolic);
extern "C" void umfpack_di_free_numeric(void** Numeric);
extern "C" void umfpack_di_report_status(const double Control [UMFPACK_CONTROL], int status);
extern "C" void umfpack_di_report_info(const double Control [UMFPACK_CONTROL], const double Info [UMFPACK_INFO]);
extern "C" int umfpack_di_report_numeric(const char name [], void* Numeric, const double Control [UMFPACK_CONTROL]);
extern "C" void umfpack_di_defaults(const double Control [UMFPACK_CONTROL]);

/*--------------------------------------------------------------------------*/
UmfMatrixBase::~UmfMatrixBase()
{
  umfpack_di_free_symbolic (&Symbolic);
  umfpack_di_free_numeric (&Numeric);
  if(Control)
  {
    delete[] Control;
    Control = NULL;
  }
  if(Info)
  {
    delete[] Info;
    Info = NULL;
  }
}

UmfMatrixBase::UmfMatrixBase() : Control(NULL), Info(NULL), Symbolic(NULL), Numeric(NULL)
{
  Control = new double[UMFPACK_CONTROL];
  umfpack_di_defaults(Control);
  Control[0] = 2;
}

UmfMatrixBase::UmfMatrixBase( const UmfMatrixBase& umfmatrixbase) : Control(NULL), Info(NULL), Symbolic(NULL), Numeric(NULL)
{
  Control = new double[UMFPACK_CONTROL];
  umfpack_di_defaults(Control);
  Control[0] = 2;
  // assert(0);
}

UmfMatrixBase& UmfMatrixBase::operator=( const UmfMatrixBase& umfmatrixbase)
{
  assert(0);
  return *this;
}

std::string UmfMatrixBase::getName() const
{
  return "UmfMatrixBase";
}

UmfMatrixBase* UmfMatrixBase::clone() const
{
  assert(0);
//return new UmfMatrixBase(*this);
}

/*--------------------------------------------------------------------------*/
const Alat::SparseMatrix& UmfMatrixBase::getSparseMatrix() const
{
  return _mysparsematrix;
}

Alat::SparseMatrix& UmfMatrixBase::getSparseMatrix()
{
  return _mysparsematrix;
}

/*---------------------------------------------------------*/
std::ostream& UmfMatrixBase::write(std::ostream& os) const
{
  char name[] = "toto";
  umfpack_di_report_numeric(name, (void*) &Numeric, Control);
  return os;
}

/*-------------------------------------------------------------*/
void UmfMatrixBase::reInit()
{
  umfpack_di_free_symbolic (&Symbolic);

  const Alat::SparsityPattern* sparsitypattern = getSparseMatrix().getSparsityPattern();

  int n = sparsitypattern->n();
  const int* sb = &( *sparsitypattern->rowstart().begin() );
  const int* cb = &( *sparsitypattern->col().begin() );

  int status = umfpack_di_symbolic(n, n, sb, cb, NULL, &Symbolic, Control, Info);

  if(status != UMFPACK_OK)
  {
    umfpack_di_report_info(Control, Info);
    umfpack_di_report_status(Control, status);
    std::string datatype("ascii");
    std::ofstream file("SPARSITYPATTERN_NOT_OK");
    sparsitypattern->saveFada(file, datatype);
    std::cerr<<"*** ERROR UmfMatrixBase::reInit(: umfpack_symbolic failed\n";
    assert(0);
    exit(1);
  }
}

/*----------------------------------------------------------*/
void UmfMatrixBase::computeLu()
{
  const Alat::SparsityPattern* sparsitypattern = getSparseMatrix().getSparsityPattern();
  const Alat::DoubleVector& mat = getSparseMatrix().getValue();
  //
  // construct LU
  //
  umfpack_di_free_numeric (&Numeric);
  const int* sb = &( *sparsitypattern->rowstart().begin() );
  const int* cb = &( *sparsitypattern->col().begin() );
  const double* mb = &( mat[0] );
  int status = umfpack_di_numeric(sb, cb, mb, Symbolic, &Numeric, Control, Info);
  if(status != UMFPACK_OK)
  {
    umfpack_di_report_info(Control, Info);
    umfpack_di_report_status(Control, status);
    std::ofstream file("MATRIX_NOT_OK");
    getSparseMatrix().write(file);
    std::cerr<<"*** ERROR UmfMatrixBase::ComputeLu():umfpack_numeric failed\n";
    assert(0);
    exit(1);
  }
  //   umfpack_report_numeric("LU von A\n",Numeric,Control);
}

/*----------------------------------------------------------*/
void UmfMatrixBase::solve(Alat::DoubleVector& x, const Alat::DoubleVector& b) const
{
  const Alat::SparsityPattern* sparsitypattern = getSparseMatrix().getSparsityPattern();
  const Alat::DoubleVector& mat = getSparseMatrix().getValue();
  assert( x.size() == b.size() );
  assert( x.size() == sparsitypattern->n() );
  const int* sb = &( *sparsitypattern->rowstart().begin() );
  const int* cb = &( *sparsitypattern->col().begin() );
  const double* mb = &( mat[0] );
  double* xb = &( *x.begin() );
  const double* bb = &( *b.begin() );
  int status = umfpack_di_solve (UMFPACK_At, sb, cb, mb, xb, bb, Numeric, Control, Info);

  if(status != UMFPACK_OK)
  {
    umfpack_di_report_info(Control, Info);
    umfpack_di_report_status(Control, status);
    std::ofstream file("MATRIX_NOT_OK");
    getSparseMatrix().write(file);
    std::cerr<<"*** ERROR UmfMatrixBase::Solve(): umfpack_di_solve failed\n";
    assert(0);
    exit(1);
  }
}

/*----------------------------------------------------------*/
void UmfMatrixBase::solveTranspose(Alat::DoubleVector& x, const Alat::DoubleVector& b) const
{
  const Alat::SparsityPattern* sparsitypattern = getSparseMatrix().getSparsityPattern();
  const Alat::DoubleVector& mat = getSparseMatrix().getValue();
  const int* sb = &( *sparsitypattern->rowstart().begin() );
  const int* cb = &( *sparsitypattern->col().begin() );
  const double* mb = &( mat[0] );
  double* xb = &( *x.begin() );
  const double* bb = &( *b.begin() );
  int status = umfpack_di_solve (UMFPACK_A, sb, cb, mb, xb, bb, Numeric, Control, Info);

  if(status != UMFPACK_OK)
  {
    umfpack_di_report_info(Control, Info);
    umfpack_di_report_status(Control, status);
    std::cerr<<"*** ERROR UmfMatrixBase::solveTranspose():umfpack_di_solve failed\n";
    assert(0);
    exit(1);
  }
}

#undef UMFPACK_OK
#undef UMFPACK_INFO
#undef UMFPACK_CONTROL

#undef UMFPACK_A
#undef UMFPACK_At
