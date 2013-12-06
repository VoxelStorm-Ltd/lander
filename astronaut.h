#ifndef ASTRONAUT_H_INCLUDED
#define ASTRONAUT_H_INCLUDED

#include "physicsbody.h"

class spacecraft;
class planet;

class astronaut : public physicsbody {
  /// The little men and women who go whizzing around the moon
public:
  enum class statetype : char {
    INACTIVE    = 0,              // "just ignore me"
    IN_VESSEL   = 1,              // normal state - inside a spacecraft, position and velocity are relative to cockpit
    EVA         = 2,              // outside a spacecraft, not in atmosphere
    ATMOSPHERIC = 3,              // outside a spacecraft, in an atmosphere
    SURFACE     = 4,              // outside a spacecraft, walking around on a planet
    DEAD        = 5               // has ceased to exist in this universe
  };
  statetype state;
  spacecraft *vessel_in;
  planet *walking_on;

  // first person graphics settings
  Vector2i windowsize;
  double fov_ratio;
  double aspect_ratio;

  astronaut();
  ~astronaut();

  // getters and setters
  double get_mass();
  double get_radius();
  // state changes
  void enter_ship(spacecraft *ship);
  void exit_ship();
  void kill();

  // render
  void render_diagram(double scale, bool labels = false);
  void render_visible();
  void render_radio();
  void render_infrared();
  void render_ultraviolet();
  // first person render setup
  void render_firstperson();
  void update_fov_ratio(double fov = 90);
  void update_window(Vector2i newwindowsize);
  void update_aspect_ratio();
  void setup_render_perspective(double nearplane = 0.1, double farplane = 20.0);
};

#endif // ASTRONAUT_H_INCLUDED
