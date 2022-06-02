#include  "Alat/tokenize.h"

namespace Alat
{
/*--------------------------------------*/

  Alat::StringVector Tokenize(const std::string& str, const std::string& sep)
  {
    Alat::StringVector tokens;
    // Skip sep at beginning.
    std::string::size_type lastPos = str.find_first_not_of(sep, 0);
    // Find first "non-delimiter".
    std::string::size_type pos     = str.find_first_of(sep, lastPos);

    while(std::string::npos != pos || std::string::npos != lastPos)
    {
      // Found a token, add it to the vector.
      tokens.push_back( str.substr(lastPos, pos-lastPos) );
      // Skip sep.  Note the "not_of"
      lastPos = str.find_first_not_of(sep, pos);
      // Find next "non-delimiter"
      pos = str.find_first_of(sep, lastPos);
    }
    return tokens;
  }

/*--------------------------------------*/

  Alat::StringVector Tokenize(const std::string& str, const std::string& sep1, const std::string& sep2)
  {
    Alat::StringVector words1 = Tokenize(str, sep1);
    Alat::StringVector words;
    for(int i = 0; i < words1.size(); i++)
    {
      Alat::StringVector words2 = Tokenize(words1[i], sep2);
      for(int ii = 0; ii < words2.size(); ii++)
      {
        words.push_back(words2[i]);
      }
    }
    return words;
  }
}