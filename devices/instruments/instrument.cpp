#include "instrument.h"
#include "instrumentpanel.h"

instrument::instrument() {
  /// Default constructor
}

instrument::~instrument() {
  /// Default destructor
}

bool instrument::attach_cabin(instrumentpanel *to_panel) {
  // this must be mounted on the instrument panel only
  return false;
}

bool instrument::attach_hull() {
  // this must be mounted on the instrument panel only
  return false;
}
