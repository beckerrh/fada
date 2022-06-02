#include  "Fada/feminterface.h"
#include  "Fada/integrationformulainterface.h"
#include  "FadaMesh/meshinterface.h"
#include  "Alat/node.h"
#include  "Fada/dofinformation.h"
#include  "Fada/transfermatrixconstructor.h"
#include  <cassert>
#include  <stdlib.h>

using namespace Fada;

/*--------------------------------------------------------------------------*/
TransferMatrixConstructor::~TransferMatrixConstructor(){}
TransferMatrixConstructor::TransferMatrixConstructor(){}
TransferMatrixConstructor::TransferMatrixConstructor( const TransferMatrixConstructor& transfermatrixconstructor)
{
  assert(0);
}
TransferMatrixConstructor::TransferMatrixConstructor(Alat::DoubleMatrix& interpolmat, Alat::DoubleMatrix& projectmat, DofInformation* dofinformation)
{
  // std::cerr << "TransferMatrixConstructor::TransferMatrixConstructor() fem="<<dofinformation->getFem()->getName()<<"\n";
  // _dim = fem->getMesh()->getDimension();
  _computePhi(dofinformation);
  _computeMatrices(interpolmat, projectmat);
}
TransferMatrixConstructor& TransferMatrixConstructor::operator=( const TransferMatrixConstructor& transfermatrixconstructor)
{
  assert(0);
  return *this;
}
std::string TransferMatrixConstructor::getName() const
{
  return "TransferMatrixConstructor";
}
TransferMatrixConstructor* TransferMatrixConstructor::clone() const
{
  return new TransferMatrixConstructor(*this);
}

/*--------------------------------------------------------------------------*/

void TransferMatrixConstructor::_setNodes(NodeVector& nodes, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax) const
{
  nodes[0].x() =  xmin;
  nodes[0].y() =  ymin;
  nodes[0].z() =  zmin;
  nodes[1].x() =  xmax;
  nodes[1].y() =  ymin;
  nodes[1].z() =  zmin;
  nodes[2].x() =  xmax;
  nodes[2].y() =  ymax;
  nodes[2].z() =  zmin;
  nodes[3].x() =  xmin;
  nodes[3].y() =  ymax;
  nodes[3].z() =  zmin;

  if(nodes.size() == 8)
  {
    nodes[4+0].x() =  xmin;
    nodes[4+0].y() =  ymin;
    nodes[4+0].z() =  zmax;
    nodes[4+1].x() =  xmax;
    nodes[4+1].y() =  ymin;
    nodes[4+1].z() =  zmax;
    nodes[4+2].x() =  xmax;
    nodes[4+2].y() =  ymax;
    nodes[4+2].z() =  zmax;
    nodes[4+3].x() =  xmin;
    nodes[4+3].y() =  ymax;
    nodes[4+3].z() =  zmax;
  }
}

/*--------------------------------------------------------------------------*/

void TransferMatrixConstructor::_computePhi(DofInformation* dofinformation)
{
  const Fada::FemInterface* fem = dofinformation->getFem();
  // std::cerr << "TransferMatrixConstructor::_computePhi() fem="<<fem->getName()<<"\n";
  Fada::IntegrationFormulaInterface* IF = fem->newIntegrationFormulaTransfer();
  assert(IF);

  int ncellsrefinedrefelement;
  if(fem->getReferenceElementType() == "Triangle")
  {
    ncellsrefinedrefelement = 4;
  }
  else if( fem->getReferenceElementType() == "Quadrilateral" )
  {
    ncellsrefinedrefelement = 4;
  }
  else if( fem->getReferenceElementType() == "Hexahedral" )
  {
    ncellsrefinedrefelement = 8;
  }
  else
  {
    std::cerr << "*** ERROR in TransferMatrixConstructor::_computePhi() unknown reference element " << fem->getReferenceElementType() << "\n";
    assert(0);
    exit(1);
  }

  int ncoarse = dofinformation->ndoflocal();
  int nfine = dofinformation->ndoflocalpatch();

  if(nfine == -1)
  {
    assert(0);
  }
  int npoints = ncellsrefinedrefelement*IF->n();
  _phicoarse.set_size(npoints, ncoarse);
  _phifine.set_size(npoints, nfine);
  _weights.set_size(npoints);
  _phifine.zeros();


  if(fem->getReferenceElementType() == "Triangle")
  {
    _computePhiTri(dofinformation, ncellsrefinedrefelement);
  }
  else if( fem->getReferenceElementType() == "Quadrilateral" )
  {
    _computePhiQuadHex(dofinformation, ncellsrefinedrefelement);
  }
  else if( fem->getReferenceElementType() == "Hexahedral" )
  {
    _computePhiQuadHex(dofinformation, ncellsrefinedrefelement);
  }
  else
  {
    assert(0);
  }

  delete IF;
}

