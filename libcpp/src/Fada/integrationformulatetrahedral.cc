#include  "Fada/integrationformulatetrahedral.h"

using namespace Fada;

/*--------------------------------------------------------------------------*/
TetrahedralDegree1::TetrahedralDegree1() : IntegrationFormulaBase<1>()
{
  w(0) = 0.1666666666666666666666667;
  c(0).x() = 0.25;
  c(0).y() = 0.25;
  c(0).z() = 0.25;
}
std::string TetrahedralDegree1::getName() const
{
  return "TetrahedralDegree1";
}

/*--------------------------------------------------------------------------*/
TetrahedralDegree2::TetrahedralDegree2() : IntegrationFormulaBase<4>()
{
  w(0) = 0.0416666666666666666666667;
  w(1) = w(0);
  w(2) = w(0);
  w(3) = w(0);

  const double a = 0.585410196624969;
  const double b = 0.138196601125011;

  c(0).x() = a;
  c(0).y() = b;
  c(0).z() = b;
  c(1).x() = b;
  c(1).y() = a;
  c(1).z() = b;
  c(2).x() = b;
  c(2).y() = b;
  c(2).z() = a;
  c(3).x() = b;
  c(3).y() = b;
  c(3).z() = b;
}
std::string TetrahedralDegree2::getName() const
{
  return "TetrahedralDegree2";
}

/*--------------------------------------------------------------------------*/
TetrahedralDegree3::TetrahedralDegree3() : IntegrationFormulaBase<5>()
{
  w(0) = -0.1333333333333333333333333;
  w(1) = 0.075;
  w(2) = w(1);
  w(3) = w(1);
  w(4) = w(1);
  const double a = 0.1666666666666666666666667;
  c(0).x() = 0.25;
  c(0).y() = 0.25;
  c(0).z() = 0.25;
  c(1).x() = 0.5;
  c(1).y() = a;
  c(1).z() = a;
  c(2).x() = a;
  c(2).y() = 0.5;
  c(2).z() = a;
  c(3).x() = a;
  c(3).y() = a;
  c(3).z() = 0.5;
  c(4).x() = a;
  c(4).y() = a;
  c(4).z() = a;
}
std::string TetrahedralDegree3::getName() const
{
  return "TetrahedralDegree3";
}

/*--------------------------------------------------------------------------*/
TetrahedralDegree4::TetrahedralDegree4() : IntegrationFormulaBase<11>()
{
  w(0) = -0.0131555555555555555555556;
  w(1) = 0.0076222222222222222222222;
  w(2) = w(1);
  w(3) = w(1);
  w(4) = w(1);
  w(5) = 0.0248888888888888888888889;
  w(6) = w(5);
  w(7) = w(5);
  w(8) = w(5);
  w(9) = w(5);
  w(10) = w(5);

  const double a = 0.785714285714286;
  const double b = 0.071428571428571;
  const double k = 0.399403576166799;
  const double d = 0.100596423833201;

  c(0).x() = 0.25;
  c(0).y() = 0.25;
  c(0).z() = 0.25;
  c(1).x() = a;
  c(1).y() = b;
  c(1).z() = b;
  c(2).x() = b;
  c(2).y() = a;
  c(2).z() = b;
  c(3).x() = b;
  c(3).y() = b;
  c(3).z() = a;
  c(4).x() = b;
  c(4).y() = b;
  c(4).z() = b;
  c(5).x() = d;
  c(5).y() = k;
  c(5).z() = k;
  c(6).x() = k;
  c(6).y() = k;
  c(6).z() = d;
  c(7).x() = k;
  c(7).y() = d;
  c(7).z() = d;
  c(8).x() = d;
  c(8).y() = k;
  c(8).z() = d;
  c(9).x() = d;
  c(9).y() = d;
  c(9).z() = k;
  c(10).x() = k;
  c(10).y() = d;
  c(10).z() = k;
}
std::string TetrahedralDegree4::getName() const
{
  return "TetrahedralDegree4";
}

/*--------------------------------------------------------------------------*/
TetrahedralDegree5::TetrahedralDegree5() : IntegrationFormulaBase<15>()
{
  w(0) = 0.030283678097089;
  w(1) = 0.006026785714286;
  w(2) = w(1);
  w(3) = w(1);
  w(4) = w(1);
  w(5) = 0.011645249086029;
  w(6) = w(5);
  w(7) = w(5);
  w(8) = w(5);
  w(9) = 0.010949141561386;
  w(10) = w(9);
  w(11) = w(9);
  w(12) = w(9);
  w(13) = w(9);
  w(14) = w(9);

  const double a = 0.;
  const double b = 0.3333333333333333333333333;
  const double k = 0.7272727272727272727272727;
  const double d = 0.0909090909090909090909091;
  const double e = 0.066550153573664;
  const double f = 0.433449846426336;

  c(0).x() =  0.25;
  c(0).y() =  0.25;
  c(0).z() =  0.25;
  c(1).x() = a;
  c(1).y() = b;
  c(1).z() = b;
  c(2).x() = b;
  c(2).y() = a;
  c(2).z() = b;
  c(3).x() = b;
  c(3).y() = b;
  c(3).z() = a;
  c(4).x() = b;
  c(4).y() = b;
  c(4).z() = b;
  c(5).x() = k;
  c(5).y() = d;
  c(5).z() = d;
  c(6).x() = d;
  c(6).y() = k;
  c(6).z() = d;
  c(7).x() = d;
  c(7).y() = d;
  c(7).z() = k;
  c(8).x() = d;
  c(8).y() = d;
  c(8).z() = d;
  c(9).x() = f;
  c(9).y() = e;
  c(9).z() = e;
  c(10).x() = e;
  c(10).y() = e;
  c(10).z() = f;
  c(11).x() = e;
  c(11).y() = f;
  c(11).z() = f;
  c(12).x() = f;
  c(12).y() = e;
  c(12).z() = f;
  c(13).x() = f;
  c(13).y() = f;
  c(13).z() = e;
  c(14).x() = e;
  c(14).y() = f;
  c(14).z() = e;
}
std::string TetrahedralDegree5::getName() const
{
  return "TetrahedralDegree5";
}
