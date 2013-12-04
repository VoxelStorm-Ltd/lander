#include "display.h"

void display::render() {
  /// Render this display's contents in the right place
  // TODO: render to texture and show that texture in 3D

  // set up the matrix for drawing in the right location

  // check if we have an input signal
  if(ports_in[0].target) {
    // call the target's render function
    ports_in[0].target->get_port_out_video_analogue(ports_in[0].target_port);
  } else {
    // draw a "no signal" output
    // TODO
  }
}
