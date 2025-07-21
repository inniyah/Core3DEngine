#pragma once

#include "../util/WeakPointer.h"
#include "BaseLitMaterial.h"
#include "ShaderMaterial.h"
#include "../common/Constants.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture;

    class BasicLitMaterial : public ShaderMaterial<BaseLitMaterial> {
        friend class Engine;

    public:
        virtual Bool build() override;
        virtual WeakPointer<Material> clone() override;

    protected:
        BasicLitMaterial(const std::string& vertexShader, const std::string& fragmentShader);
        BasicLitMaterial(const std::string& buildInShaderName);
        BasicLitMaterial();
    };
}
