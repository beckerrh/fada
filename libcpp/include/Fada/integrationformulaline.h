#ifndef __Fada_IntegrationFormula1d_h
#define __Fada_IntegrationFormula1d_h

#include  "integrationformulabase.h"

/*---------------------------------------------------------*/

namespace Fada
{
  class LineMidPoint : public IntegrationFormulaBase<1>
  {
public:  std::string getName() const
    {
      return "LineMidPoint";
    }

    LineMidPoint();
  };

  /*------------------------------------------------------------*/

  class LineTrapez : public IntegrationFormulaBase<2>
  {
public:  std::string getName() const
    {
      return "LineTrapez";
    }

    LineTrapez();
  };

  /*------------------------------------------------------------*/

  class LineSimpson : public IntegrationFormulaBase<3>
  {
public:  std::string getName() const
    {
      return "LineSimpson";
    }

    LineSimpson();
  };

  /*------------------------------------------------------------*/

  class LineVillarceau : public IntegrationFormulaBase<5>
  {
public:  std::string getName() const
    {
      return "LineVillarceau";
    }

    LineVillarceau();
  };

  /*------------------------------------------------------------*/

  class LineGauss1 : public IntegrationFormulaBase<1>
  {
public:  std::string getName() const
    {
      return "LineGauss1";
    }

    LineGauss1();
  };

  /*------------------------------------------------------------*/

  class LineGauss2 : public IntegrationFormulaBase<2>
  {
public:  std::string getName() const
    {
      return "LineGauss2";
    }

    LineGauss2();
  };

  /*------------------------------------------------------------*/

  class LineGauss3 : public IntegrationFormulaBase<3>
  {
public:  std::string getName() const
    {
      return "LineGauss3";
    }

    LineGauss3();
  };

  /*------------------------------------------------------------*/

  class LineGauss4 : public IntegrationFormulaBase<4>
  {
public:  std::string getName() const
    {
      return "LineGauss4";
    }

    LineGauss4();
  };

  /*------------------------------------------------------------*/

  class LineGauss5 : public IntegrationFormulaBase<5>
  {
public:  std::string getName() const
    {
      return "LineGauss5";
    }

    LineGauss5();
  };

  /*------------------------------------------------------------*/

  class LineGauss6 : public IntegrationFormulaBase<6>
  {
public:  std::string getName() const
    {
      return "LineGauss6";
    }

    LineGauss6();
  };

  /*------------------------------------------------------------*/

  class LineGauss7 : public IntegrationFormulaBase<7>
  {
public:  std::string getName() const
    {
      return "LineGauss7";
    }

    LineGauss7();
  };

  /*------------------------------------------------------------*/

  class LineGauss8 : public IntegrationFormulaBase<8>
  {
public:  std::string getName() const
    {
      return "LineGauss8";
    }

    LineGauss8();
  };

  /*------------------------------------------------------------*/

  class LineGauss9 : public IntegrationFormulaBase<9>
  {
public:  std::string getName() const
    {
      return "LineGauss9";
    }

    LineGauss9();
  };

  /*------------------------------------------------------------*/

  class LineGauss10 : public IntegrationFormulaBase<10>
  {
public:  std::string getName() const
    {
      return "LineGauss10";
    }

    LineGauss10();
  };
}

/*---------------------------------------------------------*/

#endif
