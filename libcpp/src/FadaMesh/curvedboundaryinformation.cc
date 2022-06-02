#include  "FadaMesh/boundaryinfo.h"
#include  "FadaMesh/curvedinteriorsideinfo.h"
#include  "FadaMesh/meshinterface.h"
#include  "FadaMesh/curvedboundarydescriptionconstructor.h"
#include  "FadaMesh/curvedboundaryinformation.h"
#include  "Alat/doublevector.h"
#include  "Alat/stringvector.h"
#include  "Alat/tokenize.h"
#include  <stdlib.h>

using namespace FadaMesh;
using namespace std;

/*--------------------------------------------------------------------------*/
CurvedBoundaryInformation::~CurvedBoundaryInformation() {}
CurvedBoundaryInformation::CurvedBoundaryInformation() {}
CurvedBoundaryInformation::CurvedBoundaryInformation( const CurvedBoundaryInformation& curvedboundaryinformation)
{
  assert(0);
}

CurvedBoundaryInformation& CurvedBoundaryInformation::operator=( const CurvedBoundaryInformation& curvedboundaryinformation)
{
  const Alat::Map<int, FadaMesh::CurvedBoundaryDescriptionInterface*>& curvedboundaries = curvedboundaryinformation._curvedboundaries;
  for(Alat::Map<int, FadaMesh::CurvedBoundaryDescriptionInterface*>::const_iterator p = curvedboundaries.begin(); p != curvedboundaries.end(); p++)
  {
    _curvedboundaries[p->first] = p->second->clone();
  }
  const Alat::IntVector& celliscurved = curvedboundaryinformation._celliscurved;
  _celliscurved.set_size( celliscurved.size() );
  _celliscurved = celliscurved;
  const Alat::SparsityPatternFixArray<2>& spfa = curvedboundaryinformation._spfa;
  std::cerr << "my _spfa " << _spfa.rowstart() << "\n";
  std::cerr << "my _spfa " << _spfa.col() << "\n";
  std::cerr << "your spfa " << spfa.rowstart()<< "\n";
  std::cerr << "your spfa " << spfa.col()<< "\n";
  _spfa = spfa;
}

std::string CurvedBoundaryInformation::getName() const
{
  return "CurvedBoundaryInformation";
}

/*--------------------------------------------------------------------------*/
Alat::SparsityPatternFixArray<2>& CurvedBoundaryInformation::curvedCellIndices()
{
  return _spfa;
}

const Alat::Map<int, FadaMesh::CurvedBoundaryDescriptionInterface*>& CurvedBoundaryInformation::get() const
{
  return _curvedboundaries;
}

const FadaMesh::CurvedBoundaryDescriptionInterface* CurvedBoundaryInformation::get(int color) const
{
  return _curvedboundaries[color];
}

FadaMesh::CurvedBoundaryDescriptionInterface* CurvedBoundaryInformation::get(int color)
{
  return _curvedboundaries[color];
}

FadaMesh::CurvedBoundaryDescriptionInterface*& CurvedBoundaryInformation::getPointer(int color)
{
  return _curvedboundaries[color];
}

bool CurvedBoundaryInformation::boundaryColorIsCurved(int color) const
{
  return _curvedboundaries.find(color) != _curvedboundaries.end();
}

const Alat::SparsityPatternFixArray<2>& CurvedBoundaryInformation::curvedCellIndices() const
{
  return _spfa;
}

bool CurvedBoundaryInformation::cellIsCurved(int iK) const
{
  if( iK >= _celliscurved.size() )
  {
    _error_string("cellIsCurved", "iK,_celliscurved.size() =", iK, _celliscurved.size());
  }
  return _celliscurved[iK] != -1;
}

int CurvedBoundaryInformation::rowstart(int iK) const
{
  if(_celliscurved[iK] == -1)
  {
    return 0;
  }
  return _spfa.rowstart(_celliscurved[iK]);
}

int CurvedBoundaryInformation::rowstop(int iK) const
{
  if(_celliscurved[iK] == -1)
  {
    return 0;
  }
  return _spfa.rowstop(_celliscurved[iK]);
}

const Alat::FixArray<2, int>& CurvedBoundaryInformation::getCurvedInfo(int pos) const
{
  return _spfa.col(pos);
}

