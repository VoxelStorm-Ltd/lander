#include "display.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void display::render() {
  /// Render this display's contents in the right place
  glPushMatrix();

  glTranslated(position.x,
               position.y,
               position.z);

  /*
  GLuint framebuffer = 0;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, display_image);        // bind the screen texture

  // generate an empty image
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_width, screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);


  //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);  // when texture area is small, bilinear filter the closest mipmap
  //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);                 // when texture area is large, bilinear filter the original
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);                // nearest neighbour filtering
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);   // nvidia recommended (see https://developer.nvidia.com/sites/default/files/akamai/gamedev/docs/opengl_rendertexture.pdf)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // texture parameters
  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);     // emissive style glow effect - see http://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  //wglGetExtensionsStringARB();     // nvidia requirement

  // colour buffer
  GLuint colourbuffer;
  glGenRenderbuffersEXT(1, &colourbuffer);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, colourbuffer);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGB, screen_width, screen_height);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, colourbuffer);

  // depth buffer
  GLuint depthbuffer;
  glGenRenderbuffersEXT(1, &depthbuffer);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthbuffer);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, screen_width, screen_height);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,   GL_RENDERBUFFER_EXT, depthbuffer);

  // stencil buffer
  //GLuint stencilbuffer;
  //glGenRenderbuffersEXT(1, &stencilbuffer);
  //glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, stencilbuffer);
  //glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_STENCIL_COMPONENTS, screen_width, screen_height);
  //glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, stencilbuffer);

  // Set texture as our colour attachment #0
  ///glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, display_image, 0);

  // set the list of draw buffers.
  GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0_EXT};
  glDrawBuffers(1, DrawBuffers);                          // "1" is the size of DrawBuffers
  // check that our framebuffer is ok
  if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT) {
    std::cout << "ERROR: frame buffer creation failed, status " << glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) << std::endl;
  }

  // render to texture
  // check if we have an input signal
  if(ports_in[0].target) {
    // call the target's render function
    //ports_in[0].target->get_port_out_video_analogue(ports_in[0].target_port, framebuffer, Vector2d(screen_width, screen_height));
  } else {
    // draw a "no signal" output
    // TODO
  }
  */

  /*
  // DEBUG ONLY:
  glClearColor(0.0, 0.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);          // unbind the framebuffer
  */

  if(ports_in[0].target) {
    // cache the old viewport
    Vector4i oldviewport;
    glGetIntegerv(GL_VIEWPORT, oldviewport);
    glViewport(0, 0, screen_width, screen_height);
    // bind the framebuffer for the display screen
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
    // call the target's render function
    ports_in[0].target->get_port_out_video_analogue(ports_in[0].target_port, Vector2d(screen_width, screen_height));
    // release the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(oldviewport[0], oldviewport[1], oldviewport[2], oldviewport[3]);
  } else {
    glBindTexture(GL_TEXTURE_2D, display_image);        // bind the screen texture
    // analogue tv style banded white noise:
    GLubyte temp_buffer[screen_width][screen_height][3];
    for(unsigned int x = 0; x != screen_width; ++x) {
      double const band = get_random_double(0.5, 1.0);
      for(unsigned int y = 0; y != screen_height; ++y) {
        unsigned char const value = get_random_int(63, 255) * band;
        temp_buffer[x][y][0] = value;                               // uniform b&w noise
        temp_buffer[x][y][1] = value;
        temp_buffer[x][y][2] = value;
        //temp_buffer[x][y][0] = value + get_random_int(-25, 25);     // pale colour noise
        //temp_buffer[x][y][1] = value + get_random_int(-25, 25);
        //temp_buffer[x][y][2] = value + get_random_int(-25, 25);
      }
    }
    glTexImage2D(GL_TEXTURE_2D,       // target
                 0,                   // mipmap level
                 GL_RGB,              // internalFormat
                 screen_width, screen_height,            // dimensions
                 0,                   // border
                 GL_RGB,              // format
                 GL_UNSIGNED_BYTE,    // type of pixel data (GLubyte), see http://www.opengl.org/sdk/docs/man/xhtml/glTexImage2D.xml
                 &temp_buffer);       // buffer or NULL to leave undefined
    glBindTexture(GL_TEXTURE_2D, 0);                              // release the screen texture
  }

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);        // unbind the framebuffer


  glColor4dv(Vector4d(0.2, 0.2, 0.2, 1.0));
  //glColor4dv(Vector4d(0.5, 0.0, 0.0, 1.0));
  //glColor4dv(Vector4d(1.0, 1.0, 1.0, 1.0));

  glBindTexture(GL_TEXTURE_2D, display_image);        // bind the screen texture
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 0.0);
  glVertex3d(0.0,    0.0,    size.z);
  glTexCoord2d(1.0, 0.0);
  glVertex3d(size.x, 0.0,    size.z);
  glTexCoord2d(1.0, 1.0);
  glVertex3d(size.x, size.y, size.z);
  glTexCoord2d(0.0, 1.0);
  glVertex3d(0.0,    size.y, size.z);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);                    // unbind the texture
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);        // unbind the framebuffer

  glPopMatrix();
}
