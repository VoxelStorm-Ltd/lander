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

double display::get_mass() {
  /// Return the weight of the device, in kilograms
  return 22.0;
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
    //std::cout << "DEBUG: no signal" << std::endl;
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

  //glColor4dv(Vector4d(0.6, 0.6, 0.6, 1.0));
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.2, 0.2, 0.2, 1.0));
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            Vector4f(0.2, 0.2, 0.2, 1.0));
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            Vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS,           2.0);                           // 0 to 127

  glBegin(GL_QUADS);
  // front
  glNormal3d(0.0, 0.0, 1.0);
  glVertex3d(0.0,          0.0,          get_size().z);
  glVertex3d(get_size().x, 0.0,          get_size().z);
  glVertex3d(get_size().x, get_size().y, get_size().z);
  glVertex3d(0.0,          get_size().y, get_size().z);
  // top
  glNormal3d(0.0, 1.0, 0.0);
  glVertex3d(0.0,          get_size().y, 0.0);
  glVertex3d(0.0,          get_size().y, get_size().z);
  glVertex3d(get_size().x, get_size().y, get_size().z);
  glVertex3d(get_size().x, get_size().y, 0.0);
  // bottom
  glNormal3d(0.0, -1.0, 0.0);
  glVertex3d(0.0,          0.0,          0.0);
  glVertex3d(get_size().x, 0.0,          0.0);
  glVertex3d(get_size().x, 0.0,          get_size().z);
  glVertex3d(0.0,          0.0,          get_size().z);
  // right
  glNormal3d(1.0, 0.0, 0.0);
  glVertex3d(get_size().x, 0.0,          0.0);
  glVertex3d(get_size().x, get_size().y, 0.0);
  glVertex3d(get_size().x, get_size().y, get_size().z);
  glVertex3d(get_size().x, 0.0,          get_size().z);
  // left
  glNormal3d(-1.0, 0.0, 0.0);
  glVertex3d(0.0,          0.0,          0.0);
  glVertex3d(0.0,          0.0,          get_size().z);
  glVertex3d(0.0,          get_size().y, get_size().z);
  glVertex3d(0.0,          get_size().y, 0.0);
  glEnd();

  Vector2d const screen_pos(0.01, 0.01);
  Vector2d const screen_size(get_size().x - 0.02, get_size().y - 0.02);

  //glColor4dv(Vector4d(0.2, 0.2, 0.2, 1.0));
  glColor4dv(Vector4d(1.0, 1.0, 1.0, 1.0));
  //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.05, 0.2, 0.05, 1.0));
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            Vector4f(1.0, 0.8, 1.0, 1.0));
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            Vector4f(0.0, 0.05, 0.0, 1.0));
  glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS,           127.0);                         // 0 to 127

  glBindTexture(GL_TEXTURE_2D, display_image);        // bind the screen texture
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);     // emissive style glow effect - see http://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml

  glBegin(GL_QUADS);
  //glNormal3d(0.0, 0.0, 1.0);
  //glTexCoord2d(0.0, 0.0);
  //glVertex3d(screen_pos.x,                 screen_pos.y,                 get_size().z + 0.001);
  //glTexCoord2d(1.0, 0.0);
  //glVertex3d(screen_pos.x + screen_size.x, screen_pos.y,                 get_size().z + 0.001);
  //glTexCoord2d(1.0, 1.0);
  //glVertex3d(screen_pos.x + screen_size.x, screen_pos.y + screen_size.y, get_size().z + 0.001);
  //glTexCoord2d(0.0, 1.0);
  //glVertex3d(screen_pos.x,                 screen_pos.y + screen_size.y, get_size().z + 0.001);
  //double const scalefactor = 0.5;
  double const scalefactor = 0.5 / screen_size.length();
  double const maxheight = (pow(screen_size.x / 2, 2) + pow(screen_size.y / 2, 2)) * scalefactor;
  double const heightinit = get_size().z + maxheight;
  double const xstep = screen_size.x / 10;
  double const ystep = screen_size.y / 10;
  for(double x = 0.0; x <= screen_size.x - xstep; x += xstep) {
    double const xdist0 = x           - (screen_size.x / 2);
    double const xdist1 = (x + xstep) - (screen_size.x / 2);
    for(double y = 0.0; y <= screen_size.y - ystep; y += ystep) {
      double const ydist0 = y           - (screen_size.y / 2);
      double const ydist1 = (y + ystep) - (screen_size.y / 2);
      // TODO: adjust normals for this curvature
      glNormal3d(0.0, 0.0, 1.0);
      glTexCoord2d(x           / screen_size.x, y           / screen_size.y);
      double const height00 = heightinit - ((xdist0 * xdist0 + ydist0 * ydist0) * scalefactor);
      glVertex3d(screen_pos.x + x,         screen_pos.y + y,         height00);
      glTexCoord2d((x + xstep) / screen_size.x, y           / screen_size.y);
      double const height10 = heightinit - ((xdist1 * xdist1 + ydist0 * ydist0) * scalefactor);
      glVertex3d(screen_pos.x + x + xstep, screen_pos.y + y,         height10);
      glTexCoord2d((x + xstep) / screen_size.x, (y + ystep) / screen_size.y);
      double const height11 = heightinit - ((xdist1 * xdist1 + ydist1 * ydist1) * scalefactor);
      glVertex3d(screen_pos.x + x + xstep, screen_pos.y + y + ystep, height11);
      glTexCoord2d(x           / screen_size.x, (y + ystep) / screen_size.y);
      double const height01 = heightinit - ((xdist0 * xdist0 + ydist1 * ydist1) * scalefactor);
      glVertex3d(screen_pos.x + x,         screen_pos.y + y + ystep, height01);
    }
  }
  // edge strip: left
  glNormal3d(-1.0, 0.0, 0.0);
  for(double y = 0.0; y <= screen_size.y - ystep; y += ystep) {
    double const xdist1 = screen_size.x / 2;
    double const ydist0 = y           - (screen_size.y / 2);
    double const ydist1 = (y + ystep) - (screen_size.y / 2);
    glTexCoord2d(0.0, y           / screen_size.y);
    glVertex3d(screen_pos.x, screen_pos.y + y,         0.0);
    glTexCoord2d(0.0, y           / screen_size.y);
    double const height10 = heightinit - ((xdist1 * xdist1 + ydist0 * ydist0) * scalefactor);
    glVertex3d(screen_pos.x, screen_pos.y + y,         height10);
    glTexCoord2d(0.0, (y + ystep) / screen_size.y);
    double const height11 = heightinit - ((xdist1 * xdist1 + ydist1 * ydist1) * scalefactor);
    glVertex3d(screen_pos.x, screen_pos.y + y + ystep, height11);
    glTexCoord2d(0.0, (y + ystep) / screen_size.y);
    glVertex3d(screen_pos.x, screen_pos.y + y + ystep, 0.0);
  }
  // edge strip: right
  glNormal3d(1.0, 0.0, 0.0);
  for(double y = 0.0; y <= screen_size.y - ystep; y += ystep) {
    double const xdist0 = screen_size.x / 2;
    double const ydist0 = y           - (screen_size.y / 2);
    double const ydist1 = (y + ystep) - (screen_size.y / 2);
    glTexCoord2d(1.0, y           / screen_size.y);
    double const height00 = heightinit - ((xdist0 * xdist0 + ydist0 * ydist0) * scalefactor);
    glVertex3d(screen_pos.x + screen_size.x, screen_pos.y + y,         height00);
    glTexCoord2d(1.0, y           / screen_size.y);
    glVertex3d(screen_pos.x + screen_size.x, screen_pos.y + y,         0.0);
    glTexCoord2d(1.0, (y + ystep) / screen_size.y);
    glVertex3d(screen_pos.x + screen_size.x, screen_pos.y + y + ystep, 0.0);
    glTexCoord2d(1.0, (y + ystep) / screen_size.y);
    double const height01 = heightinit - ((xdist0 * xdist0 + ydist1 * ydist1) * scalefactor);
    glVertex3d(screen_pos.x + screen_size.x, screen_pos.y + y + ystep, height01);
  }
  // edge strip: bottom
  glNormal3d(0.0, -1.0, 0.0);
  for(double x = 0.0; x <= screen_size.x - xstep; x += xstep) {
    double const ydist1 = screen_size.y / 2;
    double const xdist0 = x           - (screen_size.x / 2);
    double const xdist1 = (x + xstep) - (screen_size.x / 2);
    glTexCoord2d(x           / screen_size.x, 0.0);
    glVertex3d(screen_pos.x + x,         screen_pos.y, 0.0);
    glTexCoord2d((x + xstep) / screen_size.x, 0.0);
    glVertex3d(screen_pos.x + x + xstep, screen_pos.y, 0.0);
    glTexCoord2d((x + xstep) / screen_size.x, 0.0);
    double const height11 = heightinit - ((ydist1 * ydist1 + xdist1 * xdist1) * scalefactor);
    glVertex3d(screen_pos.x + x + xstep, screen_pos.y, height11);
    glTexCoord2d(x           / screen_size.x, 0.0);
    double const height10 = heightinit - ((ydist1 * ydist1 + xdist0 * xdist0) * scalefactor);
    glVertex3d(screen_pos.x + x,         screen_pos.y, height10);
  }
  // edge strip: top
  glNormal3d(0.0, 1.0, 0.0);
  for(double x = 0.0; x <= screen_size.x - xstep; x += xstep) {
    double const ydist1 = screen_size.y / 2;
    double const xdist0 = x           - (screen_size.x / 2);
    double const xdist1 = (x + xstep) - (screen_size.x / 2);
    glTexCoord2d(x           / screen_size.x, 1.0);
    glVertex3d(screen_pos.x + x,         screen_pos.y + screen_size.y, 0.0);
    glTexCoord2d(x           / screen_size.x, 1.0);
    double const height10 = heightinit - ((ydist1 * ydist1 + xdist0 * xdist0) * scalefactor);
    glVertex3d(screen_pos.x + x,         screen_pos.y + screen_size.y, height10);
    glTexCoord2d((x + xstep) / screen_size.x, 1.0);
    double const height11 = heightinit - ((ydist1 * ydist1 + xdist1 * xdist1) * scalefactor);
    glVertex3d(screen_pos.x + x + xstep, screen_pos.y + screen_size.y, height11);
    glTexCoord2d((x + xstep) / screen_size.x, 1.0);
    glVertex3d(screen_pos.x + x + xstep, screen_pos.y + screen_size.y, 0.0);
  }
  glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);                    // unbind the texture

  glPopMatrix();
}
