#include  "Fada/feminterface.h"
#include  "Fada/integrationformulainterface.h"
#include  "FadaMesh/meshinterface.h"
#include  "Fada/transformationbase.h"
#include  <math.h>
#include  <stdlib.h>

using namespace Fada;
using namespace std;

/*-------------------------------------------------------*/

TransformationBase::~TransformationBase() {}
TransformationBase::TransformationBase() : Fada::TransformationInterface(), _basicinit(0){}

TransformationBase::TransformationBase(const TransformationBase& orig)
{
  _dtrans = orig._dtrans;
  _trans.set_size(orig._trans.size());
  _trans = orig._trans;
  _nodes = orig._nodes;

  _basicinit = orig._basicinit;
  _sfcomputed = orig._sfcomputed;
  _normalhat = orig._normalhat;
  // _mesh = orig._mesh;
  _dim = orig._dim;
  _Jacobian.set_size(orig._Jacobian.n_rows, orig._Jacobian.n_cols);
  _Jacobian = orig._Jacobian;
  _InvJacobian.set_size(orig._InvJacobian.n_rows, orig._InvJacobian.n_cols);
  _InvJacobian = orig._InvJacobian;
  _CoFacJacobian.set_size(orig._CoFacJacobian.n_rows, orig._CoFacJacobian.n_cols);
  _CoFacJacobian = orig._CoFacJacobian;
  _DetJ = orig._DetJ;
  _x = orig._x;
  _iiS = orig._iiS;
  _Normal = orig._Normal;
  _NonUnitNormal = orig._NonUnitNormal;
  _SF = orig._SF;
  _dtrans.set_size(orig._dtrans.size());
  for(int i = 0; i < orig._dtrans.size(); i++)
  {
    _dtrans[i]=orig._dtrans[i];
  }
}

TransformationBase& TransformationBase::operator=( const TransformationBase& transformationinterface)
{
  assert(0);
}

/*-------------------------------------------------------*/

void TransformationBase::_memory(int dim)
{
  _dim = dim;
  int nnodes = getNNodes();
  _nodes.set_size(nnodes);
  _trans.set_size(nnodes);
  _dtrans.set_size(nnodes);
  _Jacobian.set_size(dim, dim);
  _InvJacobian.set_size(dim, dim);
  _CoFacJacobian.set_size(dim, dim);
  _basicinit = 1;
}

/*-------------------------------------------------------*/

void TransformationBase::basicInit(const FadaMesh::MeshInterface* mesh)
{
  // _mesh = mesh;
  _memory( mesh->getDimension() );
}
//
// /*-------------------------------------------------------*/
//
// void TransformationBase::basicInit(int dim)
// {
//   _mesh = NULL;
//   _memory( dim );
// }
//
// /*-------------------------------------------------------*/
//
// void TransformationBase::reInitCell(int iK)
// {
//   for(int ii = 0; ii < _nodes.size(); ii++)
//   {
//     _nodes[ii] = getMesh()->getNodeOfCell(iK, ii);
//   }
//   reInitPointCell();
// }

/*-------------------------------------------------------*/

void TransformationBase::reInitCell(const Alat::Vector<Alat::Node>& nodes)
{
  assert( nodes.size() == getNNodes() );
  for(int ii = 0; ii < _nodes.size(); ii++)
  {
    _nodes[ii] = nodes[ii];
  }
}

/*-------------------------------------------------------*/

