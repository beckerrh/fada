#include  "Alat/alatio.h"
#include  "Alat/tokenize.h"
#include  "Alat/intvector.h"
#include  "Alat/doublevector.h"
#include  <cassert>

using namespace Alat;

/*---------------------------------------------*/
std::ostream& Alat::writeBin(std::ostream& out, const std::vector<double>& vec)
{
  out.write( reinterpret_cast<const char*>( &vec[0] ), vec.size()*sizeof( double ) );
  return out;    
}
std::istream& Alat::readBin(std::istream& in, std::vector<double>& vec)
{
  in.read( reinterpret_cast<char*>( &vec[0] ), vec.size()*sizeof( double ) );
  return in;    
}
std::ostream& Alat::saveAlat(std::ostream& out, const std::vector<double>& vec, const std::string datatype)
{
  out<<vec.size()<<" "<<datatype<<" "<<std::endl;
  if(datatype == "ascii")
  {
    copy( vec.begin(), vec.end(), std::ostream_iterator<double>(out, " ") );
  }
  else
  {
    writeBin(out, vec);
  }
  return out;    
}
std::istream& Alat::loadAlat(std::istream& in, std::vector<double>& vec)
{
  int n;
  std::string datatype;
  in>>n>>datatype;
  vec.resize(n);
  if(datatype == "ascii")
  {
    std::vector<double>::iterator p = vec.begin();
    while( p != vec.end() )
    {
      in>>*p++;
    }
  }
  else if(datatype == "binary")
  {
    std::string str;
    while(str == "")
    {
      getline(in, str);
    }
    readBin(in, vec);
  }
  else
  {
    std::cerr<<"*** Vector<T>::read() : wrong datatype \""<<datatype<<"\"\n";
    assert(0);
  }
  return in;    
}

/*---------------------------------------------*/
void Alat::reInitFromString(std::string valuechain, Alat::IntVector& vec, const std::string& sep)
{
  Alat::StringVector bouts = Alat::Tokenize(valuechain, sep);
  int n = bouts.size();
  vec.set_size(n);
  for(int i = 0; i < n; i++)
  {
    vec[i] = atoi( bouts[i].c_str() );
  }
}
void Alat::reInitFromString(std::string valuechain, Alat::DoubleVector& vec, const std::string& sep)
{
  Alat::StringVector bouts = Alat::Tokenize(valuechain, sep);
  int n = bouts.size();
  vec.set_size(n);
  for(int i = 0; i < n; i++)
  {
    vec[i] = atof( bouts[i].c_str() );
  }
}
