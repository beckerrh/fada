#include  "Alat/assemblevector.h"
#include  "Fada/transformationinterface.h"
#include  "Fada/fembase.h"
#include  "Alat/map.h"
#include  "Fada/q12dtransformation.h"
#include  "Alat/armadillo.h"
#include  <math.h>

using namespace Fada;
using namespace std;

/*-------------------------------------------------------*/

FEMBase::~FEMBase()
{
  if(_transformation)
  {
    delete _transformation;
    _transformation = NULL;
  }
  if(_transformationcurved)
  {
    delete _transformationcurved;
    _transformationcurved = NULL;
  }
}

/*-------------------------------------------------------*/

FEMBase::FEMBase() : Fada::FemInterface(), _transformation(NULL), _transformationcurved(NULL), _curved(0) {}

/*-------------------------------------------------------*/

FEMBase::FEMBase(const FEMBase& orig)
{
  _curved = orig._curved;
  _transformation = orig._transformation->clone();
  _transformationcurved = orig._transformationcurved->clone();
  _d2phi.set_size(orig._d2phi.size());
  for(int i = 0; i < orig._d2phi.size(); i++)
  {
    _d2phi[i] = orig._d2phi[i];
  }
  _dphi.set_size(orig._dphi.size());
  for(int i = 0; i < orig._dphi.size(); i++)
  {
    _dphi[i] = orig._dphi[i];
  }
  _phi.set_size(orig._phi.size());
  for(int i = 0; i < orig._phi.size(); i++)
  {
    _phi[i] = orig._phi[i];
  }
}

/*-------------------------------------------------------*/

FEMBase& FEMBase::operator=(const FEMBase& F)
{
  assert(0);
  return *this;
}

/*-------------------------------------------------------*/

std::string FEMBase::getName() const
{
  return "FEMBase";
}

/*-------------------------------------------------------*/

Fada::TransformationInterface*& FEMBase::getTransformationCurvedPointer()
{
  return _transformationcurved;
}

/*-------------------------------------------------------*/

Fada::TransformationInterface*& FEMBase::getTransformationPointer()
{
  return _transformation;
}

/*-------------------------------------------------------*/

Fada::TransformationInterface* FEMBase::getTransformation()
{
  // std::cerr << "FEMBase::getTransformation() _curved " << _curved << "\n";
  if(_curved)
  {
    assert(_transformationcurved);
    return _transformationcurved;
  }
  assert(_transformation);
  return _transformation;
}

/*-------------------------------------------------------*/

const Fada::TransformationInterface* FEMBase::getTransformation() const
{
  // std::cerr << "FEMBase::getTransformation() _curved " << _curved << "\n";
  if(_curved)
  {
    assert(_transformationcurved);
    return _transformationcurved;
  }
  assert(_transformation);
  return _transformation;
}

/*-------------------------------------------------------*/

const FadaMesh::MeshInterface* FEMBase::getMesh() const
{
  assert(0);
  // assert(_mesh);
  // return _mesh;
}

/*-------------------------------------------------------*/

const Alat::DoubleMatrix& FEMBase::getTransformationInverse() const
{
  return getTransformation()->getJacobian();
}

/*-------------------------------------------------------*/

Alat::Vector<Alat::Node>& FEMBase::dphi()
{
  return _dphi;
}

/*-------------------------------------------------------*/

Alat::DoubleVector& FEMBase::phi()
{
  return _phi;
}

/*-------------------------------------------------------*/

const Alat::Node& FEMBase::NonUnitNormal()
{
  return getTransformation()->getNonUnitNormal();
}

/*-------------------------------------------------------*/

void FEMBase::basicInit(const FadaMesh::MeshInterface* mesh)
{
  // assert(0);
  int ndofs = ndoflocal();
  _dphi.set_size(ndofs);
  _phi.set_size(ndofs);
  _d2phi.set_size(ndofs);
  assert(_transformation == NULL);
  assert(_transformationcurved == NULL);
  getTransformationPointer() = newTransformation();
  getTransformationCurvedPointer() = newTransformationCurved();

  _transformation->basicInit(mesh);
  _transformationcurved->basicInit(mesh);
}

