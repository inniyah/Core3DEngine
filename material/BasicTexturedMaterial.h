#pragma once

#include "../util/WeakPointer.h"
#include "BaseMaterial.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture;

    class BasicTexturedMaterial : public BaseMaterial {
        friend class Engine;

    public:
        virtual ~BasicTexturedMaterial();
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0) override;
        virtual void sendCustomUniformsToShader() override;
        virtual void copyTo(WeakPointer<Material> target) override;
        virtual WeakPointer<Material> clone() override;
        virtual void bindShaderVarLocations() override;

        void setTexture(WeakPointer<Texture> texture);

    protected:
        BasicTexturedMaterial();
    
        Int32 uvLocation;
        PersistentWeakPointer<Texture> texture;
        Int32 textureLocation;
    };
}