/*--------------------------------------------------------------------------*/

void TransferMatrixConstructor::_computePhiTri(DofInformation* dofinformation, int ncellsrefinedrefelement)
{
  Fada::FemInterface* fem = dofinformation->getFem();
  // Fada::FemInterface* fem = dofinformation->getFem()->clone();
  // int ncoarse = fem->ndoflocal();
  // int nfine = fem->ndoflocalpatch();
  int ncoarse = dofinformation->ndoflocal();
  int nfine = dofinformation->ndoflocalpatch();

  if(nfine == -1)
  {
    assert(0);
  }
  Fada::IntegrationFormulaInterface* IF = fem->newIntegrationFormulaTransfer();
  assert(IF);
  // assert(fem->getMesh()->getDimension() == 2);
  int nfine_cells = 4;
  int npoints = nfine_cells*IF->n();
  _phicoarse.set_size(npoints, ncoarse);
  _phifine.set_size(npoints, nfine);
  _weights.set_size(npoints);
  _phifine.zeros();

  // std::cerr << "nfine ncoarse npoints " << nfine << " " << ncoarse << " " << npoints << "\n";

  NodeVector nodes;
  Alat::Vector<NodeVector> nodesfine(ncellsrefinedrefelement);
  NodeVector offsets(ncellsrefinedrefelement);

  nodes.set_size(3);
  for(int ii = 0; ii < ncellsrefinedrefelement; ii++)
  {
    nodesfine[ii].set_size(3);
  }

  // nodes[0].x() =  0.;
  // nodes[0].y() =  0.;
  // nodes[1].x() =  1.;
  // nodes[1].y() =  0.;
  // nodes[2].x() =  0.;
  // nodes[2].y() =  1.;
  // //T0
  // NodeVector& fnodes0 = nodesfine[0];
  // offsets[0].x() = 0.;
  // offsets[0].y() = 0.5;
  // fnodes0[0].x() = 0.;
  // fnodes0[0].y() = 0.5;
  // fnodes0[1].x() = 0.5;
  // fnodes0[1].y() = 0.5;
  // fnodes0[2].x() = 0.;
  // fnodes0[2].y() = 1.;
  // //T1
  // NodeVector& fnodes1 = nodesfine[1];
  // offsets[1].x() = 0.;
  // offsets[1].y() = 0.;
  // fnodes1[0].x() = 0.;
  // fnodes1[0].y() = 0.;
  // fnodes1[1].x() = 0.5;
  // fnodes1[1].y() = 0.;
  // fnodes1[2].x() = 0.;
  // fnodes1[2].y() = 0.5;
  // //T2
  // NodeVector& fnodes2 = nodesfine[2];
  // offsets[2].x() = 0.5;
  // offsets[2].y() = 0.;
  // fnodes2[0].x() = 0.5;
  // fnodes2[0].y() = 0.;
  // fnodes2[1].x() = 1.;
  // fnodes2[1].y() = 0.;
  // fnodes2[2].x() = 0.5;
  // fnodes2[2].y() = 0.5;
  // //T3
  // NodeVector& fnodes3 = nodesfine[3];
  // offsets[3].x() = 0.5;
  // offsets[3].y() = 0.5;
  // fnodes3[0].x() = 0.5;
  // fnodes3[0].y() = 0.5;
  // fnodes3[1].x() = 0.;
  // fnodes3[1].y() = 0.5;
  // fnodes3[2].x() = 0.5;
  // fnodes3[2].y() = 0.;

  nodes[0].x() =  0.;
  nodes[0].y() =  0.;
  nodes[1].x() =  1.;
  nodes[1].y() =  0.;
  nodes[2].x() =  0.;
  nodes[2].y() =  1.;
  //T0
  NodeVector& fnodes0 = nodesfine[0];
  offsets[0].x() = 0.;
  offsets[0].y() = 0.5;
  fnodes0[0].x() = 0.;
  fnodes0[0].y() = 0.5;
  fnodes0[1].x() = 0.5;
  fnodes0[1].y() = 0.5;
  fnodes0[2].x() = 0.;
  fnodes0[2].y() = 1.;
  //T1
  NodeVector& fnodes1 = nodesfine[1];
  offsets[1].x() = 0.;
  offsets[1].y() = 0.;
  fnodes1[0].x() = 0.;
  fnodes1[0].y() = 0.;
  fnodes1[1].x() = 0.5;
  fnodes1[1].y() = 0.;
  fnodes1[2].x() = 0.;
  fnodes1[2].y() = 0.5;
  //T2
  NodeVector& fnodes2 = nodesfine[2];
  offsets[2].x() = 0.5;
  offsets[2].y() = 0.;
  fnodes2[0].x() = 0.5;
  fnodes2[0].y() = 0.;
  fnodes2[1].x() = 1.;
  fnodes2[1].y() = 0.;
  fnodes2[2].x() = 0.5;
  fnodes2[2].y() = 0.5;
  //T3
  NodeVector& fnodes3 = nodesfine[3];
  offsets[3].x() = 0.5;
  offsets[3].y() = 0.5;
  fnodes3[0].x() = 0.5;
  fnodes3[0].y() = 0.5;
  fnodes3[1].x() = 0.;
  fnodes3[1].y() = 0.5;
  fnodes3[2].x() = 0.5;
  fnodes3[2].y() = 0.;

  int count = 0;
  fem->reInitCell(nodes);
  for(int iiK = 0; iiK < 3; iiK++)
  {
    for(int k = 0; k < IF->n(); k++)
    {
      Alat::Node xi;
      IF->point(xi, k);
      xi.x() = offsets[iiK].x()+0.5*xi.x();
      xi.y() = offsets[iiK].y()+0.5*xi.y();
      // std::cerr << iiK << " " << k << " -> " << xi << "\n";
      fem->reInitReferencePoint(xi);
      _weights[count] = 0.25*IF->weight(k);
      for(int ii = 0; ii < ncoarse; ii++)
      {
        _phicoarse(count, ii) = fem->phi(ii);
      }
      count++;
    }
  }
  for(int k = 0; k < IF->n(); k++)
  {
    Alat::Node xi;
    IF->point(xi, k);
    xi.x() = offsets[3].x()-0.5*xi.x();
    xi.y() = offsets[3].y()-0.5*xi.y();
    // std::cerr << iiK << " " << k << " -> " << xi << "\n";
    fem->reInitReferencePoint(xi);
    _weights[count] = 0.25*IF->weight(k);
    for(int ii = 0; ii < ncoarse; ii++)
    {
      _phicoarse(count, ii) = fem->phi(ii);
    }
    count++;
  }
  // std::cerr << "TransferMatrixConstructor::_phicoarse \n\n" << _phicoarse  << "\n";
  // assert(0);
  // std::cerr << "TransferMatrixConstructor::_ComputePhi() _weights _phicoarse _phifine\n\n" << _weights << "\n\n" << _phicoarse << "\n\n" << _phifine << "\n";
  count = 0;
  _phifine.zeros();
  for(int iiK = 0; iiK < 4; iiK++)
  {
    // std::cerr << count << " _phifine " << _phifine << "\n";
    fem->reInitCell(nodesfine[iiK]);
    Alat::IntVector indices;
    dofinformation->getLocalCellIndicesPatch(iiK, indices);
    for(int k = 0; k < IF->n(); k++)
    {
      Alat::Node xi;
      IF->point(xi, k);
      fem->reInitReferencePoint(xi);
      for(int ii = 0; ii < indices.size(); ii++)
      {
        if(_phifine(count, indices[ii]) != 0.0)
        {
          std::cerr << "*** indices=" << indices << "\n";
          std::cerr << "*** _phifine=" << _phifine << "\n";
          std::cerr << "*** _phifine=" << _phifine << "\n";
          std::cerr<<_phifine(count, indices[ii])<<" count "<<count<<" ii "<<ii<<" indices "<<indices<<"\n";
          for(int ll = 0; ll < nfine; ll++)
          {
            std::cerr<<_phifine(count, ll)<<" ";
          }
          std::cerr<<"\n";
          std::cerr<<"*** ERROR in TransferMatrixConstructor::_ComputePhi()\n";
          std::cerr<<"*** ERROR in TransferMatrixConstructor::fem->getName() "<< fem->getName()<<"\n";
          assert(0);
          exit(1);
        }
        _phifine(count, indices[ii]) = fem->phi(ii);
      }
      count++;
    }
  }
  // std::cerr << "fem->getName()="<<fem->getName()<<"\n";
  // std::cerr << "_phifine="<<_phifine<<"\n";

  // Fada::FemInterface* fem = dofinformation->getFem();
  // std::cerr << "TransferMatrixConstructor::_computePhiTri() fem="<<fem->getName()<<"\n";
  // Fada::IntegrationFormulaInterface* IF = fem->newIntegrationFormulaTransfer();
  // assert(IF);
  // int ncoarse = dofinformation->ndoflocal();
  // int nfine = dofinformation->ndoflocalpatch();
  //
  // NodeVector nodes;
  // Alat::Vector<NodeVector> nodesfine(ncellsrefinedrefelement);
  // NodeVector offsets(ncellsrefinedrefelement);
  //
  // nodes.set_size(3);
  // for(int ii = 0; ii < ncellsrefinedrefelement; ii++)
  // {
  //   nodesfine[ii].set_size(3);
  // }
  // nodes[0].x() =  0.;
  // nodes[0].y() =  0.;
  // nodes[1].x() =  1.;
  // nodes[1].y() =  0.;
  // nodes[2].x() =  0.;
  // nodes[2].y() =  1.;
  // //T0
  // NodeVector& fnodes0 = nodesfine[0];
  // offsets[0].x() = 0.;
  // offsets[0].y() = 0.5;
  // fnodes0[0].x() = 0.;
  // fnodes0[0].y() = 0.5;
  // fnodes0[1].x() = 0.5;
  // fnodes0[1].y() = 0.5;
  // fnodes0[2].x() = 0.;
  // fnodes0[2].y() = 1.;
  // //T1
  // NodeVector& fnodes1 = nodesfine[1];
  // offsets[1].x() = 0.;
  // offsets[1].y() = 0.;
  // fnodes1[0].x() = 0.;
  // fnodes1[0].y() = 0.;
  // fnodes1[1].x() = 0.5;
  // fnodes1[1].y() = 0.;
  // fnodes1[2].x() = 0.;
  // fnodes1[2].y() = 0.5;
  // //T2
  // NodeVector& fnodes2 = nodesfine[2];
  // offsets[2].x() = 0.5;
  // offsets[2].y() = 0.;
  // fnodes2[0].x() = 0.5;
  // fnodes2[0].y() = 0.;
  // fnodes2[1].x() = 1.;
  // fnodes2[1].y() = 0.;
  // fnodes2[2].x() = 0.5;
  // fnodes2[2].y() = 0.5;
  // //T3
  // NodeVector& fnodes3 = nodesfine[3];
  // offsets[3].x() = 0.5;
  // offsets[3].y() = 0.5;
  // fnodes3[0].x() = 0.5;
  // fnodes3[0].y() = 0.5;
  // fnodes3[1].x() = 0.;
  // fnodes3[1].y() = 0.5;
  // fnodes3[2].x() = 0.5;
  // fnodes3[2].y() = 0.;
  //
  // int count = 0;
  // fem->reInitCell(nodes);
  // for(int iiK = 0; iiK < 3; iiK++)
  // {
  //   for(int k = 0; k < IF->n(); k++)
  //   {
  //     Alat::Node xi;
  //     IF->point(xi, k);
  //     xi.x() = offsets[iiK].x()+0.5*xi.x();
  //     xi.y() = offsets[iiK].y()+0.5*xi.y();
  //     // std::cerr << iiK << " " << k << " -> " << xi << "\n";
  //     fem->reInitReferencePoint(xi);
  //     _weights[count] = 0.25*IF->weight(k);
  //     for(int ii = 0; ii < ncoarse; ii++)
  //     {
  //       _phicoarse(count, ii) = fem->phi(ii);
  //     }
  //     count++;
  //   }
  // }
  // for(int k = 0; k < IF->n(); k++)
  // {
  //   Alat::Node xi;
  //   IF->point(xi, k);
  //   xi.x() = offsets[3].x()-0.5*xi.x();
  //   xi.y() = offsets[3].y()-0.5*xi.y();
  //   // std::cerr << iiK << " " << k << " -> " << xi << "\n";
  //   fem->reInitReferencePoint(xi);
  //   _weights[count] = 0.25*IF->weight(k);
  //   for(int ii = 0; ii < ncoarse; ii++)
  //   {
  //     _phicoarse(count, ii) = fem->phi(ii);
  //   }
  //   count++;
  // }
  //
  // _computePhi2(IF, dofinformation, nodesfine, nfine);
  delete IF;
}

