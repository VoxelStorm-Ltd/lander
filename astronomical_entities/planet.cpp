#include "planet.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

planet::planet()
  : atmos_molarmass(       0.0),
    atmos_pressure_base(   0.0),
    atmos_temperature_base(0.0),
    //atmos_tropopause(      0.0),
    //atmos_stratopause(     0.0),
    //atmos_mesopause(       0.0),
    atmos_thermopause(     0.0) {
    //atmos_exopause(        0.0) {
  /// Default constructor
}

planet::~planet() {
  /// Default destructor
}

std::string planet::get_type() {
  /// Return the type of object this is
  return "planet";
}

double planet::get_mass() {
  if(mass != 0.0) {
    return mass;
  } else {
    // no mass assigned, take a potshot at one - in the terrestrial planetary scale
    // range by solar system standards: http://en.wikipedia.org/wiki/Planetary_mass
    random_reset();
    return get_random_double(300000000000000000000000.0, 2000000000000000000000000000.0);  // from mercury to jupiter
  }
}
double planet::get_radius() {
  if(radius != 0.0) {
    return radius;
  } else {
    // make up a radius for this based on what's likely for the mass
    // get a random density for the range
    random_reset();
    double const density = get_random_double(620.0, 5430.0);    // from saturn to mercury
    double const volume = get_mass() * (1.0 / density);
    return pow(volume / ((4.0 / 3.0) * M_PI), 1.0 / 3.0);       // radius from volume of sphere
  }
}


bool planet::check_within_physical_influence_rel(double thisradius) {
  /// Check if we're within range for physical interaction (atmosphere / collision)
  if(thisradius <= atmos_thermopause ||
     thisradius <= get_radius()) {
    return true;
  } else {
    return false;
  }
}

double planet::get_atmos_pressure(double thisradius) {
  /// Return the atmospheric pressure at this radius from the body
  // earth pressure = 101325 * ((1 - (altitude * 2.25577 * 10^-5))^5.25588)
  // see http://www.atmosculator.com/The%20Standard%20Atmosphere.html
  // and http://web.aeromech.usyd.edu.au/aero/atmosphere/atmosphere.pdf
  // JS calculator: http://web.aeromech.usyd.edu.au/aero/atmosphere/stdatm.html
  //if(thisradius < atmos_troposphere) {
  //  // we're in the troposphere
  //  // pressure / pressure_sealevel = (temperature / temperature_sealevel)^(grav_acceleration / lapse_rate * gas_constant)
  //  return pressure_sealevel * pow(temperature / temperature_sealevel, grav_acceleration / lapse_rate * gas_constant);
  //} else if(thisradius <= atmos_stratosphere) {
  //  // we're in the stratosphere
  //  // pressure / pressure_tropopause = exp((g * (altitude_tropopause - altitude)) / (gas_constant * temperature_tropopause))
  //  return pressure_tropopause * exp((g * (atmos_troposphere - thisradius)) / (gas_constant * temperature_tropopause));
  //} else if(thisradius <= atmos_mesosphere) {
  //  // we're in the mesosphere
  //  // density(in kg/m^3) = 10^(-7 * ((altitude(in km)) / 150))
  //} else if(thisradius <= atmos_thermosphere) {
  //  // we're in the thermsophere
  //  return 0.0;
  //} else if(thisradius <= atmos_exosphere) {
  //  // we're in the exosphere
  //  return 0.0;
  //} else {
  //  // we're not in atmosphere at all
  //  return 0.0;
  //}

  // simpler formula from http://en.wikipedia.org/wiki/Atmospheric_pressure
  // pressure = atmos_pressure_base * exp(-(gravity_accel * atmos_molarmass * altitude) / (gas_constant * temperature_sealevel))
  double const altitude = thisradius - get_radius();
  return atmos_pressure_base * exp(-(get_gravity_accel_rel(thisradius) * atmos_molarmass * altitude) / (gas_constant * atmos_temperature_base));
}

double planet::get_atmos_temperature(double thisradius) {
  /// Return the atmospheric temperature at this radius from the body
  // TODO
  return 0.0;
}


