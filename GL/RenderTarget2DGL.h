/*
 * class:  RenderTarget2DGL
 *
 * Author: Mark Kellogg
 *
 * OpenGL implementation of RenderTarget2D. Currently it makes use of Framebuffer Objects (FBO)
 * with a single texture color attachment at GL_COLOR_ATTACHMENT0, and single texture
 * depth attachment at GL_DEPTH_ATTACHMENT (as of now only one depth attachment is supported ).
 */

#pragma once

#include "../common/gl.h"
#include "../render/RenderTarget2D.h"
#include "../base/BitMask.h"
#include "../common/Exception.h"
#include "RenderTargetGL.h"

namespace Core {

    // forward declarations
    class TextureAttributes;
    class GraphicsGL;

    class RenderTarget2DGL final : public RenderTarget2D, public RenderTargetGL {
        friend class GraphicsGL;
    public:
        ~RenderTarget2DGL() override;
        Bool init() override;
        Bool addColorTexture(TextureAttributes attributes) override;
        void destroyColorBuffer(UInt32 index = 0) override;
        void destroyDepthBuffer() override;

    protected:

        RenderTarget2DGL(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                         const TextureAttributes& colorTextureAttributes, 
                         const TextureAttributes& depthTextureAttributes, Vector2u size,
                         Int32 initialFBOID = 0);
        Bool initColorTexture(UInt32 index);
    };
}
