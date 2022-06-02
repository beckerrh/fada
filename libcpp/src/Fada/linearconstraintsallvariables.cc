#include  "Fada/linearconstraintsallvariables.h"
#include  <cassert>
#include  <fstream>

using namespace Fada;

/*--------------------------------------------------------------------------*/
LinearConstraintsAllVariables::~LinearConstraintsAllVariables() 
{
}

LinearConstraintsAllVariables::LinearConstraintsAllVariables(): Alat::Map<std::string, LinearConstraintsOneVariable>()
{
}

LinearConstraintsAllVariables::LinearConstraintsAllVariables( const LinearConstraintsAllVariables& linearconstraintsallvariables): Alat::Map<std::string, LinearConstraintsOneVariable>(linearconstraintsallvariables)
{
(*this).operator=(linearconstraintsallvariables);
}

LinearConstraintsAllVariables& LinearConstraintsAllVariables::operator=( const LinearConstraintsAllVariables& linearconstraintsallvariables) 
{
Alat::Map<std::string, LinearConstraintsOneVariable>::operator=(linearconstraintsallvariables);
assert(0);
return *this;
}

std::string LinearConstraintsAllVariables::getName() const 
{
return "LinearConstraintsAllVariables";
}

LinearConstraintsAllVariables* LinearConstraintsAllVariables::clone() const 
{
return new LinearConstraintsAllVariables(*this);
}

/*--------------------------------------------------------------------------*/

LinearConstraintsOneVariable& LinearConstraintsAllVariables::getLinearConstraints(std::string varname)
{
  return ( *this )[varname];
}
const LinearConstraintsOneVariable& LinearConstraintsAllVariables::getLinearConstraints(std::string varname) const
{
  return ( *this )[varname];
}
void LinearConstraintsAllVariables::read(std::ifstream& file, const std::string& dirname)
{
  int nc;
  file>>nc;
  for(int i = 0; i < nc; i++)
  {
    std::string name;
    file>>name;
    std::string filename = dirname+"/"+name;
    // std::cerr << "LinearConstraints::read() nc name filename " << nc << " " << name << " "<< filename << "\n"; 
    getLinearConstraints(name).read(filename);
  }
}
void LinearConstraintsAllVariables::write(const std::string& dirname, const std::string& type) const
{
  for(Alat::Map<std::string,LinearConstraintsOneVariable>::const_iterator p=begin();p!=end();p++)
  {
    std::string name = dirname+"/"+p->first;
    p->second.write(name,type);    
  }   
}
void LinearConstraintsAllVariables::writeNames(std::ofstream& file) const
{
  file << size() << '\n';
  for(Alat::Map<std::string,LinearConstraintsOneVariable>::const_iterator p=begin();p!=end();p++)
  {
    file << p->first<<'\n';
  }   
}
