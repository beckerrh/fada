#include  "Alat/dataformathandler.h"
#include  <cassert>

using namespace Alat;

/*--------------------------------------------------------------------------*/
DataFormatHandler::~DataFormatHandler(){}
DataFormatHandler::DataFormatHandler(){}
DataFormatHandler::DataFormatHandler( const DataFormatHandler& dataformathandler)
{
  assert(0);
}
DataFormatHandler& DataFormatHandler::operator=( const DataFormatHandler& dataformathandler)
{
  assert(0);
  return *this;
}

/*--------------------------------------------------------------------------*/
void DataFormatHandler::get(std::string& f, const std::string& name)
{
  Alat::StringVector s(4);
  s[0] = "string";
  s[1] = "integer";
  s[2] = "double";
  s[3] = "bool";
  f = "";
  for(int i = 0; i < s.size(); i++)
  {
    f = search(s[i], name);
    if(f != "")
    {
      break;
    }
  }
}

/*--------------------------------------------------------------------------*/
std::string DataFormatHandler::search(std::string& fo, const std::string& name)
{
  std::set<NameType>::const_iterator p = _set_of_nametype.find( Alat::makePair(name, fo) );
  if( p != _set_of_nametype.end() )
  {
    return p->second;
  }
  std::string empty = "";
  return empty;
}

void DataFormatHandler::insert(const std::string& nm, std::string* pos)
{
  std::string type = "string";
  std::string name = nm;
  NameType p = Alat::makePair(name, type);
  _set_of_nametype.insert(p);
  _type_string.insert( make_pair(name, pos) );
}

void DataFormatHandler::insert(const std::string& nm, int* pos)
{
  std::string type = "integer";
  std::string name = nm;
  NameType p = Alat::makePair(name, type);
  _set_of_nametype.insert(p);
  _type_int.insert( make_pair(name, pos) );
}

void DataFormatHandler::insert(const std::string& nm, bool* pos)
{
  std::string type = "bool";
  std::string name = nm;
  NameType p = Alat::makePair(name, type);
  _set_of_nametype.insert(p);
  _type_bool.insert( make_pair(name, pos) );
}

void DataFormatHandler::insert(const std::string& nm, double* pos)
{
  std::string type = "double";
  std::string name = nm;
  NameType p = Alat::makePair(name, type);
  _set_of_nametype.insert(p);
  _type_double.insert( make_pair(name, pos) );
}

void DataFormatHandler::insert(const std::string& nm, std::string* pos, const std::string& def)
{
  insert(nm, pos);
  *pos = def;
}

void DataFormatHandler::insert(const std::string& nm, int* pos, int def)
{
  insert(nm, pos);
  *pos = def;
}

void DataFormatHandler::insert(const std::string& nm, bool* pos, bool def)
{
  insert(nm, pos);
  *pos = def;
}

void DataFormatHandler::insert(const std::string& nm, double* pos, double def)
{
  insert(nm, pos);
  *pos = def;
}

void DataFormatHandler::setvalue(const std::string& name, const std::string& value)
{
  TypeString::const_iterator p;
  p = _type_string.find(name);
  if( p != _type_string.end() )
  {
    *( p->second ) = value;
    return;
  }
}

void DataFormatHandler::setvalue(const std::string& name, int value)
{
  TypeInt::const_iterator p;
  p = _type_int.find(name);
  if( p != _type_int.end() )
  {
    *( p->second ) = value;
    return;
  }
}

void DataFormatHandler::setvalue(const std::string& name, bool value)
{
  TypeBool::const_iterator p;
  p = _type_bool.find(name);
  if( p != _type_bool.end() )
  {
    *( p->second ) = value;
    return;
  }
}

void DataFormatHandler::setvalue(const std::string& name, double value)
{
  TypeDouble::const_iterator p;
  p = _type_double.find(name);
  if( p != _type_double.end() )
  {
    *( p->second ) = value;
    return;
  }
}

void DataFormatHandler::print(std::ostream& s) const
{
  s << "NameTypes:  ";		
	for(std::set<NameType>::const_iterator p = _set_of_nametype.begin(); p!= _set_of_nametype.end(); p++)
	{
    s << p->first << " (" << p->second << ")   ";		
	}
  s << "\n";		
	
	for(TypeString::const_iterator p = _type_string.begin(); p!= _type_string.end(); p++)
	{
    s << p->first << "\t" << *( p->second ) << "\n";		
	}
	for(TypeInt::const_iterator p = _type_int.begin(); p!= _type_int.end(); p++)
	{
    s << p->first << "\t" << *( p->second ) << "\n";		
	}
	for(TypeDouble::const_iterator p = _type_double.begin(); p!= _type_double.end(); p++)
	{
    s << p->first << "\t" << *( p->second ) << "\n";		
	}
	for(TypeBool::const_iterator p = _type_bool.begin(); p!= _type_bool.end(); p++)
	{
    s << p->first << "\t" << *( p->second ) << "\n";		
	}
}