/*--------------------------------------------------------------------------*/

void TransferMatrixConstructor::_computePhiQuadHex(DofInformation* dofinformation, int ncellsrefinedrefelement)
{
  Fada::FemInterface* fem = dofinformation->getFem();
  // Fada::FemInterface* fem = dofinformation->getFem()->clone();
  Fada::IntegrationFormulaInterface* IF = fem->newIntegrationFormulaTransfer();
  assert(IF);
  int ncoarse = dofinformation->ndoflocal();
  int nfine = dofinformation->ndoflocalpatch();

  NodeVector nodes;
  Alat::Vector<NodeVector> nodesfine(ncellsrefinedrefelement);
  NodeVector offsets(ncellsrefinedrefelement);

  nodes.set_size(ncellsrefinedrefelement);
  for(int ii = 0; ii < ncellsrefinedrefelement; ii++)
  {
    nodesfine[ii].set_size(ncellsrefinedrefelement);
  }

  double xmin = -1.0, xmax = 1.0, ymin = -1.0, ymax = 1.0, zmin = -1.0, zmax = 1.0;
  double offsetx, offsety, offsetz;
  _setNodes(nodes, xmin, xmax, ymin, ymax, zmin, zmax);

  for(int ii = 0; ii < ncellsrefinedrefelement; ii++)
  {
    if(ii == 0)
    {
      xmin = -1.0, xmax = 0.0, ymin = -1.0, ymax = 0.0, zmin = -1.0, zmax = 0.0;
      offsetx = -0.5, offsety = -0.5, offsetz = -0.5;
    }
    else if(ii == 1)
    {
      xmin =  0.0, xmax = 1.0, ymin = -1.0, ymax = 0.0, zmin = -1.0, zmax = 0.0;
      offsetx = 0.5, offsety = -0.5, offsetz = -0.5;
    }
    else if(ii == 2)
    {
      xmin =  0.0, xmax = 1.0, ymin = 0.0, ymax = 1.0, zmin = -1.0, zmax = 0.0;
      offsetx = 0.5, offsety = 0.5, offsetz = -0.5;
    }
    else if(ii == 3)
    {
      xmin = -1.0, xmax = 0.0, ymin = 0.0, ymax = 1.0, zmin = -1.0, zmax = 0.0;
      offsetx = -0.5, offsety = 0.5, offsetz = -0.5;
    }
    else if(ii == 4)
    {
      xmin = -1.0, xmax = 0.0, ymin = -1.0, ymax = 0.0;
      zmin = 0.0;
      zmax = 1.0;
      offsetx = -0.5, offsety = -0.5, offsetz = 0.5;
    }
    else if(ii == 5)
    {
      xmin =  0.0, xmax = 1.0, ymin = -1.0, ymax = 0.0;
      zmin = 0.0;
      zmax = 1.0;
      offsetx = 0.5, offsety = -0.5, offsetz = 0.5;
    }
    else if(ii == 6)
    {
      xmin =  0.0, xmax = 1.0, ymin =  0.0, ymax = 1.0;
      zmin = 0.0;
      zmax = 1.0;
      offsetx = 0.5, offsety = 0.5, offsetz = 0.5;
    }
    else if(ii == 7)
    {
      xmin = -1.0, xmax = 0.0, ymin =  0.0, ymax = 1.0;
      zmin = 0.0;
      zmax = 1.0;
      offsetx = -0.5, offsety = 0.5, offsetz = 0.5;
    }

    _setNodes(nodesfine[ii], xmin, xmax, ymin, ymax, zmin, zmax);
    offsets[ii].x() = offsetx;
    offsets[ii].y() = offsety;
    offsets[ii].z() = offsetz;
  }


  int count = 0;
  fem->reInitCell(nodes);
  for(int iiK = 0; iiK < ncellsrefinedrefelement; iiK++)
  {
    for(int k = 0; k < IF->n(); k++)
    {
      Alat::Node xi;
      IF->point(xi, k);
      xi.x() = offsets[iiK].x()+0.5*xi.x();
      xi.y() = offsets[iiK].y()+0.5*xi.y();
      xi.z() = offsets[iiK].z()+0.5*xi.z();
      // std::cerr << iiK << " " << k << " -> " << xi << "\n";
      fem->reInitReferencePoint(xi);
      _weights[count] = ( 1.0/ncellsrefinedrefelement )*IF->weight(k);
      for(int ii = 0; ii < ncoarse; ii++)
      {
        _phicoarse(count, ii) = fem->phi(ii);
      }
      count++;
    }
  }

  int iK = -1;
  _computePhi2(IF, dofinformation, nodesfine, nfine);
  delete IF;
}

