#include "display.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

display::display()
  : display_image(0) {
  /// Default constructor
  ports_in.resize(get_port_in_count());     // anything with input ports needs this
}

display::~display() {
  /// Default destructor
  glDeleteTextures(1, &display_image);
}

std::string display::get_name() {
  /// Return the name of this device
  return "graphical display";
}

std::string display::get_model() {
  /// Return a model name for this device
  random_reset();
  unsigned int const modelnum = get_random_int(1, 9);
  std::stringstream model;
  model << "CRT0" << modelnum << "0" << modelnum << "-G";
  return model.str();
}

std::string display::get_description() {
  /// Return a detailed description of this device
  return "A very cheap and old but reliable analogue CRT monitor."
         "  Capable of withstanding a lot of punishment, both physical and electromagnetic."
         "  Technically capable of displaying any colour, but for some reason tends to tint everything with a sickly greenish hue."
         "  Slightly increases the pilot's overall radiation exposure.";
}

unsigned int display::get_port_in_count() {
  /// How many input ports it has
  return 1;
}

std::string display::get_port_in_name(unsigned int port) {
  /// Name of the input port numbered n
  switch(port) {
  case 0:
    return "analogue video";
  default:
    return "";
  }
}

std::string display::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  switch(port) {
  case 0:
    {
      std::stringstream desc;
      desc << "An analogue video or still image signal."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  default:
    return "";
  }
}

bool display::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  // all ports are needed
  return true;
}

Vector3d display::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return Vector3d(0.4, 0.4, 0.02);
}

void display::update() {
  if(ports_in[0].target) {
    display_image = ports_in[0].target->get_port_out_video_analogue(ports_in[0].target_port);
  } else {
    std::cout << "DEBUG: no signal" << std::endl;
    display_image = generate_static_analogue();
    // TODO: overlay "no signal" text on static
  }
}

void display::render() {
  /// Render this display's contents in the right place
  update();

  glPushMatrix();

  glTranslated(get_position().x,
               get_position().y,
               get_position().z);

  glColor4dv(Vector4d(0.6, 0.6, 0.6, 1.0));
  glBegin(GL_QUADS);
  // front
  glVertex3d(0.0,          0.0,          get_size().z);
  glVertex3d(get_size().x, 0.0,          get_size().z);
  glVertex3d(get_size().x, get_size().y, get_size().z);
  glVertex3d(0.0,          get_size().y, get_size().z);
  // top
  glVertex3d(0.0,          get_size().y, 0.0);
  glVertex3d(0.0,          get_size().y, get_size().z);
  glVertex3d(get_size().x, get_size().y, get_size().z);
  glVertex3d(get_size().x, get_size().y, 0.0);
  // bottom
  glVertex3d(0.0,          0.0,          0.0);
  glVertex3d(get_size().x, 0.0,          0.0);
  glVertex3d(get_size().x, 0.0,          get_size().z);
  glVertex3d(0.0,          0.0,          get_size().z);
  // right
  glVertex3d(get_size().x, 0.0,          0.0);
  glVertex3d(get_size().x, get_size().y, 0.0);
  glVertex3d(get_size().x, get_size().y, get_size().z);
  glVertex3d(get_size().x, 0.0,          get_size().z);
  // left
  glVertex3d(0.0,          0.0,          0.0);
  glVertex3d(0.0,          0.0,          get_size().z);
  glVertex3d(0.0,          get_size().y, get_size().z);
  glVertex3d(0.0,          get_size().y, 0.0);
  glEnd();

  Vector2d const screen_pos(0.01, 0.01);
  Vector2d const screen_size(get_size().x - 0.02, get_size().y - 0.02);

  glColor4dv(Vector4d(0.2, 0.2, 0.2, 1.0));
  glBindTexture(GL_TEXTURE_2D, display_image);        // bind the screen texture
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 0.0);
  glVertex3d(screen_pos.x,  screen_pos.y,  get_size().z + 0.001);
  glTexCoord2d(1.0, 0.0);
  glVertex3d(screen_size.x, screen_pos.y,  get_size().z + 0.001);
  glTexCoord2d(1.0, 1.0);
  glVertex3d(screen_size.x, screen_size.y, get_size().z + 0.001);
  glTexCoord2d(0.0, 1.0);
  glVertex3d(screen_pos.x,  screen_size.y, get_size().z + 0.001);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);                    // unbind the texture

  glPopMatrix();
}
