#include  "Fada/dofinformationinterface.h"
#include  "Fada/dofmanagerallvariables.h"
#include  "Alat/matrixallvariables.h"
#include  "Fada/variablemanager.h"
#include  "Alat/variablevector.h"
#include  "Alat/systemvector.h"
#include  "FadaMesh/multilevelmesh.h"
#include  "Alat/doublematrix.h"
#include  "Alat/stringset.h"
#include  "Fada/unstructuredmultilevelmatrixtransfer.h"
#include  "Alat/variablematrix.h"
#include  <cassert>
#include  "Alat/armadillo.h"

using namespace Fada;

/*--------------------------------------------------------------------------*/
UnstructuredMultiLevelMatrixTransfer::~UnstructuredMultiLevelMatrixTransfer(){}
UnstructuredMultiLevelMatrixTransfer::UnstructuredMultiLevelMatrixTransfer() : Fada::MultiLevelTransferAllVariablesInterface(){}
UnstructuredMultiLevelMatrixTransfer::UnstructuredMultiLevelMatrixTransfer( const UnstructuredMultiLevelMatrixTransfer& multilevelmatrixtransfer) : Fada::MultiLevelTransferAllVariablesInterface(multilevelmatrixtransfer)
{
  assert(0);
}

UnstructuredMultiLevelMatrixTransfer& UnstructuredMultiLevelMatrixTransfer::operator=( const UnstructuredMultiLevelMatrixTransfer& multilevelmatrixtransfer)
{
  Fada::MultiLevelTransferAllVariablesInterface::operator=(multilevelmatrixtransfer);
  assert(0);
  return *this;
}

std::string UnstructuredMultiLevelMatrixTransfer::getName() const
{
  return "UnstructuredMultiLevelMatrixTransfer";
}

UnstructuredMultiLevelMatrixTransfer* UnstructuredMultiLevelMatrixTransfer::clone() const
{
  return new UnstructuredMultiLevelMatrixTransfer(*this);
}

/*--------------------------------------------------------------------------*/
void UnstructuredMultiLevelMatrixTransfer::basicInit(const Fada::DomainSolverInterface* domainsolver, const FadaMesh::MeshInterface* mesh, const Fada::VariableManager& variablemanager, const Fada::DofManagerAllVariables* dofmanagerallvariables)
{
  assert(0);
}

void UnstructuredMultiLevelMatrixTransfer::prolongate(int level, Alat::SystemVectorInterface* ufine, const Alat::SystemVectorInterface* ucoarse, double d) const
{
  assert(0);
}
void UnstructuredMultiLevelMatrixTransfer::restrict(int level, Alat::SystemVectorInterface* ucoarse, const Alat::SystemVectorInterface* ufine ) const
{
  assert(0);
}
void UnstructuredMultiLevelMatrixTransfer::project(int level, Alat::SystemVectorInterface* ucoarse, const Alat::SystemVectorInterface* ufine) const
{
  assert(0);
}
void UnstructuredMultiLevelMatrixTransfer::interpolate(int level, Alat::SystemVectorInterface* ufine, const Alat::SystemVectorInterface* ucoarse) const
{
  assert(0);
}
void UnstructuredMultiLevelMatrixTransfer::constructTransfer(int level, const Alat::SystemMatrixInterface* matrixfine, const Alat::SystemMatrixInterface* matrixcoarse)
{
  assert(0);
}

