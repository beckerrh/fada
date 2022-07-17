#include  "FadaMesh/boundaryinfo.h"
#include  "Fada/dofinformationinterface.h"
#include  "Fada/dofmanagerallvariables.h"
#include  "Fada/integratorinterface.h"
#include  "Fada/integratormanager.h"
#include  "Fada/localvectors.h"
#include  "Alat/matrixallvariables.h"
#include  "FadaMesh/meshinterface.h"
#include  "Alat/sparsitypattern.h"
#include  "Alat/sparsitypatternsoft.h"
#include  "Alat/systemassemblematrix.h"
#include  "Alat/systemmatrixinterface.h"
#include  "Fada/varinfointegrator.h"
#include  "Alat/systemvector.h"
#include  "Fada/unstructuredintegrationloop.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/
UnStructuredIntegrationLoop::~UnStructuredIntegrationLoop()
{}
  // UnStructuredIntegrationLoop::UnStructuredIntegrationLoop() : Fada::DomainIntegrationLoop(), Fada::IntegrationLoop()
  UnStructuredIntegrationLoop::UnStructuredIntegrationLoop() : Fada::DomainIntegrationLoop()
{}
  // UnStructuredIntegrationLoop::UnStructuredIntegrationLoop( const UnStructuredIntegrationLoop& unstructuredintegrationloop) : Fada::DomainIntegrationLoop(unstructuredintegrationloop), Fada::IntegrationLoop(unstructuredintegrationloop)
  UnStructuredIntegrationLoop::UnStructuredIntegrationLoop( const UnStructuredIntegrationLoop& unstructuredintegrationloop) : Fada::DomainIntegrationLoop(unstructuredintegrationloop)
{
  assert(0);
}

UnStructuredIntegrationLoop& UnStructuredIntegrationLoop::operator=( const UnStructuredIntegrationLoop& unstructuredintegrationloop)
{
  Fada::DomainIntegrationLoop::operator=(unstructuredintegrationloop);
  assert(0);
  return *this;
}

std::string UnStructuredIntegrationLoop::getName() const
{
  return "UnStructuredIntegrationLoop";
}

UnStructuredIntegrationLoop* UnStructuredIntegrationLoop::clone() const
{
  return new UnStructuredIntegrationLoop(*this);
}

/*--------------------------------------------------------------------------*/
void UnStructuredIntegrationLoop::matrixConnectivityPerLevel(const std::string& varnamei, const std::string& varnamej, const Fada::IntegratorManager* integratormanager, int level, int n, Alat::VariableMatrixInterface* sparsematrix) const
{
  // std::cerr << "UnStructuredIntegrationLoop::matrixConnectivityPerLevel() level=" <<level << " n=" << n << "\n";
  Alat::SparsityPatternSoft sparsitypatternsoft(n);
  if( not integratormanager->noIntegrator(FadaEnums::Matrix, FadaEnums::Cells) )
  {
    matrixConnectivityCells(varnamei, varnamej, sparsitypatternsoft);
  }
  if( not integratormanager->noIntegrator(FadaEnums::Matrix, FadaEnums::BoundarySides) )
  {
    matrixConnectivityBoundarySides(varnamei, varnamej, sparsitypatternsoft);
  }
  if( not integratormanager->noIntegrator(FadaEnums::Matrix, FadaEnums::InteriorSides) )
  {
    matrixConnectivityInteriorSides(varnamei, varnamej, sparsitypatternsoft);
  }
  sparsematrix->set_size(sparsitypatternsoft);
  sparsematrix->zeros();
}

