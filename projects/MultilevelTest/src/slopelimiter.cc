#include  "slopelimiter.h"
#include  <cassert>

/*--------------------------------------------------------------------------*/
NoSlopeLimiter::~NoSlopeLimiter() {}
NoSlopeLimiter::NoSlopeLimiter(): SlopeLimiterInterface(){}
NoSlopeLimiter::NoSlopeLimiter( const NoSlopeLimiter& slopelimiter): SlopeLimiterInterface(slopelimiter){assert(0); (*this).operator=(slopelimiter);}
NoSlopeLimiter& NoSlopeLimiter::operator=( const NoSlopeLimiter& slopelimiter) {SlopeLimiterInterface::operator=(slopelimiter);assert(0);return *this;}
std::string NoSlopeLimiter::getName() const 
{
return "NoSlopeLimiter";
}
double NoSlopeLimiter::xi(double x, double y) const
{
  return 0.0;
}
double NoSlopeLimiter::dxidx(double x, double y) const
{
  return 0.0;
}
double NoSlopeLimiter::dxidy(double x, double y) const
{
  return 0.0;   
}

/*--------------------------------------------------------------------------*/
SlopeLimiterSS::~SlopeLimiterSS() {}
SlopeLimiterSS::SlopeLimiterSS(double c): SlopeLimiterInterface(), _c(c){}
SlopeLimiterSS::SlopeLimiterSS( const SlopeLimiterSS& slopelimiter): SlopeLimiterInterface(slopelimiter){assert(0); (*this).operator=(slopelimiter);}
SlopeLimiterSS& SlopeLimiterSS::operator=( const SlopeLimiterSS& slopelimiter) {SlopeLimiterInterface::operator=(slopelimiter);assert(0);return *this;}
std::string SlopeLimiterSS::getName() const 
{
return "SlopeLimiterSS";
}
double SlopeLimiterSS::xi(double x, double y) const
{
  if (x * y <= 0.0)
  {
    return x;    
  }
  else if(x / y > _c)
  {
    return x-_c*y;       
  }
  else
  {
    return 0.0;           
  }  
}
double SlopeLimiterSS::dxidx(double x, double y) const
{
  if ( x== 0.0 or x == _c*y)
  {
    return 1.0;    
  }
  else if (x * y <= 0.0)
  {
    return 1.0;    
  }
  else if( x / y >= _c)
  {
    return 1.0;    
  }
  else
  {
    return 0.0;     
  }
  
}
double SlopeLimiterSS::dxidy(double x, double y) const
{
  if( x == _c*y or y == 0.0)
  {
    return -_c;    
  }
  else if( x * y > 0.0 and x / y > _c)
  {
    return -_c;    
  }
  else
  {
    return 0.0;   
  }    
}
/*--------------------------------------------------------------------------*/
