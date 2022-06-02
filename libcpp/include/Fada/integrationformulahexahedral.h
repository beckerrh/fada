#ifndef __Fada_IntegrationFormulaHexahedral_h
#define __Fada_IntegrationFormulaHexahedral_h

#include  "integrationformulabase.h"
#include  "integrationformulaline.h"
#include  <sstream>

/*------------------------------------------------------------*/

namespace Fada
{
  class HexTrapez : public IntegrationFormulaBase<8>
  {
public:  std::string getName() const
    {
      return "HexTrapez";
    }

    HexTrapez();
  };

  class HexGauss3D_1_0 : public IntegrationFormulaBase<1>
  {
public:
    HexGauss3D_1_0();
    std::string getName() const
    {
      return "HexGauss3D_1_0";
    }
  };

  class HexGauss3D_6_2 : public IntegrationFormulaBase<6>
  {
public:
    HexGauss3D_6_2();
    std::string getName() const
    {
      return "HexGauss3D_6_2";
    }
  };

  class HexGauss3D_14_4 : public IntegrationFormulaBase<14>
  {
public:
    HexGauss3D_14_4();
    std::string getName() const
    {
      return "HexGauss3D_14_4";
    }
  };

  template<int N, class LineFormula>
  class TensorFormula3d : public IntegrationFormulaBase<N*N*N>
  {
public:
    TensorFormula3d();
    std::string getName() const
    {
      std::stringstream ss;
      LineFormula L;
      ss<<"TensorFormula3d<"<<L.getName()<<">";
      return ss.str();
    }
  };

/*------------------------------------------------------------*/

/*------------------------------------------------------------*/

  // typedef TensorFormula3d<2, LineTrapez> HexTrapez;
  typedef TensorFormula3d<1, LineGauss1> HexGauss1;
  typedef TensorFormula3d<2, LineGauss2> HexGauss8;
  typedef TensorFormula3d<3, LineGauss3> HexGauss27;
  typedef TensorFormula3d<4, LineGauss4> HexGauss64;
  typedef TensorFormula3d<5, LineGauss5> HexGauss125;
}

/*------------------------------------------------------------*/

#endif
