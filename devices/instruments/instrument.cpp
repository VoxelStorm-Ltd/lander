#include "instrument.h"
#include "instrumentpanel.h"

instrument::instrument() {
  /// Default constructor
}

instrument::~instrument() {
  /// Default destructor
}

std::string instrument::get_type() {
  /// Return the type of object this is
  return "device (instrument)";
}

bool instrument::attach_cabin(instrumentpanel *to_panel __attribute__((__unused__))) {
  // this must be mounted on the instrument panel only
  return false;
}

bool instrument::attach_hull() {
  // this must be mounted on the instrument panel only
  return false;
}
