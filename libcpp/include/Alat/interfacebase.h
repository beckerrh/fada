#ifndef __Alat_InterfaceBase_h
#define __Alat_InterfaceBase_h

#include  <string>

/*--------------------------------------------------------------------------*/

namespace Alat
{
  template<class T>
  class Set;
}
namespace Alat
{
  class InterfaceBase
  {
protected:
    virtual std::string getInterfaceName() const = 0;
    void _notWritten(std::string function = "") const;
    void _message_string(std::string function, std::string message) const;
    void _warning_string(std::string function, std::string message) const;
    void _error_string(std::string function, std::string message) const;
    void _error_string(std::string function, std::string message, std::string value) const;
    void _error_string(std::string function, std::string message, int value) const;
    void _error_string(std::string function, std::string message, int value, int value1) const;
    void _error_string(std::string function, std::string message, const Alat::Set<std::string>& values) const;

public:
    virtual ~InterfaceBase() {}
    InterfaceBase();
    InterfaceBase( const InterfaceBase& interfacebase);
    InterfaceBase& operator=( const InterfaceBase& interfacebase);

    virtual std::string getName() const = 0;
  };
}

/*--------------------------------------------------------------------------*/

#endif