void TransformationBase::reInitPointCell(const Alat::Node& xhat)
{
  // std::cerr << "#TransformationBase::reInitPointCell() xhat "<<xhat<<"\n";
  xhattox(xhat);
  dtrans(xhat);
  // std::cerr << "#TransformationBase::reInitPointCell() _nodes "<<_nodes<<"\n";
  _Jacobian.zeros();
  if(_dim == 1)
  {
    for(int ii = 0; ii < _nodes.size(); ii++)
    {
      _Jacobian(0, 0) += _dtrans[ii].x()*_nodes[ii].x();
    }
    _DetJ = _Jacobian(0, 0);
    _CoFacJacobian(0, 0) = _Jacobian(0, 0);
    //matrice jacobienne inverse transposée
    _InvJacobian(0, 0) = 1./_DetJ;
  }
  else if(_dim == 2)
  {
    for(int ii = 0; ii < _nodes.size(); ii++)
    {
      _Jacobian(0, 0) += _dtrans[ii].x()*_nodes[ii].x();
      _Jacobian(0, 1) += _dtrans[ii].y()*_nodes[ii].x();
      _Jacobian(1, 0) += _dtrans[ii].x()*_nodes[ii].y();
      _Jacobian(1, 1) += _dtrans[ii].y()*_nodes[ii].y();
    }
    _DetJ = _Jacobian(0, 0)*_Jacobian(1, 1)-_Jacobian(0, 1)*_Jacobian(1, 0);
    // Matrice des cofacteurs
    _CoFacJacobian(0, 0) =  _Jacobian(1, 1);
    _CoFacJacobian(0, 1) = -_Jacobian(1, 0);
    _CoFacJacobian(1, 0) = -_Jacobian(0, 1);
    _CoFacJacobian(1, 1) =  _Jacobian(0, 0);
    //matrice jacobienne inverse transposée
    _InvJacobian = _CoFacJacobian;
    _InvJacobian *= 1./_DetJ;
  }
  else if(_dim == 3)
  {
    for(int ii = 0; ii < _nodes.size(); ii++)
    {
      //
      _Jacobian(0, 0) += _dtrans[ii].x()*_nodes[ii].x();
      _Jacobian(0, 1) += _dtrans[ii].y()*_nodes[ii].x();
      _Jacobian(0, 2) += _dtrans[ii].z()*_nodes[ii].x();
      _Jacobian(1, 0) += _dtrans[ii].x()*_nodes[ii].y();
      _Jacobian(1, 1) += _dtrans[ii].y()*_nodes[ii].y();
      _Jacobian(1, 2) += _dtrans[ii].z()*_nodes[ii].y();
      _Jacobian(2, 0) += _dtrans[ii].x()*_nodes[ii].z();
      _Jacobian(2, 1) += _dtrans[ii].y()*_nodes[ii].z();
      _Jacobian(2, 2) += _dtrans[ii].z()*_nodes[ii].z();
    }

    //
    _DetJ = _Jacobian(0, 0)*_Jacobian(1, 1)*_Jacobian(2, 2)+
      _Jacobian(0, 1)*_Jacobian(1, 2)*_Jacobian(2, 0)+
      _Jacobian(0, 2)*_Jacobian(1, 0)*_Jacobian(2, 1)-
      _Jacobian(0, 2)*_Jacobian(1, 1)*_Jacobian(2, 0)-
      _Jacobian(0, 0)*_Jacobian(1, 2)*_Jacobian(2, 1)-
      _Jacobian(0, 1)*_Jacobian(1, 0)*_Jacobian(2, 2);
    //
    _CoFacJacobian(0, 0) =  _Jacobian(1, 1)*_Jacobian(2, 2) - _Jacobian(2, 1)*_Jacobian(1, 2);
    _CoFacJacobian(0, 1) = -_Jacobian(1, 0)*_Jacobian(2, 2) + _Jacobian(2, 0)*_Jacobian(1, 2);
    _CoFacJacobian(0, 2) =  _Jacobian(1, 0)*_Jacobian(2, 1) - _Jacobian(2, 0)*_Jacobian(1, 1);
    _CoFacJacobian(1, 0) = -_Jacobian(0, 1)*_Jacobian(2, 2) + _Jacobian(2, 1)*_Jacobian(0, 2);
    _CoFacJacobian(1, 1) =  _Jacobian(0, 0)*_Jacobian(2, 2) - _Jacobian(2, 0)*_Jacobian(0, 2);
    _CoFacJacobian(1, 2) = -_Jacobian(0, 0)*_Jacobian(2, 1) + _Jacobian(2, 0)*_Jacobian(0, 1);
    _CoFacJacobian(2, 0) =  _Jacobian(0, 1)*_Jacobian(1, 2) - _Jacobian(1, 1)*_Jacobian(0, 2);
    _CoFacJacobian(2, 1) = -_Jacobian(0, 0)*_Jacobian(1, 2) + _Jacobian(1, 0)*_Jacobian(0, 2);
    _CoFacJacobian(2, 2) =  _Jacobian(0, 0)*_Jacobian(1, 1) - _Jacobian(1, 0)*_Jacobian(0, 1);
    //
    _InvJacobian = _CoFacJacobian;
    _InvJacobian *= 1./_DetJ;
    if(_DetJ<0.0)
    {
        for(int ii = 0; ii < _nodes.size(); ii++)
        {
          std::cerr << "ii="<<ii<< " node =" << _nodes[ii] <<"\n";
        }
        assert(0);
    }
    // std::cerr << "#TransBase::reInitPointCell() _Jacobian "<<_Jacobian<<"\n";
    // std::cerr << "#TransBase::reInitPointCell() _InvJacobian "<<_InvJacobian<<"\n";
  }
  else
  {
    std::cerr<<"TransformationBase::reInitTransformation: dimension dim incorrecte"<<std::endl;
    assert(0);
  }
}

