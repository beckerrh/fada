#ifndef __Fada_chronometer_traits_h
#define __Fada_chronometer_traits_h

#include  <ostream>

/*---------------------------------------------------------*/

namespace Fada
{
  template < class T >
  struct chronometer_traits {
    static void init()
    {
      T::initChronometer();
    }

    static void print(std::ostream& os)
    {
      T::printChronometer(os);
    }
  };
}

/*---------------------------------------------------------*/

#endif