/*--------------------------------------------------------------------------*/
void CurvedBoundaryInformation::constructBoundaryInformation(const FadaMesh::MeshInterface* mesh)
{
  _celliscurved.clear();
  _celliscurved.set_size(mesh->getNCells());
  _celliscurved.fill(-1);

  Alat::IntMap  iK_to_count;
  const FadaMesh::BoundaryInfo* boundaryinfo = mesh->getBoundaryInfo();
  assert(boundaryinfo);
  int count = 0;
  {
    const Alat::IntVector& colors = boundaryinfo->getColors();
    for(int icol = 0; icol < colors.size(); icol++)
    {
      int color = colors[icol];
      if( _curvedboundaries.find(color) == _curvedboundaries.end() )
      {
        continue;
      }
      const Alat::IntVector& cells_of_color = boundaryinfo->getCellsOfColor(color);
      for(int i = 0; i < cells_of_color.size(); i++)
      {
        int iK = cells_of_color[i];
        if( iK_to_count.find(iK) == iK_to_count.end() )
        {
          iK_to_count[iK] = count++;
        }
      }
    }
  }
  const FadaMesh::CurvedInteriorSideInfo* curvedinteriorsideinfo = mesh->getCurvedInteriorSideInfo();
  {
    const Alat::IntVector& colors = curvedinteriorsideinfo->getColors();
    for(int icol = 0; icol < colors.size(); icol++)
    {
      int color = colors[icol];
      if( _curvedboundaries.find(color) == _curvedboundaries.end() )
      {
        continue;
      }
      const Alat::IntVector& sides_of_color = curvedinteriorsideinfo->getSidesOfColor(color);
      for(int i = 0; i < sides_of_color.size(); i++)
      {
        int iside = sides_of_color[i];
        int iKL = mesh->getCellIdOfSide(iside, 0);
        int iKR = mesh->getCellIdOfSide(iside, 1);
        if( iK_to_count.find(iKL) == iK_to_count.end() )
        {
          iK_to_count[iKL] = count++;
        }
        if( iK_to_count.find(iKR) == iK_to_count.end() )
        {
          iK_to_count[iKR] = count++;
        }
      }
    }
  }


  Alat::SparsityPatternFixArraySoft<2> sparsitypatternfixarraysoft(count);
  {
    const Alat::IntVector& colors = boundaryinfo->getColors();
    for(int icol = 0; icol < colors.size(); icol++)
    {
      int color = colors[icol];
      if( _curvedboundaries.find(color) == _curvedboundaries.end() )
      {
        continue;
      }
      const Alat::IntVector& cells_of_color = boundaryinfo->getCellsOfColor(color);
      const Alat::IntVector& localside_of_color = boundaryinfo->getSidesIdOfCellsOfColor(color);
      for(int i = 0; i < cells_of_color.size(); i++)
      {
        int iK = cells_of_color[i];
        Alat::FixArray<2, int> f;
        f[0] = color;
        f[1] = localside_of_color[i];
        int count = iK_to_count[iK];
        sparsitypatternfixarraysoft[count].insert(f);
        _celliscurved[iK] = count;
      }
    }
  }
  {
    const Alat::IntVector& colors = curvedinteriorsideinfo->getColors();
    for(int icol = 0; icol < colors.size(); icol++)
    {
      int color = colors[icol];
      bool found = _curvedboundaries.find(color) != _curvedboundaries.end();
      if( _curvedboundaries.find(color) == _curvedboundaries.end() )
      {
        continue;
      }
      const Alat::IntVector& sides_of_color = curvedinteriorsideinfo->getSidesOfColor(color);
      // std::cerr << "sides_of_color="<<sides_of_color.size() << "\n";
      for(int i = 0; i < sides_of_color.size(); i++)
      {
        int iside = sides_of_color[i];
        int iKL = mesh->getCellIdOfSide(iside, 0);
        int iKR = mesh->getCellIdOfSide(iside, 1);
        // std::cerr << "iside="<< iside<< "iKL="<<iKL << " iKR="<<iKR<<"\n";
        {
          Alat::FixArray<2, int> f;
          f[0] = color;
          f[1] = mesh->getLocalIndexOfSideInCell(iKL, iside);
          int count = iK_to_count[iKL];
          sparsitypatternfixarraysoft[count].insert(f);
          _celliscurved[iKL] = count;
        }
        {
          Alat::FixArray<2, int> f;
          f[0] = color;
          f[1] = mesh->getLocalIndexOfSideInCell(iKR, iside);
          int count = iK_to_count[iKR];
          sparsitypatternfixarraysoft[count].insert(f);
          _celliscurved[iKR] = count;
        }
      }
    }
  }
  // std::cerr << "iK_to_count="<<iK_to_count<<"\n";
  // std::cerr << "_celliscurved="<<_celliscurved<<"\n";
  // assert(0);


  _spfa.set_size(sparsitypatternfixarraysoft);
}

