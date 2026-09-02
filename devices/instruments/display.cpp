#include "display.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FTGL/ftgl.h>

extern FTFont *font_title3d;                                                    // global font definitions
extern FTFont *font_text3d;

display::display()
  : vbo_screen_v(0),
    vbo_screen_n(0),
    vbo_screen_t(0),
    ibo_screen(0),
    screen_index_count(0),
    display_image(0) {
  /// Default constructor
  ports_in.resize(get_port_in_count());                                         // anything with input ports needs this
  glGenBuffers(1, &vbo_screen_v);
  glGenBuffers(1, &vbo_screen_n);
  glGenBuffers(1, &vbo_screen_t);
  glGenBuffers(1, &ibo_screen);
  update_vbo();                                                                 // anything with a custom model needs this
}

display::~display() {
  /// Default destructor
  glDeleteBuffers(1, &vbo_screen_v);
  glDeleteBuffers(1, &vbo_screen_n);
  glDeleteBuffers(1, &vbo_screen_t);
  glDeleteBuffers(1, &ibo_screen);
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

vector3d display::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return vector3d(0.4, 0.4, 0.02);
}

void display::update() {
  if(ports_in[0].target) {
    display_image = ports_in[0].target->get_port_out_video_analogue(ports_in[0].target_port);
  } else {
    display_image = generate_static_analogue();
    // TODO: overlay "no signal" text on static
  }
}

