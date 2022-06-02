#include  "FadaMesh/markerpercentage.h"
#include  <fstream>
#ifdef  CLANG
#include  <numeric>
#else
#include  <ext/numeric>
#endif

using namespace FadaMesh;
using namespace std;

/*--------------------------------------------------------------------------*/

void MarkerPercentage::_mark()
{
  _sort();

  int nmax = _indicator.size();
  // std::cerr << "*** MarkerPercentage::_mark() _indicator.size()=" << nmax << "\n";
  double total = accumulate(_indicator.begin(), _indicator.end(), 0.0);
  double sum = 0.0;
  for(int i = 0; i < _indicator.size(); i++)
  {
    sum += _indicator[_C[i]];
    if(sum > _marking_parameter*total)
    {
      /// c'est le nombre à raffiner (et pas le dernier indice)
      nmax = i+1;
      break;
    }
  }
  // std::cerr << "*** MarkerPercentage::_mark() nmax sum total " << nmax << " " << sum << " " << total << "\n";
  _marked_cells.set_size(nmax);
  for(int i = 0; i < nmax; i++)
  {
    _marked_cells[i] = _C[i];
  }
}

/*--------------------------------------------------------------------------*/

void MarkerPercentage::write(std::string outfilename, std::string datatype)
{
  _mark();
  MarkerBase::write(outfilename, datatype);
}
