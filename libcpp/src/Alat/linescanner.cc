#include  "Alat/linescanner.h"
#include  "Alat/tokenize.h"
#include  <cassert>
#include  <iostream>
#include  <stdlib.h>
using namespace Alat;

/*--------------------------------------------------------------------------*/
LineScanner::~LineScanner()
{
  _filestream.close();
}
LineScanner::LineScanner()
{}
LineScanner::LineScanner( const LineScanner& linescanner)
{
  assert(0);
}
LineScanner& LineScanner::operator=( const LineScanner& linescanner)
{
  assert(0);
  return *this;
}
LineScanner::LineScanner(std::string filename)
{
  _filestream.open( filename.c_str() );
  if( !_filestream.is_open() )
  {
    std::cerr<<"*** ERROR LineScanner::cannot open file "<<filename<<std::endl;
    assert(0);
    exit(1);
  }
}

/*--------------------------------------------------------------------------*/

int LineScanner::nextLine(Alat::StringVector& words)
{
  std::string toto;
  getline(_filestream, toto);

  if( _filestream.eof() )
  {
    return -1;
  }

  words.resize(0);

  Alat::StringVector s1 = Alat::Tokenize(toto, " ");
  if( ( s1.size() ) && ( s1[0] != "//Block" ) )
  {
    if( ( toto[0] == '/' ) && ( toto[1] == '/' ) )
    {
      return 0;
    }
  }
  for(int i = 0; i < s1.size(); i++)
  {
    Alat::StringVector s2 = Alat::Tokenize(s1[i], "\t");
    for(int j = 0; j < s2.size(); j++)
    {
      if(s2[j] != "")
      {
        words.push_back(s2[j]);
      }
    }
  }

  return words.size();
}
