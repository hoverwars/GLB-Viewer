#include "framebuffer.h"

#include "position.h"
#include "../controllers/windowContextProvider.h"
#include "../utils/logs.h"

Framebuffer::Framebuffer()
{
    _width = -1;
    _height = -1;
    generateAssets();
}

void Framebuffer::generateAssets()
{
    glGenFramebuffers(1, &_framebufferId);
    glGenTextures(1, &_renderedTexture);
    glGenRenderbuffers(1, &_depthBuffer);
}

void Framebuffer::resizeFramebuffer(int width, int height)
{
    _width = width;
    _height = height;

    glBindTexture(GL_TEXTURE_2D, _renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _renderedTexture, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);
}

void Framebuffer::renderToFramebuffer(int width, int height, std::function<void()> renderer)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _framebufferId);
    if (_width != width || _height != height)
    {
        resizeFramebuffer(width, height);
    }
    
    Size2D oldVpSize = WindowContextProvider::getInstance()->getFramebufferSize();
    
    glViewport(0, 0, _width, _height);

    // Rendering action provided for the buffer.
    renderer();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, oldVpSize.width, oldVpSize.height);
}

GLuint Framebuffer::getTextureId()
{
    return _renderedTexture;
}