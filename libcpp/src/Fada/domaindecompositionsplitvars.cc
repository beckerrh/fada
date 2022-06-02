#include  "Fada/chronometer.h"
#include  "Fada/domainsolverinterface.h"
#include  "Alat/iterativesolvervisitorinterface.h"
#include  "Fada/domaindecompositionsplitvars.h"
#include  "Fada/solvermanagerinterface.h"
#include  "Fada/visitorsolvermanageronelevel.h"
#include  "Alat/intvector.h"
#include  "Alat/tokenize.h"
#include  <cassert>
#include  <sstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
DomainDecompositionSplitVars::~DomainDecompositionSplitVars()
{}

DomainDecompositionSplitVars::DomainDecompositionSplitVars(std::string type, int level, Fada::SolverManagerInterface* solvermanager, const Alat::GhostMatrix& ghostmatrix, std::string variables) : Alat::Preconditioner(), _type(type), _level(level), _solvermanager(solvermanager), _ghostmatrix(ghostmatrix), _variables(variables)
{}

DomainDecompositionSplitVars::DomainDecompositionSplitVars( const DomainDecompositionSplitVars& preconditionerdomaindecomposition)
{
  assert(0);
}

DomainDecompositionSplitVars& DomainDecompositionSplitVars::operator=( const DomainDecompositionSplitVars& preconditionerdomaindecomposition)
{
  Alat::Preconditioner::operator=(preconditionerdomaindecomposition);
  assert(0);
  return *this;
}

std::string DomainDecompositionSplitVars::getName() const
{
  return "DomainDecompositionSplitVars";
}

DomainDecompositionSplitVars* DomainDecompositionSplitVars::clone() const
{
  return new DomainDecompositionSplitVars(*this);
}

/*--------------------------------------------------------------------------*/
int DomainDecompositionSplitVars::getNVectors() const
{
  return 3*_nvariables;
}

/*--------------------------------------------------------------------------*/
void DomainDecompositionSplitVars::basicInit(const Alat::ParameterFile* parameterfile, std::string blockname, Alat::IterativeSolverVisitorInterface* visitor)
{  
  assert(0);
  // std::cerr << "DomainDecompositionSplitVars::basicInit()  _level="<<_level<<"\n";
  // std::cerr << "DomainDecompositionSplitVars::basicInit()  _variables="<<_variables<<"\n";
  // Alat::StringVector bouts = Alat::Tokenize(_variables, "|");
  // int nbouts = bouts.size();
  // _variablesvector.set_size(nbouts);
  // _nvariables = nbouts;
  // for(int ibout = 0; ibout < nbouts; ibout++)
  // {
  //   Alat::StringVector petitsbouts = Alat::Tokenize(bouts[ibout], ":");
  //   // std::cerr << "petitsbouts="<<petitsbouts<<"\n";
  //   int npetitsbouts = petitsbouts.size();
  //   assert(npetitsbouts);
  //   _variablesvector[ibout].set_size(npetitsbouts);
  //   for(int ipetitbout = 0; ipetitbout < npetitsbouts; ipetitbout++)
  //   {
  //     _variablesvector[ibout][ipetitbout] = petitsbouts[ipetitbout];
  //   }
  // }
  // std::cerr << "DomainDecompositionSplitVars::basicInit() _variablesvector="<<_variablesvector<<"\n";
  // _domainlinearsolvers.set_size(nbouts);
  // for(int ibout = 0; ibout < nbouts; ibout++)
  // {
  //   std::stringstream ss;
  //   ss<<"domainsolver_"<<_level<<"_"<<ibout;
  //   _domainlinearsolvers[ibout].setName( ss.str() );
  //   _domainlinearsolvers[ibout].setVariables(_variablesvector[ibout]);
  //   _domainlinearsolvers[ibout].setMatrix(_ghostmatrix);
  //   std::cerr << "DomainDecompositionSplitVars::basicInit() adding " << _domainlinearsolvers[ibout] << "\n";
  //   _solvermanager->addLinearDomainSolvers(_domainlinearsolvers[ibout]);
  // }
  // // std::cerr << "_domainlinearsolvers="<<_domainlinearsolvers<<"\n";
  // Preconditioner::basicInit(parameterfile, blockname, visitor);
  // 
  // 
  // for(int i=0;i<getNVectors();i++)
  // {
  //   // std::cerr << "DomainDecompositionSplitVars::basicInit() i="<<i<<" "<<getMemory(i)<<"\n";
  // }                                                                                              
}

/*--------------------------------------------------------------------------*/
const Alat::StringVector& DomainDecompositionSplitVars::variablesOfVector(int i) const
{
  // std::cerr << " DomainDecompositionSplitVars::variablesOfVector() i="<<i<<" "<< _variablesvector[i%_nvariables]<<"\n";
  return _variablesvector[i%_nvariables];
}

void DomainDecompositionSplitVars::reInit()
{}
void DomainDecompositionSplitVars::computePreconditioner()
{}

/*--------------------------------------------------------------------------*/
void DomainDecompositionSplitVars::setsmoothtype(std::string smoothtype)
{
  // assert(0);
  // _smoothtype = smoothtype;
  // Fada::VisitorSolverManagerOneLevel* visitor = dynamic_cast<Fada::VisitorSolverManagerOneLevel*>( getVisitor() );
  // assert(visitor);
  // visitor->setsmoothtype(_linearsolverdomain, smoothtype);
}

