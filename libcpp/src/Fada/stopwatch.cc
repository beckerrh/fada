#include  "Fada/stopwatch.h"
#include  <cassert>

using namespace Fada;

/*--------------------------------------------------------------------------*/

StopWatch::~StopWatch() {}

/*--------------------------------------------------------------------------*/

StopWatch::StopWatch()
{
  reset();
}

/*--------------------------------------------------------------------------*/

StopWatch::StopWatch(const StopWatch& S) : _running(S._running), _last_time(S._last_time), _total(S._total)
{}

/*--------------------------------------------------------------------------*/

StopWatch& StopWatch::operator=( const StopWatch& S )
{
  _running = S._running;
  _last_time  = S._last_time;
  _total = S._total;
  return *this;
}

/*--------------------------------------------------------------------------*/

double StopWatch::seconds(void)
{
  static const double secs_per_tick = 1.0 / CLOCKS_PER_SEC;
  return ( (double) clock() ) * secs_per_tick;
}

/*--------------------------------------------------------------------------*/

bool StopWatch::running()
{
  return _running;
}

/*--------------------------------------------------------------------------*/

void StopWatch::reset()
{
  _running = false;
  _last_time = 0.0;
  _total = 0.0;
}

/*--------------------------------------------------------------------------*/

void StopWatch::start()
{
  if(!_running)
  {
    _last_time = seconds();
    _running = true;
  }
}

/*--------------------------------------------------------------------------*/

double StopWatch::stop()
{
  if(_running)
  {
    _total += seconds() - _last_time;
    _running = false;
  }
  return _total;
}

/*--------------------------------------------------------------------------*/

double StopWatch::read() const
{
  return _total;
}
