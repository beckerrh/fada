#ifndef __Fada_IntegrationFormulaTriangle_h
#define __Fada_IntegrationFormulaTriangle_h

#include  "integrationformulabase.h"

/*---------------------------------------------------------*/

namespace Fada
{
  class TriangleMidPoint : public IntegrationFormulaBase<1>
  {
  public:  
    std::string getName() const;
    TriangleMidPoint();
  };

  class TriangleTrapez : public IntegrationFormulaBase<3>
  {
  public:  
    std::string getName() const;
    TriangleTrapez();
  };

  class TriangleRotatedTrapez : public IntegrationFormulaBase<3>
  {
  public:  
    std::string getName() const;
    TriangleRotatedTrapez();
  };

  class TriangleSimpson : public IntegrationFormulaBase<7>
  {
  public:  
    std::string getName() const;
    TriangleSimpson();
  };

  class TriangleDegree4 : public IntegrationFormulaBase<7>
  {
  public:  
    std::string getName() const;
    TriangleDegree4();
  };

  class TriangleDegree6 : public IntegrationFormulaBase<12>
  {
  public:  
    std::string getName() const;
    TriangleDegree6();
  };

  class TriangleDegree8 : public IntegrationFormulaBase<19>
  {
  public:  
    std::string getName() const;
    TriangleDegree8();
  };
}

/*---------------------------------------------------------*/

#endif
