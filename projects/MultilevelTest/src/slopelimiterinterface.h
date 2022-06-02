#ifndef ___SlopeLimiterInterface_h
#define ___SlopeLimiterInterface_h

#include  "Alat/interfacebase.h"

/*--------------------------------------------------------------------------*/
class SlopeLimiterInterface : public Alat::InterfaceBase
{
public:
  ~SlopeLimiterInterface();
  SlopeLimiterInterface();
  SlopeLimiterInterface( const SlopeLimiterInterface& slopelimiterinterface);
  SlopeLimiterInterface& operator=( const SlopeLimiterInterface& slopelimiterinterface);
  std::string getName() const;
  std::string getInterfaceName() const;

  virtual double xi(double x, double y) const=0;
  virtual double dxidx(double x, double y) const=0;
  virtual double dxidy(double x, double y) const=0;
};

/*--------------------------------------------------------------------------*/
#endif
