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

void FrameBuffer::addColorTexture(GLint internalFormat, GLenum format) {
  addColorTexture(internalFormat, format, GL_CLAMP_TO_BORDER);
}

void FrameBuffer::addColorTexture(GLint internalFormat, GLenum format, GLint clamp) {
  ColorTexture texture;

  texture.internalFormat = internalFormat;
  texture.format = format;
  texture.attachment = GL_COLOR_ATTACHMENT0 + colorTextures.size();

  float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

  glGenTextures(1, &texture.id);
  glBindTexture(GL_TEXTURE_2D, texture.id);
  glTexImage2D(GL_TEXTURE_2D, 0, texture.internalFormat, size.width, size.height, 0, texture.format, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp);
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, texture.attachment, GL_TEXTURE_2D, texture.id, 0);

  colorTextures.push_back(texture);
}

void FrameBuffer::addDepthStencilBuffer() {
  // glGenRenderbuffers(1, &depthStencilBuffer);
  // glBindRenderbuffer(GL_RENDERBUFFER, depthStencilBuffer);
  // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.width, size.height);
  // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilBuffer);
  glGenTextures(1, &depthStencilBuffer);
  glBindTexture(GL_TEXTURE_2D, depthStencilBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, size.width, size.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilBuffer, 0);
}

void FrameBuffer::bindColorTexture(GLuint attachment) {
  glDrawBuffer(attachment);
}

void FrameBuffer::bindColorTextures() {
  GLuint* attachments = new GLuint[colorTextures.size()];

  for (int i = 0; i < colorTextures.size(); i++) {
    attachments[i] = colorTextures[i].attachment;
  }

  glDrawBuffers(colorTextures.size(), attachments);

  delete[] attachments;
}

void FrameBuffer::clearColorTexture(GLint attachment) {
  float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

  glClearBufferfv(GL_COLOR, attachment, black);
}

void FrameBuffer::shareDepthStencilBuffer(FrameBuffer* target) {
  glBindFramebuffer(GL_FRAMEBUFFER, target->fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilBuffer, 0);
}

void FrameBuffer::startReading() {
  for (int i = 0; i < colorTextures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, colorTextures[i].id);
  }

  glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
}

void FrameBuffer::startWriting() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void FrameBuffer::transferColorTexture(GLenum source, const GLenum* targets, int size) {
  int width = this->size.width;
  int height = this->size.height;

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glReadBuffer(source);
  glDrawBuffers(size, targets);
  glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}