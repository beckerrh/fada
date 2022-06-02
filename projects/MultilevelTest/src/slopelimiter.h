#ifndef ___SlopeLimiter_h
#define ___SlopeLimiter_h

#include  "slopelimiterinterface.h"

/*--------------------------------------------------------------------------*/
class SlopeLimiterSS : public SlopeLimiterInterface
{
private:
  double _c;
  
public:
  ~SlopeLimiterSS();
  SlopeLimiterSS(double c=2.0);
  SlopeLimiterSS( const SlopeLimiterSS& slopelimiter);
  SlopeLimiterSS& operator=( const SlopeLimiterSS& slopelimiter);
  std::string getName() const;

  double xi(double x, double y) const;
  double dxidx(double x, double y) const;
  double dxidy(double x, double y) const;
};

/*--------------------------------------------------------------------------*/
class NoSlopeLimiter: public SlopeLimiterInterface
{
public:
  ~NoSlopeLimiter();
  NoSlopeLimiter();
  NoSlopeLimiter( const NoSlopeLimiter& slopelimiter);
  NoSlopeLimiter& operator=( const NoSlopeLimiter& slopelimiter);
  std::string getName() const;

  double xi(double x, double y) const;
  double dxidx(double x, double y) const;
  double dxidy(double x, double y) const;
};


/*--------------------------------------------------------------------------*/
#endif
