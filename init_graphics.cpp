#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FTGL/ftgl.h>
#include "vectorstorm/vectorstorm.h"
#include "version.h"

// globals
extern GLFWwindow *window_main;
extern FTFont *font_title;                                                      // global font definitions
extern FTFont *font_title_huge;
extern FTFont *font_text;
extern FTFont *font_title3d;
extern FTFont *font_text3d;
extern FTFont *font_7segment3d;
extern FTFont *font_monospace_small;

void callback_mousepos(    GLFWwindow *thiswindow, double x, double y);
void callback_mousebutton( GLFWwindow *thiswindow, int button, int action, int mods);
void callback_key(         GLFWwindow *thiswindow, int key, int scancode, int action, int mods);
void callback_scroll(      GLFWwindow *thiswindow, double xoffset, double yoffset);
void callback_windowresize(GLFWwindow *thiswindow, int newwidth, int newheight);
void callback_windowclose( GLFWwindow *thiswindow);

FTFont *font_load(  std::string const &filename, unsigned int size = 16);
FTFont *font_load3d(std::string const &filename, unsigned int size = 16);


void init_graphics(vector2i &windowsize) {
  std::cout << "Initialising graphics..." << std::endl;
  // initialise the opengl window
  if(glfwInit() != GL_TRUE) {
    std::cout << "ERROR: glfwInit() failed" << std::endl;
    _Exit(EXIT_FAILURE);
  }

  int nummonitors = 0;
  GLFWmonitor **monitor_list = glfwGetMonitors(&nummonitors);
  GLFWmonitor *monitor_primary = glfwGetPrimaryMonitor();
  std::cout << "Monitors: " << nummonitors << std::endl;
  for(int monitornum = 0; monitornum != nummonitors; ++monitornum) {
    GLFWmonitor *thismonitor = monitor_list[monitornum];
    int physicalwidth  = 0;
    int physicalheight = 0;
    int xpos = 0;
    int ypos = 0;
    glfwGetMonitorPhysicalSize(thismonitor, &physicalwidth, &physicalheight);
    glfwGetMonitorPos(thismonitor, &xpos, &ypos);
    const GLFWvidmode *videomode = glfwGetVideoMode(thismonitor);

    std::cout << "Monitor " << monitornum;
    if(thismonitor == monitor_primary) {
      std::cout << " (primary)";
    }
    std::cout << std::endl;
    std::cout << "  Name: " << glfwGetMonitorName(thismonitor) << std::endl;
    std::cout << "  Physical size: " << physicalwidth << " " << physicalheight << std::endl;
    std::cout << "  Position: " << xpos << " " << ypos << std::endl;
    std::cout << "  Mode: " << videomode->width << " " << videomode->height << " " << videomode->refreshRate << std::endl;
  }
  // set up window hints in advance
  //glfwWindowHint(GLFW_RED_BITS,   state->videomode->redBits);
  //glfwWindowHint(GLFW_GREEN_BITS, state->videomode->greenBits);
  //glfwWindowHint(GLFW_BLUE_BITS,  state->videomode->blueBits);
  //glfwWindowHint(GLFW_DEPTH_BITS, 32);
  //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
  //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);                         // forward compat disables all deprecated functions - we don't want that
  //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 8);

  glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_RESET_NOTIFICATION);
  glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
  window_main = glfwCreateWindow(windowsize.x,
                                 windowsize.y,
                                 "Lander",
                                 NULL,
                                 NULL);
  if(!window_main) {
    // exit if this didn't work
    std::cout << "ERROR: glfwOpenWindow returned NULL" << std::endl;
    _Exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window_main);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glfwShowWindow(window_main);                                                  // only display the window once in position
  glfwSetWindowTitle(window_main, "Lander alpha: Loading...");

  glfwSetWindowCloseCallback(window_main, callback_windowclose);                // callback for window closing

  glewExperimental = GL_TRUE;
  GLenum const glew_status = glewInit();
  bool const glew_failed = glew_status != GLEW_OK
#ifdef GLEW_ERROR_NO_GLX_DISPLAY
                        && glew_status != GLEW_ERROR_NO_GLX_DISPLAY
