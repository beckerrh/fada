#include  "FadaMesh/markeroptimalcoarse.h"
#include  <fstream>
#include  <math.h>
#ifdef  CLANG
#include  <numeric>
#else
#include  <ext/numeric>
#endif

using namespace FadaMesh;
using namespace std;
/*--------------------------------------------------------------------------*/

double MarkerOptimalCoarse::_f(int i, int n, double total, double sum) const
{
  return pow(3*i+n, 2.*_s)* ( total-_alpha*sum );
}

/*--------------------------------------------------------------------------*/

void MarkerOptimalCoarse::_mark()
{
  _sort();

  double total = accumulate(_indicator.begin(), _indicator.end(), 0.0);

  double sumlast = 0.0;
  double sum = 0.0;
  double sumnext = 0.0;
  int nmax = _indicator.size();
  int n = nmax;
  double p = 3;
  if(_dimension == 3)
  {
    p = 7;
  }
  _alpha = 1.0 - pow(p+1, -2.0*_s);
  for(int i = 1; i < n; i++)
  {
    sumlast +=  _indicator[_C[i-1]];
    sum = sumlast+_indicator[_C[i]];
    sumnext = sum+_indicator[_C[i+1]];
    double fp = _f(i-1, n, total, sumlast);
    double f  = _f(i, n, total, sum);
    double fn = _f(i+1, n, total, sumnext);

    if( ( f > fp )&&( f < fn ) )
    {
      {
        /// c'est le nombre à raffiner (et pas le dernier indice)
        nmax = i;
        break;
      }
    }
  }
  _marked_cells.set_size(n-nmax);
  for(int i = 0; i < n-nmax; i++)
  {
    _marked_cells[i] = _C[nmax+i];
  }
}

/*--------------------------------------------------------------------------*/

void MarkerOptimalCoarse::write(std::string outfilename, std::string datatype)
{
  _mark();
  MarkerBase::write(outfilename, datatype);
}

