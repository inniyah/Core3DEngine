#pragma once

#include "../util/WeakPointer.h"
#include "SkyboxMaterial.h"

namespace Core {

    // forward declarations
    class Engine;

    class PhysicalSkyboxMaterial : public SkyboxMaterial {
        friend class Engine;
    public:
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;
        virtual void copyTo(WeakPointer<Material> target) override;
        virtual void bindShaderVarLocations() override;

        void setExposure(Real exposure);

    protected:
        PhysicalSkyboxMaterial();

        Real exposure;
        Int32 exposureLocation;
    };
}
