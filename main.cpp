#include <cstdlib>
#include <iostream>
#include <boost/chrono.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FTGL/ftgl.h>
#include "vmath.h"
#include "oculusstorm.h"
#include "menu.h"
#include "universe.h"
#include "starsystem.h"
#include "body.h"
#include "astronaut.h"
#include "spacecraft.h"

// globals
bool keeprunning = true;
GLFWwindow *window_main = nullptr;          // the main game window
universe root;
astronaut *player       = nullptr;
menu *menu_target       = nullptr;          // what object's menu we currently have open, if any
oculusstorm *oculus     = nullptr;          // oculus rift controller
FTFont *font_title      = nullptr;          // global font definitions
FTFont *font_title_huge = nullptr;
FTFont *font_text       = nullptr;
FTFont *font_title3d    = nullptr;
FTFont *font_text3d     = nullptr;
unsigned int fps = 0;                       // fps counter

void init_graphics(Vector2i &windowsize);
void init_universe();

void init();
void mainloop();

void pollcontrols(GLFWwindow *thiswindow);

int main() {
  init();
  mainloop();
  return EXIT_SUCCESS;
}

void init() {       /// all the one-time initialisation we need for the engine
  Vector2i windowsize(800, 600);
  init_graphics(windowsize);
  init_universe();
  player->windowsize = windowsize;
  player->update_aspect_ratio();
  std::cout << "Initialisation complete." << std::endl;
  // these must be absolutely last:
  glfwSetTime(0.0);                                                           // reset the timer for the start of the main loop
}

void mainloop() {   /// the main rendering loop
  // fps counter setup
  boost::chrono::time_point<boost::chrono::high_resolution_clock, boost::chrono::duration<double>> time_fpsupdate(boost::chrono::high_resolution_clock::now());
  unsigned int frames_last_second = 0;

  //double deltatime = 5.0;
  double deltatime = 1.0;
  //double deltatime = 1.0 / 10.0;
  //double deltatime = 1.0 / 160.0;
  for(root.time = 0.0; keeprunning; root.time += deltatime) {
    // update the orbits for the orbital bodies in the current system
    for(auto const &it : root.currentsystem->bodies) {
      if(it) {
        it->update_state(root.time, deltatime);
      }
    }

    glfwPollEvents();
    pollcontrols(window_main);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    player->render_firstperson();

    glfwSwapBuffers(window_main);

    // fps counter update
    boost::chrono::time_point<boost::chrono::high_resolution_clock, boost::chrono::duration<double>> const time_now(boost::chrono::high_resolution_clock::now());
    if(time_now >= time_fpsupdate) {
      fps = frames_last_second;
      frames_last_second = 0;
      time_fpsupdate = time_now + boost::chrono::duration<double>(boost::chrono::milliseconds(1000));
      //std::cout << "FPS: " << fps << std::endl;
    } else {
      ++frames_last_second;
    }
  }
}

