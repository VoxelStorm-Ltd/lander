#ifndef PLANET_H_INCLUDED
#define PLANET_H_INCLUDED

#include "orbitalbody.h"
#include "corner.h"
#include "region.h"

class planet : public orbitalbody {
  /// An astronomical object large enough to be rounded by its own gravity but not to undergo fusion
private:
  double regionwidth_linear;
  double regionwidth_curved;

  unsigned int region_subdivisions;
  double chunkwidth_linear;
  double chunkwidth_curved;

  corner corners[12];
  region regions[20];

public:
  double atmos_molarmass;                                                       // average molar mass of the atmosphere
  double atmos_pressure_base;                                                   // pressure at sea level or equivalent
  double atmos_temperature_base;                                                // temperature at sea level or equivalent
  //double atmos_tropopause;                                                      // top radius of various layers of atmopause
  //double atmos_stratopause;
  //double atmos_mesopause;
  double atmos_thermopause;
  //double atmos_exopause;

  planet();
  virtual ~planet();

  // getters and setters
  virtual std::string get_type();
  virtual double get_mass();
  virtual double get_radius();

  virtual bool check_within_physical_influence_rel(double thisradius);
  virtual double get_atmos_pressure(           double thisradius);
  virtual double get_atmos_temperature(        double thisradius);

  // update
  virtual void update_model();

  // render
  virtual void render_diagram(double scale, bool labels = false);
  virtual void render_visible(unsigned int depth = 2);
  virtual void render_radio();
  virtual void render_infrared();
  virtual void render_ultraviolet();
};

#endif // PLANET_H_INCLUDED