/*-------------------------------------------------------*/

double FEMBase::J() const
{
  double d = getTransformation()->getDetJacobian();
  if(d < 0.0)
  { 
    std::cerr<< "*** ERROR in FEMBase::J(): J negative: " << d << "\n";
    std::cerr << "mesh " << getMesh()->getName()<<"\n";
    std::cerr << "nodes " << _transformation->getNodes()<<"\n";
    _error_string("J", "J negative:", d);
    // std::cerr<< "*** ERROR in FEMBase::J(): J negative: " << d << "\n";
    // std::cerr << "iK curved " << _iK << " " << getMesh()->cellIsCurved(_iK)<<"\n";
    // assert(0);
    // exit(1);
  }
  return d;
}

/*-------------------------------------------------------*/

void FEMBase::reInitCell(const Alat::Vector<Alat::Node>& nodes)
{
  _curved = 0;
  assert( getTransformation() );
  getTransformation()->reInitCell(nodes);
}

/*-------------------------------------------------------*/

void FEMBase::reInitCellAndSide(const Alat::Vector<Alat::Node>& nodes, int iiS)
{
  reInitCell(nodes);
  getTransformation()->reInitSide(iiS);
}

/*-------------------------------------------------------*/

void FEMBase::computeFunctionAndGradient(Alat::DoubleVector& u, Alat::Vector<Alat::Node>& du, const Alat::AssembleVector& uloc) const
{
  u.zeros();
  for(int i = 0; i < du.size(); i++)
  {
    du[i].zeros();
  }
  assert( uloc.nloc() == ndoflocal() );
  for(int icomp = 0; icomp < u.size(); icomp++)
  {
    for(int ii = 0; ii < ndoflocal(); ii++)
    {
      // std::cerr << "\tFEMBase::computeFunctionAndGradient() <<=" <<ii << " uloc=" << uloc[icomp][ii]<<" phi="<<_phi[ii]<<"\n";
      // u [icomp]     += uloc[icomp][ii]*_phi[ii];
      // du[icomp].x() += uloc[icomp][ii]*_dphi[ii].x();
      // du[icomp].y() += uloc[icomp][ii]*_dphi[ii].y();
      // du[icomp].z() += uloc[icomp][ii]*_dphi[ii].z();
      double uu = uloc(icomp,ii);
      u [icomp]     += uu*_phi[ii];
      du[icomp].x() += uu*_dphi[ii].x();
      du[icomp].y() += uu*_dphi[ii].y();
      du[icomp].z() += uu*_dphi[ii].z();
    }
  }
}

/*-------------------------------------------------------*/

void FEMBase::computeFunction(Alat::DoubleVector& u, const Alat::AssembleVector& uloc) const
{
  u.zeros();
  if( u.size() != uloc.ncomp() )
  {
    std::cerr<<"*** ERROR in FEMBase::computeFunction() u.size()!=uloc.ncomp() "<<u.size()<<" "<<uloc.ncomp()<<"\n";
    assert(0);
    exit(1);
  }
  assert( uloc.nloc() == ndoflocal() );
  for(int icomp = 0; icomp < u.size(); icomp++)
  {
    for(int ii = 0; ii < ndoflocal(); ii++)
    {
      // u[icomp] += uloc[icomp][ii]*_phi[ii];
      u[icomp] += uloc(icomp,ii)*_phi[ii];
    }
  }
}

/*-------------------------------------------------------*/