/*
function Compute() {
  double altitude;
  double velocity;
  double reference_length;

  double TEMPSL  = 518.67;
  double RHOSL   = 0.00237689;
  double PRESSSL = 2116.22;
  double saTheta = 1.0;
  double saSigma = 1.0;
  double saDelta = 1.0;

  double altitude_ft = altitude * 3.2808;

  if(altitude_ft < 232940) {
    // mesopause = 80km ~= 71km
    saTheta = 1.434843 - altitude_ft / 337634;
    saSigma = pow(0.79899  - altitude_ft / 606330, 11.20114);
    saDelta = pow(0.838263 - altitude_ft / 577922, 12.20114);
  } else if(altitude_ft < 167323) {
    // stratopause ~= 51km
    saTheta = 0.939268;
    saSigma = 0.00116533 * exp((altitude_ft - 154200) / -25992);
    saDelta = 0.00109456 * exp((altitude_ft - 154200) / -25992);
  } else if(altitude_ft < 154199) {
    // ? = 47km
    saTheta = 0.482561 + altitude_ft / 337634;
    saSigma = pow(0.857003 + altitude_ft / 190115, -13.20114);
    saDelta = pow(0.898309 + altitude_ft / 181373, -12.20114);
  } else if(altitude_ft < 104987) {
    // ? = 32km
    saTheta = 0.682457 + altitude_ft / 945374;
    saSigma = pow(0.978261 + altitude_ft / 659515, -35.16319);
    saDelta = pow(0.988626 + altitude_ft / 652600, -34.16319);
  } else if(altitude_ft < 65617) {
    // ? = 20km
    saTheta = 0.751865;
    saSigma = 0.297076 * exp((36089 - altitude_ft) / 20806);
    saDelta = 0.223361 * exp((36089 - altitude_ft) / 20806);
  } else if(altitude_ft < 36089) {
    // tropopause ~= 11km
    saTheta = 1.0 - altitude_ft / 145442;
    saSigma = pow(1.0 - altitude_ft / 145442, 4.255876);
    saDelta = pow(1.0 - altitude_ft / 145442, 5.255876);
  } else {
    saTheta = 0.0;
    saSigma = 0.0;
    saDelta = 0.0;
  }

  double tempVal  = TEMPSL * saTheta;
  double rhoVal   = RHOSL * saSigma;
  double pVal     = PRESSSL * saDelta;
  double viscVal  = 0.0000000226968 * pow(tempVal, 1.5) / ((tempVal) + 198.72);
  double soundVal = sqrt(1.4 * 1716.56 * (tempVal));

  double machVal  = velocity / soundVal;
  double qVal     = 0.7 * pVal * machVal * machVal;
  double reynolds = velocity * reference_length * rhoVal / viscVal;
  double cfturb   = 0.455 / pow((log(reynolds) / log(10)), 2.58);
  double cflam    = 1.328 / sqrt(reynolds);

  // unit conversions back to metric
  tempVal       = tempVal  / 1.8;
  rhoVal        = rhoVal   / 0.068521 / 0.028317;
  pVal          = pVal     / 0.020885;
  soundVal      = soundVal / 3.2808;
  viscVal       = viscVal  / 0.22481 / 0.092903;
  qVal          = qVal     / 0.020885;


  document.forms[2].temp.value   = tempVal;
  document.forms[2].rho.value    = rhoVal;
  document.forms[2].press.value  = pVal;

  document.forms[2].ssound.value = soundVal;
  document.forms[2].visc.value   = viscVal;
  document.forms[2].mach.value   = machVal;
  document.forms[2].q.value      = qVal;
  document.forms[2].cpstar.value = (Math.pow((1 / 1.2 + machVal * machVal / 6.0), 3.5) - 1) / (0.7 * machVal * machVal);
  document.forms[2].cpmin.value  = -1.0 / (0.7 * machVal * machVal);

  document.forms[2].reno.value   = reynolds;
  document.forms[2].cfl.value    = cflam;
  document.forms[2].cft.value    = cfturb;
}
*/

