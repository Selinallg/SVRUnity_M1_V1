#pragma once

#define SVR_EXPORT

#define MAX_EYE_BUFFERS 128

#define DEBUG 0

#if DEBUG
#define LogDebug(...)	__android_log_print(ANDROID_LOG_INFO, "svrUnity", __VA_ARGS__ )
#else
#define LogDebug(...)
#endif

#define LogSvr(...)
#define LogSvrError(...)
#define LogSvrT(...)
#define LOGI(...)

typedef struct {
    int Width;
    int Height;
    int Multisamples;
    int TextureSwapChainLength;
    int TextureSwapChainIndex;
    bool UseMultiview;
/*    ovrTextureSwapChain *ColorTextureSwapChain;*/
    GLuint *ColorTextureSwapChain;  //use this to store gl texture handle. nico.liu
    GLuint *DepthBuffers;
    GLuint *FrameBuffers;
} ovrFramebuffer;


static const char *GlFrameBufferStatusString(GLenum status) {
    switch (status) {
        case GL_FRAMEBUFFER_UNDEFINED:
            return "GL_FRAMEBUFFER_UNDEFINED";
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
        case GL_FRAMEBUFFER_UNSUPPORTED:
            return "GL_FRAMEBUFFER_UNSUPPORTED";
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
        default:
            return "unknown";
    }
}