#include  "Alat/filescanner.h"
#include  "Alat/linescanner.h"
#include  "Alat/parameterfile.h"
#include  "Alat/tokenize.h"
#include  <cassert>
#include  <iostream>
#include  <iterator>
#include  <stdlib.h>

using namespace Alat;

int FileScanner::_count = 0;

/*--------------------------------------------------------------------------*/
FileScanner::~FileScanner(){}
FileScanner::FileScanner(DataFormatHandler& dataformathandler, const Alat::ParameterFile* parameterfile, const std::string& blockname, bool complain) : _blockname(blockname), _complain(complain), _dataformathandler(dataformathandler)
{
  _blocksymbol = "//Block";
  Alat::StringVector tp = Alat::Tokenize(*parameterfile, "/");
  std::string filename;
  if(tp.size()>2)
  {
    std::string dirname;
    for(int i=0;i<tp.size()-1;i++)
    {
      dirname += '/' + tp[i];
    }    
  	filename = dirname + "/ALLYouCanRead";
  }
  else
  {
    filename = "ALLYouCanRead";
  }
  std::ofstream file;
	if(_count==0)
	{
		file.open(filename.c_str());		
	}
	else
	{
		file.open(filename.c_str(), std::ios_base::app);				
	}
  if( not file.is_open() )
  {
    std::cerr<<"*** ERROR cannot open file \""<<filename<<"\"\n";
		std::cerr << "_count="<<_count<<"\n";
    assert(0);
    exit(1);
  }
	file << "-----------  Block " << _blockname << " ----------\n";
	_dataformathandler.print(file);
	file << "\n";
	file.close();
	_count++;

  readfile(parameterfile);		
}

FileScanner::FileScanner( const FileScanner& filescanner) : _dataformathandler(filescanner._dataformathandler)
{
  assert(0);
}

FileScanner& FileScanner::operator=( const FileScanner& filescanner)
{
  assert(0);
  return *this;
}

std::string FileScanner::getName() const
{
  return "FileScanner";
}

/*--------------------------------------------------------------------------*/

void FileScanner::_assert(bool b, const Alat::StringVector& words) const
{
  if(!b)
  {
    std::cerr << "*** ERROR in FileScanner:\tWrong number of arguments in row\t";
    std::ostream_iterator<std::string>  os(std::cerr, " ");
    copy ( words.begin(), words.end(), os);
    std::cerr << std::endl;
    assert(0);
    exit(1);
  }
}

/*--------------------------------------------------------------------------*/

void FileScanner::print(std::ostream& os) const
{
  os << "=====================" << std::endl;
  os << _blocksymbol << " " << _blockname << std::endl << std::endl;
  _dataformathandler.print(os);
}

/*--------------------------------------------------------------------------*/

void FileScanner::readfile(const Alat::ParameterFile* parameterfile)
{
  if(parameterfile == NULL)
  {
    return;
  }

  std::string inputname = parameterfile->getName();
  LineScanner linescanner(inputname);

  Alat::StringVector words;
  int nwords = 0;
  bool searchblock = ( _blockname != "" );
  bool blockfound  = !searchblock;
  bool helpfound   = 0;
  std::string helpname = "HELP_ME";

  while(nwords >= 0)
  {
    nwords = linescanner.nextLine(words);
    if(nwords == -1)
    {
      if(_complain)
      {
        std::cerr << "FileScanner::readfile() : cannot read file or block \""<<_blockname<<"\"\n";
      }
      return;
    }

    if(nwords == 0)
    {
      continue;
    }

    if(words[0] != _blocksymbol)
    {
      continue;
    }

    if(nwords == 1)
    {
      std::cerr << "FileScanner::Block without name" << std::endl;
    }
    else if(words[1] == helpname)
    {
      helpfound = 1;
    }
    else if(words[1] == _blockname)
    {
      blockfound = 1;
      break;
    }
  }
  if(helpfound)
  {
    print(std::cerr);
  }

  if(!blockfound)
  {
    //    cout << "FileScanner::missing Block " << _blockname << endl;
    return;
  }
  //
  // scanning parameters in block
  //
  while(nwords >= 0)
  {
    nwords = linescanner.nextLine(words);

//       std::cerr << "+ " << nwords << " " << words << std::endl;

    if(nwords == -1)
    {
      break;
    }
    if(nwords == 0)
    {
      continue;
    }

    // testing if next block begins
    //
    if(words[0] == _blocksymbol)
    {
      break;
    }
    //
    // testing commentaries
    //
    if(words[0] == "/*")
    {
      continue;
    }
    if(words[0] == "//")
    {
      continue;
    }

    if(nwords == 1)
    {
      std::cerr << "*** ERROR in FileScanner::readfile(): parameter \"" << words[0] << "\" not given !" << std::endl;
      assert(0);
      exit(1);
    }
    FormatToValue(words);
  }
}

/*--------------------------------------------------------------------------*/

void FileScanner::FormatToValue(const Alat::StringVector& words)
{
  std::string keyword_type;
  std::string keyword = words[0];
  _dataformathandler.get(keyword_type, keyword);

  // std::cerr << "- \"" << keyword_type << "\" \"" << keyword << "\"" <<std::endl;

  if(keyword_type == "string")
  {
    _dataformathandler.setvalue(keyword, words[1]);
  }
  else if(keyword_type == "integer")
  {
    int value = atoi( words[1].c_str() );
    _dataformathandler.setvalue(keyword, value);
  }
  else if(keyword_type == "bool")
  {
    if( ( words[1] == "true" ) ||( words[1] == "1" ) )
    {
      _dataformathandler.setvalue(keyword, true);
    }
    else
    {
      _dataformathandler.setvalue(keyword, false);
    }
  }
  else if(keyword_type == "double")
  {
    double value = atof( words[1].c_str() );
    _dataformathandler.setvalue(keyword, value);
  }
}
