#pragma once

#include <memory>
#include <vector>

#include "util/WeakPointer.h"
#include "base/CoreObjectReferenceManager.h"
#include "image/TextureAttr.h"
#include "geometry/AttributeType.h"
#include "render/RenderState.h"
#include "render/RenderBuffer.h"
#include "render/PrimitiveType.h"
#include "render/RenderStyle.h"
#include "geometry/Vector2.h"
#include "geometry/Vector4.h"
#include "color/Color.h"

namespace Core {

    // forward declarations
    class Mesh;
    class Texture2D;
    class CubeTexture;
    class Shader;
    class AttributeArrayGPUStorage;
    class IndexBuffer;
    class Renderer;
    class Scene;
    class ShaderManager;
    class RenderTarget;
    class RenderTarget2D;
    class RenderTargetCube;
    class Material;
    class Engine;
    
    class Graphics {
    public:

        friend class Engine;

        Graphics();
        virtual ~Graphics();
        virtual void init();

        static void safeReleaseObject(WeakPointer<CoreObject> object);

        WeakPointer<Texture2D> getPlaceHolderTexture2D();
        WeakPointer<CubeTexture> getPlaceHolderCubeTexture();

        void setSharedRenderState(Bool shared);

        virtual WeakPointer<Renderer> getRenderer() = 0;
        virtual void preRender() = 0;
        virtual void postRender() = 0;
        void setRenderSize(UInt32 width, UInt32 height, Bool updateViewport = true);
        void setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);
        virtual void setDefaultViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);
        virtual void setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) = 0;
        virtual Vector4u getViewport() = 0;

        virtual WeakPointer<Texture2D> createTexture2D(const TextureAttributes& attributes) = 0;
        virtual WeakPointer<CubeTexture> createCubeTexture(const TextureAttributes& attributes) = 0;

        virtual WeakPointer<Shader> createShader(const std::string& vertex, const std::string& fragment) = 0;
        virtual WeakPointer<Shader> createShader(const std::string& vertex, const std::string& geometry, const std::string& fragment) = 0;
        virtual WeakPointer<Shader> createShader(const char vertex[], const char fragment[]) = 0;
        virtual WeakPointer<Shader> createShader(const char vertex[], const char geometry[], const char fragment[]) = 0;
        virtual void activateShader(WeakPointer<Shader> shader) = 0;

        virtual void drawBoundVertexBuffer(UInt32 vertexCount, PrimitiveType primitiveType = PrimitiveType::Triangles) = 0;
        virtual void drawBoundVertexBuffer(UInt32 vertexCount, WeakPointer<IndexBuffer> indices, PrimitiveType primitiveType = PrimitiveType::Triangles) = 0;

        virtual ShaderManager& getShaderManager() = 0;

        virtual void setBlendingEnabled(Bool enabled) = 0;
        virtual void setBlendingEquation(RenderState::BlendingEquation) = 0;
        virtual void setBlendingFactors(RenderState::BlendingFactor source, RenderState::BlendingFactor dest) = 0;

        virtual WeakPointer<RenderTarget2D> createRenderTarget2D(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                                                                 const TextureAttributes& colorTextureAttributes,
                                                                 const TextureAttributes& depthTextureAttributes, const Vector2u& size) = 0;
        virtual WeakPointer<RenderTargetCube> createRenderTargetCube(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                                                                     const TextureAttributes& colorTextureAttributes,
                                                                     const TextureAttributes& depthTextureAttributes, const Vector2u& size) = 0;

        void blit(WeakPointer<RenderTarget> source, WeakPointer<RenderTarget> destination, Int16 cubeFace, WeakPointer<Material> material, Bool includeDepth);
        virtual void lowLevelBlit(WeakPointer<RenderTarget> source, WeakPointer<RenderTarget> destination, Int16 cubeFace, Bool includeColor, Bool includeDepth) = 0;
        void renderFullScreenQuad(WeakPointer<RenderTarget> destination, Int16 cubeFace, WeakPointer<Material> material);

        virtual void setColorWriteEnabled(Bool enabled) = 0;                                                               
        virtual void setClearColor(Color color) = 0;
        virtual void clearActiveRenderTarget(Bool colorBuffer, Bool depthBuffer, Bool stencilBuffer) = 0;
        virtual void setDefaultRenderTargetToCurrent() = 0;
        virtual WeakPointer<RenderTarget> getDefaultRenderTarget() = 0;
        virtual WeakPointer<RenderTarget> getCurrentRenderTarget() = 0;
        virtual void updateDefaultRenderTargetSize(Vector2u size) = 0;
        virtual void updateDefaultRenderTargetViewport(Vector4u viewport) = 0;
        virtual Bool activateRenderTarget(WeakPointer<RenderTarget> target) = 0;
        virtual Bool activateRenderTarget2DMipLevel(UInt32 mipLevel) = 0;
        virtual Bool activateCubeRenderTargetSide(CubeTextureSide side, UInt32 mipLevel) = 0;
        virtual void setRenderStyle(RenderStyle style) = 0;

        virtual void setDepthWriteEnabled(Bool enabled) = 0;
        virtual void setDepthTestEnabled(Bool enabled) = 0;
        virtual void setDepthFunction(RenderState::DepthFunction function) = 0;

        virtual void setStencilTestEnabled(Bool enabled) = 0;
        virtual void setStencilWriteMask(UInt32 mask) = 0;
        virtual void setStencilFunction(RenderState::StencilFunction function, Int16 value, UInt16 mask) = 0;
        virtual void setStencilOperation(RenderState::StencilAction sFail, RenderState::StencilAction dpFail, RenderState::StencilAction dpPass) = 0;

        virtual void setFaceCullingEnabled(Bool enabled) = 0;
        virtual void setCullFace(RenderState::CullFace face) = 0;

        virtual void setRenderLineSize(Real size) = 0;

        virtual void saveState() = 0;
        virtual void restoreState() = 0;

    protected:

        virtual std::shared_ptr<AttributeArrayGPUStorage> createGPUStorage(UInt32 size, UInt32 componentCount, AttributeType type, Bool normalize) = 0;
        virtual std::shared_ptr<IndexBuffer> createIndexBuffer(UInt32 size) = 0;
        void addCoreObjectReference(std::shared_ptr<CoreObject>, CoreObjectReferenceManager::OwnerType ownerType);

        CoreObjectReferenceManager objectManager;

        WeakPointer<Texture2D> placeHolderTexture2D;
        WeakPointer<CubeTexture> placeHolderCubeTexture;
        Bool sharedRenderState;
    };
}