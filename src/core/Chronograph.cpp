#include <core/Chronograph.hpp>

void Chronograph::stop() {
  auto span = Chronoclock::now() - _start;
  _total += (double)std::chrono::duration_cast<std::chrono::nanoseconds>(span).count();
  start(); // for lap-like functionality
}

