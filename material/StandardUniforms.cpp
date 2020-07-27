#include <string>
#include <unordered_map>

#include "StandardUniforms.h"
#include "../common/Exception.h"

namespace Core {
    std::shared_ptr<StandardUniforms> StandardUniforms::instance = nullptr;

    StandardUniforms::StandardUniforms() {

        uniformNames =
        {   
            "MODEL_MATRIX",
            "VIEW_MATRIX",
            "MODELVIEW_MATRIX",
            "PROJECTION_MATRIX",
            "MODELVIEWPROJECTION_MATRIX",
            "MODELINVERSETRANSPOSE_MATRIX",
            "VIEWINVERSETRANSPOSE_MATRIX",
            "LIGHT_POSITION",
            "LIGHT_DIRECTION",
            "LIGHT_COLOR",
            "LIGHT_INTENSITY",
            "LIGHT_ATTENUATION",
            "LIGHT_TYPE",
            "LIGHT_RANGE",
            "LIGHT_ENABLED",
            "LIGHT_SHADOWS_ENABLED",
            "LIGHT_MATRIX",
            "LIGHT_SHADOW_MAP",
            "LIGHT_SHADOW_CUBE_MAP",
            "LIGHT_CONSTANT_SHADOW_BIAS",
            "LIGHT_ANGULAR_SHADOW_BIAS",
            "LIGHT_SHADOW_MAP_SIZE",
            "LIGHT_SHADOW_MAP_ASPECT",
            "LIGHT_SHADOW_SOFTNESS",
            "LIGHT_VIEW_PROJECTION",
            "LIGHT_CASCADE_END",
            "LIGHT_CASCADE_COUNT",
            "LIGHT_NEAR_PLANE",
            "LIGHT_IRRADIANCE_MAP",
            "LIGHT_SPECULAR_IBL_PREFILTERED_MAP",
            "LIGHT_SPECULAR_IBL_BRDF_MAP",
            "LIGHT_COUNT",
            "AMBIENT_LIGHT_COUNT",
            "AMBIENT_IBL_LIGHT_COUNT",
            "POINT_LIGHT_COUNT",
            "DIRECTIONAL_LIGHT_COUNT",
            "CAMERA_POSITION",
            "TEXTURE0",
            "DEPTH_TEXTURE",
            "SKINNING_ENABLED",
            "BONES",
            "SSAOMAP",
            "SSAOENABLED",
            "RENDERING_SHADOWS"
        };

        nameToUniform =
        {
            {uniformNames[(UInt16)StandardUniform::ModelMatrix],StandardUniform::ModelMatrix},
            {uniformNames[(UInt16)StandardUniform::ViewMatrix],StandardUniform::ViewMatrix},
            {uniformNames[(UInt16)StandardUniform::ModelViewMatrix],StandardUniform::ModelViewMatrix},
            {uniformNames[(UInt16)StandardUniform::ProjectionMatrix],StandardUniform::ProjectionMatrix},
            {uniformNames[(UInt16)StandardUniform::ModelViewProjectionMatrix],StandardUniform::ModelViewProjectionMatrix},
            {uniformNames[(UInt16)StandardUniform::ModelInverseTransposeMatrix],StandardUniform::ModelInverseTransposeMatrix},
            {uniformNames[(UInt16)StandardUniform::ViewInverseTransposeMatrix],StandardUniform::ViewInverseTransposeMatrix},
            {uniformNames[(UInt16)StandardUniform::LightPosition],StandardUniform::LightPosition},
            {uniformNames[(UInt16)StandardUniform::LightDirection],StandardUniform::LightDirection},
            {uniformNames[(UInt16)StandardUniform::LightColor],StandardUniform::LightColor},
            {uniformNames[(UInt16)StandardUniform::LightIntensity],StandardUniform::LightIntensity},
            {uniformNames[(UInt16)StandardUniform::LightAttenuation],StandardUniform::LightAttenuation},
            {uniformNames[(UInt16)StandardUniform::LightType],StandardUniform::LightType},
            {uniformNames[(UInt16)StandardUniform::LightRange],StandardUniform::LightRange},
            {uniformNames[(UInt16)StandardUniform::LightEnabled],StandardUniform::LightEnabled},
            {uniformNames[(UInt16)StandardUniform::LightShadowsEnabled],StandardUniform::LightShadowsEnabled},
            {uniformNames[(UInt16)StandardUniform::LightMatrix],StandardUniform::LightMatrix},
            {uniformNames[(UInt16)StandardUniform::LightShadowMap],StandardUniform::LightShadowMap},
            {uniformNames[(UInt16)StandardUniform::LightShadowCubeMap],StandardUniform::LightShadowCubeMap},
            {uniformNames[(UInt16)StandardUniform::LightConstantShadowBias],StandardUniform::LightConstantShadowBias},
            {uniformNames[(UInt16)StandardUniform::LightAngularShadowBias],StandardUniform::LightAngularShadowBias},
            {uniformNames[(UInt16)StandardUniform::LightShadowMapSize],StandardUniform::LightShadowMapSize},
            {uniformNames[(UInt16)StandardUniform::LightShadowMapAspect],StandardUniform::LightShadowMapAspect},
            {uniformNames[(UInt16)StandardUniform::LightShadowSoftness],StandardUniform::LightShadowSoftness},
            {uniformNames[(UInt16)StandardUniform::LightViewProjection],StandardUniform::LightViewProjection},
            {uniformNames[(UInt16)StandardUniform::LightCascadeEnd],StandardUniform::LightCascadeEnd},
            {uniformNames[(UInt16)StandardUniform::LightCascadeCount],StandardUniform::LightCascadeCount},
            {uniformNames[(UInt16)StandardUniform::LightNearPlane],StandardUniform::LightNearPlane},
            {uniformNames[(UInt16)StandardUniform::LightIrradianceMap],StandardUniform::LightIrradianceMap},
            {uniformNames[(UInt16)StandardUniform::LightSpecularIBLPreFilteredMap],StandardUniform::LightSpecularIBLPreFilteredMap},
            {uniformNames[(UInt16)StandardUniform::LightSpecularIBLBRDFMap],StandardUniform::LightSpecularIBLBRDFMap},
            {uniformNames[(UInt16)StandardUniform::LightCount],StandardUniform::LightCount},
            {uniformNames[(UInt16)StandardUniform::AmbientLightCount],StandardUniform::AmbientLightCount},
            {uniformNames[(UInt16)StandardUniform::AmbientIBLLightCount],StandardUniform::AmbientIBLLightCount},
            {uniformNames[(UInt16)StandardUniform::PointLightCount],StandardUniform::PointLightCount},
            {uniformNames[(UInt16)StandardUniform::DirectionalLightCount],StandardUniform::DirectionalLightCount},
            {uniformNames[(UInt16)StandardUniform::CameraPosition],StandardUniform::CameraPosition},
            {uniformNames[(UInt16)StandardUniform::Texture0], StandardUniform::Texture0},
            {uniformNames[(UInt16)StandardUniform::DepthTexture], StandardUniform::DepthTexture},
            {uniformNames[(UInt16)StandardUniform::SkinningEnabled], StandardUniform::SkinningEnabled},
            {uniformNames[(UInt16)StandardUniform::Bones], StandardUniform::Bones},
            {uniformNames[(UInt16)StandardUniform::SSAOMap], StandardUniform::SSAOMap},
            {uniformNames[(UInt16)StandardUniform::SSAOEnabled], StandardUniform::SSAOEnabled},
            {uniformNames[(UInt16)StandardUniform::DepthOutputOverride], StandardUniform::DepthOutputOverride}
        };
    }

    const std::string& StandardUniforms::getUniformName(StandardUniform uniform) {
        checkAndInitInstance();
        return instance->_getUniformName(uniform);
    }

    StandardUniform StandardUniforms::getUniformForName(const std::string& name) {
        checkAndInitInstance();
        return instance->_getUniformForName(name);
    }

    const std::string& StandardUniforms::_getUniformName(StandardUniform uniform) {
        return uniformNames[(UInt16)uniform];
    }

    StandardUniform StandardUniforms::_getUniformForName(const std::string& name) {
        auto result = nameToUniform.find(name);
        if (result == nameToUniform.end()) {
            return StandardUniform::_None;
        }

        return (*result).second;
    }

    void StandardUniforms::checkAndInitInstance() {
        StandardUniforms* uniformsPtr = new(std::nothrow) StandardUniforms();
        if (uniformsPtr == nullptr) {
            throw AllocationException("StandardUniforms::checkAndInitInstance -> Unable to allocate StandardUniforms.");
        }

        if (!instance) {
           instance = std::shared_ptr<StandardUniforms>(uniformsPtr);
           instance->init();
        }
    }

    void StandardUniforms::init() {

    }
}