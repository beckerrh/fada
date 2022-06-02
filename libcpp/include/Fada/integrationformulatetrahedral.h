#ifndef __Fada_IntegrationFormulaTetrahedral_h
#define __Fada_IntegrationFormulaTetrahedral_h

#include  "integrationformulabase.h"

/*------------------------------------------------------------*/

namespace Fada
{
  class TetrahedralDegree1 : public IntegrationFormulaBase<1>
  {
  public:  
    std::string getName() const;
    TetrahedralDegree1();
  };
  class TetrahedralDegree2 : public IntegrationFormulaBase<4>
  {
  public:  
    std::string getName() const;
    TetrahedralDegree2();
  };
  class TetrahedralDegree3 : public IntegrationFormulaBase<5>
  {
  public:  
    std::string getName() const;
    TetrahedralDegree3();
  };
  class TetrahedralDegree4 : public IntegrationFormulaBase<11>
  {
  public:  
    std::string getName() const;
    TetrahedralDegree4();
  };
  class TetrahedralDegree5 : public IntegrationFormulaBase<15>
  {
  public:  
    std::string getName() const;
    TetrahedralDegree5();
  };
}

/*---------------------------------------------------------*/

#endif
