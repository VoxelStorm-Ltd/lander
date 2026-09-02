#include "universe.h"
#include "starsystem.h"
#include "star.h"
#include "astronaut.h"
#include "spacecraft.h"
#include "device.h"
#include <iostream>

universe::universe() {
  /// Default constructor
}

universe::~universe() {
  /// Default destructor
}

void universe::render_visible() {
  /// Render whatever needs to be rendered
  for(auto &it : starsystems) {
    if(it = currentsystem) {
      it->render_visible();
    } else {
      // TODO: move this to the right bit of the sky and make sure near/far planes make sense
      if(it->primary) {
        it->primary->render_visible();
      }
    }
  }
  // TODO: galaxies and nebulae and other decorations
}

void universe::make_explosion(vector3d const &coords, double energy) {
  /// Create an explosion at the specified coords, and calculate the results
  if(energy > 1000000000000000000000000000000.0) {                              // supernova
    std::cout << "The incredible impact produced "
              << energy / 1000000000000000000000000000000.0
              << " times as much energy as a supernova, lighting up the night sky throughout the whole galaxy." << std::endl;
    std::cout << "The entire " << currentsystem->get_name() << " star system is destroyed!" << std::endl;
    // wipe out the entire damn system and then some - 30 lightyears effect radius
    destroy_range(coords, 2838158520000000000000000000.0);
  } else if(energy > 420000000000000000000000.0) {                              // 4.2*10^23 J - Chixslub crater
    std::cout << "The impact produces an blinding explosion, releasing " << energy / 420000000000000000000000.0 << " times as much energy as the asteroid impact that wiped out the dinosaurs." << std::endl;
    std::cout << "The flash lights up the entirety of the " << currentsystem->get_name() << ", and the afterglow persists for days." << std::endl;
    destroy_range(coords, 300000.0);
  } else if(energy > 2510000000000000000000.0) {                                // 600000 megaton TNT - Shoemaker–Levy 9 impacting Jupiter
    std::cout << "The impact produces an incredible fireball, releasing " << energy / 2510000000000000000000.0 << " times as much energy as the Shoemaker-Levy 9 comet's impact on Jupiter." << std::endl;
    std::cout << "The flash is seen even in daylight on every planet in the " << currentsystem->get_name() << "." << std::endl;
    destroy_range(coords, 300000.0);
  } else if(energy > 209200000000000000.0) {                                    // 50 megaton TNT
    std::cout << "The impact produces an incredible fireball, releasing " << energy / 209200000000000000.0 << " times as much energy as the largest nuclear bomb ever detonated." << std::endl;
    std::cout << "The damage extends for hundreds of kilometers." << std::endl;
    destroy_range(coords, 300000.0);
  } else if(energy > 125520000000000000.0) {                                    // Tunguska event
    std::cout << "The impact produced a massive fireball, releasing " << energy / 125520000000000000.0 << " times as much energy as the Tunguska impact event." << std::endl;
    std::cout << "Extensive damage occurs in the surrounding area." << std::endl;
    destroy_range(coords, 200000.0);
  } else if(energy > 4184000000000000.0) {                                      // 1 megaton TNT
    std::cout << "The impact produced a large fireball, the equivalent of " << energy / 4184000000000000.0 << " megatons of TNT." << std::endl;
    std::cout << "Vast damage occurs in the surrounding area." << std::endl;
    destroy_range(coords, 100000.0);
  } else if(energy > 4184000000000.0) {                                         // 1 kiloton TNT
    std::cout << "The impact produced a frightening fireball, the equivalent of " << energy / 4184000000000.0 << " kilotons of TNT." << std::endl;
    std::cout << "Serious damage occurs throughout the surrounding area." << std::endl;
    destroy_range(coords, 15000.0);
  } else if(energy > 4184000000.0) {                                            // 1 ton of TNT
    std::cout << "The impact produced a large explosion, the equivalent of " << energy / 4184000000.0 << " tons of TNT." << std::endl;
    std::cout << "The nearby area is showered with debris." << std::endl;
    destroy_range(coords, 500.0);
  } else {
    // generic small explosion
    std::cout << "The impact released " << energy << "J kinetic energy" << std::endl;
    destroy_range(coords, 15.0);
  }
}

void universe::destroy_range(vector3d const &coords, double range) {
  /// Kill all astronauts outside within the range, damage random ship systems
  /// within the range and destroy all ships within half of the range
  for(auto const &it : currentsystem->bodies) {
    astronaut *thisastro = dynamic_cast<astronaut*>(it);
    if(thisastro) {
      if(vector3d(thisastro->position - coords).length() < range) {
        std::cout << "" << thisastro->get_name() << " perished in the fireball." << std::endl;
        thisastro->kill();
        continue;
      }
    }
    spacecraft *thiscraft = dynamic_cast<spacecraft*>(it);
    if(thiscraft) {
      double const distance = vector3d(thiscraft->position - coords).length();
      if(distance < range / 2.0) {
        std::cout << "" << thiscraft->get_name() << "'s shielding could not withstand the explosion." << std::endl;
        thiscraft->destroy();
      } else if(distance < range) {
        // destroy the more vulnerable of the ship's systems
        for(auto &it : thiscraft->devices) {
          it->destroy();
          ++it;                                                                 // skip every second device

          // TODO: implement various damage types and a device damage tolerance check
          //it->damage_radiation();
          //it->damage_impact();
        }
      }
      continue;
    }
  }
}
