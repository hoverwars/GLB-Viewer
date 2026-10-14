#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>
#include <functional>

class Framebuffer
{
private:
    int _width, _height;
    GLuint _framebufferId;
    GLuint _renderedTexture;
    GLuint _depthBuffer;
    void generateAssets();
    void resizeFramebuffer(int width, int height);
public:
    Framebuffer();
    void renderToFramebuffer(int width, int height, std::function<void()> renderer);
    GLuint getTextureId();
};

#endif