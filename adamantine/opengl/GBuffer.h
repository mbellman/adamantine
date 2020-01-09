#include "glew.h"
#include "glut.h"

class GBuffer {
public:
  enum Attachment {
    COLOR = GL_COLOR_ATTACHMENT0,
    NORMAL_DEPTH = GL_COLOR_ATTACHMENT1,
    DEPTH = GL_DEPTH_ATTACHMENT
  };

  void attach(Attachment attachment);
  void bindTextures();
  void initialize(int width, int height);
  void startReading();
  void startWriting();
  void useDefaultFrameBuffer();

private:
  GLuint buffer;
  GLuint colorTexture;
  GLuint normalDepthTexture;
  GLuint depthRBO;
};