#include "opengl/FrameBuffer.h"

#include <cstdio>

FrameBuffer::FrameBuffer(int width, int height) {
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  size.width = width;
  size.height = height;
}

FrameBuffer::~FrameBuffer() {

}

void FrameBuffer::addColorTexture(unsigned int internalFormat, unsigned int format) {
  ColorTexture texture;

  texture.internalFormat = internalFormat;
  texture.format = format;
  texture.attachment = GL_COLOR_ATTACHMENT0 + colorTextures.size();

  glGenTextures(1, &texture.id);
  glBindTexture(GL_TEXTURE_2D, texture.id);
  glTexImage2D(GL_TEXTURE_2D, 0, texture.internalFormat, size.width, size.height, 0, texture.format, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, texture.attachment, GL_TEXTURE_2D, texture.id, 0);

  colorTextures.push_back(texture);
}

void FrameBuffer::addDepthBuffer() {
  glGenRenderbuffers(1, &depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size.width, size.height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
}

void FrameBuffer::addDepthTexture() {
  glGenTextures(1, &depthTexture);
  glBindTexture(GL_TEXTURE_2D, depthTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size.width, size.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::initializeColorTextures() {
  GLuint* attachments = new GLuint[colorTextures.size()];

  for (int i = 0; i < colorTextures.size(); i++) {
    attachments[i] = colorTextures[i].attachment;
  }

  glDrawBuffers(colorTextures.size(), attachments);

  delete[] attachments;
}

void FrameBuffer::startReading() {
  if (depthTexture > 0) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
  } else {
    for (int i = 0; i < colorTextures.size(); i++) {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, colorTextures[i].id);
    }
  }

  glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
}

void FrameBuffer::startWriting() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}