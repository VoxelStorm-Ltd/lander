#include "instrument.h"
#include "spacecraft.h"
#include "instrumentpanel.h"

instrument::instrument() {
  /// Default constructor
}

instrument::~instrument() {
  /// Default destructor
}

void instrument::attach(spacecraft *to_vessel) {
  /// Attach this device to the specified ship
  if(!to_vessel) {
    std::cout << "ERROR: tried to attach instrument " << get_name() << " to null vessel." << std::endl;
    return;
  }
  vessel = to_vessel;
  vessel->devices.push_back(this);
  // don't attach it to any panel by default
}

void instrument::attach(instrumentpanel *to_panel) {
  /// Attach this device to the specified panel
  if(!to_panel) {
    std::cout << "ERROR: tried to attach instrument " << get_name() << " to null panel." << std::endl;
    return;
  }
  panel = to_panel;
  panel->instruments.push_back(this);
}

void instrument::remove() {
  /// Remove this device from whatever ship it's attached to
  if(!vessel) {
    std::cout << "ERROR: tried to remove instrument " << get_name() << " which is already not attached to anything." << std::endl;
    return;
  }
  // detach it from whatever panel it's connected to
  remove_panel();
  // remove it from the list of the vessel's devices
  //vessel->devices.erase(std::remove(vessel->devices.begin(), vessel->devices.end(), this), vessel->devices.end());
  //vessel->instruments.erase(std::remove(vessel->instruments.begin(), vessel->devices.end(), this), vessel->instruments.end());
  vessel->devices.remove(this);
  // make sure there are no dangling connections
  for(auto const &it : vessel->devices) {
    for(unsigned int i = 0; i != it->get_port_in_count(); ++i) {
      if(it->ports_in[i].target == this) {
        it->disconnect(i);
      }
    }
  }
  vessel = nullptr;     // this must obviously come last
}

void instrument::remove_panel() {
  /// Remove this instrument from its panel only (leave it attached to any ship)
  if(!vessel) {
    //std::cout << "ERROR: tried to remove instrument " << get_name() << " which is already not attached to a panel." << std::endl;
    return;
  }
  panel->instruments.remove(this);
  panel = nullptr;     // this must obviously come last
}

//void instrument::update() {
//  /// Update the readings on this instrument and re-cache new values
//}
