#ifndef STAR_H_INCLUDED
#define STAR_H_INCLUDED

#include "body.h"

class star : public body {
  /// A star or stellar object large enough to be undergoing fusion
public:
  double brightness;                                                            // absolute brightness locally

  star();
  virtual ~star();

  // getters and setters
  virtual std::string get_type();
  virtual double get_magnitude();
  virtual double get_temperature();
  virtual vector3d get_colour();
  static vector3d temperature_to_colour(double temperature);

  // render
  virtual void render_diagram(double scale, bool labels = false);
  virtual void render_visible();
  virtual void render_radio();
  virtual void render_infrared();
  virtual void render_ultraviolet();

};

#endif // STAR_H_INCLUDED