/*---------------------------------------------------*/
void CurvedBoundaryInformation::readCurvedBoundaryDescription(std::istream& in)
{
  int n = 0;
  streampos nodestart;
  string toto;
  while( !in.eof() )
  {
    nodestart = in.tellg();
    getline(in, toto);
    // std::cerr << "toto="<<toto<<"\n";
    Alat::StringVector totos = Alat::Tokenize(toto, " ");
    if(totos.size() != 2)
    {
      continue;
    }
    if(totos[0] == "CurvedSides")
    {
      n = atoi( totos[1].c_str() );
      break;
    }
  }
  if( !in.eof() )
  {
    in.seekg(nodestart);
    // std::cerr << "file.eof()="<<file.eof()<<"\n";
    getline(in, toto);
    // std::cerr << "first="<<toto<<"\n";
  }

  // std::cerr << "CurvedBoundaryInformation n="<<n<<"\n";
  // assert(0);
  // std::cerr << "CurvedBoundaryInformation::read() n = " << n << "\n";
  CurvedBoundaryDescriptionConstructor curvedboundarydescriptionconstructor;
  Alat::StringVector names(n);
  Alat::IntVector colors(n);
  std::string name;
  int color;
  for(int i = 0; i < n; i++)
  {
    in >> color >> name;
    // std::cerr << "color="<<color<<" name"<<name<<"\n";
    _curvedboundaries[color] = curvedboundarydescriptionconstructor.newDescription(name);
    _curvedboundaries[color]->read(in);
    // file >> colors[i] >> names[i];
    // std::cerr << "CurvedBoundaryInformation::read() i = " << i << " " << colors[i] << " " << names[i] << "\n";
  }
  // CurvedBoundaryDescriptionConstructor BDC(*this, names, colors);
  // for(iterator p = _curvedboundaries.begin(); p != _curvedboundaries.end(); p++)
  // {
  //   FadaMesh::CurvedBoundaryDescriptionInterface* BDI = p->second;
  //   BDI->read(file);
  //   // std::cerr << "CurvedBoundaryInformation::read() p = " << p->first << " " << p->second->getParameters() << "\n";
  // }
  // write("toto","ascii");
  // assert(0);
}

/*---------------------------------------------------*/

void CurvedBoundaryInformation::writeCurvedBoundaryDescription(std::ostream& out) const
{
  int n = _curvedboundaries.size();
  if(n > 0)
  {
    out << "CurvedSides " << n << "\n";
    for(const_iterator p = _curvedboundaries.begin(); p != _curvedboundaries.end(); p++)
    {
      out << p->first << " " << p->second->getName() << "\n";
      p->second->write(out);
    }
    // for(const_iterator p = _curvedboundaries.begin(); p != _curvedboundaries.end(); p++)
    // {
    //   p->second->write(file);
    // }
  }
}

/*---------------------------------------------------*/

void CurvedBoundaryInformation::read(std::string filename)
{
  std::ifstream file( filename.c_str() );
  readCurvedBoundaryDescription(file);
  _celliscurved.loadFada(file);
  curvedCellIndices().loadFada(file);
  file.close();
}

/*---------------------------------------------------*/