/*--------------------------------------------------------------*/
void TransformationBase::reInitPointCell()
{
  Alat::Node xhat;
  xhat.x() = 0.;
  xhat.y() = 0.;
  xhat.z() = 0.;
  reInitPointCell(xhat);
}

/*-------------------------------------------------------*/
void TransformationBase::reInitPointSide(const Alat::Node& xhat)
{
  reInitPointCell(xhat);
  _computeNormal( _iiS );
}

/*-------------------------------------------------------*/
void TransformationBase::reInitSide(int iiS, const Alat::Node& xhat)
{
  _iiS = iiS;
  reInitPointCell(xhat);
  _computeNormal( iiS );
}

/*-------------------------------------------------------*/
void TransformationBase::reInitSide(int iiS)
{
  // assert(0);
  // _iiS = iiS;
  Alat::Node shat, xhat;
  shat.x() = 0.;
  shat.y() = 0.;
  shat.z() = 0.;
  ShatToXhat(xhat, shat, iiS);
  reInitSide(iiS, xhat);
}

/*-------------------------------------------------------*/
void TransformationBase::reInitSide(const Alat::Node& xhat, const Alat::Node& normalhat)
{
  reInitPointCell(xhat);
  _SF = _computeNonUnitNormal( normalhat );
}

/*--------------------------------------------------------------*/
void TransformationBase::computeReferencePoint(Alat::Node& xhat, const Alat::Node& shat)
{
  Alat::Node s = shat;
  correctIntegrationPoint(s);
  // std::cerr << "TransformationBase::computeReferencePoint() shat="<<shat<<" s="<<s << " xhat="<<xhat<<"\n";
  ShatToXhat( xhat, s, getLocalIndexOfSide() );
}

/*--------------------------------------------------------------*/
void TransformationBase::xhattox(const Alat::Node& xhat)
{
  trans(xhat);
  _x.x() = 0.0;
  _x.y() = 0.0;
  _x.z() = 0.0;
  for(int i = 0; i < _nodes.size(); i++)
  {
    _x.x() += _trans[i]*_nodes[i].x();
    _x.y() += _trans[i]*_nodes[i].y();
    if(_dim == 3)
    {
      _x.z() += _trans[i]*_nodes[i].z();
    }
  }
//   std::cerr << "xhattox() nodes " << _nodes << "\n";
//   std::cerr << "xhattox() _trans " << _trans << "\n";
//   std::cerr << "xhattox() " << xhat << " ==> " << _x << "\n";
}

/*---------------------------------------------------------*/
void TransformationBase::_computeNormal(int iiS)
{
//   std::cerr << "@ TransformationBase::reInitPointCell() _computeNormal iiS "<<iiS<<"\n";
  setNormalHat( iiS, _normalhat );
  // computeNormal( _normalhat );
//   std::cerr << "@@ TransformationBase::reInitPointCell() _computeNormal _normalhat "<<_normalhat<<"\n";
  _SF = _computeNonUnitNormal( _normalhat);
  _Normal = _NonUnitNormal;
  // std::cerr << "@ TransformationBase::reInitPointCell() _computeNormal _Normal "<<_Normal<<" _SF="<<_SF<<"\n";
  _Normal *= 1.0/_SF;
  // _Normal.scale(1.0/_SF);
  // std::cerr << "@@ TransformationBase::reInitPointCell() _computeNormal _Normal "<<_Normal<<" _SF="<<_SF<<"\n";
  _SF *= getSurfaceJacobianHat(iiS);
//   std::cerr<<"**********"<<"iiS= "<<iiS<<"_SF="<<_SF<<std::endl;
}

