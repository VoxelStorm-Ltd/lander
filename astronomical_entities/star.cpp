#include "star.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

star::star()
  : brightness(1.0) {
  /// Default constructor
  name = "Sun";
  description = "A massive, luminous sphere of plasma.";
}

star::~star() {
  /// Default destructor
}

std::string star::get_type() {
  /// Return the type of object this is
  return "star";
}

double star::get_magnitude() {
  /// Return the magnitude of this star relative to the observer
  // TODO
  return 1.0 / brightness;
}

//bool orbitalbody::check_within_physical_influence(double thisradius) {
//  /// Check if we're within range for physical interaction (atmosphere / collision)
//  if(thisradius <= get_radius()) {
//    // assume a rocky body with no atmosphere by default
//    return true;
//  } else {
//    return false;
//  }
//}
//
//bool orbitalbody::get_atmos_pressure(double thisradius) {
//  /// Return the atmospheric pressure at this radius from the body
//  return 0.0;   // vacuum
//}
//
//bool orbitalbody::get_atmos_temperature(double thisradius) {
//  /// Return the atmospheric temperature at this radius from the body
//  return 0.0;   // vacuum
//}

double star::get_temperature() {
  /// Surface temperature in kelvin
  // placeholder
  return 5778.0;    // surface of the sun
}

Vector3d star::get_colour() {
  /// Return absolute colour value for this star's surface based on brightness & temperature
  return temperature_to_colour(get_temperature()) * brightness;
}

Vector3d star::temperature_to_colour(double temperature) {
  /** Return RGB components based on the radiation due to temperature
      temperature is in kelvin
      Note: this is basically the same as "colour temperature" in photography
      Algorithm from http://www.tannerhelland.com/4435/convert-temperature-rgb-algorithm-code/
      modified to return dull glow values all the way down to the Draper point, 798K
  **/
  Vector3d result;
  temperature /= 100.0;

  // calculate red
  if(temperature <= 66.0) {
    if(temperature <= 10.0) {
      result.r = ((temperature - 7.98) / 2.02) * 255;       // Draper point
      if(result.r < 0.0) {
        result.r = 0.0;
      }
      if(result.r > 255.0) {
        result.r = 255.0;
      }
    } else {
      result.r = 255.0;
    }
  } else {
    result.r = 329.698727446 * pow(temperature - 60.0, -0.1332047592);
    if(result.r < 0.0) {
      result.r = 0.0;
    }
    if(result.r > 255.0) {
      result.r = 255.0;
    }
  }

  // calculate green
  if(temperature <= 66.0) {
    result.g = (99.4708025861 * log(temperature)) - 161.1195681661;
    if(result.g < 0.0) {
      result.g = 0.0;
    }
    if(result.g > 255.0) {
      result.g = 255.0;
    }
  } else {
    result.g = 288.1221695283 * pow(temperature - 60.0, -0.0755148492);
    if(result.g < 0.0) {
      result.g = 0.0;
    }
    if(result.g > 255.0) {
      result.g = 255.0;
    }
  }

  // calculate blue
  if(temperature >= 66.0) {
    result.b = 255.0;
  } else {
    if(temperature <= 19.0) {
      result.b = 0.0;
    } else {
      result.b = (138.5177312231 * log(temperature - 10.0)) - 305.0447927307;
      if(result.b < 0.0) {
        result.b = 0.0;
      }
      if(result.b > 255.0) {
        result.b = 255.0;
      }
    }
  }

  return result / 255.0;
}


