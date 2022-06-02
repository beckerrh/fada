#include  "shockcapturing.h"
#include  <cassert>

/*--------------------------------------------------------------------------*/
NoShockCapturing::~NoShockCapturing() {}
NoShockCapturing::NoShockCapturing(): ShockCapturingInterface() {}
NoShockCapturing::NoShockCapturing( const NoShockCapturing& shockcapturing): ShockCapturingInterface(shockcapturing){assert(0); (*this).operator=(shockcapturing);}
NoShockCapturing& NoShockCapturing::operator=( const NoShockCapturing& shockcapturing) {ShockCapturingInterface::operator=(shockcapturing);assert(0);return *this;}
std::string NoShockCapturing::getName() const 
{
return "NoShockCapturing";
}
double NoShockCapturing::phi(int npatch, const PatchData& pdata) const
{
  return 1.0;
}
void NoShockCapturing::dphi(int npatch, const PatchData& pdata, arma::vec& dx, arma::vec& dy) const
{
  dx.fill(0.0);
  dy.fill(0.0);
}

/*--------------------------------------------------------------------------*/
ShockCapturing::~ShockCapturing() {}
ShockCapturing::ShockCapturing(double q): ShockCapturingInterface(), _q(q){}
ShockCapturing::ShockCapturing( const ShockCapturing& shockcapturing): ShockCapturingInterface(shockcapturing){assert(0); (*this).operator=(shockcapturing);}
ShockCapturing& ShockCapturing::operator=( const ShockCapturing& shockcapturing) {ShockCapturingInterface::operator=(shockcapturing);assert(0);return *this;}
std::string ShockCapturing::getName() const 
{
return "ShockCapturing";
}
double ShockCapturing::f(double x) const
{
  // return x;
  if(x>=1.0)
  {
    return 1.0;
  }
  else
  {
    return x*x + 2.0*x*x*x -2.0*x*x*x*x;
  }
}
double ShockCapturing::df(double x) const
{
  // return 1.0;
  if(x>=1.0)
  {
    return 0.0;
  }
  else
  {
    return 2.0*x + 6.0*x*x -8.0*x*x*x;
  }
}
double ShockCapturing::phi(int npatch, const PatchData& pdata) const
{
  // double tau = 0.0;
  // double sigma = 0.0;
  double tau = 1e-16;
  double sigma = 1e-16;
  for(int ip=0;ip<npatch;ip++)
  {
    tau += pdata.diff[ip]*fabs(pdata.grad[ip] - pdata.grad2[ip]);
    sigma += pdata.diff[ip]*(fabs(pdata.grad[ip]) + fabs(pdata.grad2[ip]));
  }
  if(sigma)
  {
    return f(tau/sigma);
  }
  return 0.0;
}
void ShockCapturing::dphi(int npatch, const PatchData& pdata, arma::vec& dx, arma::vec& dy) const
{
  // double tau = 0.0;
  // double sigma = 0.0;
  double tau = 1e-16;
  double sigma = 1e-16;
  for(int ip=0;ip<npatch;ip++)
  {
    tau += pdata.diff[ip]*fabs(pdata.grad[ip] - pdata.grad2[ip]);
    sigma += pdata.diff[ip]*(fabs(pdata.grad[ip]) + fabs(pdata.grad2[ip]));
  }
  if(sigma==0.0)
  {
    dx.fill(0.0);
    dy.fill(0.0);
    return;
  }
  double toto = df(tau/sigma)/f(tau/sigma)/sigma/sigma;
  for(int ip=0;ip<npatch;ip++)
  {
    double g = pdata.grad[ip];
    double g2 = pdata.grad2[ip];
    double dtaux = 0.0;
    double dtauy = 0.0;
    double dsigmax = 1.0;
    double dsigmay = 0.0;
    if(g > g2)
    {
      dtaux = 1.0;
      dtauy = -1.0;      
    }
    else if(g < g2)
    {
      dtaux = -1.0;
      dtauy = 1.0;      
    }
    if(g>0.0)
    {
      dsigmax = 1.0;      
    }
    else if(g<0.0)
    {
      dsigmax = -1.0;            
    }
    if(g2>0.0)
    {
      dsigmay = 1.0;      
    }
    else if(g2<0.0)
    {
      dsigmay = -1.0;            
    }
    // dx[ip] = pdata.diff[ip]*(dtaux/tau - dsigmax/sigma);
    // dy[ip] = pdata.diff[ip]*(dtauy/tau - dsigmay/sigma);
    dx[ip] = toto*pdata.diff[ip]*(dtaux*sigma - dsigmax*tau);
    dy[ip] = toto*pdata.diff[ip]*(dtauy*sigma - dsigmay*tau);    
  }
}

