#include  "FadaMesh/markercoarsepercentage.h"
#include  <fstream>
#ifdef  CLANG
#include  <numeric>
#else
#include  <ext/numeric>
#endif

using namespace FadaMesh;
using namespace std;

/*--------------------------------------------------------------------------*/

void MarkerCoarsePercentage::_mark()
{
  _sort();
  int nmax = _indicator.size();
  double total = accumulate(_indicator.begin(), _indicator.end(), 0.0);
  double sum = 0.0;
  std::cerr << "_indicator.size() " << _indicator.size() << "\n";
  for(int i = 0; i < _indicator.size(); i++)
  {
    // std::cerr << _C[i] << " " << _indicator[_C[i]] << "\n";
    sum += _indicator[_C[i]];
    if(sum > (1.0-_marking_parameter)*total)
    {
      nmax = i+1;
      break;
    }
  }

  _marked_cells.set_size(_indicator.size()-nmax);
  for(int i = 0; i < _indicator.size()-nmax; i++)
  {
    _marked_cells[i] = _C[i+nmax];
  }
}

/*--------------------------------------------------------------------------*/

void MarkerCoarsePercentage::write(std::string outfilename, std::string datatype)
{
  _mark();
  MarkerBase::write(outfilename, datatype);
}