void FEMBase::computeLaplacian(Alat::DoubleVector& lu, const Alat::AssembleVector& uloc) const
{
  lu.zeros();
  for(int icomp = 0; icomp < uloc.size(); icomp++)
  {
    for(int ii = 0; ii < ndoflocal(); ii++)
    {
      // lu += uloc[icomp][ii]*( _d2phi[ii].x()+_d2phi[ii].y() );
      lu += uloc(icomp,ii)*( _d2phi[ii].x()+_d2phi[ii].y() );
    }
  }
}

/*---------------------------------------------------------*/

void FEMBase::reInitReferencePoint(const Alat::Node& xhat)
{
  // std::cerr << "??? FEMBase::reInitReferencePoint() = " << xhat << "\n";
  getTransformation()->reInitPointCell(xhat);
}

/*---------------------------------------------------------*/
void FEMBase::reInitReferenceCenterPoint()
{
   Alat::Node xhat;
   getReferenceCenterPoint(xhat); 
   reInitReferencePoint(xhat);
   getTransformation()->reInitPointCell(xhat);
}

/*---------------------------------------------------------*/

void FEMBase::reInitReferencePointBoundary(const Alat::Node& shat)
{
  Alat::Node xhat;
  getTransformation()->computeReferencePoint(xhat, shat);
  // std::cerr << "FEMBase::reInitReferencePointBoundary() shat xhat " << shat << " => " << xhat << "\n";
  // assert(0);
//  getTransformation()->reInitPointCell(xhat);
  getTransformation()->reInitPointSide(xhat);
  reInitReferencePoint(xhat);
}

/*---------------------------------------------------------*/

const Alat::Node& FEMBase::x() const
{
  assert( getTransformation() );
  return getTransformation()->x();
}

/*---------------------------------------------------------*/

double FEMBase::G() const
{
  assert(getTransformation()->getSurfaceJacobian() > 0.0);
  return getTransformation()->getSurfaceJacobian();
}

/*---------------------------------------------------------*/

double FEMBase::phi(int ii) const
{
  return _phi[ii];
}

/*---------------------------------------------------------*/

const Alat::Node& FEMBase::dphi(int ii) const
{
  return _dphi[ii];
}

/*---------------------------------------------------------*/

const Alat::Node& FEMBase::getNormal() const
{
  return getTransformation()->getNormal();
}

/*---------------------------------------------------------*/

const Alat::Node& FEMBase::NonUnitNormal() const
{
  return getTransformation()->getNonUnitNormal();
}

/*---------------------------------------------------------*/

// valeur de h pour une face
double FEMBase::getHOfSide() const
{
  return getTransformation()->getHOfSide();
}

/*---------------------------------------------------------*/

// valeur de h pour un element
double FEMBase::getHOfCell() const
{
  return getTransformation()->getHOfCell();
}

/*---------------------------------------------------------*/