void CurvedBoundaryInformation::write(std::string filename, std::string datatype) const
{
  // std::cerr << "CurvedBoundaryInformation::write() _curvedboundaries.size() = " << _curvedboundaries.size() << "\n";
  std::ofstream file( filename.c_str() );
  file << "CurvedSides "<< _curvedboundaries.size() << "\n";
  for(const_iterator p = _curvedboundaries.begin(); p != _curvedboundaries.end(); p++)
  {
    file << p->first << " " << p->second->getName() << "\n";
    p->second->write(file, datatype);
  }
  // for(const_iterator p = _curvedboundaries.begin(); p != _curvedboundaries.end(); p++)
  // {
  //   p->second->write(file, datatype);
  // }
  file << "\n";
  _celliscurved.saveFada(file, datatype);
  curvedCellIndices().saveFada(file, datatype);
  file.close();
}

/*---------------------------------------------------*/
void CurvedBoundaryInformation::set_size(const CurvedBoundaryInformation& curvedboundaryinformation)
{
  // const CurvedBoundaryInformation* curvedboundaryinformation = dynamic_cast<const CurvedBoundaryInformation*>(&curvedboundaryinformationinterface);
  // assert(curvedboundaryinformation);
  *this = curvedboundaryinformation;
}

// #include  "FadaMesh/boundaryinfo.h"
// #include  "Alat/doublevector.h"
// #include  "FadaMesh/meshinterface.h"
// #include  "Alat/stringvector.h"
// #include  "FadaMesh/curvedboundarydescriptionconstructor.h"
// #include  "FadaMesh/curvedboundaryinformation.h"
//
// using namespace FadaMesh;
// using namespace std;
//
// /*--------------------------------------------------------------------------*/
//
// void CurvedBoundaryInformation::constructBoundaryInformation(const FadaMesh::MeshInterface* M)
// {
//   _celliscurved.clear();
//   _celliscurved.set_size(M->getNCells(), -1);
//
//   for(Alat::Map<int, FadaMesh::CurvedBoundaryDescriptionInterface*>::const_iterator p=_curvedboundaries.begin(); p!=_curvedboundaries.end();p++)
//   {
//     std::cerr << "CurvedBoundaryInformation::constructBoundaryInformation() color="<<p->first << " name="<<p->second->getName() << "\n";
//   }
//
//   const FadaMesh::BoundaryInfo* BDI = M->getBoundaryInfo();
//   assert(BDI);
//   const Alat::IntVector& colors = BDI->getColors();
//
//   Alat::IntMap  iK_to_count;
//   int count = 0;
//   for(int icol = 0; icol < colors.size(); icol++)
//   {
//     int color = colors[icol];
//     if( !boundaryColorIsCurved(color) )
//     {
//       continue;
//     }
//     const Alat::IntVector& cells_of_color = BDI->getCellsOfColor(color);
//     for(int i = 0; i < cells_of_color.size(); i++)
//     {
//       int iK = cells_of_color[i];
//       if( iK_to_count.find(iK) == iK_to_count.end() )
//       {
//         iK_to_count[iK] = count++;
//       }
//     }
//   }
//   Alat::SparsityPatternFixArraySoft<2> SparameterfileAS(count);
//
//   for(int icol = 0; icol < colors.size(); icol++)
//   {
//     int color = colors[icol];
//     if( !boundaryColorIsCurved(color) )
//     {
//       continue;
//     }
//     const Alat::IntVector& cells_of_color = BDI->getCellsOfColor(color);
//     const Alat::IntVector& localside_of_color = BDI->getSidesIdOfCellsOfColor(color);
//     for(int i = 0; i < cells_of_color.size(); i++)
//     {
//       int iK = cells_of_color[i];
//       Alat::FixArray<2, int> f;
//       f[0] = color;
//       f[1] = localside_of_color[i];
//       int count = iK_to_count[iK];
//       SparameterfileAS[count].insert(f);
//       _celliscurved[iK] = count;
//     }
//   }
//   _spfa.set_size(SparameterfileAS);
// }
//
// /*---------------------------------------------------*/
//
// void CurvedBoundaryInformation::readCurvedBoundaryDescription(std::istream& in)
// {
//   int n = 0;
//   if( !file.eof() )
//   {
//     file >> n;
//   }
//   // std::cerr << "CurvedBoundaryInformation::read() n = " << n << "\n";
//   CurvedBoundaryDescriptionConstructor curvedboundarydescriptionconstructor;
//   Alat::StringVector names(n);
//   Alat::IntVector colors(n);
//   std::string name;
//   int color;
//   for(int i = 0; i < n; i++)
//   {
//     file >> color >> name;
//     std::cerr << "color="<<color<<" name"<<name<<"\n";
//     _curvedboundaries[color] = curvedboundarydescriptionconstructor.newDescription(name);
//     _curvedboundaries[color]->read(file);
//     // file >> colors[i] >> names[i];
//     // std::cerr << "CurvedBoundaryInformation::read() i = " << i << " color" << color << " name" << name << " name=" << _curvedboundaries[color]->getName() << "\n";
//   }
//   // assert(0);
//   // CurvedBoundaryDescriptionConstructor BDC(*this, names, colors);
//   // for(iterator p = _curvedboundaries.begin(); p != _curvedboundaries.end(); p++)
//   // {
//   //   FadaMesh::CurvedBoundaryDescriptionInterface* BDI = p->second;
//   //   BDI->read(file);
//   //   // std::cerr << "CurvedBoundaryInformation::read() p = " << p->first << " " << p->second->getParameters() << "\n";
//   // }
//   // write("toto","ascii");
//   // assert(0);
// }
//
// /*---------------------------------------------------*/
//
// void CurvedBoundaryInformation::writeCurvedBoundaryDescription(std::ostream& out) const
// {
//   int n = _curvedboundaries.size();
//   if(n > 0)
//   {
//     file << n << "\n";
//     for(const_iterator p = _curvedboundaries.begin(); p != _curvedboundaries.end(); p++)
//     {
//       file << p->first << " " << p->second->getName() << "\n";
//       p->second->write(file);
//     }
//     // for(const_iterator p = _curvedboundaries.begin(); p != _curvedboundaries.end(); p++)
//     // {
//     //   p->second->write(file);
//     // }
//   }
// }
//
// /*---------------------------------------------------*/
//
// void CurvedBoundaryInformation::read(std::string filename)
// {
//   std::ifstream file( filename.c_str() );
//   readCurvedBoundaryDescription(file);
//   _celliscurved.read(file);
//   curvedCellIndices().read(file);
//   file.close();
// }
//
// /*---------------------------------------------------*/
//
// void CurvedBoundaryInformation::write(std::string filename, std::string datatype) const
// {
//   // std::cerr << "CurvedBoundaryInformation::write() _curvedboundaries.size() = " << _curvedboundaries.size() << "\n";
//   std::ofstream file( filename.c_str() );
//   file << _curvedboundaries.size() << "\n";
//   for(const_iterator p = _curvedboundaries.begin(); p != _curvedboundaries.end(); p++)
//   {
//     file << p->first << " " << p->second->getName() << "\n";
//     p->second->write(file, datatype);
//   }
//   // for(const_iterator p = _curvedboundaries.begin(); p != _curvedboundaries.end(); p++)
//   // {
//   //   p->second->write(file, datatype);
//   // }
//   file << "\n";
//   _celliscurved.write(file, datatype);
//   curvedCellIndices().write(file, datatype);
//   file.close();
// }
//
// /*---------------------------------------------------*/
//
// void CurvedBoundaryInformation::set_size(const CurvedBoundaryInformationInterface& BD)
// {
//   assert(0);
//   // const Alat::Map<int, FadaMesh::CurvedBoundaryDescriptionInterface*>& MBD = BD.get();
//   // int n = MBD.size();
//   // Alat::StringVector names(n);
//   // Alat::IntVector colors(n);
//   // int count = 0;
//   // for(const_iterator p = MBD.begin(); p != MBD.end(); p++)
//   // {
//   //   colors[count] = p->first;
//   //   names[count++] = p->second->getName();
//   // }
//   // CurvedBoundaryDescriptionConstructor BDC(*this, names, colors);
//   // for(const_iterator p = MBD.begin(); p != MBD.end(); p++)
//   // {
//   //   FadaMesh::CurvedBoundaryDescriptionInterface* BDI = _curvedboundaries[p->first];
//   //   Alat::DoubleVector& myparameters = BDI->getParameters();
//   //   const Alat::DoubleVector& yourparameters = p->second->getParameters();
//   //   myparameters.set_size( yourparameters.size() );
//   //   myparameters = yourparameters;
//   // }
// }
