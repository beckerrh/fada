#ifndef __Alat_UserAndSystemInfo_h
#define __Alat_UserAndSystemInfo_h

#include  <string>

/*---------------------------------------------------------*/

namespace Alat
{
  class UserAndSystemInfo
  {
private:
    std::string _Run(std::string cmd) const;
    std::string getDate() const;
    std::string getUser() const;
    std::string getSystem() const;

public:
    ~UserAndSystemInfo();
    UserAndSystemInfo(std::ostream& os);
    UserAndSystemInfo(const UserAndSystemInfo& U);
    UserAndSystemInfo& operator=(const UserAndSystemInfo& U);
  };
}

/*---------------------------------------------------------*/

#endif