void FEMBase::reInitPhysicalPoint(const Alat::Node& x)
{
  // std::cerr << "FEMBase::reInitPhysicalPoint() getMesh()->getNCells() " << getMesh()->getNCells() << "\n";
  // std::cerr << "*** FEMBase::reInitPhysicalPoint() " << getTransformation()->getV() << "\n";
  Alat::Node xi;
  // trouver xi tq T(xi) = x;
  xi.zeros();
  xi.x() = 0.5;

  int niter = 2;
  int nrelax = 10;
  double eps = 1e-10, rrold, rr;
  Alat::Node res, dx, xa;
  bool ok = 0;
  getTransformation()->reInitPointCell(xi);
  xa =  getTransformation()->x();
  res = x;
  res.add(-1.0, xa);
  // std::cerr << " ??? x xmid " << x << "\t" << getTransformation()->x() << "\n";
  rrold = arma::norm(res);
  for(int iter = 0; iter < niter; iter++)
  {
    // std::cerr << "FEMBase::reInitPhysicalPoint() iter res xi " << iter << " " << arma::norm(res) << " : " << xi << "\n";
    if( ( arma::norm(res) < eps )or(iter == niter-1) )
    {
      ok = 1;
      break;
    }
    const Alat::DoubleMatrix& TIT = getTransformation()->getInvJacobian();
    dx.zeros();
    // std::cerr << "FEMBase::reInitPhysicalPoint() TIT -> " <<  TIT << "\n";
    TIT.productVectorTransposed(dx, res);
    // TIT.productVector(dx, res);
    // std::cerr << iter << " res=" << rr << " : " << xi << "\t" << xa <<  " =? " << x <<  "  dx=" << dx << "\n";
    // std::cerr << "FEMBase::reInitPhysicalPoint() dx -> " <<  dx << "\n";
    // std::cerr << "getTransformation() " <<  getTransformation()->getName() << "curved="<<_curved<<"\n";
    double omega = 0.25;
    for(int irelax = 0; irelax < nrelax; irelax++)
    {
      double relax = pow(omega, irelax);
      xi.add(relax, dx);
      getTransformation()->reInitPointCell(xi);
      xa =  getTransformation()->x();
      res = x;
      res.add(-1.0, xa);
      rr = arma::norm(res);
      if(irelax)
      {
        // std::cerr<<"iter irelax relax "<<iter<<" "<<irelax<<" --> "<<relax<<" rr rrold  "<<rr<<" "<<rrold<<" ==> dx xi "<<dx<<" : "<<xi<< " (xa x)" <<xa << " : " << x <<    "\n";
      }
      if(rr < rrold)
      {
        rrold = rr;
        break;
      }
      if(irelax < nrelax-1)
      {
        xi.add(-relax, dx);
      }
      else
      {
        std::cerr<<"*** ERROR FEMBase::reInitPhysicalPoint() Newton not converged "<<arma::norm(res)<<"\n";
        std::cerr<<"*** ERROR FEMBase::reInitPhysicalPoint() too many relaxations "<<irelax<<"\n";
        std::cerr << "\n" << getTransformation()->getNodes() << "\n";
        assert(0);
        exit(1);
      }
    }
  }
  if(!ok)
  {
    std::cerr<<"*** ERROR FEMBase::reInitPhysicalPoint() Newton not converged "<<arma::norm(res)<<"\n";
    std::cerr<<"*** ERROR FEMBase::reInitPhysicalPoint() Newton not converged x "<<x<<"\n";
    // xi = 0.0;
    xi.zeros();
    getTransformation()->reInitPointCell(xi);
    std::cerr<<"*** ERROR FEMBase::reInitPhysicalPoint() Newton not converged  getTransformation()->x() "<<getTransformation()->x()<<"\n";
    std::cerr<<"*** ERROR FEMBase::reInitPhysicalPoint() Newton not converged  getTransformation()->getInvJacobian() "<<getTransformation()->getInvJacobian()<<"\n";
    std::cerr << "\n" << getTransformation()->getNodes() << "\n";
    assert(0);
    exit(1);
  }
  reInitReferencePoint(xi);
  return;
}

/*---------------------------------------------------------*/

