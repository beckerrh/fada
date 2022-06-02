#include  "FadaMesh/tetrahedralmesh.h"
#include  <fstream>

using namespace FadaMesh;
using namespace std;


/*-------------------------------------------------------*/

double TetrahedralMesh::_ComputeVolume(const Tetrahedron& K) const
{
  double dx1 = getNode(K[1]).x()-getNode(K[0]).x();
  double dx2 = getNode(K[2]).x()-getNode(K[0]).x();
  double dx3 = getNode(K[3]).x()-getNode(K[0]).x();
  double dy1 = getNode(K[1]).y()-getNode(K[0]).y();
  double dy2 = getNode(K[2]).y()-getNode(K[0]).y();
  double dy3 = getNode(K[3]).y()-getNode(K[0]).y();
  double dz1 = getNode(K[1]).z()-getNode(K[0]).z();
  double dz2 = getNode(K[2]).z()-getNode(K[0]).z();
  double dz3 = getNode(K[3]).z()-getNode(K[0]).z();
  return ( dx1*( dy2*dz3-dz2*dy3 )-dy1*( dx2*dz3-dz2*dx3 )+dz1*( dx2*dy3-dx3*dy2 ) )/6.0;
}
// 
// /*----------------------------------------------------------*/
// 
// void TetrahedralMesh::writeVtk(std::string filename) const
// {
//   string name = filename;
//   name += ".vtk";
// 
//   ofstream file( name.c_str() );
//   assert( file.is_open() );
// 
//   file<<"# vtk DataFile Version 4.0 "<<std::endl;
//   file<<"output from TetrahedralMesh"<<std::endl;
//   file<<"ASCII"<<std::endl;
//   //     file << "binary" << std::endl;
//   file<<"DATASET UNSTRUCTURED_GRID"<<std::endl;
//   file<<std::endl;
// 
//   int nn = getNNodes();
// 
//   file<<"POINTS "<<nn;
//   file<<" FLOAT"<<std::endl;
//   for(int i = 0; i < nn; i++)
//   {
//     const TetrahedralMesh::Node& v = getNode(i);
//     file<<v.x()<<" "<<v.y()<<" "<<v.z()<<" "<<std::endl;
//   }
//   file<<std::endl;
// 
//   int ne = getNCells();
//   int nle = 4;
//   int length = ne*( nle+1 );
// 
//   file<<std::endl<<"CELLS "<<ne<<" "<<length<<std::endl;
// 
//   for(int ie = 0; ie < ne; ie++)
//   {
//     file<<nle<<" ";
//     for(int ii = 0; ii < nle; ii++)
//     {
//       file<<getNodeIdOfCell(ie, ii)<<" ";
//     }
//     file<<std::endl;
//   }
//   file<<std::endl<<"CELL_TYPES "<<ne<<std::endl;
//   for(int c = 0; c < ne; c++)
//   {
//     file<<10<<" ";
//   }
//   file<<std::endl;
// 
//   file.close();
// }
// 
// /*----------------------------------------------------------*/
// 
// void TetrahedralMesh::writeBoundaryVtk(std::string filename) const
// {
//   string name = filename;
//   name += "-boundary.vtk";
// 
//   ofstream file( name.c_str() );
//   assert( file.is_open() );
// 
//   file<<"# vtk DataFile Version 4.0 "<<std::endl;
//   file<<"output from TetrahedralMesh"<<std::endl;
//   file<<"ASCII"<<std::endl;
//   //     file << "binary" << std::endl;
//   file<<"DATASET UNSTRUCTURED_GRID"<<std::endl;
//   file<<std::endl;
// 
//   int nn = getNNodes();
// 
//   file<<"POINTS "<<nn;
//   file<<" FLOAT"<<std::endl;
//   for(int i = 0; i < nn; i++)
//   {
//     const TetrahedralMesh::Node& v = getNode(i);
//     file<<v.x()<<" "<<v.y()<<" "<<v.z()<<" "<<std::endl;
//   }
//   file<<std::endl;
// 
//   const FadaMesh::BoundaryInfo* BI = getBoundaryInfo();
//   int nsides = BI->getNSides();
//   const Alat::IntVector& colors = BI->getColors();
// 
//   int nle = 3;
//   int length = nsides*( nle+1 );
//   file<<std::endl<<"CELLS "<<nsides<<" "<<length<<std::endl;
// 
//   for(int i = 0; i < colors.size(); i++)
//   {
//     int color = colors[i];
//     const Alat::IntVector& sides = BI->getSidesOfColor(color);
//     for(int j = 0; j < sides.size(); j++)
//     {
//       file<<nle<<" ";
//       for(int ii = 0; ii < nle; ii++)
//       {
//         file<<getNodeIdOfSide(sides[j], ii)<<" ";
//       }
//       file<<std::endl;
//     }
//   }
//   file<<std::endl<<"CELL_TYPES "<<nsides<<std::endl;
//   for(int c = 0; c < nsides; c++)
//   {
//     file<<5<<" ";
//   }
//   file<<std::endl;
//   file<<std::endl<<"CELL_DATA "<<nsides<<std::endl;
//   file<<std::endl<<"SCALARS "<<" bdry_colors "<<" int "<<1<<std::endl;
//   file<<std::endl<<"LOOKUP_TABLE default"<<std::endl;
//   for(int i = 0; i < colors.size(); i++)
//   {
//     int color = colors[i];
//     const Alat::IntVector& sides = BI->getSidesOfColor(color);
//     for(int j = 0; j < sides.size(); j++)
//     {
//       file<<color<<" ";
//       file<<std::endl;
//     }
//     file<<std::endl;
//   }
// 
//   file.close();
// }