/*--------------------------------------------------------------------------*/

void TransferMatrixConstructor::_computePhi2(Fada::IntegrationFormulaInterface* IF, DofInformation* dofinformation, const Alat::Vector<NodeVector>& nodesfine, int nfine)
{
  Fada::FemInterface* fem = dofinformation->getFem();
  // Fada::FemInterface* fem = dofinformation->getFem()->clone();
  int count = 0;
// _phifine.zeros();
//  std::cerr<<"TransferMatrixConstructor::_computePhi2 "<<'\n';
  for(int iiK = 0; iiK < nodesfine.size(); iiK++)
  {
    // std::cerr << count << " _phifine " << _phifine << "\n";
    fem->reInitCell(nodesfine[iiK]);
    Alat::IntVector indices;
    // fem->getLocalCellIndicesPatch(iiK, indices);
    dofinformation->getLocalCellIndicesPatch(iiK, indices);
//    std::cerr<<iiK<<"  "<<indices<<'\n';
    // std::cerr<<"*** TransferMatrixConstructor::_computePhi2() " << iiK  << "-> " << indices << "\n";
    for(int k = 0; k < IF->n(); k++)
    {
      Alat::Node xi;
      IF->point(xi, k);
      fem->reInitReferencePoint(xi);
      for(int ii = 0; ii < indices.size(); ii++)
      {
        if(_phifine(count, indices[ii]) != 0.0)
        {
          std::cerr << "*** " << _phifine << "\n";
          std::cerr<<_phifine(count, indices[ii])<<" count "<<count<<" ii "<<ii<<" indices "<<indices<<"\n";
          for(int ll = 0; ll < nfine; ll++)
          {
            std::cerr<<_phifine(count, ll)<<" ";
          }
          std::cerr<<"\n";
          std::cerr<<"*** ERROR in TransferMatrixConstructor::_computePhi()\n";
          std::cerr<<"*** ERROR in TransferMatrixConstructor::fem->getName() "<< fem->getName()<<"\n";
          std::cerr<<"*** ERROR in TransferMatrixConstructor::IF->getName() "<< IF->getName()<<"\n";
          assert(0);
          exit(1);
        }
        _phifine(count, indices[ii]) = fem->phi(ii);
      }
      count++;
    }
  }
}

