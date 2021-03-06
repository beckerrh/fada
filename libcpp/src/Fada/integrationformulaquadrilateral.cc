#include  "Fada/integrationformulaquadrilateral.h"

using namespace Fada;

/*--------------------------------------------------------------------------*/
std::string PointGauss0::getName() const
{
  return "PointGauss0";
}
PointGauss0::PointGauss0() : IntegrationFormulaBase<1>()
{
  w(0) = 1.;
  c(0).x() = 1.0;
}

/*--------------------------------------------------------------------------*/
std::string QuadTrapez::getName() const
{
  return "QuadTrapez";
}
QuadTrapez::QuadTrapez() : IntegrationFormulaBase<4>()
{
  w(0) = 1.0;
  w(1) = 1.0;
  w(2) = 1.0;
  w(3) = 1.0;

  c(0).x() = -1.0;
  c(0).y() = -1.0;
  c(1).x() = 1.0;
  c(1).y() = -1.0;
  c(2).x() = 1.0;
  c(2).y() = 1.0;
  c(3).x() = -1.0;
  c(3).y() = 1.0;
}

/*--------------------------------------------------------------------------*/
std::string QuadRotatedTrapez::getName() const
{
  return "QuadRotatedTrapez";
}
QuadRotatedTrapez::QuadRotatedTrapez() : IntegrationFormulaBase<4>()
{
  w(0) = 1.0;
  w(1) = 1.0;
  w(2) = 1.0;
  w(3) = 1.0;

  c(0).x() = 0.0;
  c(0).y() = -1.0;
  c(1).x() = 1.0;
  c(1).y() = 0.0;
  c(2).x() = 0.0;
  c(2).y() = 1.0;
  c(3).x() = -1.0;
  c(3).y() = 0.0;
}

/*--------------------------------------------------------------------------*/
std::string QuadFivePoint::getName() const
{
  return "QuadFivePoint";
}
QuadFivePoint::QuadFivePoint() : IntegrationFormulaBase<5>()
{
  w(0) = ( 2.0 )/( 3.0 );
  w(1) = ( 2.0 )/( 3.0 );
  w(2) = ( 2.0 )/( 3.0 );
  w(3) = ( 2.0 )/( 3.0 );
  w(4) = ( 4.0 )/( 3.0 );

  c(0).x() = 0.0;
  c(0).y() = -1.0;
  c(1).x() = 1.0;
  c(1).y() = 0.0;
  c(2).x() = 0.0;
  c(2).y() = 1.0;
  c(3).x() = -1.0;
  c(3).y() = 0.0;
  c(4).x() = 0.0;
  c(4).y() = 0.0;
}