bool UnstructuredMultiLevelMatrixTransfer::isConstant() const
{
  return false;
}
//
// /*--------------------------------------------------------------------------*/
// void UnstructuredMultiLevelMatrixTransfer::basicInit(const FadaMesh::MeshInterface* mesh, const Fada::VariableManager& variablemanager, const Fada::DofManagerAllVariables* dofmanagerallvariables)
// {
//   // UnstructuredMultiLevelTransfer::basicInit(mesh, variablemanager, dofmanagerallvariables);
//   assert(0);
//   // const FadaMesh::MultiLevelMesh* mlmesh = dynamic_cast<const FadaMesh::MultiLevelMesh*>( mesh );
//   // assert(mlmesh);
//   // int nlevels = mesh->getNLevels();
//   // Alat::StringSet variables = variablemanager.getUnknownsNames();
//   // int nvars = variables.size();
//   // _matrix.set_size(nlevels-1);
//   // const Alat::StringIntMap& varnametoindex = dofmanagerallvariables->getVarnameToIndex();
//   // Alat::IntSet dofsunknowns;
//   // for(Alat::StringIntMap::const_iterator p = varnametoindex.begin(); p != varnametoindex.end(); p++)
//   // {
//   //   int ivar = variablemanager.getIndexOfUnknownVariable(p->first);
//   //   if(ivar >= 0)
//   //   {
//   //     dofsunknowns.insert(p->second);
//   //   }
//   // }
//   // _offsets.set_size(nvars);
//   // _ncomps.set_size(nvars);
//   // _ncompsum = 0;
//   // for(int ivar = 0; ivar < nvars; ivar++)
//   // {
//   //   int ncomp = variablemanager.getUnknown(ivar)->getNComponents();
//   //   _ncomps[ivar] = ncomp;
//   //   _ncompsum += ncomp;
//   // }
//   // _offsets[0] = 0;
//   // for(int ivar = 1; ivar < nvars; ivar++)
//   // {
//   //   _offsets[ivar] = _offsets[ivar-1] + _ncomps[ivar-1];
//   // }
//   //
//   // int ndofs = dofsunknowns.size();
//   // assert(ndofs == 1);
//   // const Fada::DofInformationInterface* dofinfo = dofmanagerallvariables->getDofInformationOfIndex(0);
//   // assert(dofinfo);
//   // assert( dofinfo->getFem() );
//   // for(int level = 0; level < nlevels-1; level++)
//   // {
//   //   Alat::MatrixAllVariables* matrixallvariables = new Alat::MatrixAllVariables;
//   //   _matrix.getMatrixPointer(level) = matrixallvariables;
//   //   matrixallvariables->set_size(variables, variables);
//   //
//   //   const FadaMesh::MeshInterface* coarsemesh = mlmesh->getMesh(level+1);
//   //   const FadaMesh::MeshInterface* finemesh = mlmesh->getMesh(level);
//   //   int ncells = coarsemesh->getNCells();
//   //   Alat::IntVector olddof, newdof;
//   //   int ndofold = dofinfo->ndof(coarsemesh);
//   //   Alat::SparsityPatternSoft sparsitypatternsoft(ndofold);
//   //
//   //   const Fada::SparseMatrix& matproj = _matrixtransfer[0][level];
//   //   const Alat::SparsityPattern* sparsitypattern = matproj.getSparsityPattern();
//   //
//   //   for(int i = 0; i < sparsitypattern->n(); i++)
//   //   {
//   //     for(int pos = sparsitypattern->rowstart(i); pos < sparsitypattern->rowstop(i); pos++)
//   //     {
//   //       sparsitypatternsoft[sparsitypattern->col(pos)].insert(i);
//   //     }
//   //   }
//   //   for(Alat::StringSet::const_iterator p = variables.begin(); p != variables.end(); p++)
//   //   {
//   //     int ncompi = variablemanager.getUnknown(*p)->getNComponents();
//   //     for(Alat::StringSet::const_iterator q = variables.begin(); q != variables.end(); q++)
//   //     {
//   //       int ncompj = variablemanager.getUnknown(*q)->getNComponents();
//   //       Alat::VariableMatrix* levelmatrix = new Alat::VariableMatrix(ncompi, ncompj, "full");
//   //       matrixallvariables->getMatrixPointer(*p, *q) = levelmatrix;
//   //       levelmatrix->set_size(sparsitypatternsoft);
//   //       matrixallvariables->getMatrix(*p, *q)->zeros();
//   //     }
//   //   }
//   //   //tous pareil
//   //   const Alat::SparsityPattern* sparsitypatternR = matrixallvariables->getMatrix(0, 0)->getSparsityPattern();
//   //   for(int i = 0; i < sparsitypattern->n(); i++)
//   //   {
//   //     for(int pos = sparsitypattern->rowstart(i); pos < sparsitypattern->rowstop(i); pos++)
//   //     {
//   //       int j = sparsitypattern->col(pos);
//   //       for(int posR = sparsitypatternR->rowstart(j); posR < sparsitypatternR->rowstop(j); posR++)
//   //       {
//   //         if(sparsitypatternR->col(posR) == i)
//   //         {
//   //           for(Alat::StringSet::const_iterator p = variables.begin(); p != variables.end(); p++)
//   //           {
//   //             for(int icomp = 0; icomp < _ncomps[ivar]; icomp++)
//   //             {
//   //               matrixallvariables->getMatrix(*p, *p)->getValue(icomp, icomp, posR) = matproj.getValue(pos);
//   //             }
//   //           }
//   //         }
//   //       }
//   //     }
//   //   }
//   // }
// }
//
// /*--------------------------------------------------------------------------*/
// bool UnstructuredMultiLevelMatrixTransfer::isConstant() const
// {
//   return false;
// }
//
// /*--------------------------------------------------------------------------*/
// void UnstructuredMultiLevelMatrixTransfer::restrict(int level, Alat::SystemVector* u, const Alat::SystemVector* f ) const
// {
//   u->zeros();
//   assert(0);
//   // _matrix.getMatrix(level-1)->matrixVectorProduct(u, f);
// }
// /*--------------------------------------------------------------------------*/
// void UnstructuredMultiLevelMatrixTransfer::constructTransfer(int level, const Alat::SystemMatrixInterface* matrixfine, const Alat::SystemMatrixInterface* matrixcoarse)
// {
//   Alat::SystemMatrixInterface* systemmatrix = _matrix.getMatrix(level);
//   assert(systemmatrix);
//   Alat::StringSet variables = systemmatrix->getVariables();
//   int nvars = variables.size();
//   assert(matrixfine->getVariables() == variables);
//   assert(matrixcoarse->getVariables() == variables);
//   // assert(nvars == 1);
//
//   assert(0);
// //   Alat::Matrix<Alat::VariableMatrix*> matrixR(nvars);
// //   Alat::Matrix<const Alat::VariableMatrix*> matrixh(nvars);
// //   Alat::Matrix<const Alat::VariableMatrix*> matrixH(nvars);
// //   for(Alat::StringSet::const_iterator p = variables.begin(); p != variables.end(); p++)
// //   {
// //     for(Alat::StringSet::const_iterator q = variables.begin(); q != variables.end(); q++)
// //     {
// //       matrixR(ivar, jvar) =  dynamic_cast<Alat::VariableMatrix*>( systemmatrix->getMatrix(ivar, jvar) );
// //       matrixh(ivar, jvar) =  dynamic_cast<const Alat::VariableMatrix*>( matrixfine->getMatrix(ivar, jvar) );
// //       matrixH(ivar, jvar) =  dynamic_cast<const Alat::VariableMatrix*>( matrixcoarse->getMatrix(ivar, jvar) );
// //       assert( matrixR(ivar, jvar) );
// //       assert( matrixh(ivar, jvar) );
// //       assert( matrixH(ivar, jvar) );
// //     }
// //   }
// //   //tous pareil
// //   const Alat::SparsityPattern* sparsitypatternR = matrixR(0, 0)->getSparsityPattern();
// //   const Alat::SparsityPattern* sparsitypatternH = matrixH(0, 0)->getSparsityPattern();
// //   const Alat::SparsityPattern* sparsitypatternh = matrixh(0, 0)->getSparsityPattern();
// //
// //   arma::mat S, R, B, R0, RES;
// //
// //   const Fada::SparseMatrix& matproj = _matrixtransfer[0][level];
// //   const Alat::SparsityPattern* sparsitypatternP = matproj.getSparsityPattern();
// //
// //   int nH = sparsitypatternR->n();
// //   assert( nH == sparsitypatternH->n() );
// //   for(int i = 0; i < nH; i++)
// //   {
// //     int ni = sparsitypatternR->rowsize(i);
// //     assert( ni == sparsitypatternH->rowsize(i) );
// //     int nligne = _ncompsum*ni;
// //     Alat::IntVector colH(ni);
// //     int count = 0;
// //     for(int pos = sparsitypatternH->rowstart(i); pos < sparsitypatternH->rowstop(i); pos++)
// //     {
// //       colH[count++] = sparsitypatternH->col(pos);
// //     }
// //     Alat::IntMap colHInv;
// //     for(int ii = 0; ii < colH.size(); ii++)
// //     {
// //       colHInv[colH[ii]] = ii;
// //     }
// //
// //     S.resize(nligne, nligne);
// //     R.resize(nligne, _ncompsum);
// //     B.resize(nligne, _ncompsum);
// //     RES.resize(nligne, _ncompsum);
// //     R0.resize(nligne, _ncompsum);
// //     S.fill(arma::fill::zeros);
// //     RES.fill(arma::fill::zeros);
// //     R0.fill(arma::fill::zeros);
// //     B.fill(arma::fill::zeros);
// //
// //     count = 0;
// //     for(int pos = sparsitypatternH->rowstart(i); pos < sparsitypatternH->rowstop(i); pos++)
// //     {
// //       for(int ivar = 0; ivar < nvars; ivar++)
// //       {
// //         int ncompi = _ncomps[ivar];
// //         for(int icomp = 0; icomp < ncompi; icomp++)
// //         {
// //           int indexi = _offsets[ivar] + icomp;
// //           for(int jvar = 0; jvar < nvars; jvar++)
// //           {
// //             int ncompj = _ncomps[jvar];
// //             for(int jcomp = 0; jcomp < ncompj; jcomp++)
// //             {
// //               int indexj = _offsets[jvar] + jcomp;
// //               B(_ncompsum*count+indexj, indexi) = matrixH(ivar, jvar)->getValue(icomp, jcomp, pos);
// //             }
// //           }
// //         }
// //       }
// //       count++;
// //     }
// //
// //     count = 0;
// //     for(int pos = sparsitypatternR->rowstart(i); pos < sparsitypatternR->rowstop(i); pos++)
// //     {
// //       for(int ivar = 0; ivar < nvars; ivar++)
// //       {
// //         int ncompi = _ncomps[ivar];
// //         for(int icomp = 0; icomp < ncompi; icomp++)
// //         {
// //           int indexi = _offsets[ivar] + icomp;
// //           for(int jvar = 0; jvar < nvars; jvar++)
// //           {
// //             int ncompj = _ncomps[jvar];
// //             for(int jcomp = 0; jcomp < ncompj; jcomp++)
// //             {
// //               int indexj = _offsets[jvar] + jcomp;
// //               R0(_ncompsum*count+indexj, indexi) = matrixR(ivar, jvar)->getValue(icomp, jcomp, pos);
// //             }
// //           }
// //         }
// //       }
// //       count++;
// //     }
// //
// //     // A_ij = \sigma_kl R_ik a_kl P_lj
// //     count = 0;
// //     for(int posR = sparsitypatternR->rowstart(i); posR < sparsitypatternR->rowstop(i); posR++)
// //     {
// //       int k = sparsitypatternR->col(posR);
// //       for(int posh = sparsitypatternh->rowstart(k); posh < sparsitypatternh->rowstop(k); posh++)
// //       {
// //         int l = sparsitypatternh->col(posh);
// //         for(int posP = sparsitypatternP->rowstart(l); posP < sparsitypatternP->rowstop(l); posP++)
// //         {
// //           double Plm = matproj.getValue(posP);
// //           int m = sparsitypatternP->col(posP);
// //           if( colHInv.find(m) != colHInv.end() )
// //           {
// //             int jj = colHInv[m];
// //             for(int jvar = 0; jvar < nvars; jvar++)
// //             {
// //               int ncompj = _ncomps[jvar];
// //               for(int jcomp = 0; jcomp < ncompj; jcomp++)
// //               {
// //                 int indexj = _offsets[jvar] + jcomp;
// //                 for(int kvar = 0; kvar < nvars; kvar++)
// //                 {
// //                   int ncompk = _ncomps[kvar];
// //                   for(int kcomp = 0; kcomp < ncompk; kcomp++)
// //                   {
// //                     int indexk = _offsets[kvar] + kcomp;
// //                     S(_ncompsum*jj+indexj, _ncompsum*count+indexk) += Plm*matrixh(kvar, jvar)->getValue(kcomp, jcomp, posh);
// //                   }
// //                 }
// //               }
// //             }
// //           }
// //         }
// //       }
// //       count++;
// //     }
// //
// //     RES = B - S*R0;
// //
// //     // std::cerr << "R0="<<R0.t();
// //     // std::cerr << "B="<<B.t();
// //     // std::cerr << "S="<<S;
// //     // std::cerr << "RES="<<RES.t();
// //
// //     if( arma::norm(RES) < 1e-10)
// //     {
// //       std::cerr << "#";
// //       continue;
// //     }
// //     if(arma::rank(S) == nligne)
// //     {
// //       std::cerr << ".";
// //       assert( arma::solve(R, S, B) );
// //       // R = R0;
// //     }
// //     else
// //     {
// //       std::cerr << "@"<<arma::rank(S)<<"("<<nligne<<")";
// //       arma::mat Q = arma::pinv(S, 1e-10);
// //       // arma::mat Q = arma::pinv(S);
// //       R = R0 + Q*RES;
// //
// //       RES = B - S*R;
// //       if( arma::norm(RES) > 1e-6)
// //       {
// //         std::cerr<< "arma::norm(RES) = " << arma::norm(RES) << "\n";
// //
// //         // std::cerr << "R0="<<R0.t();
// //         std::cerr << "B="<<B.t();
// //         // std::cerr << "S="<<S;
// //         // std::cerr << "RES="<<RES.t();
// //         std::cerr << "R="<<R.t();
// //         // assert(0);
// //       }
// //       // R = R0;
// //     }
// //     count = 0;
// //     for(int pos = sparsitypatternR->rowstart(i); pos < sparsitypatternR->rowstop(i); pos++)
// //     {
// //       for(int ivar = 0; ivar < nvars; ivar++)
// //       {
// //         int ncompi = _ncomps[ivar];
// //         for(int icomp = 0; icomp < ncompi; icomp++)
// //         {
// //           int indexi = _offsets[ivar] + icomp;
// //           for(int jvar = 0; jvar < nvars; jvar++)
// //           {
// //             int ncompj = _ncomps[jvar];
// //             for(int jcomp = 0; jcomp < ncompj; jcomp++)
// //             {
// //               int indexj = _offsets[jvar] + jcomp;
// //               matrixR(ivar, jvar)->getValue(icomp, jcomp, pos) = R(_ncompsum*count+indexj, indexi);
// //             }
// //           }
// //         }
// //       }
// //       count++;
// //     }
// // //   }
// // //
// // // // TEST !!
// // //   for(int i = 0; i < nH; i++)
// // //   {
// //     bool test = 1;
// //     if(test)
// //     {
// //       arma::mat AH(_ncompsum, _ncompsum), Ah(_ncompsum, _ncompsum), PP(_ncompsum, _ncompsum), RR(_ncompsum, _ncompsum), SUM(_ncompsum, _ncompsum);
// //       for(int posH = sparsitypatternH->rowstart(i); posH < sparsitypatternH->rowstop(i); posH++)
// //       {
// //         int j = sparsitypatternH->col(posH);
// //
// //         for(int ivar = 0; ivar < nvars; ivar++)
// //         {
// //           int ncompi = _ncomps[ivar];
// //           for(int icomp = 0; icomp < ncompi; icomp++)
// //           {
// //             int indexi = _offsets[ivar] + icomp;
// //             for(int jvar = 0; jvar < nvars; jvar++)
// //             {
// //               int ncompj = _ncomps[jvar];
// //               for(int jcomp = 0; jcomp < ncompj; jcomp++)
// //               {
// //                 int indexj = _offsets[jvar] + jcomp;
// //                 AH(indexi, indexj)  = matrixH(ivar, jvar)->getValue(icomp, jcomp, posH);
// //               }
// //             }
// //           }
// //         }
// //         SUM.fill(arma::fill::zeros);
// //
// //         for(int posR = sparsitypatternR->rowstart(i); posR < sparsitypatternR->rowstop(i); posR++)
// //         {
// //           int k = sparsitypatternR->col(posR);
// //           for(int ivar = 0; ivar < nvars; ivar++)
// //           {
// //             int ncompi = _ncomps[ivar];
// //             for(int icomp = 0; icomp < ncompi; icomp++)
// //             {
// //               int indexi = _offsets[ivar] + icomp;
// //               for(int jvar = 0; jvar < nvars; jvar++)
// //               {
// //                 int ncompj = _ncomps[jvar];
// //                 for(int jcomp = 0; jcomp < ncompj; jcomp++)
// //                 {
// //                   int indexj = _offsets[jvar] + jcomp;
// //                   RR(indexi, indexj) = matrixR(ivar, jvar)->getValue(icomp, jcomp, posR);
// //                 }
// //               }
// //             }
// //           }
// //           for(int posh = sparsitypatternh->rowstart(k); posh < sparsitypatternh->rowstop(k); posh++)
// //           {
// //             int l = sparsitypatternh->col(posh);
// //             for(int ivar = 0; ivar < nvars; ivar++)
// //             {
// //               int ncompi = _ncomps[ivar];
// //               for(int icomp = 0; icomp < ncompi; icomp++)
// //               {
// //                 int indexi = _offsets[ivar] + icomp;
// //                 for(int jvar = 0; jvar < nvars; jvar++)
// //                 {
// //                   int ncompj = _ncomps[jvar];
// //                   for(int jcomp = 0; jcomp < ncompj; jcomp++)
// //                   {
// //                     int indexj = _offsets[jvar] + jcomp;
// //                     Ah(indexi, indexj) = matrixh(ivar, jvar)->getValue(icomp, jcomp, posh);
// //                   }
// //                 }
// //               }
// //             }
// //
// //             PP.fill(arma::fill::zeros);
// //             for(int posP = sparsitypatternP->rowstart(l); posP < sparsitypatternP->rowstop(l); posP++)
// //             {
// //               int m = sparsitypatternP->col(posP);
// //
// //               for(int ivar = 0; ivar < nvars; ivar++)
// //               {
// //                 int ncompi = _ncomps[ivar];
// //                 for(int icomp = 0; icomp < ncompi; icomp++)
// //                 {
// //                   int indexi = _offsets[ivar] + icomp;
// //                   PP(indexi, indexi) = matproj.getValue(posP);
// //                 }
// //               }
// //
// //               if(m == j)
// //               {
// //                 SUM += RR*Ah*PP;
// //               }
// //             }
// //           }
// //         }
// //         SUM -= AH;
// //         if(arma::norm(SUM, 2) > 1e-10)
// //         {
// //           std::cerr << "AH="<<AH<<" R*Ah*P="<<RR*Ah*PP<<"\n";
// //           std::cerr << "B="<<B<<"\n";
// //           std::cerr << "R="<<R<<"\n";
// //           std::cerr << "R0="<<R0<<"\n";
// //           assert(0);
// //         }
// //       }
// //     }
// //   }
// }
