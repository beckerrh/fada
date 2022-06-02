#include  "FadaMesh/markermean.h"
#include  <fstream>
#ifdef  CLANG
#include  <numeric>
#else
#include  <ext/numeric>
#endif

using namespace FadaMesh;
using namespace std;

/*--------------------------------------------------------------------------*/

void MarkerMean::_mark()
{
  int n = _indicator.size();
  int nmax = n;

  double mean_indicator = accumulate(_indicator.begin(),_indicator.end(),0.0)/(double) n;

  // On calcule le nombre de mailles à marquer selon notre critère de marquage 
  int count=0;
  for(int i = 0; i < n; i++)
  {
    if( _indicator[i]>=_marking_parameter*mean_indicator)
    {
      count++;
    }
  }

  // On définit ainsi le vecteur contenant les indices des mailles marquées pour le raffinement
  _marked_cells.set_size(count);
  count=0;
  for(int i = 0; i < n; i++)
  {
    if( _indicator[i]>=_marking_parameter*mean_indicator)
    {
      _marked_cells[count++] = i;
    }
  }
}

/*--------------------------------------------------------------------------*/

void MarkerMean::write(std::string outfilename, std::string datatype)
{
  _mark();
  MarkerBase::write(outfilename, datatype);
}