/*--------------------------------------------------------------------------*/
std::string QuadGauss_1_1::getName() const
{
  return "QuadGauss_1_1";
}
QuadGauss_1_1::QuadGauss_1_1() : IntegrationFormulaBase<1>()
{
  w(0) = 4;

  c(0).x() = 0.0;
  c(0).y() = 0.0;
  c(0).z() = 0.0;
}
/*--------------------------------------------------------------------------*/
std::string QuadGauss_4_3::getName() const
{
  return "QuadGauss_4_3";
}
QuadGauss_4_3::QuadGauss_4_3() : IntegrationFormulaBase<4>()
{
  c(0).x() = 0.577350269189626;
  c(0).y() = 0.577350269189626;
  w(0) = 1.000000000000000;
  c(1).x() = 0.577350269189626;
  c(1).y() = -0.577350269189626;
  w(1) = 1.000000000000000;
  c(2).x() = -0.577350269189626;
  c(2).y() = 0.577350269189626;
  w(2) = 1.000000000000000;
  c(3).x() = -0.577350269189626;
  c(3).y() = -0.577350269189626;
  w(3) = 1.000000000000000;
}
/*--------------------------------------------------------------------------*/
std::string QuadGauss_8_5::getName() const
{
  return "QuadGauss_8_5";
}
QuadGauss_8_5::QuadGauss_8_5() : IntegrationFormulaBase<8>()
{
  c(0).x() = 0.683130051063973;
  c(0).y() = 0.000000000000000;
  w(0) = 0.816326530612245;
  c(1).x() = -0.683130051063973;
  c(1).y() = 0.000000000000000;
  w(1) = 0.816326530612245;
  c(2).x() = 0.000000000000000;
  c(2).y() = 0.683130051063973;
  w(2) = 0.816326530612245;
  c(3).x() = 0.000000000000000;
  c(3).y() = -0.683130051063973;
  w(3) = 0.816326530612245;
  c(4).x() = 0.881917103688197;
  c(4).y() = 0.881917103688197;
  w(4) = 0.183673469387755;
  c(5).x() = 0.881917103688197;
  c(5).y() = -0.881917103688197;
  w(5) = 0.183673469387755;
  c(6).x() = -0.881917103688197;
  c(6).y() = 0.881917103688197;
  w(6) = 0.183673469387755;
  c(7).x() = -0.881917103688197;
  c(7).y() = -0.881917103688197;
  w(7) = 0.183673469387755;
}
/*--------------------------------------------------------------------------*/
std::string QuadGauss_12_7::getName() const
{
  return "QuadGauss_12_7";
}
QuadGauss_12_7::QuadGauss_12_7() : IntegrationFormulaBase<12>()
{
  c(0).x() = 0.925820099772551;
  c(0).y() = 0.000000000000000;
  w(0) = 0.241975308641975;
  c(1).x() = -0.925820099772551;
  c(1).y() = 0.000000000000000;
  w(1) = 0.241975308641975;
  c(2).x() = 0.000000000000000;
  c(2).y() = 0.925820099772551;
  w(2) = 0.241975308641975;
  c(3).x() = 0.000000000000000;
  c(3).y() = -0.925820099772551;
  w(3) = 0.241975308641975;
  c(4).x() = 0.805979782918599;
  c(4).y() = 0.805979782918599;
  w(4) = 0.237431774690630;
  c(5).x() = 0.805979782918599;
  c(5).y() = -0.805979782918599;
  w(5) = 0.237431774690630;
  c(6).x() = -0.805979782918599;
  c(6).y() = 0.805979782918599;
  w(6) = 0.237431774690630;
  c(7).x() = -0.805979782918599;
  c(7).y() = -0.805979782918599;
  w(7) = 0.237431774690630;
  c(8).x() = 0.380554433208316;
  c(8).y() = 0.380554433208316;
  w(8) = 0.520592916667394;
  c(9).x() = 0.380554433208316;
  c(9).y() = -0.380554433208316;
  w(9) = 0.520592916667394;
  c(10).x() = -0.380554433208316;
  c(10).y() = 0.380554433208316;
  w(10) = 0.520592916667394;
  c(11).x() = -0.380554433208316;
  c(11).y() = -0.380554433208316;
  w(11) = 0.520592916667394;
}
/*--------------------------------------------------------------------------*/
std::string QuadGauss_20_9::getName() const
{
  return "QuadGauss_20_9";
}
QuadGauss_20_9::QuadGauss_20_9() : IntegrationFormulaBase<20>()
{
  c(0).x() = 1.121225763866564;
  c(0).y() =  0.000000000000000;
  w(0) = 0.018475842507491;
  c(1).x() = -1.121225763866564;
  c(1).y() =  0.000000000000000;
  w(1) =  0.018475842507491;
  c(2).x() = 0.000000000000000;
  c(2).y() =  1.121225763866564;
  w(2) =  0.018475842507491;
  c(3).x() = 0.000000000000000;
  c(3).y() =  -1.121225763866564;
  w(3) =  0.018475842507491;
  c(4).x() = 0.451773049920657;
  c(4).y() =  0.000000000000000;
  w(4) =  0.390052939160735;
  c(5).x() = -0.451773049920657;
  c(5).y() =  0.000000000000000;
  w(5) =  0.390052939160735;
  c(6).x() = 0.000000000000000;
  c(6).y() =  0.451773049920657;
  w(6) =  0.390052939160735;
  c(7).x() = 0.000000000000000;
  c(7).y() =  -0.451773049920657;
  w(7) =  0.390052939160735;
  c(8).x() = 0.891849420851512;
  c(8).y() =  0.891849420851512;
  w(8) =  0.083095178026482;
  c(9).x() = 0.891849420851512;
  c(9).y() =  -0.891849420851512;
  w(9) =  0.083095178026482;
  c(10).x() = -0.891849420851512;
  c(10).y() =  0.891849420851512;
  w(10) =  0.083095178026482;
  c(11).x() = -0.891849420851512;
  c(11).y() =  -0.891849420851512;
  w(11) =  0.083095178026482;
  c(12).x() = 0.824396370749276;
  c(12).y() =  0.411623426336542;
  w(12) =  0.254188020152646;
  c(13).x() = 0.824396370749276;
  c(13).y() =  -0.411623426336542;
  w(13) =  0.254188020152646;
  c(14).x() = -0.824396370749276;
  c(14).y() =  0.411623426336542;
  w(14) =  0.254188020152646;
  c(15).x() = -0.824396370749276;
  c(15).y() =  -0.411623426336542;
  w(15) =  0.254188020152646;
  c(16).x() = 0.411623426336542;
  c(16).y() =  0.824396370749276;
  w(16) =  0.254188020152646;
  c(17).x() = 0.411623426336542;
  c(17).y() = -0.824396370749276;
  w(17) =  0.254188020152646;
  c(18).x() = -0.411623426336542;
  c(18).y() =  0.824396370749276;
  w(18) =  0.254188020152646;
  c(19).x() = -0.411623426336542;
  c(19).y() =  -0.824396370749276;
  w(19) =  0.254188020152646;
}
/*--------------------------------------------------------------------------*/
std::string QuadGauss_25_11::getName() const
{
  return "QuadGauss_25_11";
}
QuadGauss_25_11::QuadGauss_25_11() : IntegrationFormulaBase<25>()
{
  c(0).x() = 0.000000000000000;
  c(0).y() =  0.000000000000000;
  w(0) =  0.365379525585903;
  c(1).x() = 1.044402915409813;
  c(1).y() =  0.000000000000000;
  w(1) =  0.027756165564204;
  c(2).x() = -1.044402915409813;
  c(2).y() =  0.000000000000000;
  w(2) =  0.027756165564204;
  c(3).x() = 0.000000000000000;
  c(3).y() =  1.044402915409813;
  w(3) =  0.027756165564204;
  c(4).x() = 0.000000000000000;
  c(4).y() =  -1.044402915409813;
  w(4) =  0.027756165564204;
  c(5).x() = 0.769799068396649;
  c(5).y() =  0.000000000000000;
  w(5) =  0.244272057751754;
  c(6).x() = -0.769799068396649;
  c(6).y() =  0.000000000000000;
  w(6) =  0.244272057751754;
  c(7).x() = 0.000000000000000;
  c(7).y() =  0.769799068396649;
  w(7) =  0.244272057751754;
  c(8).x() = 0.000000000000000;
  c(8).y() =  -0.769799068396649;
  w(8) =  0.244272057751754;
  c(9).x() = 0.935787012440540;
  c(9).y() =  0.935787012440540;
  w(9) =  0.034265103851229;
  c(10).x() = 0.935787012440540;
  c(10).y() =  -0.935787012440540;
  w(10) =  0.034265103851229;
  c(11).x() = -0.935787012440540;
  c(11).y() =  0.935787012440540;
  w(11) =  0.034265103851229;
  c(12).x() = -0.935787012440540;
  c(12).y() =  -0.935787012440540;
  w(12) =  0.034265103851229;
  c(13).x() = 0.413491953449114;
  c(13).y() =  0.413491953449114;
  w(13) =  0.308993036133713;
  c(14).x() = 0.413491953449114;
  c(14).y() =  -0.413491953449114;
  w(14) =  0.308993036133713;
  c(15).x() = -0.413491953449114;
  c(15).y() =  0.413491953449114;
  w(15) =  0.308993036133713;
  c(16).x() = -0.413491953449114;
  c(16).y() =  -0.413491953449114;
  w(16) =  0.308993036133713;
  c(17).x() = 0.883025508525690;
  c(17).y() =  0.575653595840465;
  w(17) =  0.146684377651312;
  c(18).x() = 0.883025508525690;
  c(18).y() =  -0.575653595840465;
  w(18) =  0.146684377651312;
  c(19).x() = -0.883025508525690;
  c(19).y() =  0.575653595840465;
  w(19) =  0.146684377651312;
  c(20).x() = -0.883025508525690;
  c(20).y() =  -0.575653595840465;
  w(20) =  0.146684377651312;
  c(21).x() = 0.575653595840465;
  c(21).y() =  0.883025508525690;
  w(21) =  0.146684377651312;
  c(22).x() = 0.575653595840465;
  c(22).y() =  -0.883025508525690;
  w(22) =  0.146684377651312;
  c(23).x() = -0.575653595840465;
  c(23).y() =  0.883025508525690;
  w(23) =  0.146684377651312;
  c(14).x() = -0.575653595840465;
  c(24).y() =  -0.883025508525690;
  w(24) =  0.146684377651312;
}

