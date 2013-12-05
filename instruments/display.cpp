#include "display.h"

display::display()
  : display_image(0),
    framebuffer(0) {
  /// Default constructor
  size.x = 0.400;
  size.y = 0.400;
  size.z = 0.020;
  ports_in.resize(get_port_in_count());     // anything with input ports needs this

  // create a blank texture
  glGenTextures(1, &display_image);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, display_image);        // bind the screen texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_width, screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  // texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);                // nearest neighbour filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);     // emissive style glow effect - see http://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);                    // unbind the texture

  // create a framebuffer
  glGenFramebuffers(1, &framebuffer);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,       // target
                            GL_COLOR_ATTACHMENT0_EXT, // attachment point - colour, depth, stencil or depth-stencil
                            GL_TEXTURE_2D,            // texture target / cubemap face
                            display_image,            // texture
                            0);                       // level
  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  if(status != GL_FRAMEBUFFER_COMPLETE_EXT) {
    std::cout << "framebuffer fucked: " << status;
  }
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

display::~display() {
  /// Default destructor
  glDeleteTextures(1, &display_image);
}

std::string display::get_name() {
  /// Return the name of this device
  return "graphical display";
}

std::string display::get_description() {
  /// Return a detailed description of this device
  random_reset();
  std::stringstream desc;
  desc << get_random_name_corporation() << " model CRT0606-G."
          "  A very cheap and old but reliable analogue CRT monitor."
          "  Capable of withstanding a lot of punishment, both physical and electromagnetic."
          "  Technically capable of displaying any colour, but for some reason tends to tint everything with a sickly greenish hue."
          "  Slightly increases the pilot's overall radiation exposure.";
  return desc.str();
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
