#include  "Alat/interfacebase.h"
#include  "Alat/set.h"
#include  <cassert>
#include  <iostream>
#include  <stdlib.h>

using namespace Alat;

/*--------------------------------------------------------------------------*/
InterfaceBase::InterfaceBase(){}
InterfaceBase::InterfaceBase( const InterfaceBase& interfacebase){}
InterfaceBase& InterfaceBase::operator=( const InterfaceBase& interfacebase)
{
  return *this;
}

/*--------------------------------------------------------------------------*/
void InterfaceBase::_notWritten(std::string function) const
{
  std::cerr<<"*** ERROR in "<<getName()<<std::endl;
  std::cerr<<"\""<<getInterfaceName()<<"::"<<function<<"()\" not written !! ";
  std::cerr<<"(last class : \""<<getName()<<"\")\n";
  assert(0);
  exit(1);
}

/*--------------------------------------------------------------------------*/
void InterfaceBase::_message_string(std::string function, std::string message) const
{
  std::cerr<< "*** in "<< getName() << "::" << function<<"(): " << message  << "\n";
}

/*--------------------------------------------------------------------------*/
void InterfaceBase::_warning_string(std::string function, std::string message) const
{
  std::cerr<< "*** WARNING in "<< getName() << "::" << function<<"(): " << message  << "\n";
}

/*--------------------------------------------------------------------------*/
void InterfaceBase::_error_string(std::string function, std::string message) const
{
  std::cerr<< "*** ERROR in "<< getName() << "::" << function<<"(): " << message  << "\n";
  assert(0);
  exit(1);
}

/*--------------------------------------------------------------------------*/
void InterfaceBase::_error_string(std::string function, std::string message, std::string value) const
{
  std::cerr<< "*** ERROR in "<< getName() << "::" << function<<"(): "  << message << " \"" << value  << "\"\n";
  assert(0);
  exit(1);
}
void InterfaceBase::_error_string(std::string function, std::string message, int value) const
{
  std::cerr<< "*** ERROR in "<< getName() << "::" << function<<"(): " << message << " \"" << value  << "\"\n";
  assert(0);
  exit(1);
}
void InterfaceBase::_error_string(std::string function, std::string message, int value, int value2) const
{
  std::cerr<< "*** ERROR in "<< getName() << "::" << function<<"(): " << message << " \"" << value << " \" "<< " \"" << value2  << "\"\n";
  assert(0);
  exit(1);
}
void InterfaceBase::_error_string(std::string function, std::string message, const Alat::Set<std::string>& values) const
{
  std::cerr<< "*** ERROR in "<< getName() << "::" << function<<"(): " << message << " " << values  << "\n";
  assert(0);
  exit(1);
}
