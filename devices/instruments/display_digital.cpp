#include "display_digital.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FTGL/ftgl.h>

extern FTFont *font_title;          // global font definitions
extern FTFont *font_text;

display_digital::display_digital() {
  /// Default constructor
}

display_digital::~display_digital() {
  /// Default destructor
}

std::string display_digital::get_name() {
  /// Return the name of this device
  return "digital graphical display";
}

std::string display_digital::get_model() {
  /// Return a model name for this device
  random_reset();
  unsigned int const modelnum = get_random_int(1, 9);
  std::stringstream model;
  model << "LCD" << modelnum << modelnum << "00-D";
  return model.str();
}

std::string display_digital::get_description() {
  /// Return a detailed description of this device
  return "A digital LCD monitor, much flatter and lighter than its CRT equivalents "
         "but considerably more susceptible to physical and electrical damage."
         "  Purists complain about the slow response time and the fact that black "
         "is never really completely black on these things."
         "  Useful for displaying digital signals from older devices that don't "
         "offer analogue output.";
}

double display_digital::get_mass() {
  /// Return the weight of the device, in kilograms
  return 2.5;
}

void display_digital::update() {
  /// Update the contents of this display - digital version
  if(ports_in[0].target) {
    display_image = ports_in[0].target->get_port_out_video_digital(ports_in[0].target_port);
  } else {
    display_image = generate_static_digital();
    // TODO: overlay "no signal" text on static
  }
}

void display_digital::render() {
  /// Render this display's contents in the right place
  update();

  glPushMatrix();

  glTranslated(get_position().x,
               get_position().y,
               get_position().z);

  //glColor4dv(Vector4d(0.6, 0.6, 0.6, 1.0));
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.2, 0.2, 0.2, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(0.2, 0.2, 0.2, 1.0));
  glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialf(GL_FRONT,  GL_SHININESS,           2.0);                           // 0 to 127

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

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(0.2, 0.2, 0.2, 1.0));
  glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.25, 0.25, 0.25, 1.0));
  glMaterialf(GL_FRONT,  GL_SHININESS,           20.0);                         // 0 to 127

  glBindTexture(GL_TEXTURE_2D, display_image);        // bind the screen texture
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);     // emissive style glow effect - see http://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml

  glBegin(GL_QUADS);
  glNormal3d(0.0, 0.0, 1.0);
  double const screenheight = get_size().z + 0.002;
  double const xstep = screen_size.x / 10;
  double const ystep = screen_size.y / 10;
  for(double x = 0.0; x <= screen_size.x - xstep; x += xstep) {
    for(double y = 0.0; y <= screen_size.y - ystep; y += ystep) {
      glTexCoord2d(x           / screen_size.x, y           / screen_size.y);
      glVertex3d(screen_pos.x + x,         screen_pos.y + y,         screenheight);
      glTexCoord2d((x + xstep) / screen_size.x, y           / screen_size.y);
      glVertex3d(screen_pos.x + x + xstep, screen_pos.y + y,         screenheight);
      glTexCoord2d((x + xstep) / screen_size.x, (y + ystep) / screen_size.y);
      glVertex3d(screen_pos.x + x + xstep, screen_pos.y + y + ystep, screenheight);
      glTexCoord2d(x           / screen_size.x, (y + ystep) / screen_size.y);
      glVertex3d(screen_pos.x + x,         screen_pos.y + y + ystep, screenheight);
    }
  }
  glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);                    // unbind the texture

  // manufacturer / model label
  Vector3d const thissize = get_size();
  double const scale = 0.00035277777;       // 1m / (72dpi * 39.3700787in) = 0.00035277777
  glEnable(GL_RESCALE_NORMAL);              // to allow correct lighting (faster than GL_NORMALIZE)
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.8, 0.8, 0.8, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(0.8, 0.8, 0.8, 1.0));
  glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialf(GL_FRONT,  GL_SHININESS,           2.0);                           // 0 to 127
  glPushMatrix();
  glTranslated(0.002, 0.002, thissize.z + 0.001);
  glScaled(scale, scale, scale);
  font_title->Render(get_manufacturer().c_str(), -1, FTPoint(), FTPoint(), FTGL::RENDER_FRONT);
  glPopMatrix();
  glPushMatrix();
  glTranslated(thissize.x - 0.002, 0.002, thissize.z + 0.001);
  glScaled(scale, scale, scale);
  glTranslated(-font_title->Advance(get_model().c_str(), -1), 0.0, 0.0);    // slide it back for right-align
  font_title->Render(get_model().c_str(), -1, FTPoint(), FTPoint(), FTGL::RENDER_FRONT);
  glPopMatrix();
  glDisable(GL_RESCALE_NORMAL);             // enable needed to allow correct lighting, disable for speed

  glPopMatrix();
}