/*--------------------------------------------------------------------------*/

void TransferMatrixConstructor::_computeMatrices(Alat::DoubleMatrix& interpolmat, Alat::DoubleMatrix& projectmat)
{
  int ncoarse = _phicoarse.n_cols;
  int nfine = _phifine.n_cols;
  // std::cerr<<"NEW::_phifine \n";
  // for(int k = 0; k < _weights.size(); k++)
  // {
  //   for(int i = 0; i < nfine; i++)
  //   {
  //     std::cerr<<_phifine(k, i)<<" ";
  //   }
  //   std::cerr << "\n";
  // }


  // std::cerr << "TransferMatrixConstructor::_computeMatrices() ncoarse nfine " << ncoarse << " " << nfine << "\n";

  interpolmat.set_size(ncoarse, nfine);
  projectmat.set_size(ncoarse, nfine);

  Alat::DoubleMatrix beta(ncoarse, nfine), mu(nfine, nfine), sigma(ncoarse, ncoarse);

  beta.zeros();
  mu.zeros();
  sigma.zeros();
  for(int k = 0; k < _weights.size(); k++)
  {
    for(int i = 0; i < ncoarse; i++)
    {
      for(int j = 0; j < ncoarse; j++)
      {
        sigma(i, j) += _weights[k]*_phicoarse(k, i)*_phicoarse(k, j);
      }
    }
    for(int i = 0; i < nfine; i++)
    {
      for(int j = 0; j < nfine; j++)
      {
        mu(i, j) += _weights[k]*_phifine(k, i)*_phifine(k, j);
      }
    }
    for(int i = 0; i < ncoarse; i++)
    {
      for(int j = 0; j < nfine; j++)
      {
        beta(i, j) += _weights[k]*_phicoarse(k, i)*_phifine(k, j);
      }
    }
  }

  // for(int i = 0; i < nfine; i++)
  // {
  //   std::cerr<<"TransferMatrixConstructor::_computeMatrices() i mu(i,i)= " << i <<  " "<<mu(i,i)<<"\n";
  //   for(int j = 0; j < nfine; j++)
  //   {
  //     if(i==j)
  //     {
  //       continue;
  //     }
  //     if (mu(i, j)!=0.0)
  //     {
  //       std::cerr<<"TransferMatrixConstructor::_computeMatrices() i j mu(i,j)= " << i << " " << j << " "<<mu(i,j)<<"\n";
  //     }
  //   }
  // }


  std::cerr<<"TransferMatrixConstructor::_computeMatrices() mu\n"<<mu<<"\n";
  std::cerr<<"TransferMatrixConstructor::_computeMatrices() beta\n"<<beta<<"\n";

  // Alat::DoubleMatrix sigma2(ncoarse, ncoarse), mu2(nfine, nfine);
  // mu2 = mu;
  //
  mu.gaussJordan();
  std::cerr<<"TransferMatrixConstructor::_computeMatrices() mu\n"<<mu<<"\n";
  // assert(0);
  interpolmat.zeros();
  beta.productMatrix(interpolmat, mu);
  //
  // sigma2.zeros();
  // for(int i = 0; i < ncoarse; i++)
  // {
  //   for(int j = 0; j < ncoarse; j++)
  //   {
  //     for(int m = 0; m < nfine; m++)
  //     {
  //       for(int l = 0; l < nfine; l++)
  //       {
  //         sigma2(i,j) += interpolmat(i,m)*mu2(m,l)*interpolmat(j,l);
  //       }
  //     }
  //   }
  // }
  // std::cerr<<"TransferMatrixConstructor::_computeMatrices() sigma\n"<<sigma<<"\n";
  // std::cerr<<"TransferMatrixConstructor::_computeMatrices() sigma2\n"<<sigma2<<"\n";


  sigma.gaussJordan();
  projectmat.zeros();
  sigma.productMatrix(projectmat, beta);

  std::cerr<<"TransferMatrixConstructor::_computeMatrices() interpolmat\n"<<interpolmat<<"\n";
  // std::cerr<<"TransferMatrixConstructor::_computeMatrices() projectmat\n"<<projectmat<<"\n";
  //
  // projectmat.productMatrixTransposed(sigma2, interpolmat);
  // std::cerr<<"TransferMatrixConstructor::_computeMatrices() sigma2\n"<<sigma2<<"\n";

  // std::cerr << "TransferMatrixConstructor::_computeMatrices() interpolmat, projectmat\n" << interpolmat << "\n\n" << projectmat << "\n\n";
//  bool stop = 0;
//  for(int i = 0; i < ncoarse; i++)
//  {
//    double sum = 0.0;
//    for(int j = 0; j < nfine; j++)
//    {
//      sum += projectmat(i, j);
//    }
//    if(fabs(sum-1.0) > 1e-10)
//    {
//      std::cerr << "TransferMatrixConstructor::_computeMatrices() projectmatrix sum of row " << i << " different from one " << sum << "\n";
//      exit(1);
//      stop = 1;
//    }
//  }
// if(stop) exit(2);
}
