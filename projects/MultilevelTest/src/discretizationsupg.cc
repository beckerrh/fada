#include  "discretizationsupg.h"
#include  <cassert>
#include  "FadaMesh/meshinterface.h"
#include  "Fada/applicationinterface.h"
#include  "Fada/feminterface.h"
#include  "Fada/integrationformulainterface.h"
#include  "Alat/node.h"
#include  "Fada/righthandsideinterface.h"
#include  "Alat/systemvector.h"
#include  "Alat/systemmatrixinterface.h"
#include  "Alat/variablevector.h"
#include  "Fada/domainsolverinterface.h"
#include  "Fada/domainmodelinterface.h"
#include  "FadaMesh/boundaryinfo.h"
#include  "Fada/dirichletinterface.h"
#include  "Alat/variablematrix.h"
#include  "Alat/armadillo.h"
#include  "Fada/integrationformulaline.h"
#include  "Fada/integrationformulatriangle.h"
#include  "Alat/sparsitypatternsoft.h"
#include  "slopelimiter.h"
#include  "shockcapturing.h"

/*--------------------------------------------------------------------------*/
DiscretizationSupg::~DiscretizationSupg() {}
DiscretizationSupg::DiscretizationSupg(std::string discretization): DiscretizationTransport(), _sl(NULL), _sc(NULL), _discretization(discretization)
{
  _symmetric = false;
}
DiscretizationSupg::DiscretizationSupg( const DiscretizationSupg& discretizationsupg): DiscretizationTransport(discretizationsupg)
{
  (*this).operator=(discretizationsupg);
}
DiscretizationSupg& DiscretizationSupg::operator=( const DiscretizationSupg& discretizationsupg) 
{
  DiscretizationTransport::operator=(discretizationsupg);
  assert(0);
  return *this;
}
std::string DiscretizationSupg::getName() const 
{
  return "DiscretizationSupg";
}
DiscretizationSupg* DiscretizationSupg::clone() const 
{
  return new DiscretizationSupg(*this);
}

/*--------------------------------------------------------------------------*/
void DiscretizationSupg::basicInit(const Fada::DomainSolverInterface* solver)
{
  DiscretizationTransport::basicInit(solver);
  if(_discretization=="supg")
  {
    _sl = new NoSlopeLimiter;
    _sc = new NoShockCapturing;    
  }
  else if(_discretization=="supgsl")
  {
    _sl = new SlopeLimiterSS(1.1);
    _sc = new NoShockCapturing;    
  }
  else if(_discretization=="supgslsc")
  {
    _sl = new SlopeLimiterSS(4.0);
    _sc = new ShockCapturing;
    // _sc = new ShockCapturingBadiaBonilla;
  }
  else
  {
    _error_string("basicInit", "unknown discretization", _discretization);
  }
}

