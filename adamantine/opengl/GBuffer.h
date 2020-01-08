#include "glew.h"
#include "glut.h"

class GBuffer {
public:
  enum Attachment {
    COLOR = GL_COLOR_ATTACHMENT0,
    NORMAL = GL_COLOR_ATTACHMENT1,
    DEPTH = GL_DEPTH_ATTACHMENT
  };

  void allowReading();
  void allowWriting();
  void attach(Attachment attachment);
  void initialize(int width, int height);
  void useDefaultFrameBuffer();

private:
  GLuint buffer;
  GLuint colorBuffer;
  GLuint normalBuffer;
  GLuint depthBuffer;
};