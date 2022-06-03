#ifndef ___Loop_h
#define ___Loop_h

#include  "Fada/loop.h"

/*--------------------------------------------------------------------------*/
class Loop : public Fada::Loop
{
protected:
  Alat::GhostVector _help;
  std::string _todo;

public:
  ~Loop();
  Loop(std::string todo);
  Loop( const Loop& loop);
  Loop& operator=( const Loop& loop);
  std::string getName() const;
  Loop* clone() const;
  FadaEnums::looptype getType() const;
  void run();
  void basicInit(Fada::ModelInterface* model, Fada::SolverManager* solvermanager, const std::string& rundirectory, const Alat::ParameterFile* parameterfile);
};

/*--------------------------------------------------------------------------*/
#endif
