#ifndef ___ShockCapturing_h
#define ___ShockCapturing_h

#include  "shockcapturinginterface.h"

/*--------------------------------------------------------------------------*/
class ShockCapturing : public ShockCapturingInterface
{
private:
  double _q;
  double f(double x) const;
  double df(double x) const;
  
public:
  ~ShockCapturing();
  ShockCapturing(double q=1.0);
  ShockCapturing( const ShockCapturing& shockcapturing);
  ShockCapturing& operator=( const ShockCapturing& shockcapturing);
  std::string getName() const;

  double phi(int npatch, const PatchData& pdata) const;
  void dphi(int npatch, const PatchData& pdata, arma::vec& dx, arma::vec& dy) const;
};

/*--------------------------------------------------------------------------*/
class ShockCapturingBadiaBonilla : public ShockCapturingInterface
{
private:
  double _gamma;
  double f(double x) const;
  double df(double x) const;
  
public:
  ~ShockCapturingBadiaBonilla();
  ShockCapturingBadiaBonilla(double gamma=1e-20);
  ShockCapturingBadiaBonilla( const ShockCapturingBadiaBonilla& shockcapturing);
  ShockCapturingBadiaBonilla& operator=( const ShockCapturingBadiaBonilla& shockcapturing);
  std::string getName() const;

  double phi(int npatch, const PatchData& pdata) const;
  void dphi(int npatch, const PatchData& pdata, arma::vec& dx, arma::vec& dy) const;
};

/*--------------------------------------------------------------------------*/
class NoShockCapturing : public ShockCapturingInterface
{
public:
  ~NoShockCapturing();
  NoShockCapturing();
  NoShockCapturing( const NoShockCapturing& shockcapturing);
  NoShockCapturing& operator=( const NoShockCapturing& shockcapturing);
  std::string getName() const;

  double phi(int npatch, const PatchData& pdata) const;
  void dphi(int npatch, const PatchData& pdata, arma::vec& dx, arma::vec& dy) const;
};

/*--------------------------------------------------------------------------*/
#endif
