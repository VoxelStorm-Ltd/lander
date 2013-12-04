#include "instrument.h"

void instrument::render() {
  /// Show this instrument on the control panel
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
}
