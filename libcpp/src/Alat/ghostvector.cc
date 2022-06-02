#include  "Alat/ghostvector.h"

using namespace Alat;

/*--------------------------------------------------------------------------*/
GhostVector::~GhostVector()
{}
GhostVector::GhostVector() : Ghost(), _type("none")
{}
GhostVector::GhostVector( const GhostVector& ghostvector) : Ghost(ghostvector), _type(ghostvector._type)
{}
GhostVector::GhostVector(const std::string name, const std::string& type, int level) : Ghost(name, level), _type(type) {}
GhostVector::GhostVector(const std::string name, const std::string& type, const Alat::StringSet& variables, int level) : Ghost(name, variables, level), _type(type) {}
GhostVector& GhostVector::operator=( const GhostVector& ghostvector)
{
  Ghost::operator=(ghostvector);
  _type = ghostvector._type;
  return *this;
}

std::string GhostVector::getClassName() const
{
  return "GhostVector";
}

/*--------------------------------------------------------------------------*/
void GhostVector::setType(const std::string& type)
{
  _type = type;
}

const std::string& GhostVector::getType() const
{
  return _type;
}

/*--------------------------------------------------------------------------*/
std::ostream& Alat::operator<<(std::ostream& os, const GhostVector& g)
{
  os << "(Name/description/type/variables/level:) " << g.getName() <<"/"<< g.getDescription()<<"/"<< g.getType()<<"/"<< g.getVariables()<<"/"<<g.getLevel();
  return os;
}