/*--------------------------------------------------------------------------*/
template<int N, class LineFormula>
std::string TensorFormula2d<N, LineFormula>::getName() const
{
  std::stringstream ss;
  LineFormula L;
  ss<<"TensorFormula2d<"<<L.getName()<<">";
  return ss.str();
}

template<int N, class LineFormula>
TensorFormula2d<N, LineFormula>::TensorFormula2d() : IntegrationFormulaBase<N*N>()
{
  LineFormula L;

  assert(L.n() == N);

  for(int ix = 0; ix < N; ix++)
  {
    for(int iy = 0; iy < N; iy++)
    {
      int i = N*ix+iy;
      Alat::Node pi;
      L.point(pi, ix);
      IntegrationFormulaBase<N*N>::c(i).x() = 2.0*pi.x()-1.0;
      L.point(pi, iy);
      IntegrationFormulaBase<N*N>::c(i).y() = 2.0*pi.x()-1.0;
      IntegrationFormulaBase<N*N>::w(i) = 4.0*L.weight(ix)*L.weight(iy);
      IntegrationFormulaBase<N*N>::c(i).z() = 0.0;
    }
  }
}
/*--------------------------------------------------------------------------*/
template class Fada::TensorFormula2d<1,LineGauss1>;
template class Fada::TensorFormula2d<2,LineGauss2>;
template class Fada::TensorFormula2d<3,LineGauss3>;
template class Fada::TensorFormula2d<4,LineGauss4>;
template class Fada::TensorFormula2d<5,LineGauss5>;
template class Fada::TensorFormula2d<6,LineGauss6>;
template class Fada::TensorFormula2d<7,LineGauss7>;
template class Fada::TensorFormula2d<8,LineGauss8>;
template class Fada::TensorFormula2d<9,LineGauss9>;
template class Fada::TensorFormula2d<10,LineGauss10>;