/*--------------------------------------------------------------*/

double TransformationBase::_computeNonUnitNormal( const Alat::Node& normalhat)
{
  if(_dim == 1)
  {
    _NonUnitNormal.x() = _CoFacJacobian(0, 0)*normalhat.x();
    _NonUnitNormal.y() = 0.0;
    _NonUnitNormal.z() = 0.0;

    // _SF = sqrt( _NonUnitNormal.x()*_NonUnitNormal.x() );
    return  sqrt( _NonUnitNormal.x()*_NonUnitNormal.x() );
    //std::cerr<<"TransformationBase::Non_Unit_Normal= "<<_NonUnitNormal<< " SF= "<<_SF<<std::endl;
  }
  else if(_dim == 2)
  {
    _NonUnitNormal.x() = _CoFacJacobian(0, 0)*normalhat.x()+_CoFacJacobian(0, 1)*normalhat.y();
    _NonUnitNormal.y() = _CoFacJacobian(1, 0)*normalhat.x()+_CoFacJacobian(1, 1)*normalhat.y();
    _NonUnitNormal.z() = 0.0;
    // _SF = sqrt( _NonUnitNormal.x()*_NonUnitNormal.x()+_NonUnitNormal.y()*_NonUnitNormal.y() );
    return sqrt( _NonUnitNormal.x()*_NonUnitNormal.x()+_NonUnitNormal.y()*_NonUnitNormal.y() );
    // return pow( _NonUnitNormal.x()*_NonUnitNormal.x()+_NonUnitNormal.y()*_NonUnitNormal.y(), 0.5 );
    //std::cerr<<"TransformationBase::Non_Unit_Normal= "<<_NonUnitNormal<< " SF= "<<_SF<<std::endl;
  }
  else if(_dim == 3)
  {
    _NonUnitNormal.x() = _CoFacJacobian(0, 0)*normalhat.x()+_CoFacJacobian(0, 1)*normalhat.y()+_CoFacJacobian(0, 2)*normalhat.z();
    _NonUnitNormal.y() = _CoFacJacobian(1, 0)*normalhat.x()+_CoFacJacobian(1, 1)*normalhat.y()+_CoFacJacobian(1, 2)*normalhat.z();
    _NonUnitNormal.z() = _CoFacJacobian(2, 0)*normalhat.x()+_CoFacJacobian(2, 1)*normalhat.y()+_CoFacJacobian(2, 2)*normalhat.z();
    // _SF = sqrt( _NonUnitNormal.x()*_NonUnitNormal.x()+_NonUnitNormal.y()*_NonUnitNormal.y()+_NonUnitNormal.z()*_NonUnitNormal.z() );
    return sqrt( _NonUnitNormal.x()*_NonUnitNormal.x()+_NonUnitNormal.y()*_NonUnitNormal.y()+_NonUnitNormal.z()*_NonUnitNormal.z() );
    // std::cerr<<"TransformationBase::Non_Unit_Normal= "<<_NonUnitNormal<< " SF= "<<_SF<<std::endl;
  }
  else
  {
    std::cerr<<"*** ERROR TransformationBase::NonUnitNormal(): error dimension space in TransformationBase::Non_Unit_Normal"<<std::endl;
    exit(1);
  }
}

//
// /*--------------------------------------------------------------*/
// void TransformationBase::NonUnitNormal( int iiS)
// {
//   NormalHat( iiS );
//   NonUnitNormal( getNormalHat( ) );
// }

/*--------------------------------------------------------------*/
/*---------------------------------------------------------*/

const double TransformationBase::getHOfSide() const
{
  if(_dim == 1)
  {
    return 1.0;
  }
  return getDetJacobian()/getSurfaceJacobian();
  return pow ( getSurfaceJacobian(), 1.0/( _dim-1.0 ) );
}

/*---------------------------------------------------------*/

const double TransformationBase::getHOfCell() const
{
  return pow(getDetJacobian(), 1.0/_dim);
}