/*--------------------------------------------------------------------------*/
void DiscretizationSupg::computePatchDiff(const FadaMesh::MeshInterface* mesh, DiscretizationData& discdata) const
{
  const arma::cube& downwindcoef = discdata.downwindcoef;
  const FadaMesh::PatchInfo& patchinfo = discdata.patchinfo;
  arma::field<arma::vec>& patchdiff = discdata.patchdiff;
  for(int iN = 0; iN < mesh->getNNodes(); iN++)
  {
    patchdiff[iN].zeros();
  }
  int ncells = mesh->getNCells();
  for(int iC = 0; iC < ncells; iC++)
  {
    for(int ii=0;ii<3;ii++)
    {
      int iN = mesh->getNodeIdOfCell(iC,ii);
      int npatch = patchinfo.nodes[iN].size();
      for(int jj=0;jj<3;jj++)
      {
        int jN = mesh->getNodeIdOfCell(iC,jj);
        double d = fmax(0.0, downwindcoef(iC, 1, ii)*downwindcoef(iC, 0, jj));
        for(int pp=0;pp<npatch;pp++)
        {
          if (patchinfo.nodes[iN][pp] == jN)
          {
            patchdiff[iN][pp] += d;
          }
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void DiscretizationSupg::rightHandSideCells(AlatEnums::residualstatus& status, Alat::SystemVector* fs, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const
{
  Fada::FemInterface* fem = discdata.fem;
  const arma::cube& downwindcoef = discdata.downwindcoef;
  if(_righthandside[0]==NULL)
  {
    return;
  }
  Alat::VariableVector* fv = dynamic_cast<Alat::VariableVector*>(fs->getVector(_variablenames[0]));
  assert(fv);
  int ncells = mesh->getNCells();
  Alat::Vector<Alat::Node> Fl( mesh->getNNodesPerCell(0) );
  Alat::Node vhat;
  for(int iC = 0; iC < ncells; iC++)
  {
    mesh->getNodesOfCell(iC, Fl);
    fem->reInitCell( Fl);
    double w = 0.0;
    _help2[0].zeros();
    Fada::TriangleMidPoint IFcell2;
    // for(int k = 0; k < _IFcell->n(); k++)
    for(int k = 0; k < IFcell2.n(); k++)
    {
      // _IFcell->point(vhat, k);
      IFcell2.point(vhat, k);
      fem->reInitReferencePoint( vhat);
      // double weight = _IFcell->weight(k)*fem->J();
      double weight = IFcell2.weight(k)*fem->J();
      w += weight;
      const Alat::Node& v = fem->x();
      _righthandside[0]->getValue( _help1[0], v.x(), v.y(), v.z(), getSolver()->getTime() );
      _help2[0].add(weight, _help1[0]);
    }
    _help2[0].scale(1./w);
    for(int ii=0;ii<3;ii++)
    {
      int iN = mesh->getNodeIdOfCell(iC,ii);
      (*fv)(0, iN) += _help2[0][0]*downwindcoef(iC, 1, ii);
    }
  }
}


/*--------------------------------------------------------------------------*/
void DiscretizationSupg::_computePatchGrads(int iN, PatchData& pdata, const Alat::VariableVector* uv , const FadaMesh::PatchInfo& patchinfo, const arma::field<arma::vec>& patchdiff) const
{
  int npatch = patchinfo.nodes[iN].size();
  for(int ip=0;ip<npatch;ip++)
  {
    int jN = patchinfo.nodes[iN][ip];
    int i0 = patchinfo.sideindices[iN](ip, 0);
    int i1 = patchinfo.sideindices[iN](ip, 1);
    double a0 = patchinfo.sidecoeffs[iN](ip, 0);
    double a1 = patchinfo.sidecoeffs[iN](ip, 1);
    pdata.grad[ip] = (*uv)(0, jN) - (*uv)(0, iN);
    pdata.grad2[ip] = a0*((*uv)(0, i0) - (*uv)(0, iN)) + a1*((*uv)(0, i1) - (*uv)(0, iN));
    pdata.diff[ip] = patchdiff[iN][ip];
    pdata.jN[ip] = jN;
    pdata.i0[ip] = i0;
    pdata.i1[ip] = i1;
    pdata.a0[ip] = a0;
    pdata.a1[ip] = a1;
  }  
}

/*--------------------------------------------------------------------------*/
void DiscretizationSupg::formCells(AlatEnums::residualstatus& status, Alat::SystemVector* fs, const Alat::SystemVector* us, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const
{
  Fada::FemInterface* fem = discdata.fem;
  const arma::cube& downwindcoef = discdata.downwindcoef;
  Alat::VariableVector* fv = dynamic_cast<Alat::VariableVector*>(fs->getVector(_variablenames[0]));
  assert(fv);
  const Alat::VariableVector* uv = dynamic_cast<const Alat::VariableVector*>(us->getVector(_variablenames[0]));
  assert(uv);
  int ncells = mesh->getNCells();
  for(int iC = 0; iC < ncells; iC++)
  {
    _help2[0].zeros();
    for(int ii=0;ii<3;ii++)
    {
      int iN = mesh->getNodeIdOfCell(iC,ii);
      _help2[0][0] += (*uv)(0, iN)*downwindcoef(iC, 0, ii);
    }    
    for(int ii=0;ii<3;ii++)
    {
      int iN = mesh->getNodeIdOfCell(iC,ii);
      (*fv)(0, iN) -= _help2[0][0]*downwindcoef(iC, 1, ii);
    }
  }
  // stab
  const arma::field<arma::vec>& patchdiff = discdata.patchdiff;
  const FadaMesh::PatchInfo& patchinfo = discdata.patchinfo;
  int nnodes = mesh->getNNodes();
  Alat::Node wrongnormal;
  PatchData pdata(discdata.npatchmax);
  for(int iN = 0; iN < nnodes; iN++)
  {
    int npatch = patchinfo.nodes[iN].size();
    bool boundary = npatch != patchinfo.cells[iN].size();
    if (boundary )
    {    
      for(int ip=0;ip<npatch;ip++)
      {
        int jN = patchinfo.nodes[iN][ip];
        double diff = patchdiff[iN][ip];
        double du = (*uv)(0, jN) - (*uv)(0, iN);
        if (boundary )
        {
          double ducor = 0.0;
          if( _model->getApplication()->getName() == "ApplicationWithExactSolution")
          {
            const Alat::Node& v = mesh->getNode(iN);
            _dirichlet[0][0]->getValue( _help1[0], 111, wrongnormal, v.x(), v.y(), v.z(), getSolver()->getTime() );
            const Alat::Node& v2 =  mesh->getNode(jN);
            _dirichlet[0][0]->getValue( _help2[0], 111, wrongnormal, v2.x(), v2.y(), v2.z(), getSolver()->getTime() );
            ducor = _help2[0][0]-_help1[0][0];
          }
          du -= ducor;
          (*fv)(0, jN) -= diff*du;
          (*fv)(0, iN) += diff*du;
        }
      }
    }
    else
    {
      _computePatchGrads(iN, pdata, uv, patchinfo, patchdiff);
      double sc = _sc->phi(npatch, pdata);
      for(int ip=0;ip<npatch;ip++)
      {
        double stab = sc*pdata.diff[ip]*_sl->xi(pdata.grad[ip], pdata.grad2[ip]);        
        (*fv)(0, pdata.jN[ip]) -= stab;
        (*fv)(0, iN) += stab;
        if(_symmetric)
        {
          assert(0);
          (*fv)(0, iN) += (pdata.a0[ip]+pdata.a1[ip])*stab;
          (*fv)(0, pdata.i0[ip]) -= pdata.a0[ip]*stab;
          (*fv)(0, pdata.i1[ip]) -= pdata.a1[ip]*stab;
        }
      }
    }
  }
}
/*--------------------------------------------------------------------------*/
void DiscretizationSupg::matrixSparsityPattern(const std::string& varnamei, const std::string& varnamej, Alat::SparsityPatternSoft& sparsitypatternsoft, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const
{
  int ncells = mesh->getNCells();
  for(int iC = 0; iC < ncells; iC++)
  {
    for(int ii=0;ii<3;ii++)
    {
      for(int jj=0;jj<3;jj++)
      {
        sparsitypatternsoft[mesh->getNodeIdOfCell(iC,ii)].insert(mesh->getNodeIdOfCell(iC,jj));
      }
    }
  }    
  const FadaMesh::PatchInfo& patchinfo = discdata.patchinfo;
  int nnodes = mesh->getNNodes();
  for(int iN = 0; iN < nnodes; iN++)
  {
    int npatch = patchinfo.nodes[iN].size();
    bool boundary = npatch != patchinfo.cells[iN].size();
    for(int ip=0;ip<npatch;ip++)
    {
      int jN = patchinfo.nodes[iN][ip];
      if (not boundary)
      {
        int i0 = patchinfo.sideindices[iN](ip, 0);
        int i1 = patchinfo.sideindices[iN](ip, 1);
        sparsitypatternsoft[iN].insert(i0);
        sparsitypatternsoft[iN].insert(i1);
        sparsitypatternsoft[jN].insert(i0);
        sparsitypatternsoft[jN].insert(i1);
        if(_symmetric)
        {
          sparsitypatternsoft[i0].insert(iN);
          sparsitypatternsoft[i1].insert(iN);
          sparsitypatternsoft[i0].insert(jN);
          sparsitypatternsoft[i1].insert(jN);
        }
        for(int jp=0;jp<npatch;jp++)
        {
          int j0 = patchinfo.sideindices[iN](jp, 0);
          int j1 = patchinfo.sideindices[iN](jp, 1);
          int jjN = patchinfo.nodes[iN][jp];
          sparsitypatternsoft[iN].insert(j0);
          sparsitypatternsoft[iN].insert(j1);
          sparsitypatternsoft[iN].insert(jjN);
          sparsitypatternsoft[jN].insert(j0);
          sparsitypatternsoft[jN].insert(j1);
          sparsitypatternsoft[jN].insert(jjN);
        }        
      }
    }
  }
}
 
/*--------------------------------------------------------------------------*/
void DiscretizationSupg::matrixCells(AlatEnums::residualstatus& status, Alat::SystemMatrixInterface* As, const Alat::SystemVector* us, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const
{
  Fada::FemInterface* fem = discdata.fem;
  const arma::cube& downwindcoef = discdata.downwindcoef;
  Alat::VariableMatrix* Av = dynamic_cast<Alat::VariableMatrix*>(As->getMatrix(_variablenames[0], _variablenames[0]));
  assert(Av);
  const Alat::VariableVector* uv = dynamic_cast<const Alat::VariableVector*>(us->getVector(_variablenames[0]));
  assert(uv);
  int ncells = mesh->getNCells();
  Alat::IntVector indices(3), jndices(3), indices2(2), jndicessc;
  Alat::AssembleMatrix Aloc, Aloc2, Alocsc;
  int ncomp = 1;
  Aloc.set_size(ncomp, ncomp, 3, 3);
  Aloc2.set_size(ncomp, ncomp, 2, 2);
  // double scale = 1.0;
  const Alat::StringDoubleVectorMap& scale = _solver->getVariablesScales();
  for(int iC = 0; iC < ncells; iC++)
  {
    for(int ii=0;ii<3;ii++)
    {
      indices[ii] = mesh->getNodeIdOfCell(iC,ii);
    }    
    for(int ii=0;ii<3;ii++)
    {
      for(int jj=0;jj<3;jj++)
      {
        Aloc(0,0, ii, jj) = downwindcoef(iC, 1, ii)*downwindcoef(iC, 0, jj);
      }
    }
    Av->assemble(Aloc, indices, indices, scale["U"], scale["U"]);
  }
  // stab
  if(_symmetric)
  {
    Aloc.set_size(ncomp, ncomp, 4, 4);    
  }
  else
  {
    Aloc.set_size(ncomp, ncomp, 2, 4);    
  }
  indices.set_size(2);
  jndices.set_size(4);
  const arma::field<arma::vec>& patchdiff = discdata.patchdiff;
  const FadaMesh::PatchInfo& patchinfo = discdata.patchinfo;
  int nnodes = mesh->getNNodes();
  PatchData pdata(discdata.npatchmax);
  for(int iN = 0; iN < nnodes; iN++)
  {
    int npatch = patchinfo.nodes[iN].size();
    bool boundary = npatch != patchinfo.cells[iN].size();
    if (boundary)
    {
      for(int ip=0;ip<npatch;ip++)
      {
        int jN = patchinfo.nodes[iN][ip];
        double diff = patchdiff[iN][ip];
        indices2[0] = iN;
        indices2[1] = jN;
        Aloc2(0,0, 0, 0) = diff;
        Aloc2(0,0, 0, 1) = -diff;
        Aloc2(0,0, 1, 0) = -diff;
        Aloc2(0,0, 1, 1) = diff;
        Av->assemble(Aloc2, indices2, indices2, scale["U"], scale["U"]);       
      }
    }
    else
    {
      _computePatchGrads(iN, pdata, uv, patchinfo, patchdiff);
      double sc = _sc->phi(npatch, pdata);
      for(int ip=0;ip<npatch;ip++)
      {
        double stab = sc*pdata.diff[ip]*_sl->xi(pdata.grad[ip], pdata.grad2[ip]);        
        double dx = sc*pdata.diff[ip]*_sl->dxidx(pdata.grad[ip], pdata.grad2[ip]);        
        double dy = sc*pdata.diff[ip]*_sl->dxidy(pdata.grad[ip], pdata.grad2[ip]);        
        double a0 = pdata.a0[ip];
        double a1 = pdata.a1[ip];        
        indices[0] = iN;
        indices[1] = pdata.jN[ip];
        jndices[0] = indices[0];
        jndices[1] = indices[1];
        jndices[2] = pdata.i0[ip];
        jndices[3] = pdata.i1[ip];

        Aloc(0,0, 0,0) =  dx + dy*(a0+a1);
        Aloc(0,0, 0,1) =  -dx;
        Aloc(0,0, 0,2) =  -dy*a0;
        Aloc(0,0, 0,3) =  -dy*a1;
        
        Aloc(0,0, 1,0) =  -Aloc(0,0, 0,0);
        Aloc(0,0, 1,1) =  -Aloc(0,0, 0,1);
        Aloc(0,0, 1,2) =  -Aloc(0,0, 0,2);
        Aloc(0,0, 1,3) =  -Aloc(0,0, 0,3);
        Av->assemble(Aloc, indices, jndices, scale["U"], scale["U"]);                
        
        // - sym        
        if(_symmetric)
        {
          assert(0);
          // Aloc(0,0, 0,0) +=  (a0+a1)*(  (a0+a1)*(diff-dy) -dx);
          // Aloc(0,0, 0,1) +=  (a0+a1)*dx;
          // Aloc(0,0, 0,2) +=  (dy-diff)*a0*(a0+a1);
          // Aloc(0,0, 0,3) +=  (dy-diff)*a1*(a0+a1);
          //
          // Aloc(0,0, 2,0) =  a0* ( (dy-diff)*(a0+a1)+dx);
          // Aloc(0,0, 2,1) =  -a0*dx;
          // Aloc(0,0, 2,2) =  a0*a0*(diff-dy);
          // Aloc(0,0, 2,3) =  a0*a1*(diff-dy);
          //
          // Aloc(0,0, 3,0) =  a1* ( (dy-diff)*(a0+a1)+dx);
          // Aloc(0,0, 3,1) =  -a1*dx;
          // Aloc(0,0, 3,2) =  a1*a0*(diff-dy);
          // Aloc(0,0, 3,3) =  a1*a1*(diff-dy);
          // Av->assemble(Aloc, jndices, jndices, scale);
        }
      }
      arma::vec dphidx(npatch), dphidy(npatch);
      _sc->dphi(npatch, pdata, dphidx, dphidy);
      jndicessc.set_size(3*npatch+1);
      Alocsc.set_size(ncomp, ncomp, 2, 3*npatch+1);
      for(int ip=0;ip<npatch;ip++)
      {
        // double stab = sc*pdata.diff[ip]*_sl->xi(pdata.grad[ip], pdata.grad2[ip]);
        // (*fv)(0, pdata.jN[ip]) -= stab;
        // (*fv)(0, iN) += stab;
        double stab = sc*pdata.diff[ip]*_sl->xi(pdata.grad[ip], pdata.grad2[ip]);        
        Alocsc.zeros();
        jndicessc.fill(-1);
        int jN = patchinfo.nodes[iN][ip];
        indices2[0] = iN;
        indices2[1] = jN;
        for(int jp=0;jp<npatch;jp++)
        {
          double a0 = pdata.a0[jp];
          double a1 = pdata.a1[jp];        
          jndicessc[3*jp] = pdata.jN[jp];
          jndicessc[3*jp+1] = pdata.i0[jp];
          jndicessc[3*jp+2] = pdata.i1[jp];
          Alocsc(0,0, 0,3*jp) = -stab*dphidx[jp];
          Alocsc(0,0, 0,3*jp+1) = -stab*a0*dphidy[jp];
          Alocsc(0,0, 0,3*jp+2) = -stab*a1*dphidy[jp];
          Alocsc(0,0, 0,3*npatch) += stab*( dphidx[jp] + (a0+a1)* dphidy[jp] );
          Alocsc(0,0, 1,3*jp)        = -Alocsc(0,0, 0,3*jp);
          Alocsc(0,0, 1,3*jp+1)      = -Alocsc(0,0, 0,3*jp+1);
          Alocsc(0,0, 1,3*jp+2)      = -Alocsc(0,0, 0,3*jp+2);
        }
        Alocsc(0,0, 1,3*npatch) = -Alocsc(0,0, 0,3*npatch);
        jndicessc[3*npatch] = iN;
        Av->assemble(Alocsc, indices2, jndicessc, scale["U"], scale["U"]);                
       }        
      
    }      
  }
}      

/*--------------------------------------------------------------------------*/
void DiscretizationSupg::rightHandSideBoundarySides(AlatEnums::residualstatus& status, Alat::SystemVector* fs, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const
{
  Alat::VariableVector* fv = dynamic_cast<Alat::VariableVector*>(fs->getVector(_variablenames[0]));
  assert(fv);
  Fada::FemInterface* fem = discdata.fem;
  const arma::vec& betaside = discdata.betaside;  
  const FadaMesh::BoundaryInfo* boundaryinfo = mesh->getBoundaryInfo();
  const Alat::IntVector colors = boundaryinfo->getColors();
  Alat::Vector<Alat::Node> Fl( mesh->getNNodesPerCell(0) );
  Alat::Node vhat;
  for(int ic = 0; ic < colors.size(); ic++)
  {
    int color = colors[ic];
    const Alat::IntVector& sides = boundaryinfo->getSidesOfColor(color);
    const Alat::IntVector& cells = boundaryinfo->getCellsOfColor(color);
    const Alat::IntVector& sideids = boundaryinfo->getSidesIdOfCellsOfColor(color);
    for(int i = 0; i < sides.size(); i++)
    {
      int iS = sides[i];
      int iK = cells[i];
      int iil = sideids[i];
      mesh->getNodesOfCell(iK, Fl);
      fem->reInitCellAndSide( Fl, iil);
      double w = 0.0;
      double bs = betaside[iS];
      _help2[0].zeros();
      Alat::Node beta;
      for(int k = 0; k < _IFside->n(); k++)
      {
        _IFside->point(vhat, k);
        fem->reInitReferencePointBoundary( vhat);
        double weight = _IFside->weight(k)*fem->G();
        w += weight;
        _betafct->getValue(beta, fem->x());
        const Alat::Node& v = fem->x();
        _help1[0].zeros();
        _dirichlet[0][ic]->getValue( _help1[0], color, fem->getNormal(), v.x(), v.y(), v.z(), getSolver()->getTime() );
        _help2[0].add(weight, _help1[0]);
      }
      _help2[0].scale(1./w);
      if (bs>=0.0)
      {
        continue;
      }
      for(int ii=0;ii<2;ii++)
      {
        int iN = mesh->getNodeIdOfSide(iS,ii);
        const Alat::Node& v = mesh->getNode(iN);
        _help1[0].zeros();
        _dirichlet[0][ic]->getValue( _help1[0], color, fem->getNormal(), v.x(), v.y(), v.z(), getSolver()->getTime() );
        (*fv)(0, iN) -= 0.5*_help1[0][0]*bs;
      }
      // for(int k = 0; k < _IFside->n(); k++)
      // {
      //   _IFside->point(vhat, k);
      //   fem->reInitReferencePointBoundary( vhat);
      //   double weight = _IFside->weight(k)*fem->G();
      //   const Alat::Node& v = fem->x();
      //   _dirichlet[0][ic]->getValue( _help1[0], color, fem->getNormal(), v.x(), v.y(), v.z(), getSolver()->getTime() );
      //   _betafct->getValue(beta, fem->x());
      //   // bn += weight* arma::dot(beta,fem->getNormal());
      //   double bn = arma::dot(beta,fem->getNormal());
      //   double bnm = fmin(0.0, bn);
      //   for(int ii=0;ii<3;ii++)
      //   {
      //     // std::cerr << " weight = " << weight << " " << bnm << " " << fem->phi(ii) << "\n";
      //     int iN = mesh->getNodeIdOfCell(iK,ii);
      //     (*fv)(0, iN) -= weight*_help1[0][0]*bnm*fem->phi(ii);
      //     // (*fv)(0, iN) -= weight*_help2[0][0]*bnm*fem->phi(ii);
      //   }
      // }
      
      
      
      // double bn = 0.0;
      // Fada::LineTrapez IFside2;
      // // for(int k = 0; k < _IFside->n(); k++)
      //   for(int k = 0; k < IFside2.n(); k++)
      // {
      //   // _IFside->point(vhat, k);
      //   IFside2.point(vhat, k);
      //   fem->reInitReferencePointBoundary( vhat);
      //   // double weight = _IFside->weight(k)*fem->G();
      //   double weight = IFside2.weight(k)*fem->G();
      //   w += weight;
      //   _betafct->getValue(beta, fem->x());
      //   // bn += weight* arma::dot(beta,fem->getNormal());
      //   double bn = arma::dot(beta,fem->getNormal());
      //   double bnm = fmin(0.0, bn);
      //   const Alat::Node& v = fem->x();
      //   _help1[0].zeros();
      //   _dirichlet[0][ic]->getValue( _help1[0], color, fem->getNormal(), v.x(), v.y(), v.z(), getSolver()->getTime() );
      //   // std::cerr << "dir = " << _help1 << "\n";
      //   _help2[0].add(weight, _help1[0]);
      //   for(int ii=0;ii<3;ii++)
      //   {
      //     // std::cerr << " weight = " << weight << " " << bnm << " " << fem->phi(ii) << "\n";
      //     int iN = mesh->getNodeIdOfCell(iK,ii);
      //     // (*fv)(0, iN) -= weight*_help1[0][0]*bnm*fem->phi(ii);
      //     // (*fv)(0, iN) -= weight*_help2[0][0]*bnm*fem->phi(ii);
      //     (*fv)(0, iN) -= weight*_help2[0][0]*bs*fem->phi(ii);
      //   }
      // }
      // assert(bn == betaside[iS]);
      // _help2[0].scale(1./w);
      //  for(int k = 0; k < _IFside->n(); k++)
      //  {
      //    _IFside->point(vhat, k);
      //    fem->reInitReferencePointBoundary( vhat);
      //    double weight = _IFside->weight(k)*fem->G();
      //    _betafct->getValue(beta, fem->x());
      //    // bn += weight* arma::dot(beta,fem->getNormal());
      //    double bn = arma::dot(beta,fem->getNormal());
      //    double bnm = fmin(0.0, bn);
      //    for(int ii=0;ii<3;ii++)
      //    {
      //      // std::cerr << " weight = " << weight << " " << bnm << " " << fem->phi(ii) << "\n";
      //      int iN = mesh->getNodeIdOfCell(iK,ii);
      //      (*fv)(0, iN) -= weight*_help2[0][0]*bnm*fem->phi(ii);
      //    }
      //  }
      // std::cerr << "w " << w << "dir = " << _help2 << "\n";
      // // std::cerr << "betaside[iS] = " << betaside[iS] << "\n";
      // for(int ii=0;ii<2;ii++)
      // {
      //   int iN = mesh->getNodeIdOfSide(iS,ii);
      //   (*fv)(0, iN) -= 0.5*_help2[0][0]*betaside[iS];
      // }
    }
  }
}
/*--------------------------------------------------------------------------*/
void DiscretizationSupg::formBoundarySides(AlatEnums::residualstatus& status, Alat::SystemVector* fs, const Alat::SystemVector* us, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const
{
  Alat::VariableVector* fv = dynamic_cast<Alat::VariableVector*>(fs->getVector(_variablenames[0]));
  assert(fv);
  const Alat::VariableVector* uv = dynamic_cast<const Alat::VariableVector*>(us->getVector(_variablenames[0]));
  assert(uv);
  
  // Fada::FemInterface* fem = discdata.fem;
  // const arma::vec& betaside = discdata.betaside;
  // const FadaMesh::BoundaryInfo* boundaryinfo = mesh->getBoundaryInfo();
  // const Alat::IntVector colors = boundaryinfo->getColors();
  // Alat::Vector<Alat::Node> Fl( mesh->getNNodesPerCell(0) );
  // Alat::Node vhat;
  // for(int ic = 0; ic < colors.size(); ic++)
  // {
  //   int color = colors[ic];
  //   const Alat::IntVector& sides = boundaryinfo->getSidesOfColor(color);
  //   const Alat::IntVector& cells = boundaryinfo->getCellsOfColor(color);
  //   const Alat::IntVector& sideids = boundaryinfo->getSidesIdOfCellsOfColor(color);
  //   for(int i = 0; i < sides.size(); i++)
  //   {
  //     int iS = sides[i];
  //     int iK = cells[i];
  //     int iil = sideids[i];
  //     mesh->getNodesOfCell(iK, Fl);
  //     fem->reInitCellAndSide( Fl, iil);
  //     double w = 0.0;
  //     _help2[0].zeros();
  //     Alat::Node beta;
  //     for(int k = 0; k < _IFside->n(); k++)
  //     {
  //       _IFside->point(vhat, k);
  //       fem->reInitReferencePointBoundary( vhat);
  //       double weight = _IFside->weight(k)*fem->G();
  //       w += weight;
  //       _betafct->getValue(beta, fem->x());
  //       double bn = arma::dot(beta,fem->getNormal());
  //       double bnm = fmin(0.0, bn);
  //       const Alat::Node& v = fem->x();
  //       _help1[0].zeros();
  //       for(int ii=0;ii<3;ii++)
  //       {
  //         int iN = mesh->getNodeIdOfCell(iK,ii);
  //         _help1[0][0] += (*uv)(0, iN)*fem->phi(ii);
  //       }
  //       for(int ii=0;ii<3;ii++)
  //       {
  //         int iN = mesh->getNodeIdOfCell(iK,ii);
  //         (*fv)(0, iN) += weight*_help1[0][0]*bnm*fem->phi(ii);
  //       }
  //     }
  //   }
  // }
  
  
  // std::cerr  << "fv \n";
  // fv->writeAscii(std::cerr);
  // std::cerr  << "\n";
  // std::cerr  << "uv \n";
  // uv->writeAscii(std::cerr);
  // std::cerr  << "\n";
  const arma::vec& betaside = discdata.betaside;
  const FadaMesh::BoundaryInfo* boundaryinfo = mesh->getBoundaryInfo();
  const Alat::IntVector colors = boundaryinfo->getColors();
  // Alat::Node wrongnormal;
  for(int ic = 0; ic < colors.size(); ic++)
  {
    int color = colors[ic];
    const Alat::IntVector& sides = boundaryinfo->getSidesOfColor(color);
    const Alat::IntVector& cells = boundaryinfo->getCellsOfColor(color);
    const Alat::IntVector& sideids = boundaryinfo->getSidesIdOfCellsOfColor(color);
    for(int i = 0; i < sides.size(); i++)
    {
      int iS = sides[i];
      if (betaside[iS]>=0.0)
      {
        continue;
      }
      for(int ii=0;ii<2;ii++)
      {
        int iN = mesh->getNodeIdOfSide(iS,ii);
        const Alat::Node& v = mesh->getNode(iN);
        // _help1[0].zeros();
        // _dirichlet[0][ic]->getValue( _help1[0], color, wrongnormal, v.x(), v.y(), v.z(), getSolver()->getTime() );
        // (*fv)(0, iN) += 0.5*( (*uv)(0, iN) - _help1[0][0] ) *betaside[iS];
        (*fv)(0, iN) += 0.5*(*uv)(0, iN)*betaside[iS];
      }
    }
  }
  // std::cerr  << "fv \n";
  // fv->writeAscii(std::cerr);
  // std::cerr  << "\n";
} 
/*--------------------------------------------------------------------------*/
void DiscretizationSupg::matrixBoundarySides(AlatEnums::residualstatus& status, Alat::SystemMatrixInterface* As, const Alat::SystemVector* us, const FadaMesh::MeshInterface* mesh, const DiscretizationData& discdata) const
{
  Alat::VariableMatrix* Av = dynamic_cast<Alat::VariableMatrix*>(As->getMatrix(_variablenames[0], _variablenames[0]));
  assert(Av);
  // const Alat::VariableVector* uv = dynamic_cast<const Alat::VariableVector*>(us->getVector(_variablenames[0]));
  // assert(uv);
  // Alat::IntVector indices(3);
  // Alat::AssembleMatrix Aloc;
  // int ncomp = 1;
  // Aloc.set_size(ncomp, ncomp, 3, 3);
  // double scale = 1.0;
  // Fada::FemInterface* fem = discdata.fem;
  // const arma::vec& betaside = discdata.betaside;
  // const FadaMesh::BoundaryInfo* boundaryinfo = mesh->getBoundaryInfo();
  // const Alat::IntVector colors = boundaryinfo->getColors();
  // Alat::Vector<Alat::Node> Fl( mesh->getNNodesPerCell(0) );
  // Alat::Node vhat;
  // for(int ic = 0; ic < colors.size(); ic++)
  // {
  //   int color = colors[ic];
  //   const Alat::IntVector& sides = boundaryinfo->getSidesOfColor(color);
  //   const Alat::IntVector& cells = boundaryinfo->getCellsOfColor(color);
  //   const Alat::IntVector& sideids = boundaryinfo->getSidesIdOfCellsOfColor(color);
  //   for(int i = 0; i < sides.size(); i++)
  //   {
  //     int iS = sides[i];
  //     int iK = cells[i];
  //     int iil = sideids[i];
  //     mesh->getNodesOfCell(iK, Fl);
  //     fem->reInitCellAndSide( Fl, iil);
  //     double w = 0.0;
  //     _help2[0].zeros();
  //     Alat::Node beta;
  //     for(int k = 0; k < _IFside->n(); k++)
  //     {
  //       _IFside->point(vhat, k);
  //       fem->reInitReferencePointBoundary( vhat);
  //       double weight = _IFside->weight(k)*fem->G();
  //       w += weight;
  //       _betafct->getValue(beta, fem->x());
  //       double bn = arma::dot(beta,fem->getNormal());
  //       double bnm = fmin(0.0, bn);
  //       const Alat::Node& v = fem->x();
  //       _help1[0].zeros();
  //       for(int ii=0;ii<3;ii++)
  //       {
  //         indices[ii] = mesh->getNodeIdOfCell(iK,ii);
  //         for(int jj=0;jj<3;jj++)
  //         {
  //           Aloc(0,0, ii, jj) = -weight*bnm*fem->phi(ii)*fem->phi(jj);
  //         }
  //       }
  //       Av->assemble(Aloc, indices, indices, scale);
  //      }
  //   }
  // }
  
  
  
  Fada::FemInterface* fem = discdata.fem;
  const arma::cube& downwindcoef = discdata.downwindcoef;
  const Alat::VariableVector* uv = dynamic_cast<const Alat::VariableVector*>(us->getVector(_variablenames[0]));
  assert(uv);
  int ncells = mesh->getNCells();
  Alat::IntVector indices(2);
  Alat::AssembleMatrix Aloc;
  int ncomp = 1;
  Aloc.set_size(ncomp, ncomp, 2, 2);
  // double scale = 1.0;
  const Alat::StringDoubleVectorMap& scale = _solver->getVariablesScales();
  const arma::vec& betaside = discdata.betaside;
  const FadaMesh::BoundaryInfo* boundaryinfo = mesh->getBoundaryInfo();
  const Alat::IntVector colors = boundaryinfo->getColors();
  for(int ic = 0; ic < colors.size(); ic++)
  {
    int color = colors[ic];
    const Alat::IntVector& sides = boundaryinfo->getSidesOfColor(color);
    const Alat::IntVector& cells = boundaryinfo->getCellsOfColor(color);
    const Alat::IntVector& sideids = boundaryinfo->getSidesIdOfCellsOfColor(color);
    for(int i = 0; i < sides.size(); i++)
    {
      int iS = sides[i];
      if (betaside[iS]>=0.0)
      {
        continue;
      }
      for(int ii=0;ii<2;ii++)
      {
        indices[ii] = mesh->getNodeIdOfSide(iS,ii);
        Aloc(0,0, ii, ii) = -0.5*betaside[iS];
      }
      Av->assemble(Aloc, indices, indices, scale["U"], scale["U"]);
    }
  }
}

