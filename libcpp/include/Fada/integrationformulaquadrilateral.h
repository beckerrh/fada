#ifndef __Fada_IntegrationFormulaQuadrilateral_h
#define __Fada_IntegrationFormulaQuadrilateral_h

#include  "integrationformulabase.h"
#include  "integrationformulaline.h"
#include  <sstream>

/*------------------------------------------------------------*/

namespace Fada
{
  class PointGauss0 : public IntegrationFormulaBase<1>
  {
  public:  
    std::string getName() const;
    PointGauss0();
  };

  class QuadTrapez : public IntegrationFormulaBase<4>
  {
  public:  
    std::string getName() const;
    QuadTrapez();
  };  
  
  class QuadRotatedTrapez : public IntegrationFormulaBase<4>
  {
  public:  
    std::string getName() const;
    QuadRotatedTrapez();
  };

  class QuadFivePoint : public IntegrationFormulaBase<5>
  {
  public:  
    std::string getName() const;
    QuadFivePoint();
  };

  class QuadGauss_1_1 : public IntegrationFormulaBase<1>
  {
  public:  
    std::string getName() const;
    QuadGauss_1_1();
  };

  class QuadGauss_4_3 : public IntegrationFormulaBase<4>
  {
  public:  
    std::string getName() const;
    QuadGauss_4_3();
  };
  class QuadGauss_8_5 : public IntegrationFormulaBase<8>
  {
  public:  
    std::string getName() const;
    QuadGauss_8_5();
  };

  class QuadGauss_12_7 : public IntegrationFormulaBase<12>
  {
  public:  
    std::string getName() const;
    QuadGauss_12_7();
  };
  class QuadGauss_20_9 : public IntegrationFormulaBase<20>
  {
  public:  
    std::string getName() const;
    QuadGauss_20_9();
  };
  class QuadGauss_25_11 : public IntegrationFormulaBase<25>
  {
  public:  
    std::string getName() const;
    QuadGauss_25_11();
  };
  /*------------------------------------------------------------*/

  template<int N, class LineFormula>
  class TensorFormula2d : public IntegrationFormulaBase<N*N>
  {
  public:
    std::string getName() const;
    TensorFormula2d();
  };

  /*------------------------------------------------------------*/

  typedef TensorFormula2d<1, LineMidPoint> QuadMidPoint;
  // typedef TensorFormula2d<2, LineTrapez>   QuadTrapez;
  typedef TensorFormula2d<3, LineSimpson>  QuadSimpson;

  typedef TensorFormula2d<1, LineGauss1>   QuadGauss1;
  typedef TensorFormula2d<2, LineGauss2>   QuadGauss4;
  typedef TensorFormula2d<3, LineGauss3>   QuadGauss9;
  typedef TensorFormula2d<4, LineGauss4>   QuadGauss16;
  typedef TensorFormula2d<5, LineGauss5>   QuadGauss25;
  typedef TensorFormula2d<6, LineGauss6>   QuadGauss36;
  typedef TensorFormula2d<7, LineGauss7>   QuadGauss49;
  typedef TensorFormula2d<8, LineGauss8>   QuadGauss64;
  typedef TensorFormula2d<9, LineGauss9>   QuadGauss81;
  typedef TensorFormula2d<10, LineGauss10> QuadGauss100;
}

/*------------------------------------------------------------*/

#endif