void star::render_diagram(double scale, bool labels) {
  /// Render in the orthographic diagram view
  glPushMatrix();
  // move into position
  glTranslated(position.x, position.y, position.z);
  // undo rotation - billboard effect
  Matrix4d modelview;
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  for(unsigned int i = 0; i != 3; ++i) {
    for(unsigned int j = 0; j != 3; ++j) {
      if(i == j) {
        modelview[i * 4 + j] = 1.0;
      } else {
        modelview[i * 4 + j] = 0.0;
      }
    }
  }
  // set the modelview matrix with no rotations and scaling
  glLoadMatrixd(modelview);

  double thisradius = get_radius();
  if(thisradius * scale < 2.0) {
    thisradius = 2.0 / scale;
  }

  // draw a filled circle at the radius
  glColor3dv(get_colour());
  double const circlestep = M_PI / 32.0;
  glBegin(GL_TRIANGLE_FAN);
  glVertex3d(0.0, 0.0, 0.0);
  for(double angle = 0.0; angle < (M_PI * 2.0) + circlestep; angle += circlestep) {
    glVertex3d(sin(angle) * thisradius, cos(angle) * thisradius, 0.0);
  }
  glEnd();

  // radial glow lines
  glBegin(GL_LINES);
  for(double angle = 0.0; angle <= M_PI * 2.0; angle += M_PI / 8.0) {
    glVertex3d(sin(angle) * (thisradius * 1.1), cos(angle) * (thisradius * 1.1), 0.0);
    glVertex3d(sin(angle) * (thisradius * 1.3), cos(angle) * (thisradius * 1.3), 0.0);
  }
  glEnd();

  // circle outline
  //glColor4dv(Vector4d(1.0, 1.0, 1.0, 1.0));
  //glBegin(GL_LINE_LOOP);
  //for(double angle = 0.0; angle <= M_PI * 2.0; angle += circlestep) {
  //  glVertex3d(sin(angle) * thisradius, cos(angle) * thisradius, 0.0);
  //}
  //glEnd();

  // restore rotation
  glPopMatrix();
}
void star::render_visible() {
  /// Render in the visible spectrum
  glPushMatrix();
  // move into position
  glTranslated(position.x, position.y, position.z);

  Vector3d const thiscolour = get_colour();

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(thiscolour.r, thiscolour.g, thiscolour.b, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(thiscolour.r, thiscolour.g, thiscolour.b, 1.0));
  glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(thiscolour.r, thiscolour.g, thiscolour.b, 1.0));
  glMaterialf( GL_FRONT, GL_SHININESS,           0.0);                           // 0 to 127

  double const thisradius = get_radius() / 2;

  double const r = 1.0 * thisradius;
  double const t = ((1.0 + sqrt(5.0)) / 2.0) * thisradius;

  std::vector<Vector3d> points;

  points.push_back(Vector3d(-r,  t,  0));
  points.push_back(Vector3d( r,  t,  0));
  points.push_back(Vector3d(-r, -t,  0));
  points.push_back(Vector3d( r, -t,  0));

  points.push_back(Vector3d( 0, -r,  t));
  points.push_back(Vector3d( 0,  r,  t));
  points.push_back(Vector3d( 0, -r, -t));
  points.push_back(Vector3d( 0,  r, -t));

  points.push_back(Vector3d( t,  0, -r));
  points.push_back(Vector3d( t,  0,  r));
  points.push_back(Vector3d(-t,  0, -r));
  points.push_back(Vector3d(-t,  0,  r));

  glColor4dv(Vector4d(0.25, 0.25, 0.25, 1.0));
  glBegin(GL_TRIANGLES);

  // 5 faces around point 0
  glNormal3dv(points[0].normalise_copy());
  glVertex3dv(points[0]);
  glNormal3dv(points[11].normalise_copy());
  glVertex3dv(points[11]);
  glNormal3dv(points[5].normalise_copy());
  glVertex3dv(points[5]);
  glNormal3dv(points[0].normalise_copy());
  glVertex3dv(points[0]);
  glNormal3dv(points[5].normalise_copy());
  glVertex3dv(points[5]);
  glNormal3dv(points[1].normalise_copy());
  glVertex3dv(points[1]);
  glNormal3dv(points[0].normalise_copy());
  glVertex3dv(points[0]);
  glNormal3dv(points[1].normalise_copy());
  glVertex3dv(points[1]);
  glNormal3dv(points[7].normalise_copy());
  glVertex3dv(points[7]);
  glNormal3dv(points[0].normalise_copy());
  glVertex3dv(points[0]);
  glNormal3dv(points[7].normalise_copy());
  glVertex3dv(points[7]);
  glNormal3dv(points[10].normalise_copy());
  glVertex3dv(points[10]);
  glNormal3dv(points[0].normalise_copy());
  glVertex3dv(points[0]);
  glNormal3dv(points[10].normalise_copy());
  glVertex3dv(points[10]);
  glNormal3dv(points[11].normalise_copy());
  glVertex3dv(points[11]);
  // 5 adjacent faces
  glNormal3dv(points[1].normalise_copy());
  glVertex3dv(points[1]);
  glNormal3dv(points[5].normalise_copy());
  glVertex3dv(points[5]);
  glNormal3dv(points[9].normalise_copy());
  glVertex3dv(points[9]);
  glNormal3dv(points[5].normalise_copy());
  glVertex3dv(points[5]);
  glNormal3dv(points[11].normalise_copy());
  glVertex3dv(points[11]);
  glNormal3dv(points[4].normalise_copy());
  glVertex3dv(points[4]);
  glNormal3dv(points[11].normalise_copy());
  glVertex3dv(points[11]);
  glNormal3dv(points[10].normalise_copy());
  glVertex3dv(points[10]);
  glNormal3dv(points[2].normalise_copy());
  glVertex3dv(points[2]);
  glNormal3dv(points[10].normalise_copy());
  glVertex3dv(points[10]);
  glNormal3dv(points[7].normalise_copy());
  glVertex3dv(points[7]);
  glNormal3dv(points[6].normalise_copy());
  glVertex3dv(points[6]);
  glNormal3dv(points[7].normalise_copy());
  glVertex3dv(points[7]);
  glNormal3dv(points[1].normalise_copy());
  glVertex3dv(points[1]);
  glNormal3dv(points[8].normalise_copy());
  glVertex3dv(points[8]);
  // 5 faces around point 3
  glNormal3dv(points[3].normalise_copy());
  glVertex3dv(points[3]);
  glNormal3dv(points[9].normalise_copy());
  glVertex3dv(points[9]);
  glNormal3dv(points[4].normalise_copy());
  glVertex3dv(points[4]);
  glNormal3dv(points[3].normalise_copy());
  glVertex3dv(points[3]);
  glNormal3dv(points[4].normalise_copy());
  glVertex3dv(points[4]);
  glNormal3dv(points[2].normalise_copy());
  glVertex3dv(points[2]);
  glNormal3dv(points[3].normalise_copy());
  glVertex3dv(points[3]);
  glNormal3dv(points[2].normalise_copy());
  glVertex3dv(points[2]);
  glNormal3dv(points[6].normalise_copy());
  glVertex3dv(points[6]);
  glNormal3dv(points[3].normalise_copy());
  glVertex3dv(points[3]);
  glNormal3dv(points[6].normalise_copy());
  glVertex3dv(points[6]);
  glNormal3dv(points[8].normalise_copy());
  glVertex3dv(points[8]);
  glNormal3dv(points[3].normalise_copy());
  glVertex3dv(points[3]);
  glNormal3dv(points[8].normalise_copy());
  glVertex3dv(points[8]);
  glNormal3dv(points[9].normalise_copy());
  glVertex3dv(points[9]);
  // 5 adjacent faces
  glNormal3dv(points[4].normalise_copy());
  glVertex3dv(points[4]);
  glNormal3dv(points[9].normalise_copy());
  glVertex3dv(points[9]);
  glNormal3dv(points[5].normalise_copy());
  glVertex3dv(points[5]);
  glNormal3dv(points[2].normalise_copy());
  glVertex3dv(points[2]);
  glNormal3dv(points[4].normalise_copy());
  glVertex3dv(points[4]);
  glNormal3dv(points[11].normalise_copy());
  glVertex3dv(points[11]);
  glNormal3dv(points[6].normalise_copy());
  glVertex3dv(points[6]);
  glNormal3dv(points[2].normalise_copy());
  glVertex3dv(points[2]);
  glNormal3dv(points[10].normalise_copy());
  glVertex3dv(points[10]);
  glNormal3dv(points[8].normalise_copy());
  glVertex3dv(points[8]);
  glNormal3dv(points[6].normalise_copy());
  glVertex3dv(points[6]);
  glNormal3dv(points[7].normalise_copy());
  glVertex3dv(points[7]);
  glNormal3dv(points[9].normalise_copy());
  glVertex3dv(points[9]);
  glNormal3dv(points[8].normalise_copy());
  glVertex3dv(points[8]);
  glNormal3dv(points[1].normalise_copy());
  glVertex3dv(points[1]);
  glEnd();

  // restore rotation
  glPopMatrix();
}

void star::render_radio() {
  /// Render in the radio spectrum, i.e. radar reflection
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}

void star::render_infrared() {
  /// Render in the infrared spectrum
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}

void star::render_ultraviolet() {
  /// Render in the ultraviolet spectrum
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}
