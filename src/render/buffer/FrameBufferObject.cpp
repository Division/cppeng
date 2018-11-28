//
// Created by Sidorenko Nikita on 11/25/18.
//

#include "FrameBufferObject.h"
#include "render/texture/Texture.h"
#include "system/Logging.h"

FrameBufferObject::FrameBufferObject(unsigned int width, unsigned int height, bool color, bool depth) {
  if (!color && !depth) {
    throw std::runtime_error("either color or depth should be enabled for FBO");
  }

  glGenFramebuffers(1, &_fboID);
  glBindFramebuffer(GL_FRAMEBUFFER, _fboID);

  if (color) {
    _colorBuffer = std::make_shared<Texture>();
    _colorBuffer->initTexture2D(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, nullptr, false);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorBuffer->id(), 0);
  } else {
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
  }

  if (depth) {
    _depthBuffer = std::make_shared<Texture>();
    _depthBuffer->initTexture2D(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr, false);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthBuffer->id(), 0);
  }

  auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    auto statusString = _getStatusString(status);
    ENGLog("Framebuffer incomplete: %s", statusString.c_str());
    throw std::runtime_error("Framebuffer incomplete: " + statusString);
  } else {
    ENGLog("FBO created (%ix%i) Color: (%i), Depth: (%i)", width, height, color, depth);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  engine::checkGLError();
}

FrameBufferObject::~FrameBufferObject() {
  glDeleteFramebuffers(1, &_fboID);
}

std::string FrameBufferObject::_getStatusString(GLenum status) {
  switch (status) {
    case GL_FRAMEBUFFER_UNDEFINED: return "GL_FRAMEBUFFER_UNDEFINED";
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
    case GL_FRAMEBUFFER_UNSUPPORTED: return "GL_FRAMEBUFFER_UNSUPPORTED";
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
    case 0: return "GL_FRAMEBUFFER_COMPLETE";
    default: return "Unknown";
  }
}

void FrameBufferObject::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
}

void FrameBufferObject::unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


std::shared_ptr<FrameBufferObject> SwappableFrameBufferObject::createBuffer() {
  return std::make_shared<FrameBufferObject>(_width, _height, _color, _depth);
}