/*--------------------------------------------------------------------------*/
void DomainDecompositionSplitVars::solveApproximate(AlatEnums::iterationstatus& status, const Alat::GhostMatrix& A, Alat::GhostVector& u, const Alat::GhostVector& f, int iteration) const
{
  assert(0);
  // const Alat::IntVector& permutation = getVisitor()->getDomainsPermutation(iteration);
  // for(int ivariables = 0; ivariables < _variablesvector.size(); ivariables++)
  // {
  //   Alat::GhostVector& hf = getMemory(ivariables);
  //   Alat::GhostVector& hv = getMemory(_nvariables+ivariables);
  //   Alat::GhostVector& hu = getMemory(2*_nvariables+ivariables);
  //   // std::cerr << "hf="<<hf << " hv="<<hv<<" hu="<<hu<<"\n";
  //   
  //   getVisitor()->vectorCopy(hf, f);
  //   getVisitor()->vectorCopy(hu, u);
  //   for(int jvariables = 0; jvariables < ivariables; jvariables++)
  //   {
  //     Alat::GhostVector& huj = getMemory(2*_nvariables+jvariables);
  //     // std::cerr << "hf="<<hf <<" huj="<<huj<<"\n";
  //     getVisitor()->matrixVectorProduct(_ghostmatrix, hf, huj, -1.0, _variablesvector[jvariables]);
  //   }    
  //   for(int p = 0; p < permutation.size(); p++)
  //   {
  //     int i = permutation[p];
  //     getVisitor()->vectorEqualOnDomain(i, hv, hf, _variablesvector[ivariables]);
  //     assert(0);
  //     // getVisitor()->matrixVectorProductCoupling(i, A, hv, hu, -1.0, _variablesvector[ivariables]);
  //     getVisitor()->solveOnDomain(i, _domainlinearsolvers[ivariables], _ghostmatrix, hu, hv, _variablesvector[ivariables]);
  //   }
  //   getVisitor()->vectorCopyBack(u, hu);
  // }
  // assert(0);
//   // std::cerr << "DomainDecompositionSplitVars::solveApproximate() _type="<<_type<< " _level="<<_level<< "\n";
// //  Alat::GhostVector& w = getMemory(1);
//
//   const Alat::IntVector& permutation = getVisitor()->getDomainsPermutation(iteration);
//   // std::cerr << "permutation="<<permutation<<"\n";
//
//   Alat::GhostVector& v = getMemory(0);
//   if(_type == "jacobi")
//   {
//     Alat::GhostVector& w = getMemory(1);
//     for(int i = 0; i < permutation.size(); i++)
//     {
//       getVisitor()->solveOnDomain(i, _linearsolverdomain, _ghostmatrix, v, f);
//     }
//     for(int i = 0; i < permutation.size(); i++)
//     {
//       getVisitor()->vectorEqualOnDomain(i, w, f);
//       getVisitor()->matrixVectorProductCoupling(i, A, w, v, -1.0);
//       getVisitor()->solveOnDomain(i, _linearsolverdomain, _ghostmatrix, u, w);
//     }
//   }
//   else if(_type == "gs")
//   {
//     for(int p = 0; p < permutation.size(); p++)
//     {
//       int i = permutation[p];
//       getVisitor()->vectorEqualOnDomain(i, v, f);
//       getVisitor()->matrixVectorProductCoupling(i, A, v, u, -1.0);
//       getVisitor()->solveOnDomain(i, _linearsolverdomain, _ghostmatrix, u, v);
//     }
//   }
//   else if(_type == "ags")
//   {
//     if(_smoothtype == "pre")
//     {
//       for(int p = 0; p < permutation.size(); p++)
//       {
//         int i = permutation[p];
//         getVisitor()->vectorEqualOnDomain(i, v, f);
//         getVisitor()->matrixVectorProductCoupling(i, A, v, u, -1.0);
//         getVisitor()->solveOnDomain(i, _linearsolverdomain, _ghostmatrix, u, v);
//       }
//     }
//     else
//     {
//       for(int p = permutation.size()-1; p >= 0; p--)
//       {
//         int i = permutation[p];
//         getVisitor()->vectorEqualOnDomain(i, v, f);
//         getVisitor()->matrixVectorProductCoupling(i, A, v, u, -1.0);
//         getVisitor()->solveOnDomain(i, _linearsolverdomain, _ghostmatrix, u, v);
//       }
//     }
//   }
//   else if(_type == "sgs")
//   {
//     Alat::GhostVector& v = getMemory(0);
//     for(int p = 0; p < permutation.size(); p++)
//     {
//       int i = permutation[p];
//       getVisitor()->vectorEqualOnDomain(i, v, f);
//       getVisitor()->matrixVectorProductCoupling(i, A, v, u, -1.0);
//       getVisitor()->solveOnDomain(i, _linearsolverdomain, _ghostmatrix, u, v);
//     }
//     for(int p = permutation.size()-1; p >= 0; p--)
//     {
//       int i = permutation[p];
//       getVisitor()->vectorEqualOnDomain(i, v, f);
//       getVisitor()->matrixVectorProductCoupling(i, A, v, u, -1.0);
//       getVisitor()->solveOnDomain(i, _linearsolverdomain, _ghostmatrix, u, v);
//     }
//   }
//   else
//   {
//     assert(0);
//   }
}
