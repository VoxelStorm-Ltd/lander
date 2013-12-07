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
  Vector2d mouse_sensitivity;     // degrees per pixel moved for both horizontal and vertical mouse movement
  Vector2d mouse_last;            // mouse coords relative to origin
  Vector2i windowsize;            // set by glfw - modifying this directly changes nothing
  double fov_ratio;               // updated by update_fov_ratio() from degrees
  double aspect_ratio;            // updated by update_aspect_ratio() from windowsize
  double rotation_head_yaw;       // euler angles for head rotation - we want gimbal limitation here
  double rotation_head_pitch;
  //Quatd rotation_head;            // which way the astronaut is facing - updated by update_rotation_head()

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
