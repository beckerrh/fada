#include  "Alat/directoryandfileexists.h"
#include  <fstream>
#include  <iostream>
#include  <sys/stat.h>   // For stat().
#include  <sys/types.h>  // For stat().

#include <dirent.h>

namespace Alat
{
  bool _DirectoryExists( std::string pzPath )
  {
    DIR* pDir;
    bool bExists = false;

    pDir = opendir ( pzPath.c_str() );

    if(pDir != NULL)
    {
      bExists = true;
      (void) closedir (pDir);
    }

    return bExists;
  }

/*--------------------------------------------------------------------------*/

  bool _FileExists(std::string strPath)
  {
    std::ifstream file( strPath.c_str() );
    bool filexists= file.is_open();
    file.close();
    return filexists;
  }
}
