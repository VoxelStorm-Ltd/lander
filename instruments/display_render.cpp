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
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);


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
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGB, 512, 512);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, colourbuffer);

  // depth buffer
  GLuint depthbuffer;
  glGenRenderbuffersEXT(1, &depthbuffer);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthbuffer);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, 512, 512);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,   GL_RENDERBUFFER_EXT, depthbuffer);

  // stencil buffer
  //GLuint stencilbuffer;
  //glGenRenderbuffersEXT(1, &stencilbuffer);
  //glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, stencilbuffer);
  //glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_STENCIL_COMPONENTS, 512, 512);
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
    //ports_in[0].target->get_port_out_video_analogue(ports_in[0].target_port, framebuffer, Vector2d(512, 512));
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

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, display_image);        // bind the screen texture
  // generate an empty image
  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);  // NULL = leave data undefined
  GLubyte temp_buffer[512][512][3];
  // analogue tv style banded white noise:
  for(unsigned int x = 0; x != 512; ++x) {
    double const band = get_random_double(0.5, 1.0);
    for(unsigned int y = 0; y != 512; ++y) {
      unsigned char const value = get_random_int(63, 255) * band;
      temp_buffer[x][y][0] = value;
      temp_buffer[x][y][1] = value;
      temp_buffer[x][y][2] = value;
    }
  }
  glTexImage2D(GL_TEXTURE_2D,       // target
               0,                   // mipmap level
               GL_RGB,              // internalFormat
               512, 512,            // dimensions
               0,                   // border
               GL_RGB,              // format
               GL_UNSIGNED_BYTE,    // type of pixel data (GLubyte), see http://www.opengl.org/sdk/docs/man/xhtml/glTexImage2D.xml
               &temp_buffer);       // buffer or NULL to leave undefined
  // texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);                // nearest neighbour filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);     // emissive style glow effect - see http://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);



  glColor4dv(Vector4d(0.2, 0.2, 0.2, 1.0));
  //glColor4dv(Vector4d(0.5, 0.0, 0.0, 1.0));
  //glColor4dv(Vector4d(1.0, 1.0, 1.0, 1.0));

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

  ///// check if we have an input signal
  ///if(ports_in[0].target) {
  ///  // call the target's render function
  ///  ports_in[0].target->get_port_out_video_analogue(ports_in[0].target_port, 0, Vector2d(512, 512));
  ///} else {
  ///  // draw a "no signal" output
  ///  // TODO
  ///}

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);          // unbind the framebuffer

  glPopMatrix();
}