void display::update_vbo() {
  /// Update the display's vertex buffer object
  vector3d const thissize = get_size();
  // first the plastic base
  GLdouble vbodata_vertex[] = {
    // front
    0.0,        0.0,        thissize.z,
    thissize.x, 0.0,        thissize.z,
    thissize.x, thissize.y, thissize.z,
    0.0,        thissize.y, thissize.z,
    // top
    0.0,        thissize.y, 0.0,
    0.0,        thissize.y, thissize.z,
    thissize.x, thissize.y, thissize.z,
    thissize.x, thissize.y, 0.0,
    // bottom
    0.0,        0.0,        0.0,
    thissize.x, 0.0,        0.0,
    thissize.x, 0.0,        thissize.z,
    0.0,        0.0,        thissize.z,
    // right
    thissize.x, 0.0,        0.0,
    thissize.x, thissize.y, 0.0,
    thissize.x, thissize.y, thissize.z,
    thissize.x, 0.0,        thissize.z,
    // left
    0.0,        0.0,        0.0,
    0.0,        0.0,        thissize.z,
    0.0,        thissize.y, thissize.z,
    0.0,        thissize.y, 0.0,
  };
  GLdouble vbodata_normal[] = {
    // front
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    // top
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    // bottom
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    // right
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    // left
    -1.0, 0.0, 0.0,
    -1.0, 0.0, 0.0,
    -1.0, 0.0, 0.0,
    -1.0, 0.0, 0.0,
  };
  GLuint ibodata[] = {
     0,  1,  2,  3,                                                             // front
     4,  5,  6,  7,                                                             // top
     8,  9,  10, 11,                                                            // bottom
     12, 13, 14, 15,                                                            // right
     16, 17, 18, 19,                                                            // left
  };
  glBindBuffer(GL_ARRAY_BUFFER, vbo_v);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vbodata_vertex), vbodata_vertex, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_n);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vbodata_normal), vbodata_normal, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibodata), ibodata, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // the glass screen
  vector2d const screen_pos(0.01, 0.01);
  vector2d const screen_size(thissize.x - 0.02, thissize.y - 0.02);
  //double const scalefactor = 0.5;
  double const scalefactor = 0.5 / screen_size.length();
  double const maxheight = (pow(screen_size.x / 2, 2) + pow(screen_size.y / 2, 2)) * scalefactor;
  double const heightinit = get_size().z + maxheight;
  double const xstep = screen_size.x / 10;
  double const ystep = screen_size.y / 10;

  std::vector<GLdouble> vbodata_screen_vertex;
  std::vector<GLdouble> vbodata_screen_normal;
  std::vector<GLdouble> vbodata_screen_texture;
  std::vector<GLuint> ibodata_screen;

  for(double x = 0.0; x <= screen_size.x - xstep; x += xstep) {
    double const xdist0 = x           - (screen_size.x / 2);
    double const xdist1 = (x + xstep) - (screen_size.x / 2);
    for(double y = 0.0; y <= screen_size.y - ystep; y += ystep) {
      double const ydist0 = y           - (screen_size.y / 2);
      double const ydist1 = (y + ystep) - (screen_size.y / 2);
      // TODO: adjust normals for this curvature

      double const height00 = heightinit - ((xdist0 * xdist0 + ydist0 * ydist0) * scalefactor);
      vbodata_screen_vertex.push_back(screen_pos.x + x);
      vbodata_screen_vertex.push_back(screen_pos.y + y);
      vbodata_screen_vertex.push_back(height00);
      vbodata_screen_normal.push_back(0.0);
      vbodata_screen_normal.push_back(0.0);
      vbodata_screen_normal.push_back(1.0);
      vbodata_screen_texture.push_back(x / screen_size.x);
      vbodata_screen_texture.push_back(y / screen_size.y);
      ibodata_screen.push_back(ibodata_screen.size());

      double const height10 = heightinit - ((xdist1 * xdist1 + ydist0 * ydist0) * scalefactor);
      vbodata_screen_vertex.push_back(screen_pos.x + x + xstep);
      vbodata_screen_vertex.push_back(screen_pos.y + y);
      vbodata_screen_vertex.push_back(height10);
      vbodata_screen_normal.push_back(0.0);
      vbodata_screen_normal.push_back(0.0);
      vbodata_screen_normal.push_back(1.0);
      vbodata_screen_texture.push_back((x + xstep) / screen_size.x);
      vbodata_screen_texture.push_back(y / screen_size.y);
      ibodata_screen.push_back(ibodata_screen.size());

      double const height11 = heightinit - ((xdist1 * xdist1 + ydist1 * ydist1) * scalefactor);
      vbodata_screen_vertex.push_back(screen_pos.x + x + xstep);
      vbodata_screen_vertex.push_back(screen_pos.y + y + ystep);
      vbodata_screen_vertex.push_back(height11);
      vbodata_screen_normal.push_back(0.0);
      vbodata_screen_normal.push_back(0.0);
      vbodata_screen_normal.push_back(1.0);
      vbodata_screen_texture.push_back((x + xstep) / screen_size.x);
      vbodata_screen_texture.push_back((y + ystep) / screen_size.y);
      ibodata_screen.push_back(ibodata_screen.size());

      double const height01 = heightinit - ((xdist0 * xdist0 + ydist1 * ydist1) * scalefactor);
      vbodata_screen_vertex.push_back(screen_pos.x + x);
      vbodata_screen_vertex.push_back(screen_pos.y + y + ystep);
      vbodata_screen_vertex.push_back(height01);
      vbodata_screen_normal.push_back(0.0);
      vbodata_screen_normal.push_back(0.0);
      vbodata_screen_normal.push_back(1.0);
      vbodata_screen_texture.push_back(x / screen_size.x);
      vbodata_screen_texture.push_back((y + ystep) / screen_size.y);
      ibodata_screen.push_back(ibodata_screen.size());
    }
  }
  /**
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
  **/

  glBindBuffer(GL_ARRAY_BUFFER, vbo_screen_v);
  glBufferData(GL_ARRAY_BUFFER, vbodata_screen_vertex.size() * sizeof(GLdouble), vbodata_screen_vertex.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_screen_n);
  glBufferData(GL_ARRAY_BUFFER, vbodata_screen_normal.size() * sizeof(GLdouble), vbodata_screen_normal.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_screen_t);
  glBufferData(GL_ARRAY_BUFFER, vbodata_screen_texture.size() * sizeof(GLdouble), vbodata_screen_texture.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_screen);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibodata_screen.size() * sizeof(GLuint), ibodata_screen.data(), GL_STATIC_DRAW);
  screen_index_count = static_cast<GLsizei>(ibodata_screen.size());
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void display::render() {
  /// Render this display's contents in the right place
  update();

  glPushMatrix();

  glTranslated(get_position().x,
               get_position().y,
               get_position().z);

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, vector4f(0.2, 0.2, 0.2, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            vector4f(0.2, 0.2, 0.2, 1.0));
  glMaterialfv(GL_FRONT, GL_EMISSION,            vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialf(GL_FRONT,  GL_SHININESS,           2.0);                          // 0 to 127

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_v);
  glVertexPointer(3, GL_DOUBLE, 0, 0);
  glEnableClientState(GL_NORMAL_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_n);
  glNormalPointer(GL_DOUBLE, 0, 0);

  glDrawElements(GL_QUADS, 5 * 4, GL_UNSIGNED_INT, 0);                          // draw 5 quads (4 points each)

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  //glColor4dv(vector4d(0.2, 0.2, 0.2, 1.0));
  glColor4dv(vector4d(1.0, 1.0, 1.0, 1.0));
  //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, vector4f(0.05, 0.2, 0.05, 1.0));
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            vector4f(1.0, 0.8, 1.0, 1.0));
  glMaterialfv(GL_FRONT, GL_EMISSION,            vector4f(0.0, 0.05, 0.0, 1.0));
  glMaterialf(GL_FRONT,  GL_SHININESS,           127.0);                        // 0 to 127

  glBindTexture(GL_TEXTURE_2D, display_image);                                  // bind the screen texture
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);                       // emissive style glow effect - see http://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml

  // draw the screen vbo
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_screen);
  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_screen_v);
  glVertexPointer(3, GL_DOUBLE, 0, 0);
  glEnableClientState(GL_NORMAL_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_screen_n);
  glNormalPointer(GL_DOUBLE, 0, 0);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_screen_t);
  glTexCoordPointer(2, GL_DOUBLE, 0, 0);

  glDrawElements(GL_QUADS, screen_index_count, GL_UNSIGNED_INT, 0);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindTexture(GL_TEXTURE_2D, 0);                                              // unbind the texture

  // manufacturer / model label
  vector3d const thissize = get_size();
  double const scale = 0.00035277777;                                           // 1m / (72dpi * 39.3700787in) = 0.00035277777
  //glEnable(GL_NORMALIZE);                                                       // to allow correct lighting
  glEnable(GL_RESCALE_NORMAL);                                                  // to allow correct lighting (faster than GL_NORMALIZE)
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, vector4f(0.8, 0.8, 0.8, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            vector4f(0.8, 0.8, 0.8, 1.0));
  glMaterialfv(GL_FRONT, GL_EMISSION,            vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialf(GL_FRONT,  GL_SHININESS,           2.0);                          // 0 to 127
  glPushMatrix();
  glTranslated(0.002, 0.002, thissize.z + 0.001);
  glScaled(scale, scale, scale);
  font_title3d->Render(get_manufacturer().c_str(), -1, FTPoint(), FTPoint(), FTGL::RENDER_FRONT);
  glPopMatrix();
  glPushMatrix();
  glTranslated(thissize.x - 0.002, 0.002, thissize.z + 0.001);
  glScaled(scale, scale, scale);
  glTranslated(-font_title3d->Advance(get_model().c_str(), -1), 0.0, 0.0);      // slide it back for right-align
  font_title3d->Render(get_model().c_str(), -1, FTPoint(), FTPoint(), FTGL::RENDER_FRONT);
  glPopMatrix();
  //glDisable(GL_NORMALIZE);                                                      // enable needed to allow correct lighting, disable for speed
  glDisable(GL_RESCALE_NORMAL);                                                 // enable needed to allow correct lighting, disable for speed

  glPopMatrix();
}