void FEMBase::reInitPhysicalPoint1(const Alat::Node& x)
{
  const Q12DTransformation* q12dtrafo = dynamic_cast<const Q12DTransformation*>( getTransformation() );
  assert(q12dtrafo);
  Alat::Vector<Alat::Node> nodes = q12dtrafo->getNodes();

  Alat::DoubleMatrix A(2,2), M(2,2), MI(2,2), T(2,2);
  Alat::Node b, c, f;

  // std::cerr << "nodes = " << nodes << "\n";

  b[0] = 0.25*( nodes[0].x() + nodes[1].x() + nodes[2].x() + nodes[3].x() );
  b[1] = 0.25*( nodes[0].y() + nodes[1].y() + nodes[2].y() + nodes[3].y() );

  c[0] = 0.25*( nodes[0].x() - nodes[1].x() + nodes[2].x() - nodes[3].x() );
  c[1] = 0.25*( nodes[0].y() - nodes[1].y() + nodes[2].y() - nodes[3].y() );


  A(0, 0) = 0.25*( -nodes[0].x() + nodes[1].x() + nodes[2].x() - nodes[3].x() );
  A(1, 0) = 0.25*( -nodes[0].y() + nodes[1].y() + nodes[2].y() - nodes[3].y() );

  A(0, 1) = 0.25*( -nodes[0].x() - nodes[1].x() + nodes[2].x() + nodes[3].x() );
  A(1, 1) = 0.25*( -nodes[0].y() - nodes[1].y() + nodes[2].y() + nodes[3].y() );




  f = x;
  f.add(-1.0, b);

  Alat::Node xi;
  xi.zeros();
  xi[0] = 3.0;
  xi[1] = -2.0;


  // Alat::Vector<Alat::Node> refcoord(4);
  // refcoord[0].x() = -1.0; refcoord[0].y() = -1.0;
  // refcoord[1].x() =  1.0; refcoord[1].y() = -1.0;
  // refcoord[2].x() =  1.0; refcoord[2].y() =  1.0;
  // refcoord[3].x() = -1.0; refcoord[3].y() =  1.0;
  // for(int i=0;i<4;i++)
  // {
  //   Alat::Node x = b;
  //   A.productVector(x,refcoord[i]);
  //   x.x() += c[0]*refcoord[i].x()*refcoord[i].y();
  //   x.y() += c[1]*refcoord[i].x()*refcoord[i].y();
  //   x.add(-1.0, nodes[i]);
  //   std::cerr << "i=" << i <<  " x="  << x << "\n";
  // }
  // assert(0);

  std::cerr << "A = " << A << "\n";
  std::cerr << "c = " << c << "\n";

  double detA = A(0, 0)*A(1, 1)-A(1, 0)*A(0, 1);
  std::cerr << "det A = " << detA << "\n";

  double cx = A(0, 0)*c[1]-A(1, 0)*c[0];
  double cy = A(1, 1)*c[0]-A(0, 1)*c[1];
  std::cerr << "COEFF EQU SING " << cx << "  >===< "  << cy << "\n";

  std::cerr << "EQU SING  y=" << -detA/cy << " + " << -cx/cy << " *x "   << "\n";

  // getTransformation()->reInitPointCell(xi);
  // std::cerr << "getTransformation()->x() = " << getTransformation()->x() << "\n";
  // std::cerr << "getTransformation()->getJacobian() = " << getTransformation()->getJacobian() << "\n";


  
  
  
  
  Alat::Node bchap, cchap;
  Alat::DoubleMatrix B(A);
  B = A;
  B.gaussJordan();
  cchap.zeros();
  B.productVector(cchap, c);
  B.productVector(bchap, b);

  double d = cchap[1]*bchap[0]-cchap[0]*bchap[1];
  double Delta = ( d+1.0 )*( d+1.0 ) + 4.0*cchap[0]*bchap[1];

  std::cerr << "Delta = " << Delta;
  std::cerr << "Delta = " << Delta;
  std::cerr << "Delta = " << Delta;
  std::cerr << "Delta = " << Delta;
  assert(0);

  Alat::Node lambda;
  lambda.zeros();

  int niter = 50;
  int nrelax = 20;
  double eps = 1e-10, rrold;
  Alat::Node res, dx, res2;
  bool ok = 0;

  res = f;
  A.productVector(res, xi, -1.0);
  res[0] -= c[0]*xi[0]*xi[1];
  res[1] -= c[1]*xi[0]*xi[1];


  rrold = arma::norm(res);
  for(int iter = 0; iter < niter; iter++)
  {
    std::cerr << "iter=" << iter << " res=" << arma::norm(res) << " xi= " << xi << "\n";
    if(arma::norm(res) < eps)
    {
      getTransformation()->reInitPointCell(xi);
      // std::cerr << iter << " OK ?? " << xi << "\t" << getTransformation()->x() <<  " =? " << x <<  "\n";
      ok = 1;
      break;
    }
    T = A;
    T(0, 0) += c[0]*xi[1];
    T(0, 1) += c[0]*xi[0];
    T(1, 0) += c[1]*xi[1];
    T(1, 1) += c[1]*xi[0];
    std::cerr << "\tT=" << T << "\n";
    T.gaussJordan();
    dx.zeros();
    T.productVector(dx, res);
    std::cerr << "\tdx=" << dx << "\n";

    double omega = 0.6;
    for(int irelax = 0; irelax < nrelax; irelax++)
    {
      double relax = pow(omega, irelax);
      xi.add(relax, dx);

      res = f;
      A.productVector(res, xi, -1.0);
      res[0] -= c[0]*xi[0]*xi[1];
      res[1] -= c[1]*xi[0]*xi[1];
      // std::cerr << "res res2 " << res << " =? " << res2 << "\n";
      // res2.add(-1.0,res);
      assert(arma::norm(res2) < 1e-10);

      double rr = arma::norm(res);
      if(irelax)
      {
        std::cerr<<"iter irelax relax rr rrold dx xi "<<iter<<" "<<irelax<<" --> "<<relax<<" "<<rr<<" "<<rrold<<" ==> "<<dx<<" : "<<xi<<"\n";
      }
      if(rr < rrold)
      {
        rrold = rr;
        break;
      }
      if(irelax < nrelax-1)
      {
        xi.add(-relax, dx);
      }
      else
      {
        std::cerr<<"*** ERROR FEMBase::reInitPhysicalPoint() Newton not converged "<<arma::norm(res)<<"\n";
        std::cerr<<"*** ERROR FEMBase::reInitPhysicalPoint() too many relaxations "<<irelax<<"\n";
        assert(0);
        exit(1);
      }
    }
  }
  if(!ok)
  {
    std::cerr<<"*** ERROR FEMBase::reInitPhysicalPoint() Newton not converged "<<arma::norm(res)<<"\n";
    std::cerr<<"*** ERROR FEMBase::reInitPhysicalPoint() Newton not converged x "<<x<<"\n";
    // xi = 0.0;
    xi.zeros();
    getTransformation()->reInitPointCell(xi);
    std::cerr<<"*** ERROR FEMBase::reInitPhysicalPoint() Newton not converged  getTransformation()->x() "<<getTransformation()->x()<<"\n";
    std::cerr<<"*** ERROR FEMBase::reInitPhysicalPoint() Newton not converged  getTransformation()->getInvJacobian() "<<getTransformation()->getInvJacobian()<<"\n";
    assert(0);
    exit(1);
  }
  reInitReferencePoint(xi);
  return;
}

/*---------------------------------------------------------*/

void FEMBase::reInitPhysicalPoint2(const Alat::Node& x)
{
  Alat::Node xi;
  xi.zeros();

  Alat::Node xa, xia, xmin, xmax;

  std::ofstream file("toto.vtk");


  xmin.x() = -3.0;
  xmin.y() = -3.0;
  xmax.x() =  3.0;
  xmax.y() =  3.0;

  int nx = 30, ny = 30, nz = 1;
  double dx = (xmax.x()-xmin.x())/nx, dy = (xmax.y()-xmin.y())/ny;
  
  file << "# vtk DataFile Version 3.0\n";
  file << "vtk output\n";
  file << "ASCII\n";
  file << "DATASET STRUCTURED_GRID\nDIMENSIONS "<< nx << " " << ny << " " <<  nz << "\n";
  file << "POINTS "<< nx*ny*nz << " float\n";

  for(int ix = 0; ix < nx; ix++)
  {
    for(int iy = 0; iy < ny; iy++)
    {
      xia.x() = ix*dx + xmin.x();
      xia.y() = iy*dy + xmin.y();
      
      getTransformation()->reInitPointCell(xia);
      xa =  getTransformation()->x();
      file << xa << "\n";
    }
  }
  file.close();
}
