#include  "Alat/ghost.h"
#include  <iostream>

using namespace Alat;

/*--------------------------------------------------------------------------*/
Ghost::~Ghost()
{}
Ghost::Ghost() : _name("none"), _description("allvars"), _level(0)
{}
Ghost::Ghost(const std::string name, int level) : _name(name), _description("allvars"), _level(level) {}
Ghost::Ghost(const std::string name, const std::string description, int level) : _name(name), _description(description), _level(level) {}
Ghost::Ghost(const std::string name, const Alat::StringSet& variables, int level) : _name(name), _description("allvars"), _level(level)
{
  _variables = variables;
}

Ghost::Ghost( const Ghost& ghost) : _name(ghost._name), _description(ghost._description), _variables(ghost._variables), _level(ghost._level)
{}
Ghost& Ghost::operator=( const Ghost& ghost)
{
  _name = ghost._name;
  _description = ghost._description;
  _variables = ghost._variables;
  _level = ghost._level;
  return *this;
}

std::string Ghost::getClassName() const
{
  return "Ghost";
}

std::ostream& Alat::operator<<(std::ostream& os, const Ghost& g)
{
  os << "(Name/description/variables/level:) " << g.getName() <<"/"<< g.getDescription() <<"/"<< g.getVariables()<<"/"<<g.getLevel();
  return os;
}

/*--------------------------------------------------------------------------*/
void Ghost::setVariables(const Alat::StringSet& variables)
{
  _variables = variables;
}

const Alat::StringSet& Ghost::getVariables() const
{
  return _variables;
}

void Ghost::setName(const std::string& name)
{
  _name = name;
}

const std::string& Ghost::getName() const
{
  return _name;
}

void Ghost::setDescription(const std::string& description)
{
  _description = description;
}

const std::string& Ghost::getDescription() const
{
  return _description;
}

int Ghost::getLevel() const
{
  return _level;
}

void Ghost::setLevel(int level)
{
  _level = level;
}

/*--------------------------------------------------------------------------*/
bool Ghost::operator<(const Ghost& v) const
{
  if( getLevel() < v.getLevel() )
  {
    return true;
  }
  if( getLevel() > v.getLevel() )
  {
    return false;
  }
  if( getName() < v.getName() )
  {
    return true;
  }
  if( getName() > v.getName() )
  {
    return false;
  }
  return false;
}

bool Ghost::operator==(const Ghost& v) const
{
  return ( getLevel() == v.getLevel() ) && ( getName() == v.getName() );
}
