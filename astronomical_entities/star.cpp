#include "star.h"

star::star()
  : brightness(1.0) {
  /// Default constructor
  name = "Sun";
  description = "A massive, luminous sphere of plasma.";
}

star::~star() {
  /// Default destructor
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
      result.r = (temperature - 7.98) / (2.02 * 255);       // Draper point
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