#endif
                        ;
  if(glew_failed) {
    std::cout << "ERROR: GLEW returned " << glew_status << ": "
              << glewGetErrorString(glew_status) << std::endl;
    _Exit(EXIT_FAILURE);
  }
  std::cout << "GL_VERSION:  " << glGetString(GL_VERSION)  << std::endl;
  std::cout << "GL_VENDOR:   " << glGetString(GL_VENDOR)   << std::endl;
  std::cout << "GL_RENDERER: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "GL version major " << glfwGetWindowAttrib(window_main, GLFW_CONTEXT_VERSION_MAJOR)
            << " minor "           << glfwGetWindowAttrib(window_main, GLFW_CONTEXT_VERSION_MINOR)
            << " revision "        << glfwGetWindowAttrib(window_main, GLFW_CONTEXT_REVISION)
            << " API "             << glfwGetWindowAttrib(window_main, GLFW_CLIENT_API)
            << " profile  "        << glfwGetWindowAttrib(window_main, GLFW_OPENGL_PROFILE) << std::endl;;
  if(!GLEW_ARB_vertex_array_object) {
    std::cout << "GLEW_ARB_vertex_array_object not available..." << std::endl;
    if(!GLEW_ATI_vertex_array_object) {
      if(!GLEW_APPLE_vertex_array_object) {
        //config->hasvao = false;
      }
    }
  }
  if(glGenVertexArrays == 0) {
    std::cout << "glGenVertexArrays does not appear to be supported." << std::endl;
    //config->hasvao = false;
  }

  glFrontFace(GL_CCW);                                                          // set up counter-clockwise polygon winding
  glCullFace(GL_BACK);                                                          // may be redundant to cull back-faces
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);                                                      // go on, use the zbuffer
  glEnable(GL_DITHER);                                                          // may marginally increase shading quality
  //glEnable(GL_LIGHTING);                                                        // obviously we want lighting... right?
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);                          // enable local lighting
  //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);                         // to make flat shading ok
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,     GL_FALSE);                         // ensure one-sided lighting
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glEnable(GL_MINMAX);                                                          // allow min and max colour tables for HDR effects
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);                                    // filled
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);                                    // wireframe
  glShadeModel(GL_SMOOTH);                                                      // SMOOTH or FLAT
  //glShadeModel(GL_FLAT);    //may look more spectacular for a cube world
  glDisable(GL_NORMALIZE);
  //glDisable(GL_RESCALE_NORMALS);
  glHint(GL_GENERATE_MIPMAP_HINT, GL_FASTEST);                                  // GL_FASTEST, GL_NICEST or GL_DONT_CARE


  srand(1337);                                                                  // seed the random generator predictably

  // fog settings
  glDisable(GL_FOG);
  //vector4f fogcolour(0.82, 0.85, 1.0, 1);
  //glFogfv(GL_FOG_COLOR, fogcolour);
  //glFogi(GL_FOG_MODE, GL_EXP2);                                                 // GL_LINEAR GL_EXP GL_EXP2
  //glFogf(GL_FOG_DENSITY, 0.00005);                                              // only used for exponential fog
  //glFogi(GL_FOG_START, 10);                                                     // only used for linear fog
  //glFogi(GL_FOG_END, 4000);

  /*// temporary material definition
  GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat mat_shininess[] = {50.0};
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);*/

  // set up some lights
  glEnable(GL_LIGHTING);
  ////GLfloat ambientlightcol[] = {.25,0,0,1};
  //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, vector4f(0.0, 0.0, 0.5, 1.0));         // global ambient
  ////glLightfv(GL_LIGHT0, GL_AMBIENT, vector4f(0.0, 0.0, 0.5, 1.0));
  //glLightfv(GL_LIGHT0, GL_DIFFUSE,  vector4f(0.8, 0.7, 0.5, 1.0));
  //glLightfv(GL_LIGHT0, GL_SPECULAR, vector4f(1.0, 1.0, 0.5, 1.0));
  //glEnable(GL_LIGHT0);
  ////glLightfv(GL_LIGHT1, GL_POSITION, directionallightdir);
  ////glLightfv(GL_LIGHT1, GL_DIFFUSE,  directionallightdiff);
  ////glLightfv(GL_LIGHT1, GL_SPECULAR, directionallightspec);
  ////glEnable(GL_LIGHT1);

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // initialise fonts
  //fontconsole = font_load("resources/FuturaExt-Normal.ttf");
  font_title           = font_load(  "resources/Futurak.ttf");
  font_title3d         = font_load3d("resources/Futurak.ttf");
  font_title_huge      = font_load3d("resources/Futurak.ttf", 100);
  font_text            = font_load(  "resources/FuturaExt-Normal.ttf");
  font_text3d          = font_load3d("resources/FuturaExt-Normal.ttf");
  font_7segment3d      = font_load3d("resources/7-Segment-Display-Extended.ttf");
  font_monospace_small = font_load(  "resources/FSEX300.ttf", 12);
  //font_monospace_small = font_load(  "resources/Perfect DOS VGA 437.ttf", 12);
  //font_monospace_small = font_load(  "resources/5by7.ttf", 12);
  if(!font_title ||
     !font_title_huge ||
     !font_text ||
     !font_title3d ||
     !font_text3d ||
     !font_7segment3d ||
     !font_monospace_small) {
    std::cout << "Failed to load one or more fonts - exiting" << std::endl;
    exit(EXIT_FAILURE);
  }

  // callbacks
  glfwSetCursorPosCallback(  window_main, callback_mousepos);
  glfwSetMouseButtonCallback(window_main, callback_mousebutton);
  glfwSetKeyCallback(        window_main, callback_key);
  glfwSetScrollCallback(     window_main, callback_scroll);
  glfwSetWindowSizeCallback( window_main, callback_windowresize);
  glfwSetWindowCloseCallback(window_main, callback_windowclose);
  // input settings
  //glfwSetInputMode(window_main, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  //glfwSetInputMode(window_main, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  glfwSetInputMode(window_main, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  std::stringstream title;
  title << "Lander " << AutoVersion::STATUS << " " << AutoVersion::FULLVERSION_STRING;
  glfwSetWindowTitle(window_main, title.str().c_str());                         // set the title to the main run's title
  std::cout << "Graphics initialised." << std::endl;
}
