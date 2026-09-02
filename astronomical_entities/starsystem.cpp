#include "starsystem.h"
//#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include "body.h"
#include "star.h"

starsystem::starsystem() {
  /// Default constructor
}

starsystem::~starsystem() {
  /// Default destructor
}

std::string starsystem::get_name() {
  if(name.size() != 0) {
    return name;
  } else {
    // random star name
    random_reset();                                                             // reset the generator to its seed
    std::stringstream randomname;
    return get_random_name_ancient();
  }
}

void starsystem::set_name(std::string const &newname) {
  name = newname;
}

std::string starsystem::get_type() {
  /// Return the type of object this is
  return "star system";
}

std::string starsystem::get_designation() {
  if(designation.size() != 0) {
    return designation;
  } else {
    // generate a random catalogue designation by some catalogue style i.e.
    // HE1327-2326 or R136a1 or Westerlund 1 BKS AS or W1-26
    // see http://en.wikipedia.org/wiki/Star_catalogue#Full-sky_catalogues
    random_reset();                                                             // reset the generator to its seed
    std::stringstream designation;
    designation << get_random_char_alpha_upper()                                // cluster
                << get_random_uint(1, 5000)                                     // cluster number
                << "-"
                << get_random_uint(1, 200);                                     // discovery number?
    return designation.str();
  }
}

void starsystem::set_designation(std::string const &newdesignation) {
  designation = newdesignation;
}

std::string starsystem::get_description() {
  if(description.size() != 0) {
    return description;
  } else {
    // describe it verbally based on its known traits:
    // distance, magnitude, colours, age, planets
    // TODO
    return "A star system.";
  }
}

void starsystem::set_description(std::string const &newdescription) {
  description = newdescription;
}

void starsystem::render_visible() {
  /// Render everything that's in this star system
  for(auto &it : bodies) {                                                      // set up light sources first
    if(static_cast<star*>(it) == primary) {                                     // placeholder
      glEnable( GL_LIGHT0);
      glLightfv(GL_LIGHT0, GL_DIFFUSE,               vector4f(1.0, 1.0, 1.0, 1.0));
      glLightfv(GL_LIGHT0, GL_AMBIENT,               vector4f(0.0, 0.0, 0.0, 1.0));
      glLightfv(GL_LIGHT0, GL_SPECULAR,              vector4f(1.0, 1.0, 1.0, 1.0));
      //glLightfv(GL_LIGHT0, GL_POSITION,              vector4f(it->position.x, it->position.y, it->position.z, 1.0));
      glLightfv(GL_LIGHT0, GL_POSITION,              vector4f(0.0, 0.0, 0.0, 1.0));
      //glLightfv(GL_LIGHT0, GL_POSITION,              vector4f(0.0, 0.0, 1.0, 0.0));
      glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION,        vector3f(0.0, 0.0, 1.0));
      glLightf( GL_LIGHT0, GL_SPOT_EXPONENT,         0.0);                      // accepts 0-128
      glLightf( GL_LIGHT0, GL_SPOT_CUTOFF,           180.0);                    // width of angle of spotlight, 180 = all-round
      glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION,  1.0);                      // attenuation factor = 1 / (constant + linear * d + quadratic * d^2)
      glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION,    0.0);
      glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);
      glEnable( GL_LIGHT0);

      // DEBUG ONLY: testing light
      //glEnable( GL_LIGHT0);
      //glLightfv(GL_LIGHT0, GL_DIFFUSE,               vector4f(1.0, 1.0, 1.0, 1.0));
      //glLightfv(GL_LIGHT0, GL_AMBIENT,               vector4f(0.0, 0.0, 0.0, 1.0));
      //glLightfv(GL_LIGHT0, GL_SPECULAR,              vector4f(1.0, 1.0, 1.0, 1.0));
      //glLightfv(GL_LIGHT0, GL_POSITION,              vector4f(0.0, 0.0, -1.0, 0.0));
      //glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION,        vector3f(0.0, 0.0, -1.0));
      //glLightf( GL_LIGHT0, GL_SPOT_EXPONENT,         0.0);                      // accepts 0-128
      //glLightf( GL_LIGHT0, GL_SPOT_CUTOFF,           180.0);                    // width of angle of spotlight, 180 = all-round
      //glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION,  1.0);                      // attenuation factor = 1 / (constant + linear * d + quadratic * d^2)
      //glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION,    0.0);
      //glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);
      //glEnable( GL_LIGHT0);
    }
  }
  for(auto &it : bodies) {                                                      // iterate through all bodies and render them
    it->render_visible(2);
  }
}
