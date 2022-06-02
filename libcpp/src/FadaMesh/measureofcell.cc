#include  "FadaMesh/measureofcell.h"
#include  <fstream>
#include  <cassert>

using namespace FadaMesh;
using namespace std;

/*--------------------------------------------------------------------------*/

void MeasureOfCell::read(std::string filename)
{
  ifstream file( filename.c_str() );
  if( not file.is_open() )
  {
    cerr << "*** MeasureOfCell::read() : cannot read file \"" << filename << "\"\n";
    assert(0);
  }
  Alat::DoubleVector::loadFada(file);
}

/*--------------------------------------------------------------------------*/

void MeasureOfCell::write(std::string filename, std::string datatype) const
{
  ofstream file( filename.c_str() );
  if( not file.is_open() )
  {
    cerr << "*** MeasureOfCell::write() : cannot write file \"" << filename << "\"\n";
    assert(0);
  }
  Alat::DoubleVector::saveFada(file, datatype);
}