/*--------------------------------------------------------------------------*/
ShockCapturingBadiaBonilla::~ShockCapturingBadiaBonilla() {}
ShockCapturingBadiaBonilla::ShockCapturingBadiaBonilla(double gamma): ShockCapturingInterface(), _gamma(gamma){}
ShockCapturingBadiaBonilla::ShockCapturingBadiaBonilla( const ShockCapturingBadiaBonilla& shockcapturing): ShockCapturingInterface(shockcapturing){assert(0); (*this).operator=(shockcapturing);}
ShockCapturingBadiaBonilla& ShockCapturingBadiaBonilla::operator=( const ShockCapturingBadiaBonilla& shockcapturing) {ShockCapturingInterface::operator=(shockcapturing);assert(0);return *this;}
std::string ShockCapturingBadiaBonilla::getName() const 
{
  return "ShockCapturingBadiaBonilla";
}
double ShockCapturingBadiaBonilla::f(double x) const
{
  if(x>=1.0)
  {
    return 1.0;
  }
  else
  {
    // return x+x*x-x*x*x;
    return x*x + 2.0*x*x*x -2.0*x*x*x*x;
    return 2.0*x*x*x*x - 5.0*x*x*x + 3.0*x*x + x;
  }
}
double ShockCapturingBadiaBonilla::df(double x) const
{
  if(x>=1.0)
  {
    return 0.0;
  }
  else
  {
    // return 1.0+2.0*x-3.0*x*x;
    return 2.0*x + 6.0*x*x -8.0*x*x*x;
    return 8.0*x*x*x - 15.0*x*x + 6.0*x + 1.0;
  }
}

double ShockCapturingBadiaBonilla::phi(int npatch, const PatchData& pdata) const
{
  double tau = 0.0;
  double sigma = 0.0;
  for(int ip=0;ip<npatch;ip++)
  {
    tau += pdata.diff[ip]*fabs(pdata.grad[ip] - pdata.grad2[ip]);
    sigma += pdata.diff[ip]*(fabs(pdata.grad[ip]) + fabs(pdata.grad2[ip]));
  }
  return f(tau+_gamma/sigma+_gamma);
}
void ShockCapturingBadiaBonilla::dphi(int npatch, const PatchData& pdata, arma::vec& dx, arma::vec& dy) const
{
  double tau = 0.0;
  double sigma = 0.0;
  for(int ip=0;ip<npatch;ip++)
  {
    tau += pdata.diff[ip]*fabs(pdata.grad[ip] - pdata.grad2[ip]);
    sigma += pdata.diff[ip]*(fabs(pdata.grad[ip]) + fabs(pdata.grad2[ip]));
  }
  tau += _gamma;
  sigma += _gamma;
  for(int ip=0;ip<npatch;ip++)
  {
    double g = pdata.grad[ip];
    double g2 = pdata.grad2[ip];
    double dtaux = 0.0;
    double dtauy = 0.0;
    double dsigmax = 0.0;
    double dsigmay = 0.0;
    if(g > g2)
    {
      dtaux = 1.0;
      dtauy = -1.0;      
    }
    else if(g < g2)
    {
      dtaux = -1.0;
      dtauy = 1.0;      
    }
    if(g>0.0)
    {
      dsigmax = 1.0;      
    }
    else if(g<0.0)
    {
      dsigmax = -1.0;            
    }
    if(g2>0.0)
    {
      dsigmay = 1.0;      
    }
    else if(g2<0.0)
    {
      dsigmay = -1.0;            
    }
    dx[ip] = pdata.diff[ip]*( (sigma*dtaux - tau*dsigmax)/sigma/sigma)*df(tau/sigma)/f(tau/sigma);
    dy[ip] = pdata.diff[ip]*( (sigma*dtauy - tau*dsigmay)/sigma/sigma)*df(tau/sigma)/f(tau/sigma);    
  }
}