void planet::render_diagram(double scale, bool labels) {
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
  if(thisradius * scale < 1.0) {
    thisradius = 1.0 / scale;
  }

  // draw a filled circle at the radius
  double const circlestep = M_PI / 32.0;
  glColor4dv(Vector4d(0.25, 0.25, 0.25, 1.0));
  glBegin(GL_TRIANGLE_FAN);
  glVertex3d(0.0, 0.0, 0.0);
  for(double angle = 0.0; angle < (M_PI * 2.0) + circlestep; angle += circlestep) {
    glVertex3d(sin(angle) * thisradius, cos(angle) * thisradius, 0.0);
  }
  glEnd();
  // circle outline
  //glDisable(GL_DEPTH_TEST);
  //glColor4dv(Vector4d(1.0, 1.0, 1.0, 1.0));
  //glBegin(GL_LINE_LOOP);
  //for(double angle = 0.0; angle <= M_PI * 2.0; angle += circlestep) {
  //  glVertex3d(sin(angle) * thisradius, cos(angle) * thisradius, 0.0);
  //}
  //glEnd();
  //glEnable(GL_DEPTH_TEST);

  // atmosphere
  glColor4dv(Vector4d(0.2, 0.5, 0.5, 1.0));
  glBegin(GL_LINES);
  for(double angle = 0.0; angle <= M_PI * 2.0; angle += circlestep) {
    glVertex3d(sin(angle) * (thisradius + 100000), cos(angle) * (thisradius + 100000), 0.0);
  }
  glEnd();

  // restore rotation
  glPopMatrix();
}

void planet::render_visible() {
  /// Render in the visible spectrum
  glPushMatrix();
  // move into position
  glTranslated(position.x, position.y, position.z);

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.8, 0.8, 0.8, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(0.8, 0.8, 0.8, 1.0));
  glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialf(GL_FRONT,  GL_SHININESS,           20.0);                           // 0 to 127

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

  random_reset();     // TESTING ONLY

  // 5 faces around point 0
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[0]); glVertex3dv(points[11]); glVertex3dv(points[5]);
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[0]); glVertex3dv(points[5 ]); glVertex3dv(points[1]);
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[0]); glVertex3dv(points[1 ]); glVertex3dv(points[7]);
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[0]); glVertex3dv(points[7 ]); glVertex3dv(points[10]);
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[0]); glVertex3dv(points[10]); glVertex3dv(points[11]);
  // 5 adjacent faces
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[1 ]); glVertex3dv(points[5 ]); glVertex3dv(points[9]);
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[5 ]); glVertex3dv(points[11]); glVertex3dv(points[4]);
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[11]); glVertex3dv(points[10]); glVertex3dv(points[2]);
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[10]); glVertex3dv(points[7 ]); glVertex3dv(points[6]);
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[7 ]); glVertex3dv(points[1 ]); glVertex3dv(points[8]);
  // 5 faces around point 3
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[3]); glVertex3dv(points[9]); glVertex3dv(points[4]);
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[3]); glVertex3dv(points[4]); glVertex3dv(points[2]);
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[3]); glVertex3dv(points[2]); glVertex3dv(points[6]);
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[3]); glVertex3dv(points[6]); glVertex3dv(points[8]);
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[3]); glVertex3dv(points[8]); glVertex3dv(points[9]);
  // 5 adjacent faces
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[4]); glVertex3dv(points[9]); glVertex3dv(points[5 ]);
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[2]); glVertex3dv(points[4]); glVertex3dv(points[11]);
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[6]); glVertex3dv(points[2]); glVertex3dv(points[10]);
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[8]); glVertex3dv(points[6]); glVertex3dv(points[7 ]);
  glMaterialfv(GL_FRONT, GL_EMISSION, Vector4f(get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), get_random_double(0.0, 1.0), 1.0));
  glVertex3dv(points[9]); glVertex3dv(points[8]); glVertex3dv(points[1 ]);
  glEnd();

  // restore rotation
  glPopMatrix();
}

void planet::render_radio() {
  /// Render in the radio spectrum, i.e. radar reflection
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}

void planet::render_infrared() {
  /// Render in the infrared spectrum
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}

void planet::render_ultraviolet() {
  /// Render in the ultraviolet spectrum
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}
