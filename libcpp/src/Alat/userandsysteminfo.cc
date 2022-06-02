#include  "Alat/userandsysteminfo.h"
#include  <cassert>
#include  <iostream>
#include  <stdio.h>

using namespace Alat;
using namespace std;

/*---------------------------------------------------------*/

UserAndSystemInfo::~UserAndSystemInfo()
{}

/*---------------------------------------------------------*/

UserAndSystemInfo::UserAndSystemInfo(std::ostream& os)
{
  os <<  "|>~~~ Date="<< getDate() << " User="<< getUser() << " System="<<getSystem() << "\n";
}

/*---------------------------------------------------------*/

UserAndSystemInfo::UserAndSystemInfo(const UserAndSystemInfo& U)
{
  assert(0);
}

/*---------------------------------------------------------*/

UserAndSystemInfo& UserAndSystemInfo::operator=(const UserAndSystemInfo& U)
{
  assert(0);
  return *this;
}

/*---------------------------------------------------------*/

std::string UserAndSystemInfo::_Run(std::string cmd) const
{
  FILE* in = NULL;
  char buff[512];
  std::string output;
  in = popen(cmd.c_str(), "r");
  assert(in);
  while(fgets(buff, sizeof( buff ), in) != NULL)
  {
    output.append(buff);
  }
  pclose(in);

  size_t endpos = output.find_last_not_of("\n");
  if( string::npos != endpos )
  {
    output = output.substr( 0, endpos+1 );
  }
  return output;
}

/*---------------------------------------------------------*/

std::string UserAndSystemInfo::getDate() const
{
  return _Run("date \"+%d-%m-%Y--%H:%M\"");
}

/*---------------------------------------------------------*/

std::string UserAndSystemInfo::getUser() const
{
  return _Run("whoami");
}

/*---------------------------------------------------------*/

std::string UserAndSystemInfo::getSystem() const
{
  std::string output = _Run("uname -m");
  output += "_";
  output += _Run("uname -s");
  output += "_";
  output += _Run("uname -r");
  return output;
}
