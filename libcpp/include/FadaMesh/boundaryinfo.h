#ifndef  __FadaMesh_BoundaryInfo_h
#define  __FadaMesh_BoundaryInfo_h

#include  "Alat/intvector.h"
#include  "Alat/map.h"
#include  "Alat/vector.h"

/*---------------------------------------------------------*/
namespace FadaMesh
{
  class BoundaryInfo
  {
protected:
    Alat::IntMap  _index_of_col;
    Alat::IntVector _colors;
    Alat::Vector<Alat::IntVector> _cells;
    Alat::Vector<Alat::IntVector> _sides;
    Alat::Vector<Alat::IntVector> _sideids_of_cells;

    void _ConstructIntOfColor();
    int _IndexOfColor(int color) const;

public: ~BoundaryInfo();
    BoundaryInfo();
    BoundaryInfo(const BoundaryInfo& boundaryinfo);
    BoundaryInfo(const std::string& filename);
    BoundaryInfo& operator=(const BoundaryInfo& boundaryinfo);

    void set_size(const std::map<int, int>& size_of_color);

    void read(const std::string& filename);
    void write(const std::string& filename, const std::string& type = "binary") const;

    // int getNBoundaries();
    int getNSides() const;
    Alat::IntVector& getColors();
    const Alat::IntVector& getColors() const;
    Alat::IntVector& getCellsOfColor(int color);
    const Alat::IntVector& getCellsOfColor(int color) const;
    Alat::IntVector& getSidesOfColor(int color);
    const Alat::IntVector& getSidesOfColor(int color) const;
    Alat::IntVector& getSidesIdOfCellsOfColor(int color);
    const Alat::IntVector& getSidesIdOfCellsOfColor(int color) const;
    // void removeColor(int color);
  };
}

#endif