/*--------------------------------------------------------------------------*/
void UnStructuredIntegrationLoop::matrixConnectivityCells(const std::string& varnamei, const std::string& varnamej, Alat::SparsityPatternSoft& sparsitypatternsoft) const
{
  int ndofpercellout = _dofmanagerallvariables->getDofInformationOfVariable(varnamei)->ndofpercell();
  int ndofpercellin = _dofmanagerallvariables->getDofInformationOfVariable(varnamej)->ndofpercell();
  // std::cerr << "UnStructuredIntegrationLoop::matrixConnectivityCells() varnamei="<<varnamei<<" varnamej"<<varnamej<<" ndofpercellin="<<ndofpercellin<<" ndofpercellout="<<ndofpercellout<<"\n";

  const Fada::DofInformationInterface* dofinfoI = _dofmanagerallvariables->getDofInformationOfVariable(varnamei);
  const Fada::DofInformationInterface* dofinfoJ = _dofmanagerallvariables->getDofInformationOfVariable(varnamej);

  if(dofinfoI->getFem() == NULL)
  {
    if(dofinfoJ->getFem() == NULL)
    {
      sparsitypatternsoft[0].insert(0);
    }
    else
    {
      Alat::IntVector jndices(ndofpercellin );
      for(int iK = 0; iK < _mesh->getNCells(); iK++)
      {
        dofinfoJ->getCellIndices(iK, jndices);
        for(int jj = 0; jj < jndices.size(); jj++)
        {
          sparsitypatternsoft[0].insert(jndices[jj]);
        }
      }
    }
  }
  else if(dofinfoJ->getFem() == NULL)
  {
    Alat::IntVector indices( ndofpercellout );
    for(int iK = 0; iK < _mesh->getNCells(); iK++)
    {
      dofinfoI->getCellIndices(iK, indices);
      for(int ii = 0; ii < indices.size(); ii++)
      {
        sparsitypatternsoft[indices[ii]].insert(0);
      }
    }
  }
  else
  {
    Alat::IntVector indices( ndofpercellout ), jndices(ndofpercellin );
    for(int iK = 0; iK < _mesh->getNCells(); iK++)
    {
      dofinfoI->getCellIndices(iK, indices);
      dofinfoJ->getCellIndices(iK, jndices);
      for(int ii = 0; ii < indices.size(); ii++)
      {
        assert(indices[ii]<sparsitypatternsoft.size());
        for(int jj = 0; jj < jndices.size(); jj++)
        {
          assert(jndices[jj]<sparsitypatternsoft.size());
          sparsitypatternsoft[indices[ii]].insert(jndices[jj]);
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void UnStructuredIntegrationLoop::matrixConnectivityBoundarySides(const std::string& varnamei, const std::string& varnamej, Alat::SparsityPatternSoft& sparsitypatternsoft) const
{
  int ndofpercellout = _dofmanagerallvariables->getDofInformationOfVariable(varnamei)->ndofpercell();
  int ndofpercellin = _dofmanagerallvariables->getDofInformationOfVariable(varnamej)->ndofpercell();
  const FadaMesh::BoundaryInfo* BI = _mesh->getBoundaryInfo();
  const Alat::IntVector colors = BI->getColors();
  const Fada::DofInformationInterface* dofinfoI = _dofmanagerallvariables->getDofInformationOfVariable(varnamei);
  const Fada::DofInformationInterface* dofinfoJ = _dofmanagerallvariables->getDofInformationOfVariable(varnamej);

  if(dofinfoI->getFem() == NULL)
  {
    if(dofinfoJ->getFem() == NULL)
    {
      sparsitypatternsoft[0].insert(0);
    }
    else
    {
      Alat::IntVector jndices(ndofpercellin );
      for(int ic = 0; ic < colors.size(); ic++)
      {
        int color = colors[ic];
        const Alat::IntVector& sides = BI->getSidesOfColor(color);
        for(int i = 0; i < sides.size(); i++)
        {
          int iS = sides[i];
          int iK = _mesh->getCellIdOfSide(iS, 0);
          if(_mesh->getCellIdOfSide(iS, 1) != -1)
          {
            std::cerr <<"color "<<color<<" problem " << iS << " " << iK << " " << _mesh->getCellIdOfSide(iS, 1) << "\n";
          }
          assert(_mesh->getCellIdOfSide(iS, 1) == -1);
          dofinfoJ->getCellIndices(iK, jndices);
          for(int jj = 0; jj < jndices.size(); jj++)
          {
            sparsitypatternsoft[0].insert(jndices[jj]);
          }
        }
      }
    }
  }
  else if(dofinfoJ->getFem() == NULL)
  {
    Alat::IntVector indices( ndofpercellout );
    for(int ic = 0; ic < colors.size(); ic++)
    {
      int color = colors[ic];
      const Alat::IntVector& sides = BI->getSidesOfColor(color);
      for(int i = 0; i < sides.size(); i++)
      {
        int iS = sides[i];
        int iK = _mesh->getCellIdOfSide(iS, 0);
        if(_mesh->getCellIdOfSide(iS, 1) != -1)
        {
          std::cerr <<"color "<<color<<" problem " << iS << " " << iK << " " << _mesh->getCellIdOfSide(iS, 1) << "\n";
        }
        assert(_mesh->getCellIdOfSide(iS, 1) == -1);
        dofinfoI->getCellIndices(iK, indices);
        for(int ii = 0; ii < indices.size(); ii++)
        {
          sparsitypatternsoft[indices[ii]].insert(0);
        }
      }
    }
  }
  else
  {
    Alat::IntVector indices( ndofpercellout ), jndices( ndofpercellin );
    for(int ic = 0; ic < colors.size(); ic++)
    {
      int color = colors[ic];
      const Alat::IntVector& sides = BI->getSidesOfColor(color);
      for(int i = 0; i < sides.size(); i++)
      {
        int iS = sides[i];
        int iK = _mesh->getCellIdOfSide(iS, 0);
        if(_mesh->getCellIdOfSide(iS, 1) != -1)
        {
          std::cerr <<"color "<<color<<" problem " << iS << " " << iK << " " << _mesh->getCellIdOfSide(iS, 1) << "\n";
        }
        assert(_mesh->getCellIdOfSide(iS, 1) == -1);
        dofinfoI->getCellIndices(iK, indices);
        dofinfoJ->getCellIndices(iK, jndices);
        for(int ii = 0; ii < indices.size(); ii++)
        {
          for(int jj = 0; jj < jndices.size(); jj++)
          {
            sparsitypatternsoft[indices[ii]].insert(jndices[jj]);
          }
        }
      }
    }
  }
}

/*--------------------------------------------------------------------------*/
void UnStructuredIntegrationLoop::matrixConnectivityInteriorSides(const std::string& varnamei, const std::string& varnamej, Alat::SparsityPatternSoft& sparsitypatternsoft) const
{
  int ndofpercellout = _dofmanagerallvariables->getDofInformationOfVariable(varnamei)->ndofpercell();
  int ndofpercellin = _dofmanagerallvariables->getDofInformationOfVariable(varnamej)->ndofpercell();
  Alat::IntVector indicesli( ndofpercellout ), indiceslj( ndofpercellin );
  Alat::IntVector indicesri( ndofpercellout ), indicesrj( ndofpercellin );
  for(int iS = 0; iS < _mesh->getNSides(); iS++)
  {
    int iKl = _mesh->getCellIdOfSide(iS, 0);
    int iKr = _mesh->getCellIdOfSide(iS, 1);
    if(iKr == -1)
    {
      continue;
    }
    _dofmanagerallvariables->getDofInformationOfVariable(varnamei)->getCellIndices(iKl, indicesli);
    _dofmanagerallvariables->getDofInformationOfVariable(varnamej)->getCellIndices(iKl, indiceslj);
    _dofmanagerallvariables->getDofInformationOfVariable(varnamei)->getCellIndices(iKr, indicesri);
    _dofmanagerallvariables->getDofInformationOfVariable(varnamej)->getCellIndices(iKr, indicesrj);
    for(int ii = 0; ii < indicesli.size(); ii++)
    {
      for(int jj = 0; jj < indiceslj.size(); jj++)
      {
        sparsitypatternsoft[indicesli[ii]].insert(indiceslj[jj]);
      }
      for(int jj = 0; jj < indicesrj.size(); jj++)
      {
        sparsitypatternsoft[indicesli[ii]].insert(indicesrj[jj]);
      }
    }
    for(int ii = 0; ii < indicesri.size(); ii++)
    {
      for(int jj = 0; jj < indiceslj.size(); jj++)
      {
        sparsitypatternsoft[indicesri[ii]].insert(indiceslj[jj]);
      }
      for(int jj = 0; jj < indicesrj.size(); jj++)
      {
        sparsitypatternsoft[indicesri[ii]].insert(indicesrj[jj]);
      }
    }
  }
}
