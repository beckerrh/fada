#ifndef __Fada_LinearConstraintsAllVariables_h
#define __Fada_LinearConstraintsAllVariables_h

#include  "Alat/map.h"
#include  "linearconstraintsonevariable.h"

/*--------------------------------------------------------------------------*/
namespace Fada
{
class LinearConstraintsAllVariables : public Alat::Map<std::string, LinearConstraintsOneVariable>
{
public:
  ~LinearConstraintsAllVariables();
  LinearConstraintsAllVariables();
  LinearConstraintsAllVariables( const LinearConstraintsAllVariables& linearconstraintsallvariables);
  LinearConstraintsAllVariables& operator=( const LinearConstraintsAllVariables& linearconstraintsallvariables);
  std::string getName() const;
  LinearConstraintsAllVariables* clone() const;
  
  LinearConstraintsOneVariable& getLinearConstraints(std::string varname);
  const LinearConstraintsOneVariable& getLinearConstraints(std::string varname) const;
  void read(std::ifstream& file, const std::string& dirname);
  void write(const std::string& dirname, const std::string& type) const;
  void writeNames(std::ofstream& file) const;  
};
}

/*--------------------------------------------------------------------------*/
#endif
