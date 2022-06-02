#ifndef ___ShockCapturingInterface_h
#define ___ShockCapturingInterface_h

#include  "Alat/interfacebase.h"
#include  "patchdata.h"

/*--------------------------------------------------------------------------*/
class ShockCapturingInterface : public Alat::InterfaceBase
{
public:
  ~ShockCapturingInterface();
  ShockCapturingInterface();
  ShockCapturingInterface( const ShockCapturingInterface& shockcapturing);
  ShockCapturingInterface& operator=( const ShockCapturingInterface& shockcapturing);
  std::string getName() const;
  std::string getInterfaceName() const;

  virtual double phi(int npatch, const PatchData& pdata) const=0;
  virtual void dphi(int npatch, const PatchData& pdata, arma::vec& dx, arma::vec& dy) const=0;
};

/*--------------------------------------------------------------------------*/
#endif
