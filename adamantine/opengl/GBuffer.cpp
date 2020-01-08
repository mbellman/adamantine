#include "opengl/GBuffer.h"

void GBuffer::allowReading() {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, buffer);
}

void GBuffer::allowWriting() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buffer);
}

void GBuffer::attach(Attachment attachment) {
  glReadBuffer(attachment);
}

void GBuffer::initialize(int width, int height) {
  glGenFramebuffers(1, &buffer);
  glBindFramebuffer(GL_FRAMEBUFFER, buffer);

  // Create color buffer
  glGenTextures(1, &colorBuffer);
  glBindTexture(GL_TEXTURE_2D, colorBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, Attachment::COLOR, GL_TEXTURE_2D, colorBuffer, 0);

  // Create normal buffer
  glGenTextures(1, &normalBuffer);
  glBindTexture(GL_TEXTURE_2D, normalBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, Attachment::NORMAL, GL_TEXTURE_2D, normalBuffer, 0);

  // Create depth buffer
  glGenRenderbuffers(1, &depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, Attachment::DEPTH, GL_RENDERBUFFER, depthBuffer);

  GLenum buffers[] = { Attachment::COLOR, Attachment::NORMAL };
  glDrawBuffers(2, buffers);

  useDefaultFrameBuffer();
}

void GBuffer::useDefaultFrameBuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}