#ifndef CONTROL_H_INCLUDED
#define CONTROL_H_INCLUDED

#include <vector>
#include <list>
#include "instrument.h"

class control : public instrument {
  /// Base class for any controls that are bindable to buttons or keys
private:
  double sensitivity;
  double deadzone;
  double range;

  // maximum buttons - these numbers are arbitrary, and may need to be changed later
  static constexpr unsigned int maxkeys         = 512;
  static constexpr unsigned int maxmousebuttons = 16;
  static constexpr unsigned int maxjoybuttons   = 64;

protected:
  control();       // prevent this class being instantiated directly
public:
  enum class boundtype : char {
    UNBOUND = 0,          // not bound to anything
    KEYBOARD,             // bound to a keyboard key
    MOUSEBUTTON,          // bound to a mouse button
    MOUSEAXIS_X,          // bound to a mouse axis
    MOUSEAXIS_Y,          // bound to a mouse axis
    MOUSEWHEEL,           // bound to the mouse wheel
    MOUSEWHEEL_X,         // bound to the mouse wheel, horizontal
    JOYBUTTON,            // bound to a joystick button
    JOYAXIS               // bound to a joystick axis
  };

  boundtype boundto;

  typedef std::list<control*> boundlist;
  static std::vector<boundlist> keys;            // control devices bound to keys
  static std::vector<boundlist> mousebuttons;    // control devices bound to mouse buttons
  static std::vector<boundlist> joybuttons;      // control devices bound to joystick buttons

  virtual ~control();

  void unbind();

  void bind_key(unsigned int key);
  void bind_mousebutton(unsigned int button);
  void bind_mouseaxis_x(double sensitivity);
  void bind_mouseaxis_y(double sensitivity);
  void bind_mousewheel(double sensitivity);
  void bind_mousewheel_x(double sensitivity);
  void bind_joybutton(unsigned int button);
  void bind_joyaxis(unsigned int axis, double sensitivity, double deadzone, double range);

  static void test_key(unsigned int key);
  static void test_mousebutton(unsigned int button);
  static void test_mouseaxis_x(int value);
  static void test_mouseaxis_y(int value);
  static void test_mousewheel(int value);
  static void test_mousewheel_x(int value);
  static void test_joybutton(unsigned int button);
  static void test_joyaxis(unsigned int axis, double value);
};

#endif // CONTROL_H_INCLUDED
