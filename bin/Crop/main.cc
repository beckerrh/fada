#include  "FadaMesh/hexahedralmesh.h"
#include  "FadaMesh/quadrilateralmesh.h"
#include  "Alat/map.h"
#include  "Alat/set.h"
#include  "Alat/tokenize.h"
#include  <algorithm>
#include  <iostream>

/*--------------------------------------------------------------------------*/

int main(int argc, char** argv)
{
  if(argc != 8 and argc != 9)
  {
    std::cerr << "*** ERROR in " << argv[0] <<" usage: filein  fileout meshtype datatype xmin:xmax ymin:ymax zmin:zmax inverse<false>\n";
    return 1;
  }
  std::string infilename(argv[1]);
  std::string outfilename(argv[2]);
  std::string mesh_type(argv[3]);
  std::string datatype(argv[4]);
  std::string xboundstr(argv[5]);
  std::string yboundstr(argv[6]);
  std::string zboundstr(argv[7]);
  bool inverse = false;
  if(argc == 9)
  {
    inverse = true;
  }
  std::cerr <<  argv[0] << " inverse = "<<inverse<<"\n";

  Alat::StringVector vx = Alat::Tokenize(xboundstr, ":");
  Alat::StringVector vy = Alat::Tokenize(yboundstr, ":");
  Alat::StringVector vz = Alat::Tokenize(zboundstr, ":");
  assert(vx.size() == 2);
  assert(vy.size() == 2);
  assert(vz.size() == 2);
  double xmin = atof( vx[0].c_str() );
  double ymin = atof( vy[0].c_str() );
  double zmin = atof( vz[0].c_str() );
  double xmax = atof( vx[1].c_str() );
  double ymax = atof( vy[1].c_str() );
  double zmax = atof( vz[1].c_str() );
  Alat::Node boundl(xmin, ymin, zmin), boundu(xmax, ymax, zmax);

  FadaMesh::MeshInterface* mesh;
  FadaMesh::MeshInterface* meshcropped;
  if(mesh_type == "quad")
  {
    mesh = new FadaMesh::QuadrilateralMesh;
    meshcropped = new FadaMesh::QuadrilateralMesh;
    FadaMesh::QuadrilateralMesh* quadmesh = dynamic_cast<FadaMesh::QuadrilateralMesh*>( mesh );
    infilename += ".quad";
    quadmesh->readQuad(infilename);
  }
  else if(mesh_type == "hex")
  {
    mesh = new FadaMesh::HexahedralMesh;
    // FadaMesh::HexahedralMesh* hexmeshcropped = new FadaMesh::HexahedralMesh;
    // meshcropped = hexmeshcropped;
    meshcropped = new FadaMesh::HexahedralMesh;
    FadaMesh::HexahedralMesh* hexmesh = dynamic_cast<FadaMesh::HexahedralMesh*>( mesh );
    infilename += ".hex";
    hexmesh->readHex(infilename);
  }
  else
  {
    std::cerr<<"***ERROR in " << argv[0] <<"  : invalid mesh type. Must be quad, hex or tri, but "<<mesh_type<<" given."<<'\n';
    assert(0);
    exit(1);
  }

  Alat::Set<int> nodestodelete;
  for(int i = 0; i < mesh->getNNodes(); i++)
  {
    const Alat::Node& node = mesh->getNode(i);
    // ---------- criterion
    if(inverse)
    // cut the bounding box
    {
      if( ( (node.x() <= boundu.x()) and (node.x() >= boundl.x()) )and ( (node.y() <= boundu.y())  and (node.y() >= boundl.y()) ) and ( (node.z() <= boundu.z())  and (node.z() >= boundl.z()) ) )
      {                                                                   
        std::cerr << "deleting: " << node << "\n";
        nodestodelete.insert(i);
      }
    }
    else
    // keep only the bounding box
    {
      if( ( node.x() >= boundu.x() )or ( node.x() <= boundl.x() ) or ( node.y() >= boundu.y() ) or ( node.y() <= boundl.y() ) or ( node.z() >= boundu.z() ) or ( node.z() <= boundl.z() ) )
      {
        nodestodelete.insert(i);
      }
    }
    // ---------- criterion
  }
  // std::cerr << "nodestodelete:\n" << nodestodelete << "\n";

  Alat::Set<int> cellstodelete;
  for(int i = 0; i < mesh->getNCells(); i++)
  {
    // const FadaMesh::HexahedralMesh::Hexahedral& cell = hexmesh->getCell(i);
    for(int ii = 0; ii < mesh->getNNodesPerCell(i); ii++)
    {
      int inode = mesh->getNodeIdOfCell(i, ii);
      if( nodestodelete.find(inode) != nodestodelete.end() )
      {
        cellstodelete.insert(i);
        break;
      }
    }
  }
  // std::cerr << "cellstodelete:\n" << cellstodelete << "\n";

  Alat::Set<int> sidestodelete;
  for(int i = 0; i < mesh->getNSides(); i++)
  {
    // const FadaMesh::HexahedralMesh::Side& side = hexmesh->getSide(i);
    for(int ii = 0; ii < mesh->getNNodesPerSide(0); ii++)
    {
      int inode = mesh->getNodeIdOfSide(i, ii);
      if( nodestodelete.find(inode) != nodestodelete.end() )
      {
        sidestodelete.insert(i);
        break;
      }
    }
    int icell1 = mesh->getCellIdOfSide(i, 0);
    int icell2 = mesh->getCellIdOfSide(i, 1);
    if( cellstodelete.find(icell1) != cellstodelete.end() and ( ( icell2 = -1 )or ( cellstodelete.find(icell2) != cellstodelete.end() ) ) )
    {
      sidestodelete.insert(i);
    }
    // std::cerr << "sidestodelete:\n" << sidestodelete << "\n";
  }


  Alat::IntMap node_oldtonew;
  int count = 0;
  for(int i = 0; i < mesh->getNNodes(); i++)
  {
    if( nodestodelete.find(i) == nodestodelete.end() )
    {
      node_oldtonew[i] = count++;
    }
  }
  // std::cerr << "node_oldtonew:\n" << node_oldtonew << "\n";

  Alat::IntMap cell_oldtonew;
  count = 0;
  for(int i = 0; i < mesh->getNCells(); i++)
  {
    if( cellstodelete.find(i) == cellstodelete.end() )
    {
      cell_oldtonew[i] = count++;
    }
  }
  // std::cerr << "cell_oldtonew:\n" << cell_oldtonew << "\n";

  Alat::IntMap side_oldtonew;
  count = 0;
  for(int i = 0; i < mesh->getNSides(); i++)
  {
    if( sidestodelete.find(i) == sidestodelete.end() )
    {
      side_oldtonew[i] = count++;
    }
  }
  // std::cerr << "side_oldtonew:\n" << side_oldtonew << "\n";

  Alat::Vector<Alat::Node>& nodescropped = meshcropped->getAllNodes();
  nodescropped.set_size( node_oldtonew.size() );
  for(Alat::IntMap::const_iterator p = node_oldtonew.begin(); p != node_oldtonew.end(); p++)
  {
    nodescropped[p->second] = mesh->getNode(p->first);
  }

  if(mesh_type == "quad")
  {
    FadaMesh::QuadrilateralMesh* quadmesh = dynamic_cast<FadaMesh::QuadrilateralMesh*>( mesh );
    assert(quadmesh);
    FadaMesh::QuadrilateralMesh* quadmeshcropped = dynamic_cast<FadaMesh::QuadrilateralMesh*>( meshcropped );
    assert(quadmeshcropped);
    Alat::Vector<FadaMesh::QuadrilateralMesh::Quadrilateral>& cellscropped =  quadmeshcropped->getCells();
    cellscropped.set_size( cell_oldtonew.size() );
    for(Alat::IntMap::const_iterator p = cell_oldtonew.begin(); p != cell_oldtonew.end(); p++)
    {
      cellscropped[p->second] = quadmesh->getCell(p->first);
    }
    for(int i = 0; i < cellscropped.size(); i++)
    {
      for(int ii = 0; ii < mesh->getNNodesPerCell(i); ii++)
      {
        int inode = cellscropped[i][ii];
        cellscropped[i][ii] = node_oldtonew[inode];
      }
    }

    // new boundary sides
    FadaMesh::QuadrilateralMesh::BoundarySideToColor bstc;
    const FadaMesh::BoundaryInfo* boundaryinfo = mesh->getBoundaryInfo();
    const Alat::IntVector& colors = boundaryinfo->getColors();
    for(int i = 0; i < colors.size(); i++)
    {
      int color = colors[i];
      const Alat::IntVector& sides = boundaryinfo->getSidesOfColor(color);
      for(int j = 0; j < sides.size(); j++)
      {
        if( sidestodelete.find(sides[j]) == sidestodelete.end() )
        {
          FadaMesh::QuadrilateralMesh::Side S;
          for(int ii = 0; ii < mesh->getNNodesPerSide(0); ii++)
          {
            int inode = quadmesh->getNodeIdOfSide(sides[j], ii);
            assert( nodestodelete.find(inode) == nodestodelete.end() );
            S[ii] = node_oldtonew[inode];
          }
          std::sort( S.begin(), S.end() );
          bstc[S] = color;
        }
      }
    }
    // std::cerr << " bstc " << bstc << "\n";
    quadmeshcropped->constructSidesFromCells(bstc, 111);
  }
  else
  {
    FadaMesh::HexahedralMesh* hexmesh = dynamic_cast<FadaMesh::HexahedralMesh*>( mesh );
    assert(hexmesh);
    FadaMesh::HexahedralMesh* hexmeshcropped = dynamic_cast<FadaMesh::HexahedralMesh*>( meshcropped );
    assert(hexmeshcropped);
    Alat::Vector<FadaMesh::HexahedralMesh::Hexahedral>& cellscropped =  hexmeshcropped->getCells();
    cellscropped.set_size( cell_oldtonew.size() );
    for(Alat::IntMap::const_iterator p = cell_oldtonew.begin(); p != cell_oldtonew.end(); p++)
    {
      cellscropped[p->second] = hexmesh->getCell(p->first);
    }
    for(int i = 0; i < cellscropped.size(); i++)
    {
      for(int ii = 0; ii < mesh->getNNodesPerCell(i); ii++)
      {
        int inode = cellscropped[i][ii];
        cellscropped[i][ii] = node_oldtonew[inode];
      }
    }

    // new boundary sides
    FadaMesh::HexahedralMesh::BoundarySideToColor bstc;
    const FadaMesh::BoundaryInfo* boundaryinfo = mesh->getBoundaryInfo();
    const Alat::IntVector& colors = boundaryinfo->getColors();
    for(int i = 0; i < colors.size(); i++)
    {
      int color = colors[i];
      const Alat::IntVector& sides = boundaryinfo->getSidesOfColor(color);
      for(int j = 0; j < sides.size(); j++)
      {
        if( sidestodelete.find(sides[j]) == sidestodelete.end() )
        {
          FadaMesh::HexahedralMesh::Side S;
          for(int ii = 0; ii < mesh->getNNodesPerSide(0); ii++)
          {
            int inode = hexmesh->getNodeIdOfSide(sides[j], ii);
            assert( nodestodelete.find(inode) == nodestodelete.end() );
            S[ii] = node_oldtonew[inode];
          }
          std::sort( S.begin(), S.end() );
          bstc[S] = color;
        }
      }
    }
    // std::cerr << " bstc " << bstc << "\n";
    hexmeshcropped->constructSidesFromCells(bstc, 111);
  }

// meshcropped->writeFadaMesh(outfilename, datatype);
  meshcropped->writeVtk(outfilename);
  meshcropped->writeBoundaryVtk(outfilename);
  meshcropped->write(outfilename);

  delete mesh;
  delete meshcropped;

  return 0;
}
